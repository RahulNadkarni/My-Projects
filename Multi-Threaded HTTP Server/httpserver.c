// Asgn 2: A simple HTTP server.
// By: Eugene Chou
//     Andrew Quinn
//     Brian Zhao

#include "asgn2_helper_funcs.h"
#include "connection.h"
#include "debug.h"
#include "response.h"
#include "request.h"
#include "queue.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/file.h>
#include <sys/stat.h>

void handle_connection(int);

void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);
int worker_threads();

queue_t *q = NULL;

void audit_log(char *req, char *uri, int status, char *rid);

pthread_rwlock_t rwlock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gmutex;
int main(int argc, char **argv) {

    if (argc < 2) {
        warnx("wrong arguments: %s port_num", argv[0]);
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //get the number of threads - if not specified set the default to four
    int opt = 0;
    int threads = 0;
    //int in = 0;
    while ((opt = getopt(argc, argv, "t: ")) != -1) {
        switch (opt) {
        case 't': threads = atoi(optarg); break;

        default:
            //			    in = 1;
            threads = 4;
        }
    }

    char *endptr = NULL;
    size_t port = (size_t) strtoull(argv[argc - 1], &endptr, 10);
    if (endptr && *endptr != '\0') {
        warnx("invalid port number: %s", argv[argc - 1]);
        return EXIT_FAILURE;
    }

    signal(SIGPIPE, SIG_IGN);
    Listener_Socket sock;
    listener_init(&sock, port);
    //create threads
    pthread_t wt[threads];
    //create the bounded buffer queue of size threads
    q = queue_new(threads);

    for (int i = 0; i < threads; i++) {
        pthread_create(&wt[i], NULL, (void *(*) (void *) ) worker_threads, NULL);
    }
    //fprintf(stderr,"h");
    //accept a connection and push it to the queue
    while (1) {
        uintptr_t connfd = listener_accept(&sock);
        queue_push(q, (void *) connfd);
        //    close(connfd);
    }

    return EXIT_SUCCESS;
}

//worker threads function waits until there is an element in the bounded buffer to pop
int worker_threads() {
    uintptr_t connfd;
    while (1) {
        queue_pop(q, (void **) &connfd); //pop off the connection
        handle_connection(connfd); //handle
        close(connfd); //close
    }
    return 0;
}

//audit log function
//takes in the request, uri, status, and request id and outputs to standard error
void audit_log(char *req, char *uri, int status, char *rid) {
    fprintf(stderr, "%s,/%s,%d,%s\n", req, uri, status, rid);
}

void handle_connection(int connfd) {

    conn_t *conn = conn_new(connfd);
    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        conn_send_response(conn, res);
    } else {
        //       debug("%s", conn_str(conn));
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }

    conn_delete(&conn);
}

void handle_get(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    char *rid = NULL;
    rid = conn_get_header(conn, "Request-Id");
    char *req = "GET";

    //    debug("GET request not implemented. But, we want to get %s", uri);

    // What are the steps in here?

    // 1. Open the file.
    // If  open it returns < 0, then use the result appropriately
    //   a. Cannot access -- use RESPONSE_FORBIDDEN
    //   b. Cannot find the file -- use RESPONSE_NOT_FOUND
    //   c. other error? -- use RESPONSE_INTERNAL_SERVER_ERROR
    // (hint: check errno for these cases)!

    // 2. Get the size of the file.
    // (hint: checkout the function fstat)!

    // Get the size of the file.

    // 3. Check if the file is a directory, because directories *will*
    // open, but are not valid.
    // (hint: checkout the macro "S_IFDIR", which you can use after you call fstat!)

    // 4. Send the file
    // (hint: checkout the conn_send_file function!)

    //mutual exclusion for file opening
    pthread_mutex_lock(&mutex);
    int fp = open(uri, O_RDONLY, 0666);

    //error cases
    if (fp < 0) {
        if (errno == EACCES) {
            audit_log(req, uri, 403, rid);
            conn_send_response(conn, &RESPONSE_FORBIDDEN);
        } else if (errno == ENOENT) {
            audit_log(req, uri, 404, rid);
            //	    close(fp);
            conn_send_response(conn, &RESPONSE_NOT_FOUND);
        } else {
            audit_log(req, uri, 500, rid);
            //	    close(fp);
            conn_send_response(conn, &RESPONSE_INTERNAL_SERVER_ERROR);
        }
        pthread_mutex_unlock(
            &mutex); //unlock the mutex if one of these conditions are true and return
        close(fp);
        return;
    }

    //check if is directory
    struct stat check;
    stat(uri, &check);
    if (S_ISDIR(check.st_mode)) {
        conn_send_response(conn, &RESPONSE_FORBIDDEN);
        audit_log(req, uri, 403, rid);
        pthread_mutex_unlock(&mutex); //unlock and return
        close(fp);
        return;
    }

    //if none of the above are true, then unlock the code and put a shared reader lock on the file
    pthread_mutex_unlock(&mutex);

    flock(fp, LOCK_SH);
    //get the content length of the file
    int contentLength = 0;
    struct stat file_stat;
    fstat(fp, &file_stat);
    contentLength = file_stat.st_size;
    //get
    conn_send_file(conn, fp, contentLength);
    //audit log
    audit_log(req, uri, 200, rid);
    //instead of using LOCK_UN, unlock the file by closing it
    close(fp);
}
void handle_unsupported(conn_t *conn) {
    debug("handling unsupported request");

    // send responses
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}

void handle_put(conn_t *conn) {
    //pthread_mutex_lock(&mutex);
    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;
    //debug("handling put request for %s", uri);

    char *rid = NULL;
    rid = conn_get_header(conn, "Request-Id");
    char *req = "PUT";

    //mutex lock around file opening and creation
    pthread_mutex_lock(&mutex);
    bool existed = access(uri, F_OK) == 0;

    int fd = open(uri, O_CREAT | O_WRONLY, 0600);
    //lock the file and truncate outside of the open to put mutual exclusion per file
    flock(fd, LOCK_EX);
    ftruncate(fd, 1);

    int status = 0;
    if (fd < 0) {
        debug("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            status = 403;
            //if error unlock
            res = &RESPONSE_FORBIDDEN;
            pthread_mutex_unlock(&mutex);
            goto out;
        } else {
            //if error unlock
            status = 500;
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            pthread_mutex_unlock(&mutex);
            goto out;
        }
    }
    //if at this point, then unlock the mutex
    pthread_mutex_unlock(&mutex);
    //put
    res = conn_recv_file(conn, fd);
    //get the correct status code and send to audit log
    if (res == NULL && existed) {
        status = 200;
        res = &RESPONSE_OK;
        goto out;
    } else if (res == NULL && !existed) {
        status = 201;
        res = &RESPONSE_CREATED;
        goto out;
    }

out:
    //send to audit long and send response.
    audit_log(req, uri, status, rid);
    conn_send_response(conn, res);
    close(fd);
}

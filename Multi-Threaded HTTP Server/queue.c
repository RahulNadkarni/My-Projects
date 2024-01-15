#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
#include <pthread.h>

typedef struct queue {
    int size;
    pthread_mutex_t lock;
    pthread_cond_t full;
    pthread_cond_t empty;
    int count;
    void **buf;
    int in;
    int out;
} queue_t;

queue_t *queue_new(int size) {
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    q->size = size;
    q->in = 0;
    q->out = 0;
    q->count = 0;
    q->buf = (void **) malloc(size * sizeof(void *));
    if (q->buf == NULL) {
        free(q);
        return NULL;
    }
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->full, NULL);
    pthread_cond_init(&q->empty, NULL);

    return q;
}

void queue_delete(queue_t **q) {
    if (*q && (*q)->buf) {
        pthread_mutex_destroy(&(*q)->lock);
        pthread_cond_destroy(&(*q)->full);
        pthread_cond_destroy(&(*q)->empty);
        free((*q)->buf);
        free((*q));
        *q = NULL;
    }
}
bool queue_push(queue_t *q, void *elem) {
    //pthread_mutex_lock(&q->lock);

    if (q == NULL) {
        return 0;
    }
    pthread_mutex_lock(&q->lock);
    while (q->count == q->size) {
        pthread_cond_wait(&q->full, &q->lock);
    }
    //pthread_mutex_lock(&q->lock);
    q->buf[q->in] = elem;
    q->in = (q->in + 1) % q->size;
    q->count += 1;
    pthread_cond_signal(&q->empty);
    pthread_mutex_unlock(&q->lock);
    return 1;
}
bool queue_pop(queue_t *q, void **elem) {

    if (q == NULL) {
        return 0;
    }
    pthread_mutex_lock(&q->lock);
    while (q->count == 0) {
        pthread_cond_wait(&q->empty, &q->lock);
    }
    //pthread_mutex_lock(&q->lock);
    *elem = q->buf[q->out];
    q->out = (q->out + 1) % q->size;
    q->count -= 1;
    pthread_cond_signal(&q->full);
    pthread_mutex_unlock(&q->lock);
    return 1;
}

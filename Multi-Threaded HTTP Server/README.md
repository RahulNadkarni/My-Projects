# Multi Threaded HTTP Server 

This assignment was done for CSE-130 at UCSC. 

# Description

The goal of this assignment was to manage conucrrency using synchronization. This assignment was the culmination of all the programming assignments in the quarter up to this point. It utilizes string parsing, the http server, and the bounded buffer. This HTTP server can only run two commands: get and put. The following should be true: Multiple gets, puts, or gets and puts, should be able to run simultaneously without deadlocking. 

# Running

To run the file, run make to compile the files and make sure you have clang installed. Create two windows in your terminal: one for the client and one for the server. In this case, you can create multiple clients for the server and you will not run into any issues. Running get will retrieve the contents from a file, and running put will write contents to a file. 






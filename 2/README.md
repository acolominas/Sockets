Unbounded concurrent server: update the server code to enable it to serve concurrent clients.
The changes required are to implement the doServiceFork function and to call this function
each time the server accepts a client connection (instead of calling doService function). Func-
tion doServiceFork just creates a new process which executes the doService function and then
ends the execution. In this scheme the number of concurrent accepted clients is unbounded:
the server accepts all the client requests received.

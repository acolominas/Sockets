Bounded concurrent server: this scheme is a variation of the unbounded concurrent server,
which controls the number of concurrent accepted clients. The goal is to accept only the
clients that it is able to serve without suffering performance deterioration. In this scheme,
the server have to keep a counter of its current alive children and when this counter reaches
an upper bound it has to stop accepting new client connections and has to block until a
child ends the execution. You have to choose the value for this upper bound and define a
constant in the server code or add a new parameter to the main program.

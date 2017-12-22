#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CHILDS 25

int num_connections = 0;

void handleChildExit(int numSignal) {
	num_connections--;
}

void write_connection(){
	char buffer[30];
	sprintf(buffer,"\nnum_connections %d\n",num_connections);
	write (1, buffer, strlen (buffer));
}

void doService(int fd) {
int i = 0;
char buff[80];
char buff2[80];
int ret;
int socket_fd = (int) fd;

	ret = read(socket_fd, buff, sizeof(buff));
	while(ret > 0) {
		buff[ret]='\0';
		sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
		write(1, buff2, strlen(buff2));
		ret = write(fd, "caracola ", 8);
		if (ret < 0) {
			perror ("Error writing to socket");
			exit(1);
		}
		ret = read(socket_fd, buff, sizeof(buff));
	}
	if (ret < 0) {
			perror ("Error reading from socket");

	}
	sprintf(buff2, "Server [%d] ends service\n", getpid());
	write(1, buff2, strlen(buff2));

}

void doServiceFork(int fd)
{
	write_connection();
	if(num_connections >= MAX_CHILDS) {
		while(waitpid(-1, NULL, 0) > WNOHANG);
	}
	int pid = fork();
	if(pid < 0) {
		perror ("Error creating child");
		exit(1);
	}
	else if(pid == 0) {
		doService(fd);
		exit(0);
	}
	else ++num_connections;
}

void main (int argc, char *argv[])
{
  int socketFD;
  int connectionFD;
  char buffer[80];
  int ret;
  int port;


  if (argc != 2)
    {
      strcpy (buffer, "Usage: ServerSocket PortNumber\n");
      write (2, buffer, strlen (buffer));
      exit (1);
    }

  port = atoi(argv[1]);
  socketFD = createServerSocket (port);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }

  signal(SIGCHLD, handleChildExit);
  while (1) {
	  connectionFD = acceptNewConnections (socketFD);
	  if (connectionFD < 0)
	  {
		  perror ("Error establishing connection \n");
		  deleteSocket(socketFD);
		  exit (1);
	  }
	  doServiceFork(connectionFD);
  }

}

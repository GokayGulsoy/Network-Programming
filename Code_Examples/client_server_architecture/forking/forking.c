/* server that uses forking */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/* signal handler for zombie 
   child processes to die 
*/
void sigchld_handler(int signo) {
    while (waitpid(-1,NULL,WNOHANG) > 0);
}

/* main forking server code */
int main(int argc, char *argv[])
{
    struct sockaddr_in sAddr;
    int listensock;
    int newsock;
    char buffer[25];
    int result;
    int nread;
    int pid;
    int val;

    // creating a listening socket
    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    val = 1;
    // setting socket options 
    result = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (result < 0) {
        perror("socket options cannot be set");
        return 0;
    }

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(1972);
    sAddr.sin_addr.s_addr = INADDR_ANY;
    result = bind(listensock, (struct sockaddr *) &sAddr, sizeof(sAddr));
    if (result < 0) {
        perror("server2");
        return 0;
    }

    // setting socket to listening mode
    result = listen(listensock, 5);
    if (result < 0) {
        perror("server2");
        return 0;
    }

    // we install our signal handler
    signal(SIGCHLD,sigchld_handler);

    while (1) {
        // waiting connections from clients
        newsock = accept(listensock, NULL, NULL);
        if ((pid = fork()) == 0) {
            
            printf("child process %i created.\n", getpid());
            close(listensock);
            nread = recv(newsock, buffer, 25, 0);
            buffer[nread] = '\0';
            printf("%s\n", buffer);
            send(newsock, buffer, nread, 0);
            close(newsock);
            printf("child process %i finished.\n", getpid());
            exit(0);
            } 

            close(newsock);
        }
}
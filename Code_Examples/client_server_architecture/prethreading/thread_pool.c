/* server that uses prethreading */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// function prototype for threads
void* thread_proc(void *arg);

int main(int argc, char *argv[])
{

    struct sockaddr_in sAddr;
    int listensock;
    int result;
    int nchildren = 1;
    pthread_t thread_id;
    int x;
    int val;

    if (argc > 1) {
        nchildren = atoi(argv[1]);
    }

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

    // binding to listening socket
    result = bind(listensock, (struct sockaddr *) &sAddr, sizeof(sAddr));
    if (result < 0) {
    perror("listening socket can not be bouund");
    return 0;
    }

    // setting socket into listening mode
    result = listen(listensock, 5);
    if (result < 0) {
        perror("server5");
        return 0;
    }

    for (x = 0; x < nchildren; x++) {
        result = pthread_create(&thread_id, NULL, thread_proc,
        (void *) listensock);
        if (result != 0) {
            printf("Could not create thread.\n");
        }
        
        sched_yield();
    }

    pthread_join(thread_id,NULL);
}


// definition of thread_proc function
void* thread_proc(void* arg) {
    int listensock,
    sock;
    char buffer[25];
    int nread;
    listensock = (int) arg;

    while (1) {

        // accepting connections from client 
        sock = accept(listensock, NULL, NULL);
        
        printf("client connected to child thread %i with pid %i.\n",
        (int)pthread_self(), getpid());
        // receiving data from client
        nread = recv(sock, buffer, 25, 0);
        buffer[nread] = '\0';
        printf("%s\n", buffer);
        // sending data back to client
        send(sock, buffer, nread, 0);
        close(sock);
        printf("client disconnected from child thread %i with pid %i.\n",
        (int)pthread_self(), getpid());

    }
}
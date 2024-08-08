/* server that uses multithreading */
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

// function prototype for thread operations
void *thread_proc(void *arg);

int main(int argc, char *argv[])
{
    struct sockaddr_in sAddr;
    int listensock;
    int newsock;
    int result;
    pthread_t thread_id;
    int val;

    // creating a socket for listening incoming connections
    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    val = 1;
    result = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (result < 0)
    {
        perror("server4");
        return 0;
    }

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(1972);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    // binding to listening socket
    result = bind(listensock, (struct sockaddr *)&sAddr, sizeof(sAddr));
    if (result < 0)
    {
        perror("server4");
        return 0;
    }

    result = listen(listensock, 5);
    if (result < 0)
    {
        perror("socket can not be listened");
        return 0;
    }

    while (1)
    {
        newsock = accept(listensock, NULL, NULL);
        result = pthread_create(&thread_id, NULL, thread_proc, (void *)newsock);

        if (result != 0)
        {
            printf("Could not create thread.\n");
        }

        pthread_detach(thread_id);
        sched_yield();
    }
}

// implementation of thread function
void *thread_proc(void *arg)
{
    int sock;
    char buffer[25];
    int nread;

    printf("child thread %i with pid %i created.\n",(int) pthread_self(),
    getpid());
    sock = (int) arg;
    nread = recv(sock, buffer, 25, 0);
    buffer[nread] = '\0';
    printf("%s\n", buffer);
    send(sock, buffer, nread, 0);
    close(sock);
    printf("child thread %i with pid %i finished.\n",(int) pthread_self(),
    getpid());
}
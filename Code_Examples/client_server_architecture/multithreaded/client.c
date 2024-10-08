/* client */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>


/* function prototype for child_func */
void child_func(int childnum);

int main(int argc,char *argv[]) {
    int nchildren = 1;
    int pid;
    int x;

    if (argc > 1) {
        nchildren = atoi(argv[1]);
    }    

    for (x = 0; x < nchildren; x++) {
        if ((pid = fork()) == 0) {
            child_func(x+1);
            exit(0);
        }
    }

    // parent process waits for children to finish 
    wait(NULL);
    return 0;
}

// child function implementation
void child_func(int childnum) {
     int sock;
     struct sockaddr_in sAddr;
     char buffer[25];   

    // creating client socket and binding to local port
    memset((void *)&sAddr,0,sizeof(struct sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sAddr.sin_port = htons(1972);

    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    bind(sock,(const struct sockaddr *)&sAddr,sizeof(sAddr));

  

    if (connect(sock,(const struct sockaddr *)&sAddr,sizeof(sAddr)) != 0) {
        perror("client cannot connect to socket");
        return;
    }

    snprintf(buffer,128,"data from client#%i.",childnum);
    sleep(1);    
    printf("child #%i sent %i chars\n",childnum,(int)send(sock,buffer,strlen(buffer),0));
    sleep(1);
    printf("child #%i received %i chars\n",childnum,(int)recv(sock,buffer,25,0));    

    sleep(1);
    close(sock);
}
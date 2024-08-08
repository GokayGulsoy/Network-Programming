#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// setting server port number to 8888
#define SERVERPORT 8888
// setting maximum size for transfer buffers 
#define MAXBUF 1024

int main(int argc, char* argv[]) {

    int sockd;
    int counter;
    int fd;
    struct sockaddr_in xferServer;
    char buf[MAXBUF];
    int returnStatus;

    // ensuring that command line parameters are given correctly
    if (argc < 3) {
        fprintf(stderr,"Usage: %s <ip address> <filename> [dest filename]\n",
        argv[0]);
        exit(1);
    }

    /* create a socket */
    sockd = socket(AF_INET,SOCK_STREAM,0);

    if (sockd == -1) {
        fprintf(stderr,"Could not create a socket!\n");
        exit(1);
    }

    /* set up the server information */
    xferServer.sin_family = AF_INET;
    xferServer.sin_addr.s_addr = inet_addr(argv[1]);
    xferServer.sin_port = htons(SERVERPORT);

    /* connect to the server */
    returnStatus = connect(sockd,(struct sockaddr*)&xferServer,sizeof(xferServer));

    if (returnStatus == -1) {

        fprintf(stderr,"Could not send filename to server!\n");
        exit(1);
    }

    /* send the name of the file we want from server */
    returnStatus = write(sockd,argv[2],strlen(argv[2])+1);

    if (returnStatus == -1) {

        fprintf(stderr,"Could not send filename to server!\n");
        exit(1);
    }   

    /* call shutdown to set our socket to read-only */
    shutdown(sockd,SHUT_WR);    
    
    /* open up a handle to our destination file to 
      receive the contents from the server */ 

    fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1) {

        // fprintf(stderr,"Could not open destination file, using stdout\n");
        fd = 1;
    }


    /* read the file from the socket as long as there is data */
    while ((counter = read(sockd,buf,MAXBUF)) > 0) {   
        write(fd,buf,counter);
    }

    if (counter == -1) {

        fprintf(stderr,"Could not read file from socket!\n");
        exit(1);
    }

    printf("\n\nTransfer completed!\n");
    close(sockd);
    return 0;
}

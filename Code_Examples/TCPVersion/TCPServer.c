#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// setting port number to 8888
#define SERVERPORT 8888
// setting maximum size for transfer buffers
#define MAXBUF 1024

int main() {

    int socket1,socket2;
    int addrlen;
    struct sockaddr_in xferServer, xferClient;
    int returnStatus;

    /*create a socket*/
    socket1 = socket(AF_INET,SOCK_STREAM,0);

    if (socket1 == -1) {

        fprintf(stderr,"Could not create socket!\n");
        exit(1);
    }

    printf("Socket created.\n");
    /*bind to a socket use INADDR_ANY for all local addresses*/
    xferServer.sin_family = AF_INET;
    xferServer.sin_addr.s_addr = INADDR_ANY;
    xferServer.sin_port = htons(SERVERPORT);

    returnStatus = bind(socket1,(struct sockaddr *)&xferServer,sizeof(xferServer));

    if (returnStatus == -1) {
        fprintf(stderr,"Could not bind to socket!\n");
        exit(1);
    }

    printf("Bind completed!\n");
    returnStatus = listen(socket1,5);

    if (returnStatus == -1) {
        fprintf(stderr,"Could not listen on socket!\n");
        exit(1);
    }

    while(1) {
        int fd;
        int i,readCounter,writeCounter;
        char* bufptr;
        char buf[MAXBUF];
        char filename[MAXBUF];

        /* wait for an incoming connection */
        addrlen = sizeof(xferClient);
        /* use accept function to handle incoming requests */
        /* and free up the original socket for other requests */
        socket2 = accept(socket1,(struct sockaddr*)&xferClient,&addrlen);

        if (socket2 == -1) {
            fprintf(stderr,"Could not accept connection!\n");
            exit(1);
        }

        /* get the filename from the client over the socket */
        i = 0;

        if ((readCounter = read(socket2,filename + i,MAXBUF)) > 0) {

            i += readCounter;
        }

        if (readCounter == -1) {
            fprintf(stderr,"Could not read the filename from socket!\n");
            close(socket2);
            continue;
        }

        filename[i+1] = '\0';

        printf("Reading the file %s\n",filename);
        /* open the file for reading */
        fd = open(filename,O_RDONLY); 

        /* reset the read counter */    
        readCounter = 0;

        while ((readCounter = read(fd, buf,MAXBUF)) > 0) {

              writeCounter = 0;  
              bufptr = buf;  

              while (writeCounter < readCounter) {

                readCounter -= writeCounter;
                bufptr += writeCounter;
                writeCounter = write(socket2, bufptr ,readCounter);

                if (writeCounter == -1) {
                    fprintf(stderr,"Could not write file to client!\n");
                    close(socket2);
                    continue;
                }
              } 
        }

        close(fd);
        close(socket2);
    }

    // close the server socket
    close(socket1);
    return 0;
}

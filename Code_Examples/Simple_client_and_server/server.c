#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
Above header files are
used for working with the
sockets API ...
*/
/*
 Simple server that accepts
 incoming connections from
 a client and sends string
 to client who makes a request
*/

// String to be send to client by server
const char APRESSMESSAGE[] = "APPRESS - For Professionals, by Professionals!\n";

int main(int argc, char *argv[])
{

    // defining port number as simplePort
    // defining socket qaulifier as simpleSocket
    int simplePort = 0;
    int simpleSocket = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;

    /* make sure we have a port number */
    if (2 != argc)
    {
        fprintf(stderr, "Usage %s <port>\n", argv[0]);
        exit(1);
    }

    /* creating a streaming socket */
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (simpleSocket == -1)
    {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }

    else
    {
        fprintf(stderr, "Socket created!\n");
    }

    /* end of creation of socket */

    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);
    /* set up the address structure */
    /* use INADDR_ANY to bind to all local addresess*/
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);

    /* bind to the address and port with our socket */
    returnStatus = bind(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));

    if (returnStatus == 0)
    {
        fprintf(stderr, "Bind completed!\n");
    }

    else
    {
        fprintf(stderr, "Could not bind to address!\n");
        close(simpleSocket);
        exit(1);
    }

    while (1)
    {
        struct sockaddr_in clientName = {0};
        int simpleChildSocket = 0;
        int clientNameLength = sizeof(clientName);

        /* wait here */
        simpleChildSocket = accept(simpleSocket, (struct sockaddr *)&clientName, &clientNameLength);

        if (simpleChildSocket == -1)
        {
            fprintf(stderr, "Cannot accept connections!\n");
            close(simpleChildSocket);
            exit(1);
        }

        /* handle new connection requests */
        /* write our message to the client */
        write(simpleChildSocket, APRESSMESSAGE, strlen(APRESSMESSAGE));
        close(simpleChildSocket);
    }

    // closing the simpleSocket
    close(simpleSocket);
    return 0;
}
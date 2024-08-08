# Instructions for experimenting the UDP server and client

1. compile the UDP server code with: gcc UDP_server.c -o UDP_server
2. compile the UDP client code with: gcc UDP_client.c -o UDP_client
3. chose port number and execute the server code: ./UDP_server 8888
4. execute the client code by specifiying local IPv4 address: ./UDP_client 10.0.1.14 8888

NOTE: in the execution of UDP client code 10.0.1.14 part should be
replaced with your own computers local IPv4 address

NOTE: local IPv4 address of your network interface can be found by using
ifconfig or hostname -I command from terminal.  

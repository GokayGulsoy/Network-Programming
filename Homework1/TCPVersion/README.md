# Instructions for running the server and client

Compile the TCP server code as follows

```shell
gcc TCPServer.c -o TCPServer
```

then start the server for listening the default port 8888 which is set in TCPServer.c

```shell
./TCPSServer
```

Compile the TCP client code as follows

```shell
gcc TCPClient.c -o TCPClient
```

then request a particular file from server as follows

```shell
./TCPClient <your_local_ip_address> <filename_in_server> <transfered_file_name> 
```

Above command will transfer file to client with a name given by you

You can find your loop back IP address with `ifconfig command`

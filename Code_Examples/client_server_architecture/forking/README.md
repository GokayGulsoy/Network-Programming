# Execution instructions

compile the client code as follows

```shell
    gcc client.c -o client
```

compile the multiprocess server (forking)

```shell
    gcc forking.c -o forking
```

execute the multiprocess server

```shell
    ./forking
```

execute the client with specified number of clients

```shell
    ./client 5
```

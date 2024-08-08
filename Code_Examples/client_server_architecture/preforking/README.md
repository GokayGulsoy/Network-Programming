# Execution instructions

compile the client code as follows

```shell
    gcc client.c -o client
```

compile the multiprocess server (preforking)

```shell
    gcc process_pool.c -o process_pool
```

execute the multiprocess server

```shell
    ./process_pool 3
```

execute the client with specified number of clients

```shell
    ./client 5
```

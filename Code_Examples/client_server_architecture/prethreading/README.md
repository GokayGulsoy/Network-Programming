# Execution instructions

compile the client code as follows

```shell
    gcc client.c -o client
```

compile the multithreaded server (thread pool)

```shell
    gcc thread_pool.c -o thread_pool
```

if above command does not work also try the following code which explicitly links to pthread library

```shell
    gcc thread_pool.c -lpthread -o thread_pool
```

execute the multithreaded server (thread pool) with given number of threads

```shell
    ./thread_pool 5
```

execute the client with given number of clients

```shell
    ./client 3
```

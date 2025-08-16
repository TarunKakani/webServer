# Simple TCP server in C

This project is a basic tcp server implemented from scratch in pure C, following [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).  
Currently, it only supports connections on `localhost`. Future updates will aim for public accessibility.

## Compilation

```sh
gcc server.c -o server
gcc client.c -o client
```

## Usage

1. **Start the server**  
    In one terminal:
    ```sh
    ./server
    ```

2. **Run the client**  
    In another terminal:
    ```sh
    ./client localhost
    ```

## Notes

- Only works on `localhost` for now.
- Designed for learning and experimentation.

## License

MIT License.

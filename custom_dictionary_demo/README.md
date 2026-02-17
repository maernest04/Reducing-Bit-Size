# Custom dictionary: string encoding over 32-bit medium

Strings are encoded with a 6-bit character set (a-z, A-Z). Up to 6 characters fit in two 32-bit words (5 chars in word0, 1 in word1). Sender prints `Sending "Pltog"` to stderr; receiver decodes the binary from stdin, prints `binary translated to "Pltog"`, then runs the matching test function (same pattern as the 3-bit demo).

## Build and run

```
make
./sender | ./receiver
```

Or:

```
./demo
```

```
make clean
```

## Files

- encoder.c, encoder.h — encode string to 2 x uint32_t, decode 2 x uint32_t to string
- send_main.c — for each command: print Sending "...", encode, write 8 bytes to stdout
- recv_main.c — read 8 bytes, decode, print binary translated to "...", call receiver_handle(str)
- receiver.c, receiver.h — test_* functions and receiver_handle(str) that calls the right test by string comparison
- demo.c — run sender and receiver with a pipe between them

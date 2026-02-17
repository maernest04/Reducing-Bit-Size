# 3-bit command demo

Send test commands from one program to another over a pipe. Commands are encoded as 3-bit values (0-7) instead of strings so only one byte is sent per command.

## How it works

- **Sender** (send_main.c + sender.c): Has the list of 8 command names (Pltog, Plstat, Plmode, Pltarg, Plkwlm, Plinit, Pltqcm, Plclmp). For each command it encodes the name to a number 0-7 and writes that single byte to stdout.
- **Receiver** (recv_main.c + receiver.c): Reads bytes from stdin. For each byte it uses a switch on the value (0-7) and runs the matching test function. No string comparison on the receiver side.
- **Medium**: A pipe connects the two. When you run `sender | receiver`, the sender's stdout is the receiver's stdin. The bytes go through the OS pipe.

The receiver never sees the original strings. It only sees the 3-bit codes and runs the right test by number.

## How to run

Build everything:

```
make
```

Run sender and receiver connected by a pipe (two options):

```
./sender | ./receiver
```

Or run the demo program, which does the same thing (fork, pipe, exec sender and receiver):

```
./demo
```

Clean build artifacts and executables:

```
make clean
```

## Files

- send_main.c, sender.c, sender.h — sender program (encode command to 0-7, write byte to stdout)
- recv_main.c, receiver.c, receiver.h — receiver program (read byte from stdin, switch on value, run test)
- demo.c — runs sender and receiver with a pipe between them
# Reducing-Bit-Size

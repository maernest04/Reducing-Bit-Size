# Reducing Memory Problem

Two ways to send test commands over a 32-bit-limited medium: 3-bit command codes (fixed set) or custom dictionary string encoding (arbitrary a-z, A-Z strings in 32-bit words).

## Version 1: three_bit_demo

Commands are one of 8 fixed strings. Each command is encoded as a 3-bit value (0-7) and sent as one byte. Receiver uses a switch on the value and runs the matching test. No string is sent; only the code.

### Compile and run

```
cd three_bit_demo
make
./sender | ./receiver
```

Or run both in one process:

```
./demo
```

Clean:

```
make clean
```

## Version 2: custom_dictionary_demo (custom dictionary)

Commands are sent as actual strings over the medium. Each string is encoded with 7 bits per character (ASCII 32-126, all printable keyboard characters) and packed into two 32-bit words (up to 8 characters per command). Sender prints `Sending "Pltog"` (etc.); receiver decodes the binary, prints `binary translated to "Pltog"`, then runs the matching test function (same test_* pattern as the 3-bit demo).

### Compile and run

```
cd custom_dictionary_demo
make
./sender | ./receiver
```

Or:

```
./demo
```

Clean:

```
make clean
```

## Summary

| Folder                  | Encoding              | Per command on wire      |
|-------------------------|-----------------------|---------------------------|
| three_bit_demo          | 3-bit code (0-7)      | 1 byte                    |
| custom_dictionary_demo  | 7-bit chars (ASCII 32-126), 2 words | 8 bytes (2 x 32-bit)     |

Both use a pipe between sender and receiver; replace the pipe with your real 32-bit medium when deploying.

# Huffman encoding demo

Encodes the 8 test commands with Huffman (variable-length codes from character frequencies). Comparison is printed: how many bits a fixed 7-bit-per-character encoding would use vs how many bits Huffman actually used.

- **Baseline (fixed):** 7 bits per character (e.g. "Pltog" = 5 * 7 = 35 bits).
- **Huffman:** Builds a tree from character frequencies across all 8 commands, assigns shorter codes to more frequent characters (e.g. 'l', 't', 'P' appear often), so total bits is less.

Sender prints per command and total, e.g.:
`Sending "Pltog"  fixed (7-bit): 35 bits  Huffman: 21 bits`
`---`
`Total: fixed (7-bit) would use 329 bits, Huffman actually used 205 bits`

Receiver decodes the bit stream and runs the same test functions.

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

## Protocol

Sender writes: 4 bytes (total bit count, uint32_t), then the Huffman-encoded bit stream. Receiver builds the same tree from the known 8 commands, reads total bits and payload, decodes one command at a time (until END symbol), then calls receiver_handle.

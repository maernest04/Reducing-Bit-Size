# Reducing Memory Problem

Four ways to send test commands over a 32-bit-limited medium: 3-bit command codes (fixed set), custom dictionary (7-bit ASCII 32-126), 5-bit suffix encoding, or Huffman (variable-length) with a fixed-vs-Huffman bit comparison.

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

## Version 3: five_bit_suffix_demo (5-bit, suffix only)

For a medium that only allows **5-bit encoding** (32 symbols: space, a-z, 0-4). All 8 test commands start with "Pl", so only the **suffix** is sent (tog, stat, mode, targ, kwlm, init, tqcm, clmp); the receiver prepends "Pl" after decoding. One 32-bit word per command (6 chars at 5 bits each).

### Compile and run

```
cd five_bit_suffix_demo
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

## Version 4: huffman_demo (Huffman encoding)

Encodes the 8 commands with Huffman (variable-length codes from character frequencies). Sender prints a **comparison** for each command and total: how many bits a fixed 7-bit-per-character encoding would use vs how many bits Huffman actually used (e.g. `Total: fixed (7-bit) would use 329 bits, Huffman actually used 205 bits`). Receiver decodes and runs the same test functions.

### Compile and run

```
cd huffman_demo
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

| Folder                  | Encoding              | Comparison / notes        |
|-------------------------|-----------------------|---------------------------|
| three_bit_demo          | 3-bit code (0-7)      | 1 byte per command        |
| custom_dictionary_demo  | 7-bit chars (ASCII 32-126), 2 words | 8 bytes per command       |
| five_bit_suffix_demo    | 5-bit chars, suffix only | 4 bytes per command     |
| huffman_demo            | Huffman variable-length | Prints fixed vs Huffman bits used |

All use a pipe between sender and receiver; replace the pipe with your real 32-bit medium when deploying.

## Performance Comparison Tool

Run all 4 encoding methods and compare their performance side-by-side:

```bash
make
make run-comparison
```

Or build and run manually:

```bash
make build-demos
make compare_all
./compare_all
```

The comparison tool will:
- Run all 4 demos automatically
- Measure execution time for each
- Calculate bytes per command and total bytes
- Display a comparison table with metrics
- Show compression ratios relative to the baseline
- Identify the most efficient and fastest methods

The tool captures stderr output from each demo and parses metrics including:
- Number of commands sent
- Total bytes transmitted
- Execution time
- Success/failure status
- Compression ratio vs baseline (Custom Dictionary)

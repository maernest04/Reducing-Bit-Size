CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Build all demos and the comparison tool
all: build-demos compare_all

# Build all demo executables
build-demos:
	@echo "Building three_bit_demo..."
	@cd three_bit_demo && $(MAKE) clean && $(MAKE)
	@echo "Building custom_dictionary_demo..."
	@cd custom_dictionary_demo && $(MAKE) clean && $(MAKE)
	@echo "Building five_bit_suffix_demo..."
	@cd five_bit_suffix_demo && $(MAKE) clean && $(MAKE)
	@echo "Building huffman_demo..."
	@cd huffman_demo && $(MAKE) clean && $(MAKE)

# Build the comparison tool
compare_all: compare_all.c
	$(CC) $(CFLAGS) -o compare_all compare_all.c

# Run the comparison
run-comparison: compare_all
	./compare_all

# Clean everything
clean:
	rm -f compare_all
	@cd three_bit_demo && $(MAKE) clean
	@cd custom_dictionary_demo && $(MAKE) clean
	@cd five_bit_suffix_demo && $(MAKE) clean
	@cd huffman_demo && $(MAKE) clean

.PHONY: all build-demos compare_all run-comparison clean

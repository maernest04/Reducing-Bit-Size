#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

#define MAX_OUTPUT_SIZE 8192
#define MAX_LINE_SIZE 256

typedef struct {
    const char *name;
    const char *path;
    int bytes_per_command;
    int total_bytes;
    double execution_time_ms;
    int commands_sent;
    char output[MAX_OUTPUT_SIZE];
    int success;
} DemoResult;

static double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

static int run_demo(const char *name, const char *demo_path, DemoResult *result) {
    pid_t pid;
    int pipefd[2];
    char buffer[MAX_OUTPUT_SIZE];
    size_t total_read = 0;
    double start_time, end_time;

    memset(result, 0, sizeof(DemoResult));
    result->name = name;
    result->path = demo_path;
    result->success = 0;

    if (pipe(pipefd) != 0) {
        fprintf(stderr, "Failed to create pipe for %s: %s\n", name, strerror(errno));
        return -1;
    }

    start_time = get_time_ms();

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Failed to fork for %s: %s\n", name, strerror(errno));
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0) {
        // Child: redirect stderr to pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        
        // Change to demo directory
        if (chdir(demo_path) != 0) {
            fprintf(stderr, "Failed to chdir to %s: %s\n", demo_path, strerror(errno));
            _exit(1);
        }
        
        // Run demo executable
        execl("./demo", "./demo", NULL);
        fprintf(stderr, "Failed to exec ./demo: %s\n", strerror(errno));
        _exit(1);
    }

    // Parent: read from pipe
    close(pipefd[1]);
    
    ssize_t n;
    while ((n = read(pipefd[0], buffer + total_read, MAX_OUTPUT_SIZE - total_read - 1)) > 0) {
        total_read += n;
        if (total_read >= MAX_OUTPUT_SIZE - 1) break;
    }
    buffer[total_read] = '\0';
    close(pipefd[0]);

    int status;
    waitpid(pid, &status, 0);
    end_time = get_time_ms();

    result->execution_time_ms = end_time - start_time;
    result->success = (WIFEXITED(status) && WEXITSTATUS(status) == 0);
    
    if (total_read > 0) {
        strncpy(result->output, buffer, MAX_OUTPUT_SIZE - 1);
        result->output[MAX_OUTPUT_SIZE - 1] = '\0';
    }

    return result->success ? 0 : -1;
}

static void parse_result(DemoResult *result) {
    char *line;
    char *saveptr;
    char output_copy[MAX_OUTPUT_SIZE];
    
    strncpy(output_copy, result->output, MAX_OUTPUT_SIZE - 1);
    output_copy[MAX_OUTPUT_SIZE - 1] = '\0';
    
    line = strtok_r(output_copy, "\n", &saveptr);
    result->commands_sent = 0;
    
    while (line != NULL) {
        // Count "Sending" lines to get number of commands
        if (strstr(line, "Sending") != NULL) {
            result->commands_sent++;
        }
        
        // Parse bytes based on demo type
        if (strcmp(result->name, "3-bit") == 0) {
            // 3-bit: 1 byte per command
            result->bytes_per_command = 1;
        } else if (strcmp(result->name, "Custom Dictionary") == 0) {
            // Custom dictionary: 8 bytes per command (2 words)
            result->bytes_per_command = 8;
        } else if (strcmp(result->name, "5-bit Suffix") == 0) {
            // 5-bit suffix: 4 bytes per command (1 word)
            result->bytes_per_command = 4;
        } else if (strcmp(result->name, "Huffman") == 0) {
            // Huffman: variable, parse from output
            if (strstr(line, "Total:") != NULL) {
                // Line format: "Total: fixed (7-bit) would use X bits, Huffman actually used Y bits"
                // We'll calculate bytes from bits
                // For now, estimate based on typical compression
                result->bytes_per_command = -1; // Variable, will calculate total
            }
        }
        
        line = strtok_r(NULL, "\n", &saveptr);
    }
    
    // Calculate total bytes
    if (result->bytes_per_command > 0) {
        result->total_bytes = result->bytes_per_command * result->commands_sent;
    } else if (strcmp(result->name, "Huffman") == 0) {
        // Parse Huffman total bits from output
        char *total_line = strstr(result->output, "Total:");
        if (total_line != NULL) {
            // Try to extract bits used
            char *bits_str = strstr(total_line, "actually used");
            if (bits_str != NULL) {
                unsigned long bits = 0;
                if (sscanf(bits_str, "actually used %lu bits", &bits) == 1) {
                    result->total_bytes = (int)((bits + 7) / 8); // Round up to bytes
                }
            }
        }
        if (result->total_bytes == 0) {
            // Fallback: estimate ~3 bytes per command average for Huffman
            result->total_bytes = result->commands_sent * 3;
        }
    }
}

static void print_comparison(DemoResult *results, int count) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    PERFORMANCE COMPARISON REPORT                         ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Header
    printf("%-20s | %8s | %10s | %12s | %8s\n", 
           "Encoding Method", "Commands", "Total Bytes", "Time (ms)", "Status");
    printf("────────────────────┼──────────┼────────────┼──────────────┼────────\n");
    
    // Results
    for (int i = 0; i < count; i++) {
        const char *status = results[i].success ? "✓ PASS" : "✗ FAIL";
        if (results[i].bytes_per_command == -1) {
            printf("%-20s | %8d | %10d | %12.2f | %s\n",
                   results[i].name,
                   results[i].commands_sent,
                   results[i].total_bytes,
                   results[i].execution_time_ms,
                   status);
        } else {
            printf("%-20s | %8d | %10d | %12.2f | %s\n",
                   results[i].name,
                   results[i].commands_sent,
                   results[i].total_bytes,
                   results[i].execution_time_ms,
                   status);
        }
    }
    
    printf("\n");
    printf("════════════════════════════════════════════════════════════════════════════\n");
    printf("\n");
    
    // Find best performers
    int best_bytes_idx = -1;
    int best_time_idx = -1;
    int min_bytes = INT_MAX;
    double min_time = 1e9;
    
    for (int i = 0; i < count; i++) {
        if (results[i].success && results[i].total_bytes > 0) {
            if (results[i].total_bytes < min_bytes) {
                min_bytes = results[i].total_bytes;
                best_bytes_idx = i;
            }
            if (results[i].execution_time_ms < min_time) {
                min_time = results[i].execution_time_ms;
                best_time_idx = i;
            }
        }
    }
    
    if (best_bytes_idx >= 0) {
        printf("🏆 Most Efficient (Smallest Size): %s (%d bytes)\n", 
               results[best_bytes_idx].name, results[best_bytes_idx].total_bytes);
    }
    
    if (best_time_idx >= 0) {
        printf("⚡ Fastest Execution: %s (%.2f ms)\n", 
               results[best_time_idx].name, results[best_time_idx].execution_time_ms);
    }
    
    printf("\n");
    
    // Bytes per command comparison
    printf("Bytes per Command:\n");
    printf("──────────────────\n");
    for (int i = 0; i < count; i++) {
        if (results[i].success) {
            if (results[i].bytes_per_command > 0) {
                printf("  %-18s: %d bytes/command\n", 
                       results[i].name, results[i].bytes_per_command);
            } else {
                printf("  %-18s: ~%.1f bytes/command (variable-length)\n", 
                       results[i].name, 
                       (double)results[i].total_bytes / results[i].commands_sent);
            }
        }
    }
    
    printf("\n");
    
    // Compression ratio (relative to custom dictionary as baseline)
    printf("Compression Ratio (vs Custom Dictionary baseline):\n");
    printf("───────────────────────────────────────────────────\n");
    int baseline_bytes = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(results[i].name, "Custom Dictionary") == 0 && results[i].success) {
            baseline_bytes = results[i].total_bytes;
            break;
        }
    }
    
    if (baseline_bytes > 0) {
        for (int i = 0; i < count; i++) {
            if (results[i].success && results[i].total_bytes > 0) {
                double ratio = (double)results[i].total_bytes / baseline_bytes;
                double savings = (1.0 - ratio) * 100.0;
                printf("  %-18s: %.1f%% of baseline", results[i].name, ratio * 100.0);
                if (savings > 0) {
                    printf(" (%.1f%% smaller)", savings);
                } else if (savings < 0) {
                    printf(" (%.1f%% larger)", -savings);
                }
                printf("\n");
            }
        }
    }
    
    printf("\n");
}

static void print_detailed_output(DemoResult *results, int count) {
    printf("\n");
    printf("════════════════════════════════════════════════════════════════════════════\n");
    printf("                          DETAILED OUTPUT LOGS                             \n");
    printf("════════════════════════════════════════════════════════════════════════════\n");
    printf("\n");
    
    for (int i = 0; i < count; i++) {
        printf("────────────────────────────────────────────────────────────────────────────\n");
        printf("%s Output:\n", results[i].name);
        printf("────────────────────────────────────────────────────────────────────────────\n");
        if (strlen(results[i].output) > 0) {
            printf("%s", results[i].output);
        } else {
            printf("(No output captured)\n");
        }
        printf("\n");
    }
}

int main(void) {
    DemoResult results[4];
    int count = 0;
    const char *cwd = getcwd(NULL, 0);
    
    printf("Running all encoding method comparisons...\n");
    printf("Current directory: %s\n\n", cwd ? cwd : "(unknown)");
    free(cwd);
    
    // Run each demo
    printf("[1/4] Running 3-bit demo...\n");
    if (run_demo("3-bit", "three_bit_demo", &results[count]) == 0) {
        parse_result(&results[count]);
        count++;
    } else {
        fprintf(stderr, "Warning: 3-bit demo failed\n");
    }
    
    printf("[2/4] Running Custom Dictionary demo...\n");
    if (run_demo("Custom Dictionary", "custom_dictionary_demo", &results[count]) == 0) {
        parse_result(&results[count]);
        count++;
    } else {
        fprintf(stderr, "Warning: Custom Dictionary demo failed\n");
    }
    
    printf("[3/4] Running 5-bit Suffix demo...\n");
    if (run_demo("5-bit Suffix", "five_bit_suffix_demo", &results[count]) == 0) {
        parse_result(&results[count]);
        count++;
    } else {
        fprintf(stderr, "Warning: 5-bit Suffix demo failed\n");
    }
    
    printf("[4/4] Running Huffman demo...\n");
    if (run_demo("Huffman", "huffman_demo", &results[count]) == 0) {
        parse_result(&results[count]);
        count++;
    } else {
        fprintf(stderr, "Warning: Huffman demo failed\n");
    }
    
    // Print comparison
    print_comparison(results, count);
    
    // Print detailed output if requested (uncomment to see full logs)
    // print_detailed_output(results, count);
    
    return (count == 4) ? 0 : 1;
}

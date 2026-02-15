#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    // Create a pipe to send the 3-bit code to the receiver
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return 1;
    }

    // Create a sender process
    pid_t pid_sender = fork();
    if (pid_sender < 0) {
        perror("fork");
        return 1;
    }

    // If the sender process is created, close the read end of the pipe and redirect the write end to stdout
    if (pid_sender == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("./sender", "sender", NULL);
        perror("execl sender");
        _exit(1);
    }

    // Create a receiver process
    pid_t pid_receiver = fork();
    if (pid_receiver < 0) {
        perror("fork");
        return 1;
    }

    // If the receiver process is created, close the write end of the pipe and redirect the read end to stdin
    if (pid_receiver == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execl("./receiver", "receiver", NULL);
        perror("execl receiver");
        _exit(1);
    }

    // Close the pipe
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid_sender, NULL, 0);
    waitpid(pid_receiver, NULL, 0);
    return 0;
}

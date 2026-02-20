#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return 1;
    }

    pid_t pid_sender = fork();
    if (pid_sender < 0) {
        perror("fork");
        return 1;
    }
    if (pid_sender == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("./sender", "sender", NULL);
        perror("execl sender");
        _exit(1);
    }

    pid_t pid_receiver = fork();
    if (pid_receiver < 0) {
        perror("fork");
        return 1;
    }
    if (pid_receiver == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execl("./receiver", "receiver", NULL);
        perror("execl receiver");
        _exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid_sender, NULL, 0);
    waitpid(pid_receiver, NULL, 0);
    return 0;
}

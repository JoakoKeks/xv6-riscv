#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_MESSAGES 5

void writer() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Message %d from PID %d", i, getpid());
        if (send(getpid(), msg) < 0) {
            printf("Failed to send message\n");
        }
    }
    exit(0);
}

void reader() {
    char buffer[128];
    for (int i = 0; i < NUM_MESSAGES; i++) {
        int sender = receive(buffer);
        if (sender >= 0) {
            printf("Received from PID %d: %s\n", sender, buffer);
        } else {
            printf("Failed to receive message\n");
        }
    }
    exit(0);
}

int main() {
    if (fork() == 0) {
        writer();
    } else {
        reader();
    }
    wait(0);
    wait(0);
    exit(0);
}

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>

void printChar(char string[], int curr, int length) {
    if(curr < length) {
        int pid = fork();
        if(pid != 0) {
            printf("Character: \"%c\" PID: %d\n", string[curr], pid);
            sleep((rand() % 4) + 1);
            printChar(string, curr+1, length);
        }
    }
    return;
}

int main() {
    printChar("Hello World", 0, 11);
    return 0;
}
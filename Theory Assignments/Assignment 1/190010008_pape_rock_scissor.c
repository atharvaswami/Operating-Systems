#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void exitChildchildC (int sig) {
    printf("\nChild C Exited\n");
    exit(0);
}

void exitChildchildD (int sig) {
    printf("Child D Exited\n");
    exit(0);
}

int main() {
    int fds1[2]; 
    int fds2[2];

    if (pipe(fds1) == -1) {
        fprintf(stderr, "Pipe 1 Failed");
        return 1;
    }

    if (pipe(fds2) == -1) {
        fprintf(stderr, "Pipe 2 Failed");
        return 1;
    }

    pid_t childC, childD;
    childC = fork();
 
    if (childC < 0) {
        fprintf(stderr, "Fork 1 Failed");
        return 1;
    }

    // Child Process C
    else if (childC == 0) {
        kill(getpid(), SIGSTOP);
        srand(time(NULL)^ getpid());
        close(fds1[0]);
        int choice1;

        while (1) {
            signal(SIGUSR2, exitChildchildC);
            choice1 = (rand() % 3) + 1;
            printf("Child C Plays: %d\n",choice1);
            write(fds1[1], &choice1, sizeof(choice1));
            kill(getpid(), SIGSTOP);
        }

        close(fds1[1]);
        return 0;
    }

    else {
        childD = fork();

        if (childD < 0) {
            fprintf(stderr, "Fork 2 Failed");
            return 1;
        }

        // Child Process D
        else if (childD == 0) {
            srand(time(NULL)^ getpid());
            close(fds2[0]);
            int choice2;
            
            while (1) {
                signal(SIGUSR2, exitChildchildD);
                choice2 = (rand() % 3) + 1;
                printf("Child D Plays: %d\n",choice2);
                write(fds2[1], &choice2, sizeof(choice2));
                kill(getpid(), SIGSTOP);
            }

            close(fds2[1]);
            return 0;
        }

        // Parent Process
        else {
            close(fds1[1]);
            close(fds2[1]);

            int childCchoice, childDchoice;
            float childCscore = 0, childDscore = 0;

            while (1) {
                // ready signals for the child processes
                kill(childC, SIGCONT);
                kill(childD, SIGCONT);

                read(fds1[0], &childCchoice, sizeof(childCchoice));
                read(fds2[0], &childDchoice, sizeof(childDchoice));

                if (childCchoice == 1 && childDchoice == 2) {
                    childDscore += 1;
                }
                else if (childCchoice == 2 && childDchoice == 1) {
                    childCscore += 1;
                }
                else if (childCchoice == 1 && childDchoice == 3) {
                    childCscore += 1;
                }
                else if (childCchoice == 3 && childDchoice == 1) {
                    childDscore += 1;
                }
                else if (childCchoice == 2 && childDchoice == 3) {
                    childDscore += 1;
                }
                else if (childCchoice == 3 && childDchoice == 2) {
                    childCscore += 1;
                }
                else {
                    childCscore += 0.5;
                    childDscore += 0.5;
                }

                printf("Child C Score: %.1f, Child D Score: %.1f\n\n", childCscore, childDscore);

                if (childCscore > 10.0 || childDscore > 10.0) 
                    break;
            }

            if(childCscore == childDscore) {
                printf("There was a tie!\nSo generated 2 Random Numbers =>\n");

                srand(time(NULL)^ getpid());
                int num1 = rand();
                printf("Random Number 1: %d\n", num1);

                int num2 = rand();
                printf("Random Number 2: %d\n", num2);

                if(num1 > num2) {
                    printf("Number 1 > Number 2\nChild C Won!\n");
                } else {
                    printf("Number 1 < Number 2\nChild D Won!\n");
                }
            } 
            else if (childCscore > 10.0) {
                printf("Child C Won!\n");
            } 
            else {
                printf("Child D Won!\n");
            }

            close(fds1[0]);
            close(fds2[0]);

            int exit_status1, exit_status2;

            kill(childC, SIGUSR2);
            waitpid(childC, &exit_status1, 0);

            kill(childD, SIGUSR2);
            waitpid(childD, &exit_status2, 0);

            if (exit_status1 == 0 && exit_status2 == 0) {
                printf("Parent P Exited\n");
                exit(0);
            }
        }
    }
	
	return 0;
} 
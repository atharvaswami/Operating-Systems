#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

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

    pid_t player1, player2;
    player1 = fork();
 
    if (player1 < 0) {
        fprintf(stderr, "Fork 1 Failed");
        return 1;
    }

    // Child Process 1
    else if (player1 == 0) {
        srand(time(NULL)^ getpid());
        close(fds1[0]);
        int choice1;

        while (1) {
            choice1 = (rand() % 3) + 1;
            printf("Player 1 Chooses: %d\n",choice1);
            write(fds1[1], &choice1, sizeof(choice1));
            kill(getpid(), SIGSTOP);
        }

        close(fds1[1]);
        return 0;
    }

    else {
        player2 = fork();

        if (player2 < 0) {
            fprintf(stderr, "Fork 2 Failed");
            return 1;
        }

        // Child Process 2
        else if (player2 == 0) {
            srand(time(NULL)^ getpid());
            close(fds2[0]);
            int choice2;
            
            while (1) {
                choice2 = (rand() % 3) + 1;
                printf("Player 2 Chooses: %d\n",choice2);
                write(fds2[1], &choice2, sizeof(choice2));
                kill(getpid(), SIGSTOP);
            }

            close(fds2[1]);
            return 0;
        }

        // Parent Process
        else {
            // kill(player1, SIGSTOP);
            // kill(player2, SIGSTOP);

            close(fds1[1]);
            close(fds2[1]);

            // int returnStatus1, returnStatus2;
            // waitpid(player1, &returnStatus1, 0);
            // waitpid(player2, &returnStatus2, 0);

            int player1choice, player2choice;
            float player1score = 0, player2score = 0;

            while (player1score < 10.0 && player2score < 10.0) {
                kill(player2, SIGCONT);

                read(fds1[0], &player1choice, sizeof(player1choice));
                // printf("Player 1 Chooses %d\n", player1choice);

                read(fds2[0], &player2choice, sizeof(player2choice));
                // printf("Player 2 Chooses %d\n", player2choice);

                if (player1choice == 1 && player2choice == 2) {
                    player2score += 1;
                }
                else if (player1choice == 2 && player2choice == 1) {
                    player1score += 1;
                }
                else if (player1choice == 1 && player2choice == 3) {
                    player1score += 1;
                }
                else if (player1choice == 3 && player2choice == 1) {
                    player2score += 1;
                }
                else if (player1choice == 2 && player2choice == 3) {
                    player2score += 1;
                }
                else if (player1choice == 3 && player2choice == 2) {
                    player1score += 1;
                }
                else {
                    player1score += 0.5;
                    player2score += 0.5;
                }
                printf("Player 1 Score: %.1f, Player 2 Score: %.1f\n\n", player1score, player2score);

                if (player1score > 10.0 || player2score > 10.0) 
                    break;

                kill(player1, SIGCONT);
            }

            if(player1score >= 10.0 && player2score >= 10.0) {
                printf("There was a tie! So generated 2 Random Numbers =>\n");

                srand(time(NULL)^ getpid());
                int num1 = rand();
                printf("Random Number 1: %d\n", num1);

                int num2 = rand();
                printf("Random Number 2: %d\n", num2);

                if(num1 > num2) {
                    printf("Player 1 Won!\n");
                } else {
                    printf("Player 2 Won!\n");
                }
            } 
            else if (player1score >= 10.0) {
                printf("Player 1 Won!\n");
            } 
            else {
                printf("Player 2 Won!\n");
            }

            close(fds1[0]);
            close(fds2[0]);

            kill(player1, SIGKILL);
            kill(player2, SIGKILL);
        }
    }
	
	return 0;
}


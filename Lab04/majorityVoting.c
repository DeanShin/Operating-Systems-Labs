#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

int main() {
    int numVoters;
    // Loop until we receive a valid number of voters
    while(1) {
        printf("Input how many voters you want: ");
        scanf("%d", &numVoters);
        printf("\n");

        if(numVoters >= 3) {
            break;
        } 

        printf("Please input >= 3 voters.\n");
    }

    int numRounds;
    // Loop until we receive a valid number of rounds
    while(1) {
        printf("Input the number of rounds: ");
        scanf("%d", &numRounds);
        printf("\n");

        if(numRounds >= 1) {
            break;
        }

        printf("Please input >= 1 rounds.\n");
    }

    // Allocate enough memory to store all of the parent <==> child pipes.
    int** pipes;
    pipes = (int**) malloc(sizeof(int*) * numVoters);
    for(int i = 0; i < numVoters; i++) {
        pipes[i] = (int*) malloc(sizeof(int) * 2);
    }

    // Default to -1 to represent parent process.
    int voterid = -1;
    for(int i = 0; i < numVoters; i++) {
        pipe(pipes[i]);
        int childpid = fork();
        if(childpid == 0) {
            // Child process
            voterid = i;
            // The child process only writes to its pipe.
            close(pipes[i][0]);
            // Seed the random generator
            srand(time(NULL) + voterid);
            break;
        } else {
            // Parent process
            // The parent process only reads from each pipe.
            close(pipes[i][1]);
        }
    }

    // Loop through every round.
    for(int roundIdx = 1; roundIdx <= numRounds; roundIdx++) {
        if(voterid == -1) {
            // Parent process
            printf("---------- Round %d ----------\n", roundIdx);

            // Tabulate the votes in the current round.
            int yesVotes = 0;
            int noVotes = 0;
            for(int i = 0; i < numVoters; i++) {
                int vote; 
                // Receive the vote from the ith child process.
                read(pipes[i][0], &vote, sizeof(vote));
                printf("Round %d: Voter %d votes: ", roundIdx, i);
                if(vote == 0) {
                    noVotes++;
                    printf("NO");
                } else {
                    yesVotes++;
                    printf("YES");
                }
                printf("\n");
            }

            // Print out the results of the round.
            printf("----- Round %d: The voting result is a ", roundIdx);
            if(yesVotes > noVotes) {
                printf("YES");
            } else if (yesVotes == noVotes) {
                printf("TIE");
            } else {
                printf("NO");
            }
            printf("\n\n");
        } else {
            // Child process
            // Randomly generate a vote. vote == 0 => no vote, vote == 1 => yes vote.
            int vote = rand() % 2;
            // Send the vote to the parent process.
            write(pipes[voterid][1], &vote, sizeof(vote));
        }
    }

    // Clean-up dynamically allocated memory.
    for(int i = 0; i < numVoters; i++) {
        free(pipes[i]);
    }
    free(pipes);

    if(voterid == -1) {
        int status = 0;
        // Wait for the child processes to terminate before ending the program.
        while(wait(&status) > 0);
        return 0;
    } else {
        exit(0);
    }
}
/*
2026FA-ENGR-2304-3-AZAK-ISMAIL-P07
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define SIZE 9

void pauseGame(void) {
#ifdef _WIN32
    Sleep(800);             // Pause for 800 milliseconds
#else
    usleep(800000);
#endif
}

void initializeBoard(char board[]) {
    for (int i = 0; i < SIZE; i++) {
        board[i] = '1' + i;
    }
}

void displayBoard(char board[]) {
    printf("\n");
    printf(" %c | %c | %c \n", board[0], board[1], board[2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[3], board[4], board[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[6], board[7], board[8]);
    printf("\n");
}

int checkWin(char board[], char symbol) {
    // Rows
    for (int i = 0; i < SIZE; i += 3) {
        if (board[i] == symbol &&
            board[i + 1] == symbol &&
            board[i + 2] == symbol) {
            return 1;
        }
    }

    // Columns
    for (int i = 0; i < 3; i++) {
        if (board[i] == symbol &&
            board[i + 3] == symbol &&
            board[i + 6] == symbol) {
            return 1;
        }
    }

    // Diagonals
    if (board[0] == symbol &&
        board[4] == symbol &&
        board[8] == symbol) {
        return 1;
    }

    if (board[2] == symbol &&
        board[4] == symbol &&
        board[6] == symbol) {
        return 1;
    }

    return 0;
}

int boardFull(char board[]) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i] >= '1' && board[i] <= '9') {
            return 0;
        }
    }

    return 1;
}

int getMove(char board[], const char *player) {
    int position;
    int result;

    while (1) {
        printf("%s, select a square (1-9): ", player);
        result = scanf("%d", &position);

        if (result == EOF) {
            exit(0);
        }

        if (result != 1) {
            printf("Invalid input. Enter a number from 1 to 9.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                // Clear invalid input
            }

            continue;
        }

        if (position < 1 || position > 9) {
            printf("Choose a number between 1 and 9.\n");
            continue;
        }

        if (board[position - 1] == 'X' ||
            board[position - 1] == 'O') {
            printf("That square is occupied. Try again.\n");
            continue;
        }

        return position - 1;
    }
}

int findWinningMove(char board[], char symbol) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i] >= '1' && board[i] <= '9') {
            char original = board[i];
            board[i] = symbol;

            if (checkWin(board, symbol)) {
                board[i] = original;
                return i;
            }

            board[i] = original;
        }
    }

    return -1;
}

int computerMove(char board[], char computer, char human) {
    int position;

    // Win if possible.
    position = findWinningMove(board, computer);
    if (position != -1) {
        return position;
    }

    // Block the human player's winning move.
    position = findWinningMove(board, human);
    if (position != -1) {
        return position;
    }

    // Take the center.
    if (board[4] == '5') {
        return 4;
    }

    // Take a corner.
    int corners[] = {0, 2, 6, 8};
    int availableCorners[4];
    int count = 0;

    for (int i = 0; i < 4; i++) {
        if (board[corners[i]] >= '1' &&
            board[corners[i]] <= '9') {
            availableCorners[count++] = corners[i];
        }
    }

    if (count > 0) {
        return availableCorners[rand() % count];
    }

    // Take any remaining square.
    for (int i = 0; i < SIZE; i++) {
        if (board[i] >= '1' && board[i] <= '9') {
            return i;
        }
    }

    return -1;
}

void playGame(void) {
    char board[SIZE];
    char human, computer;
    int mode;
    int turn;
    int position;

    initializeBoard(board);

    printf("\n=== TIC-TAC-TOE ===\n");
    printf("1. Play against the computer\n");
    printf("2. Two-player mode\n");

    while (1) {
        printf("Select game mode (1 or 2): ");

        if (scanf("%d", &mode) != 1) {
            if (feof(stdin)) {
                exit(0);
            }

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                // Clear invalid input
            }

            printf("Invalid input. Enter 1 or 2.\n");
            continue;
        }

        if (mode == 1 || mode == 2) {
            break;
        }

        printf("Choose 1 or 2.\n");
    }

    if (mode == 1) {
        char choice;

        while (1) {
            printf("Choose your symbol (X or O): ");

            if (scanf(" %c", &choice) != 1) {
                exit(0);
            }

            choice = (char)toupper((unsigned char)choice);

            if (choice == 'X' || choice == 'O') {
                human = choice;
                computer = (human == 'X') ? 'O' : 'X';
                break;
            }

            printf("Invalid choice. Enter X or O.\n");
        }
    } else {
        human = 'X';
        computer = 'O';
        printf("Player 1 is X. Player 2 is O.\n");
    }

    // Randomly select the first symbol.
    turn = rand() % 2;

    printf("\nRandomly selected first player: %c\n",
           turn == 0 ? 'X' : 'O');

    displayBoard(board);

    while (1) {
        char current = (turn == 0) ? 'X' : 'O';
        int isComputer = (mode == 1 && current == computer);

        if (isComputer) {
            printf("Computer (%c) is thinking...\n", computer);
            pauseGame();

            position = computerMove(board, computer, human);

            if (position == -1) {
                break;
            }

            board[position] = computer;
        } else {
            const char *playerName;

            if (mode == 2) {
                playerName = (current == 'X')
                             ? "Player 1 (X)"
                             : "Player 2 (O)";
            } else {
                playerName = "You";
            }

            position = getMove(board, playerName);
            board[position] = current;
        }

        displayBoard(board);

        if (checkWin(board, current)) {
            if (isComputer) {
                printf("Computer (%c) wins!\n", computer);
            } else if (mode == 1) {
                printf("You win!\n");
            } else {
                printf("Player %c wins!\n", current);
            }

            return;
        }

        if (boardFull(board)) {
            printf("The game is a draw!\n");
            return;
        }

        // Switch between X and O.
        turn = 1 - turn;
    }
}

int main(void) {
    char replay;

    srand((unsigned int)time(NULL));

    do {
        playGame();

        while (1) {
            printf("\nPlay again? (Y/N): ");

            if (scanf(" %c", &replay) != 1) {
                return 0;
            }

            replay = (char)toupper((unsigned char)replay);

            if (replay == 'Y' || replay == 'N') {
                break;
            }

            printf("Enter Y or N.\n");
        }

    } while (replay == 'Y');

    printf("Thanks for playing!\n");

    return 0;
}

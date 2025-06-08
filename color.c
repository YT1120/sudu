#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ref/rlutil.h"

#define SIZE 9

// Function declarations
void printBoard();
bool isValidForGrid(int grid[SIZE][SIZE], int row, int col, int num);
bool solveSudoku(int grid[SIZE][SIZE]);
bool isBoardFull();
bool makeMove(int row, int col, int num);

// Global Sudoku board
int board[SIZE][SIZE] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

// Solution board
int solution[SIZE][SIZE];

// Original board for checking given numbers
int originalBoard[SIZE][SIZE];

// Cursor position (0-based)
int cursorRow = 0;
int cursorCol = 0;

void printBoard() {
    cls();
    printf("    ");
    for (int i = 0; i < SIZE; i++) {
        printf(" %d", i + 1);
        if ((i + 1) % 3 == 0 && i != SIZE - 1) printf("  ");
    }
    printf("\n");
    printf("   +-------+-------+-------+\n");
    for (int i = 0; i < SIZE; i++) {
        printf(" %d |", i + 1);
        for (int j = 0; j < SIZE; j++) {
            int isCursor = (i == cursorRow && j == cursorCol);
            if (isCursor) {
                setBackgroundColor(YELLOW);
                setColor(BLACK);
            } else if (originalBoard[i][j] != 0) {
                setBackgroundColor(BLACK);
                setColor(WHITE); // Original numbers: white
            } else if (board[i][j] == 0) {
                setBackgroundColor(BLACK);
                setColor(GREY); // Empty: grey
            } else if (board[i][j] == solution[i][j]) {
                setBackgroundColor(BLACK);
                setColor(GREEN); // Correct: green
            } else {
                setBackgroundColor(BLACK);
                setColor(RED); // Incorrect: red
            }
            if (board[i][j] == 0)
                printf(" .");
            else
                printf(" %d", board[i][j]);
            resetColor();
            if ((j + 1) % 3 == 0)
                printf(" |");
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("   +-------+-------+-------+\n");
    }
}

bool isValidForGrid(int grid[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;
    return true;
}

bool solveSudoku(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValidForGrid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid))
                            return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool isBoardFull() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
                return false;
    return true;
}

bool makeMove(int row, int col, int num) {
    row--; col--;
    if (originalBoard[row][col] != 0) return false;
    if (!isValidForGrid(board, row, col, num)) return false;
    board[row][col] = num;
    return true;
}

int main() {
    cls();
    printf("=== SUDOKU GAME ===\n");
    printf("Calculating solution...\n");

    // Copy the original board to solution and originalBoard
    memcpy(solution, board, sizeof(board));
    memcpy(originalBoard, board, sizeof(board));
    if (!solveSudoku(solution)) {
        printf("Error: Unable to solve this Sudoku puzzle!\n");
        return 1;
    }

    printf("Solution calculated! Game ready.\n");
    printf("Use arrow keys to navigate, 1-9 to fill numbers, ESC to quit.\n");
    printf("Press any key to start...\n");
    getch();

    cls();
    printBoard();
    printf("Position: Row %d, Col %d\n", cursorRow + 1, cursorCol + 1);
    printf("Use arrow keys to move, 1-9 to enter number, ESC to quit\n");

    while (true) {
        if (isBoardFull()) {
            gotoxy(1, 17);
            printf("Congratulations! You solved the Sudoku!\n");
            printf("Press any key to exit...\n");
            getch();
            break;
        }

        if (kbhit()) {
            int key = getch();

            if (key == 0 || key == 224) { // Special key prefix
                key = getch();
                int oldRow = cursorRow, oldCol = cursorCol;
                switch (key) {
                    case 72: // Up
                        if (cursorRow > 0) cursorRow--;
                        break;
                    case 80: // Down
                        if (cursorRow < SIZE - 1) cursorRow++;
                        break;
                    case 75: // Left
                        if (cursorCol > 0) cursorCol--;
                        break;
                    case 77: // Right
                        if (cursorCol < SIZE - 1) cursorCol++;
                        break;
                }
                if (oldRow != cursorRow || oldCol != cursorCol) {
                    printBoard();
                    gotoxy(1, 17);
                    printf("Position: Row %d, Col %d", cursorRow + 1, cursorCol + 1);
                }
            }
            else if (key >= '1' && key <= '9') {
                int num = key - '0';
                if (originalBoard[cursorRow][cursorCol] == 0) {
                    board[cursorRow][cursorCol] = num;
                    printBoard();
                    gotoxy(1, 17);
                    printf("Position: Row %d, Col %d", cursorRow + 1, cursorCol + 1);
                }
            }
            else if (key == ' ' || key == KEY_DELETE) {
                if (originalBoard[cursorRow][cursorCol] == 0) {
                    board[cursorRow][cursorCol] = 0;
                    printBoard();
                    gotoxy(1, 17);
                    printf("Position: Row %d, Col %d", cursorRow + 1, cursorCol + 1);
                }
            }
            else if (key == 27) { // ESC
                gotoxy(1, 17);
                printf("Thanks for playing!\n");
                break;
            }
        }
    }

    return 0;
}

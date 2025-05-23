#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "queue.h"
#include "vector.h"

#define MAX_ROWS 100
#define MAX_COLS 100

using namespace std;

typedef struct {
    int row;
    int col;
} Position;

int main(int argc, char *argv[]) {
    char maze[MAX_ROWS][MAX_COLS];
    int rows = 0, cols = 0;
    Position start = {-1, -1};
    int distances[MAX_ROWS][MAX_COLS];
    const char* filename = "input.txt";

    if (argc > 1) {
        filename = argv[1];
    }    
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error opening input file: " << filename << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line) && rows < MAX_ROWS) {
        size_t len = min(line.length(), (size_t)MAX_COLS - 1);
        line.copy(maze[rows], len, 0); 
        maze[rows][len] = '\0'; 
        rows++;

        for (size_t j = 0; j < len; j++) {
            if (maze[rows - 1][j] == 'X') {
                start.row = rows - 1;
                start.col = j;
            }
        }
    }

    inputFile.close();

    if (start.row == -1) {
        cerr << "Error: Starting position 'X' not found in maze." << endl;
        return 1;
    }
    if (rows > 0) {
        cols = (int)strlen(maze[0]);
    } else {
        cerr << "Error: Maze is empty." << endl;
        return 1;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            distances[i][j] = -1; 
        }
    }

    Queue *queue = queue_create();
    queue_insert(queue, (start.row * cols) + start.col);
    distances[start.row][start.col] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!queue_empty(queue)) {
        int current_pos = queue_get(queue);
        queue_remove(queue);
        int current_row = current_pos / cols;
        int current_col = current_pos % cols;

        for (int i = 0; i < 4; i++) {
            int new_row = current_row + dr[i];
            int new_col = current_col + dc[i];

            if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols &&
                maze[new_row][new_col] != '#' && distances[new_row][new_col] == -1) {
                distances[new_row][new_col] = distances[current_row][current_col] + 1;
                queue_insert(queue, (new_row * cols) + new_col);
            }
        }
    }

    int min_distance = -1;
    char closest_digit = '\0';

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] >= '0' && maze[i][j] <= '9' && distances[i][j] != -1) {
                if (min_distance == -1 || distances[i][j] < min_distance) {
                    min_distance = distances[i][j];
                    closest_digit = maze[i][j];
                }
            }
        }
    }
    if (closest_digit != '\0') {
        cout << closest_digit << endl;
    } else {
        cout << "No reachable digit found." << endl;
    }

    queue_delete(queue);
    return 0;
}

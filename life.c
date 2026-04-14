#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h> // dla Sleep w win
#else
#include <unistd.h> // dla usleep w lnx / mac
#endif

#define WIDTH 70
#define HEIGHT 50

int grid[HEIGHT][WIDTH];
int newGrid[HEIGHT][WIDTH];

void init() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = rand() % 2;
        }
    }
}

// liczy sasiadow
int countNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // pomija sama siebie

            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH) { // zeby nie wyjsc poza tablice
                count += grid[nx][ny]; // dodaje do zywych komorek
            }
        }
    }
    return count; // liczba zywych sasiadow
}

// robi update planszy
void update() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = countNeighbors(i, j);

            if (grid[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) // umiera jak za malo (<2) albo za duzo sasiadow (>3)
                    newGrid[i][j] = 0;
                else
                    newGrid[i][j] = 1;
            } else {
                if (neighbors == 3) // jak jest 3 sasiadow wokol martwej komorki to sie ozywia
                    newGrid[i][j] = 1;
                else
                    newGrid[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = newGrid[i][j]; // update planszy
        }
    }
}

/*
 * czysci ekran
 * windows -> cls
 * lnx / mac -> clear
*/
void draw() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf(grid[i][j] ? "#" : ".");
        }
        printf("\n");
    }
}

void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

int main() {
    srand(time(NULL)); // aktualny czas w sekundach
    init();

    while (1) {
        draw();
        update();
        sleep_ms(100);
    }

    return 0;
}
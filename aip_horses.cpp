#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<locale.h>
#include <time.h>
#include <windows.h>

#define MAXN 15

int n;
int board[MAXN][MAXN];     // 0 - пусто, 1 - конь
int attacked[MAXN][MAXN];  // сколько коней бьют клетку
int best;                  // мин число коней
int knights;               // сколько коней

// Ходы коня
int dx[8] = { 2, 2, -2, -2, 1, -1, 1, -1 };
int dy[8] = { 1, -1, 1, -1, 2, 2, -2, -2 };

// тупо в доске или нет
int inside(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < n;
}

void set_knight(int x, int y, int put) {
    // обновляем текущее число коней
    if (put)
        knights++;
    else
        knights--;

    board[x][y] = put; //0 или 1 для клетки

    // обновляем клетки которые бьёт конь
    for (int k = 0; k < 8; k++) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (inside(nx, ny)) {
            if (put)
                attacked[nx][ny] += 1;
            else
                attacked[nx][ny] -= 1;
        }
    }

    // сама клетка под конем считается под атакой 
    if (put)
        attacked[x][y] += 1;
    else
        attacked[x][y] -= 1;
}

int count_uncovered() {
    int c = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (attacked[i][j] == 0)
                c++;
    return c;
}

//просто чек что все бьется
int all_covered() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (attacked[i][j] == 0)
                return 0;
    return 1;
}

// найти первую непокрытую клетку (выбор варианта)
int find_uncovered(int* x, int* y) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (attacked[i][j] == 0) {
                *x = i;
                *y = j;
                return 1;
            }
    return 0;
}

// move(n) — основной рекурсивный поиск
void move() {
    int x, y;

    if (knights >= best) return; // отсечение заведомо неверных вариантов(если коней уже больше чем мин вариант > некст сразу)

    // нижняя граница
    int U = count_uncovered();
    int need = (U + 8) / 9;
    if (knights + need >= best) return;

    if (all_covered()) {
        // решение полное
        if (knights < best) best = knights;
        return;
    }

    // выбор варианта
    if (!find_uncovered(&x, &y)) return;

    int options[8][2];
    int m = 0;

    for (int k = 0; k < 8; k++) {
        int nx = x + dx[k], ny = y + dy[k];
        if (inside(nx, ny) && board[nx][ny] == 0) {
            options[m][0] = nx;
            options[m][1] = ny;
            m++;
        }
    }

    // цикл по всем вариантам
    for (int i = 0; i < m; i++) {
        int nx = options[i][0];
        int ny = options[i][1];

        if (board[nx][ny] == 0) {
            // запись варианта
            set_knight(nx, ny, 1);

            // рекурсивный вызов move
            move();

            // стирание варианта
            set_knight(nx, ny, 0);
        }
    }
}

// try()  инициализация и запуск 
void try_() {
    int success = 0;       // удача
    int no_options = 0;    // нет вариантов

    // инициализация
    knights = 0;
    best = n * n; //можно ли уменьшить бест заранее?


    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            board[i][j] = 0;
            attacked[i][j] = 0;
        }

    do {
        // выбор варианта
        move();

        // если найдено решение то удача
        if (best < n * n)
            success = 1;
        else
            no_options = 1;

    } while (!(success || no_options));
}

int main() {
    

    setlocale(LC_ALL, "");
    printf("Введите размер доски n: ");
    scanf("%d", &n);

    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    double elapsed;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    try_();

    printf("Минимальное количество коней: %d\n", best);

    QueryPerformanceCounter(&end);
    elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("Время выполнения: %f секунд\n", elapsed);
    return 0;
}







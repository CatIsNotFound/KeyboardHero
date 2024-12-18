#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum D {U = 'A', D, L, R};

void move_cur(const int LINE, const int COL) {
    char command[128] = {'\0'};
    strcat(command, "echo \033[");
    char t[4] = {'\0'};
    itoa(LINE, t, 10);
    strcat(command, t);
    strcat(command, ";");
    itoa(COL, t, 10);
    strcat(command, t);
    strcat(command, "H");
//    printf("%s", command);
    system(command);
}

int main() {
    move_cur(10, 10);
    printf("\033[2J\033[H");
    system("pause");
    return 0;
}
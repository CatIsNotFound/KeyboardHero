
#include "Control.h"

// 等待输入
void wait() {
    getch();
}

// 返回输入的字符
char get_key() {
    return (char)getch();
}

// 键盘输入
void input_key(int keyCode) {
    char ch = '\0';
    while (ch != keyCode) {
        if (kbhit()) {
            ch = get_key();
        }
    }
}

// 敲击键位
char hit_key() {
    return get_key();
}


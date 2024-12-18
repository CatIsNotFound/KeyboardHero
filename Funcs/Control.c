
#include "Control.h"

// 等待输入
void wait() {
    getch();
}

// 输出输入的字符 Ascii 码
char get_key() {
    return getch();
}

// 测试键盘输入
void test_key(int keyCode) {
    char ch = '\0';
    while (ch != keyCode) {
        ch = get_key();
        printf("%d ", ch);
    }
}


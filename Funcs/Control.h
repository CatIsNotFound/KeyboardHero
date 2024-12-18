#include <conio.h>
#include <stdio.h>

// 定义特殊的键，如回车键、退出键、强制退出键等
enum KEYS {
    K_Ctrl_C = 3,
    K_BackSpace = 8,
    K_Enter = 13,
    K_Esc = 27
};

void wait();
char get_key();
void test_key();

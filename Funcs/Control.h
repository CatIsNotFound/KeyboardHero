#include <conio.h>
#include <stdio.h>

// ��������ļ�����س������˳�����ǿ���˳�����
enum KEYS {
    K_Ctrl_C = 3,
    K_BackSpace = 8,
    K_Enter = 13,
    K_Esc = 27,
    K_Space = 32
};

void wait();
char get_key();
void input_key(int keyCode);
char hit_key();


#include "Control.h"

// �ȴ�����
void wait() {
    getch();
}

// ����������ַ�
char get_key() {
    return (char)getch();
}

// ��������
void input_key(int keyCode) {
    char ch = '\0';
    while (ch != keyCode) {
        if (kbhit()) {
            ch = get_key();
        }
    }
}

// �û���λ
char hit_key() {
    return get_key();
}


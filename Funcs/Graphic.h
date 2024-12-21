
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Misc */
// ����
enum Direction {Up = 'A', Down, Right, Left};
// �������� λ��
struct Position {
    int line, col;
};
// ����ߴ��С
struct Size {
    int width, height;
};

// ���尴ť
struct Button {
    int id;
    char title[64];
};

// ���尴ť��
struct ButtonGroup {
    int gid;
    struct Button *groups;
    int count;
};

// ����Ի�������
enum dia_type {INFO, YES_NO, YES_NO_CANCEL};

/* SCREEN */
void clear_scr();
void move_cursor(const int, const int);
void move_cur(const enum Direction, const int);
void show_cursor();
void hide_cursor();

/* Widgets */
void draw_frame(const struct Position, const struct Size, const char* TITLE, const char* TEXT);
void draw_button(const struct Position, const struct Button button, const struct Size btn_size);
void draw_buttonGroup(const struct Position, const struct Button groups[16], const int btn_count);
void set_focus(const struct Position position, const struct ButtonGroup buttonGroup, const int INDEX, const int is_focused);

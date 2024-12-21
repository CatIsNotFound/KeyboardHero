#include "Graphic.h"

// 清屏
void clear_scr() {
    system("cls");
}

// 移动光标
void move_cursor(const int LINE, const int COL) {
    if (COL > 120) return;
    char command[16] = {'\0'};
    strcat(command, "\033[");
    char t[9] = {'\0'};
    itoa(LINE, t, 10);
    strcat(command, t);
    strcat(command, ";");
    itoa(COL, t, 10);
    strcat(command, t);
    strcat(command, "H");
    printf("%s", command);
}

// 向某个方向移动 P 个光标
void move_cur(const enum Direction direction, const int P) {
    char command[16] = {'\0'};
    strcat(command, "\033[");
    char t[4] = {'\0'};
    itoa(P, t, 10);
    strcat(command, t);
    char d[2] = {direction, '\0'};
    strcat(command, d);
    printf("%s", command);
}

// 显示光标
void show_cursor() {
    printf("%s", "\033[?25h");
}

// 隐藏光标
void hide_cursor() {
    printf("%s", "\033[?25l");
}

// 绘制框架
void draw_frame(const struct Position start_pos, const struct Size frame_size, const char* TITLE, const char* TEXT) {
    if (frame_size.width < 8 && frame_size.height < 4) return;
    move_cursor(start_pos.line, start_pos.col);
    int k = 0;
    while (k < frame_size.height) {
        if (k == 0 || k == frame_size.height - 1) {
            printf("+");
            for (int i = 1; i < frame_size.width - 1; ++i) {
                printf("-");
            }
            printf("+");
        } else {
            printf("|");
            // move_cur(Right, frame_size.width - 2);
            for (int i = 0; i < frame_size.width - 2; ++i) {
                printf(" ");
            }
            printf("|");
        }
        k += 1;
        move_cursor(start_pos.line + k, start_pos.col);
    }
    int len = (int)strlen(TITLE);
    if (len) {
        int st = frame_size.width / 2 - len / 2 - 2 + !(frame_size.width % 2);
        move_cursor(start_pos.line, start_pos.col + st);
        printf(" %s ", TITLE);
    }
    len = (int)strlen(TEXT);
    if (len) {
        k = 0;
        int p = 1;
        move_cursor(start_pos.line + (p++), start_pos.col + 1);
        while (k < len) {
            char code[256] = {'\0'};
            for (int i = 0; i < frame_size.width - 2; ++i) {
                if (TEXT[k] == '\n') {
                    k += 1;
                    break;
                }
                code[i] = TEXT[k];
                k++;
            }
            printf("%s", code);
            move_cursor(start_pos.line + (p++), start_pos.col + 1);
        }
    }
    move_cursor(start_pos.line + frame_size.height, start_pos.col );
}

// 绘制单个按钮
void draw_button(const struct Position position, const struct Button button, const struct Size btn_size) {
    if (btn_size.width < 6 || btn_size.height < 3) return;
    move_cursor(position.line, position.col);
    draw_frame(position, btn_size, "", button.title);
}

// 绘制按钮组
void draw_buttonGroup(const struct Position start_pos, const struct Button groups[16], const int btn_count) {
    if (btn_count > 16) return;
    move_cursor(start_pos.line, start_pos.col);
    int lens[16] = {0}, max = 0;
    for (int i = 0; i < btn_count; ++i) {
        lens[i] = (int)strlen(groups[i].title);
        if (lens[i] > max) max = lens[i];
    }
    int bg_width = max + 6;
    int bid = 0;
    for (int i = 0; i < btn_count * 2 + 1; ++i) {
        if (!(i % 2)) {
            for (int j = 0; j < bg_width; j++) {
                if (j == 0 || j == bg_width - 1) printf("+");
                else printf("-");
            }
        } else {
            printf("|");
            int st_col = (bg_width - 2) / 2 - strlen(groups[bid].title) / 2;
            move_cur(Right, st_col);
            printf("%s", groups[bid++].title);
            move_cursor(start_pos.line + i, start_pos.col + bg_width - 1);
            printf("|");
        }
        move_cur(Down, 1);
        move_cur(Left, bg_width);
    }
}

// 设置或取消设置按钮组中的焦点
void set_focus(const struct Position pos, const struct ButtonGroup group, const int INDEX, const int is_focused) {
    move_cursor(pos.line, pos.col);
    int group_count = group.count;
    int max = -1;
    for (int i = 0; i < group_count; ++i) {
        if (i == INDEX) {
            move_cur(Down, 2 * i + 1);
            move_cur(Right, 1);
            if (is_focused)
                printf(">");
            else
                printf(" ");
        }
        int len = (int)strlen(group.groups[i].title);
        if (len > max) max = len;
    }
    move_cur(Right, max + 2);
    if (is_focused)
        printf("<");
    else
        printf(" ");
}




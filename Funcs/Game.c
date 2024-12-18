#include "Game.h"

int option = 0;
struct Files game_datas;

// 初始化游戏
void init_game() {
    clear_scr();
    hide_cursor();
    int h = get_list("saved", &game_datas);
    main_menu(h);
}

// 摧毁游戏
void destory_game() {
    show_cursor();
    clear_scr();
    exit(0);
}

// 选择菜单项目事件
void selected_event(int selected_id) {
    switch (selected_id) {
        case 1:

            break;
        case 2:
            select_datas();
            break;
        case 3:
            destory_game();
            break;
    }
}

// 主菜单
void main_menu(const int HAVE_DATAS) {
    clear_scr();
    struct Position position = {2, 3};
    struct Size size = {81, 7};
    draw_frame(position, size, "",
                                 " K   K EEEEE Y   Y BBBB   OOO    A   RRRR  DDDD        H   H EEEEE RRRR   OOO \n"
                                 " K  K  E      Y Y  B   B O   O  A A  R   R D   D       H   H E     R   R O   O\n"
                                 " KKK   EEEE    Y   BBBB  O   O AAAAA RRRR  D   D       HHHHH EEEE  RRRR  O   O\n"
                                 " K  K  E       Y   B   B O   O A   A R  R  D   D       H   H E     R  R  O   O\n"
                                 " K   K EEEEE   Y   BBBB   OOO  A   A R   R DDDD        H   H EEEEE R   R  OOO \n");
    move_cur(Right, 28);
    printf("Made by CatIsNotFound.\n");
    move_cur(Right, 34);
    printf("Version 0.11\n");
    struct Button btns[3] = {
            {1, "  New  "},
            {2, "  Load  "},
            {3, "  Quit  "}
    };
    struct ButtonGroup btg = {1, btns, 3};
    position.line = 12; position.col = 33;
    set_focus(position, btg, 0, 1);
    draw_buttonGroup(position, btg.groups, btg.count);
    printf("%d", HAVE_DATAS);
    option = 0;
    selected_event(select_item(position, btg));
}

// 选择菜单项目
int select_item(const struct Position position, const struct ButtonGroup group) {
    char ch = '\0';
    while (ch != K_Enter) {
        ch = get_key();
        set_focus(position, group, option, 0);
        switch (ch) {
            case 'w':
            case 'W':
            case 'i':
            case 'I':
                if (option > 0) option--;
                break;
            case 's':
            case 'S':
            case 'k':
            case 'K':
                if (option < group.count - 1) option++;
                break;
        }
        set_focus(position, group, option, 1);
    }
    return group.groups[option].id;
}

// 选择存档
void select_datas() {
    clear_scr();
    struct Position position = {2, 3};
    struct Size size = {79, 20};
    draw_frame(position, size, "Load Game", " Select your save data: ");

}

// 暂停菜单
void pause_menu() {

}

void thread() {
    while (get_key() != K_Esc) {

    }
}
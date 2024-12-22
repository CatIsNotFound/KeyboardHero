
#include "UI.h"

struct Area area_game, area_pLive, area_pScore, area_pGoal, area_level, area_blocks;
struct Files game_files;

// 选择菜单项目
int select_item(const struct Position position, const struct ButtonGroup group) {
    int option = 0;
    set_focus(position, group, option, 1);
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
            default:
                break;
        }
        set_focus(position, group, option, 1);
    }
    return group.groups[option].id;
}



// 主菜单
void main_menu() {
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
            {11, "新游戏"},
            {12, "继续游戏"},
            {-1, "退出游戏"}
    };
    struct ButtonGroup btg = {1, btns, 3};
    position.line = 12; position.col = 33;
    set_focus(position, btg, 0, 1);
    draw_buttonGroup(position, btg.groups, btg.count);
    selected_event(select_item(position, btg));
}

// TODO: 选择存档
void select_datas(const enum Mode mode, struct GameDatas *gameDatas) {
    clear_scr();
    struct Position position = {2, 3};
    struct Size size = {81, 27};
    if (mode) {
        draw_frame(position, size, "存档", " 选择任一数据并按下 Enter 键存档：");
    } else {
        draw_frame(position, size, "继续", " 选择任一数据并按下 Enter 键继续：");
    }
    struct Position dt_pos = {7, 5};
    struct Size dt_size = {25, 7};
    char dir_name[6] = {'s', 'a', 'v', 'e', 'd', '/'};
    struct Position t_pos = {9, 24};
    struct Size t_size = {32, 10};
    int err = get_list(dir_name, &game_files);
    if (err == 0 && mode == Load) {
        dialogbox(t_pos, t_size, "找不到存档", "\n  当前没有任何存档！\n  请先游玩游戏并存档！", INFO);
        main_menu();
        return;
    } else {
        for (int i = 0; i < game_files.count; i++) {
            char file_name[32];
            strcpy(file_name, dir_name);
            strcat(file_name, game_files.name[i]);
            if (load_data(file_name, gameDatas) == 2) {
                dialogbox(t_pos, t_size, "存档读取出现问题", "\n  某个存档文件可能存在篡改！无法直接读取文件！",
                          INFO);
                return;
            }
        }
    }
    struct Area data_area[9];
    char title[7] = {'\0'};
    for (int i = 0; i < 9; ++i) {
        data_area[i].a_pos.col = (i % 3) * 26 + 5;
        data_area[i].a_pos.line = (i / 3) * 8 + 5;
        char id[3] = {'\0', '\0', '\0'};
        strcpy(title, "Data");
        itoa(i + 1, id, 10);
        strncat(title, id, 2);
        draw_frame(data_area[i].a_pos, dt_size, title, "");
    }
    if (mode == Save) {
        int num = select_one_data(data_area, 10);
        if (num + 1) {
            char file_name[32] = {'\0'};
            strcpy(file_name, "saved/");
            char id[3] = {'\0', '\0', '\0'};
            strcpy(title, "Data");
            itoa(num + 1, id, 10);
            strncat(title, id, 2);
            strcat(file_name, title);
            save_data(file_name, gameDatas);
        }
    }
}

// 选择一个存档
int select_one_data(struct Area select_area[], const int select_count) {
    int option = 0;
    selected_one_data(true, select_area[option].a_pos);
    char ch = '\0';
    while (ch != K_Enter && ch != K_Esc && ch != K_BackSpace) {
        ch = get_key();
        selected_one_data(false, select_area[option].a_pos);
        switch (ch) {
            case 'W':
            case 'w':
            case 'I':
            case 'i':
                if (option > 2) option -= 3;
                break;
            case 'S':
            case 's':
            case 'K':
            case 'k':
                if (option < 6) option += 3;
                break;
            case 'A':
            case 'a':
            case 'J':
            case 'j':
                if (option % 3 > 0) option--;
                break;
            case 'D':
            case 'd':
            case 'L':
            case 'l':
                if (option % 3 < 2) option++;
                break;
            default:
                break;
        }
        selected_one_data(true, select_area[option].a_pos);
    }
    if (ch == K_Esc || ch == K_BackSpace) return -1;
    return option;
}

// 显示选中的存档
void selected_one_data(bool is_selected, struct Position pos) {
    move_cursor(pos.line, pos.col);
    char ch = ' ';
    if (is_selected) ch = '*';
    move_cur(Right, 8);
    printf("%c", ch);
    move_cur(Right, 5);
    printf("%c", ch);
}

// 游戏画面
void game_view() {
    clear_scr();
    // 信息栏
    struct Position i_pos = {2, 2};
    struct Size i_size = {80, 3};
    draw_frame(i_pos, i_size, "", " 心     分        剩余     \t\t\t\t 目标分        关卡    ");
    area_pLive.a_pos.line = 3; area_pLive.a_pos.col = 6;
    area_pScore.a_pos.line = 3; area_pScore.a_pos.col = 14;
    area_blocks.a_pos.line = 3; area_blocks.a_pos.col = 28;
    area_pGoal.a_pos.line = 3; area_pGoal.a_pos.col = 65;
    area_level.a_pos.line = 3; area_level.a_pos.col = 77;
    // 主画面
    struct Position g_pos = {4, 2};
    struct Size g_size = {80, 26};
    draw_frame(g_pos, g_size, "", "");
    g_pos.col++; g_pos.line++;
    area_game.a_pos = g_pos;

}

// 更新信息
void update_info(int id, const struct GameDatas *DATAS) {
    switch (id) {
        case 1:
            move_cursor(area_pLive.a_pos.line, area_pLive.a_pos.col);
            printf("%3lld", DATAS->player_lives);
            break;
        case 2:
            move_cursor(area_pScore.a_pos.line, area_pScore.a_pos.col);
            printf("%5lld", DATAS->player_score);
            break;
        case 3:
            move_cursor(area_pGoal.a_pos.line, area_pGoal.a_pos.col);
            printf("%5lld", DATAS->game_level);
            break;
        case 4:
            move_cursor(area_level.a_pos.line, area_level.a_pos.col);
            printf("%3lld", DATAS->game_level);
            break;
        case 5:
            move_cursor(area_blocks.a_pos.line, area_blocks.a_pos.col);
            printf("%3d", DATAS->block_count);
            break;
        default:
            break;
    }
}

// 暂停菜单
void pause_menu() {
    clear_scr();
    struct Button buttons[4] = {
            {21, "继续"},
            {22, "存档"},
            {23, "返回"},
            {24, "退出"}
    };
    struct ButtonGroup group = {2, buttons, 4};
    struct Position pos = {9, 30};
    struct Position t_pos = {pos.line + 1, pos.col + 2};
    struct Size size = {14, 11};
    draw_frame(pos, size, "Menu", "");
    draw_buttonGroup(t_pos, group.groups, group.count);

    int s = select_item(t_pos, group);
    selected_event(s);
}

// 对话框
int dialogbox(struct Position position, struct Size box_size, const char *title, const char *text, enum dia_type type) {
    if (box_size.width < 20 || box_size.height < 9) return -1;
    draw_frame(position, box_size, title, text);
    struct Button ok, yes, no, cancel;
    struct Size t_size = {13, 3};
    switch (type) {
        case INFO:
            ok.id = 1;
            strcpy(ok.title, " 好(Enter)");
            struct Position t_pos = {position.line + box_size.height - 4,
                                position.col + (box_size.width / 2 - t_size.width / 2)};
            draw_button(t_pos, ok, t_size);
            break;
        case YES_NO:
            yes.id = 1; no.id = 0;
            strcpy(yes.title, " 是(Enter)");
            strcpy(no.title, " 否(Bcksp)");
            struct Position yes_pos = {position.line + box_size.height - 4, position.col + 1};
            struct Position no_pos = {position.line + box_size.height - 4,
                                position.col + box_size.width - t_size.width - 1};
            draw_button(yes_pos, yes, t_size);
            draw_button(no_pos, no, t_size);
            break;
        case YES_NO_CANCEL:
            yes.id = 1; no.id = 2; cancel.id = 0;
            strcpy(yes.title, " 是(Enter)");
            strcpy(no.title, " 否(Bcksp)");
            strcpy(cancel.title, " 取消(Esc)");
            struct Position y_pos = {position.line + box_size.height - 4, position.col + 1};
            struct Position n_pos = {position.line + box_size.height - 4,
                                position.col + (box_size.width / 2 - t_size.width / 2)};
            struct Position c_pos = {position.line + box_size.height - 4,
                                position.col + box_size.width - t_size.width - 1};
            draw_button(y_pos, yes, t_size);
            draw_button(n_pos, no, t_size);
            draw_button(c_pos, cancel, t_size);
            break;
    }
    while (true) {
        char ch = get_key();
        switch (type) {
            case INFO:
                if (ch == K_Enter) return ok.id;
                break;
            case YES_NO:
                if (ch == K_Enter) return yes.id;
                if (ch == K_BackSpace) return no.id;
                break;
            case YES_NO_CANCEL:
                if (ch == K_Enter) return yes.id;
                if (ch == K_BackSpace) return no.id;
                if (ch == K_Esc) return cancel.id;
                break;
        }
    }
}


// 初始化
void init() {
    clear_scr();
    hide_cursor();
    main_menu();
}

// 释放并退出
void destroy() {
    show_cursor();
    clear_scr();
    exit(0);
}



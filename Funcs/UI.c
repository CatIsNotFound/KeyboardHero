
#include "UI.h"

struct Area area_game, area_pLive, area_pScore, area_pGoal, area_level, area_blocks;
struct Files game_files;
struct GameDatas g_datas[10];

void clear_gf_datas();
void draw_gf_datas(int no);

// ѡ��˵���Ŀ
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
                else option = group.count - 1;
                break;
            case 's':
            case 'S':
            case 'k':
            case 'K':
                if (option < group.count - 1) option++;
                else option = 0;
                break;
            default:
                break;
        }
        set_focus(position, group, option, 1);
    }
    return group.groups[option].id;
}



// ���˵�
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
    printf("Made by XinKai Liao.\n");
    move_cur(Right, 34);
    printf("Version 0.11\n");
    struct Button btns[3] = {
            {11, "����Ϸ"},
            {12, "������Ϸ"},
            {-1, "�˳���Ϸ"}
    };
    struct ButtonGroup btg = {1, btns, 3};
    position.line = 12; position.col = 33;
    set_focus(position, btg, 0, 1);
    draw_buttonGroup(position, btg.groups, btg.count);
    selected_event(select_item(position, btg));
}

// TODO: ѡ��浵
int select_datas(const enum Mode mode, struct GameDatas *gameDatas) {
    clear_scr();
    clear_gf_datas();
    struct Position position = {2, 3};
    struct Size size = {81, 27};
    struct Area data_area[9];
    if (mode) {
        draw_frame(position, size, "�浵", " ѡ����һ���ݲ����� Enter ���浵��");
    } else {
        draw_frame(position, size, "����", " ѡ����һ���ݲ����� Enter ��������");
    }
    struct Position dt_pos = {7, 5};
    struct Size dt_size = {25, 7};
    char dir_name[7] = {'s', 'a', 'v', 'e', 'd', '/', '\0'};
    struct Position t_pos = {9, 24};
    struct Size t_size = {32, 10};
    int err = get_list(dir_name, &game_files);
    if (err == 0 && mode == Load) {
        dialogbox(t_pos, t_size, "�Ҳ����浵", "\n  ��ǰû���κδ浵��\n  ����������Ϸ���浵��", INFO);
        return -1;
    } else {
        for (int i = 0; i < err; i++) {
            char file_name[32];
            strcpy(file_name, dir_name);
            strcat(file_name, game_files.name[i]);
            int e_code = load_data(file_name, &g_datas[i]);
            if (e_code == 2) {
                dialogbox(t_pos, t_size, game_files.name[i], "\n  �ô浵�ļ����ܴ��ڴ۸ģ�\n  �����޷�ֱ�Ӷ�ȡ�ļ���",
                          INFO);
                return -1;
            } else if (e_code == 1) {
                dialogbox(t_pos, t_size, game_files.name[i], "\n  �ô浵�ļ��Ѷ�ʧ��\n  �޷�ֱ�ӻ�ȡ���ݣ�",
                          INFO);
                return -1;
            }
        }
    }

    char title[7] = {'\0'};
    for (int i = 0; i < 9; ++i) {
        data_area[i].a_pos.col = (i % 3) * 26 + 5;
        data_area[i].a_pos.line = (i / 3) * 8 + 5;
        char id[3] = {'\0'};
        strcpy(title, "Data");
        itoa(i + 1, id, 10);
        strncat(title, id, 2);
        draw_frame(data_area[i].a_pos, dt_size, title, "");
    }

    for (int i = 0; i < game_files.count; ++i) {
        draw_gf_datas(game_files.id[i] - 1);
    }

    // TODO: ѡ���ȡ
    if (mode == Load) {
        int num = select_one_data(data_area, 10);
        if (num + 1) {
            if (g_datas[num].played_time > 0) {
                gameDatas->played_time = g_datas[num].played_time;
                gameDatas->game_level = g_datas[num].game_level;
                gameDatas->player_score = g_datas[num].player_score;
                gameDatas->player_lives = g_datas[num].player_lives;
                gameDatas->block_count = g_datas[num].block_count;
                return 1;
            } else {
                dialogbox(t_pos, t_size, "�޷�����", "\n  �޷�������ȫ�հ׵����ݣ�\n  ������ѡ�����ݣ�", INFO);
                return -2;
            }
        }
    }

    // ѡ�񱣴�
    if (mode == Save) {
        int num = select_one_data(data_area, 10);
        if (num + 1) {
            if (g_datas[num].played_time != -1) {
                int n = dialogbox(t_pos, t_size, "���Ǳ��棿", "\n  �������Ѵ��ڣ�\n  �Ƿ�ȷ�ϸ������ݣ�", YES_NO);
                if (!n) {
                    return 1;
                }
            }
            char file_name[32] = {'\0'};
            strcpy(file_name, "saved/");
            char id[3] = {'\0', '\0', '\0'};
            strcpy(title, "Data");
            itoa(num + 1, id, 10);
            strncat(title, id, 2);
            strcat(file_name, title);
            move_cursor(1, 2);
            save_data(file_name, gameDatas);
            return 1;
        }
    }
    return 0;
}

// ���� / �����Ϸ�ļ�������
void draw_gf_datas(int no) {
    struct Area data_area[9];
    if (g_datas[no].played_time == -1) return;
    data_area[no].a_pos.col = (no % 3) * 26 + 6;
    data_area[no].a_pos.line = (no / 3) * 8 + 6;
    show_game_time(data_area[no].a_pos, &g_datas[no]);
    move_cursor(++data_area[no].a_pos.line, data_area[no].a_pos.col);
    printf(" �ؿ��Ѷȣ�%d", g_datas[no].game_level);
    move_cursor(++data_area[no].a_pos.line, data_area[no].a_pos.col);
    printf(" ����������%d", g_datas[no].player_lives);
    move_cursor(++data_area[no].a_pos.line, data_area[no].a_pos.col);
    printf(" ����ܷ֣�%ld", g_datas[no].player_score);
}

// �����Ϸ�ļ�����
void clear_gf_datas() {
    for (int i = 0; i < 10; ++i) {
        g_datas[i].game_level = 0;
        g_datas[i].block_count = 0;
        g_datas[i].player_lives = 0;
        g_datas[i].player_score = 0;
        g_datas[i].played_time = -1;
    }
}

// ��ʽ�������Ϸ��ʱ��
void show_game_time(struct Position pos, const struct GameDatas *GD) {
    move_cursor(pos.line, pos.col);
    int hour, min, sec;
    hour = GD->played_time / 60 / 60;
    min = GD->played_time / 60 % 60;
    sec = GD->played_time % 3600 % 60;
    printf(" ����ʱ����%02d:%02d:%02d", hour, min, sec);
}

// ѡ��һ���浵
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

// ��ʾѡ�еĴ浵
void selected_one_data(bool is_selected, struct Position pos) {
    move_cursor(pos.line, pos.col);
    char ch = ' ';
    if (is_selected) ch = '*';
    move_cur(Right, 8);
    printf("%c", ch);
    move_cur(Right, 5);
    printf("%c", ch);
}

// ��Ϸ����
void game_view() {
    clear_scr();
    // ��Ϣ��
    struct Position i_pos = {2, 2};
    struct Size i_size = {80, 3};
    draw_frame(i_pos, i_size, "", " ��     ��        ʣ��     \t\t\t\t Ŀ���        �ؿ�    ");
    area_pLive.a_pos.line = 3; area_pLive.a_pos.col = 6;
    area_pScore.a_pos.line = 3; area_pScore.a_pos.col = 14;
    area_blocks.a_pos.line = 3; area_blocks.a_pos.col = 28;
    area_pGoal.a_pos.line = 3; area_pGoal.a_pos.col = 65;
    area_level.a_pos.line = 3; area_level.a_pos.col = 77;
    // ������
    struct Position g_pos = {4, 2};
    struct Size g_size = {80, 26};
    draw_frame(g_pos, g_size, "", "");
    g_pos.col++; g_pos.line++;
    area_game.a_pos = g_pos;

}

// ������Ϣ
void update_info(int id, const struct GameDatas *DATAS) {
    switch (id) {
        case 1:
            move_cursor(area_pLive.a_pos.line, area_pLive.a_pos.col);
            printf("%3d", DATAS->player_lives);
            break;
        case 2:
            move_cursor(area_pScore.a_pos.line, area_pScore.a_pos.col);
            printf("%5lld", DATAS->player_score);
            break;
        case 3:
            move_cursor(area_pGoal.a_pos.line, area_pGoal.a_pos.col);
            printf("%5lld", DATAS->player_goal);
            break;
        case 4:
            move_cursor(area_level.a_pos.line, area_level.a_pos.col);
            printf("%3d", DATAS->game_level);
            break;
        case 5:
            move_cursor(area_blocks.a_pos.line, area_blocks.a_pos.col);
            printf("%3d", DATAS->block_count);
            break;
        default:
            break;
    }
}

// ��ͣ�˵�
void pause_menu() {
    struct Button buttons[4] = {
            {21, "����"},
            {22, "�浵"},
            {23, "����"},
            {24, "�˳�"}
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

// �Ի���
int dialogbox(struct Position position, struct Size box_size, const char *title, const char *text, enum dia_type type) {
    if (box_size.width < 20 || box_size.height < 9) return -1;
    draw_frame(position, box_size, title, text);
    struct Button ok, yes, no, cancel;
    struct Size t_size = {13, 3};
    switch (type) {
        case INFO:
            ok.id = 1;
            strcpy(ok.title, " ��(Enter)");
            struct Position t_pos = {position.line + box_size.height - 4,
                                position.col + (box_size.width / 2 - t_size.width / 2)};
            draw_button(t_pos, ok, t_size);
            break;
        case YES_NO:
            yes.id = 1; no.id = 0;
            strcpy(yes.title, " ��(Enter)");
            strcpy(no.title, " ��(Bcksp)");
            struct Position yes_pos = {position.line + box_size.height - 4, position.col + 1};
            struct Position no_pos = {position.line + box_size.height - 4,
                                position.col + box_size.width - t_size.width - 1};
            draw_button(yes_pos, yes, t_size);
            draw_button(no_pos, no, t_size);
            break;
        case YES_NO_CANCEL:
            yes.id = 1; no.id = 2; cancel.id = 0;
            strcpy(yes.title, " ��(Enter)");
            strcpy(no.title, " ��(Bcksp)");
            strcpy(cancel.title, " ȡ��(Esc)");
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


// ��ʼ��
void init() {
    clear_scr();
    hide_cursor();
    main_menu();
}

// �ͷŲ��˳�
void destroy() {
    show_cursor();
    clear_scr();
    exit(0);
}



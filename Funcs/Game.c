#include "Game.h"

// ȫ��
bool is_running = false;
bool is_gaming = false;
pthread_t game_thread[4], input_thread;
pthread_mutex_t mutex;
struct Timer game_time;
struct GameDatas game_datas;
struct Block blocks[999];
int is_falling[999] = {-1};
int falling_count = 0;
char charset[127] = {'\0'};

// ��Ϸ�߳�
void run_thread() {
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&input_thread, NULL, await, NULL);
    pthread_create(&game_thread[0], NULL, keep, NULL);
//    pthread_create(&game_thread[1], NULL, keep, NULL);
//    pthread_create(&game_thread[2], NULL, keep, NULL);
//    pthread_create(&game_thread[3], NULL, keep, NULL);
    pthread_join(input_thread, NULL);
    pthread_join(game_thread[0], NULL);
//    pthread_join(game_thread[1], NULL);
//    pthread_join(game_thread[2], NULL);
//    pthread_join(game_thread[3], NULL);
    pthread_mutex_destroy(&mutex);
}


// ��ʼ����Ϸ
void init_game() {
    clear_scr();
    sleep(500);
    game_view();
    for (int i = 1; i < 5; ++i) {
        update_info(i, &game_datas);
    }
    sleep(1000);
    is_gaming = true;
    is_running = true;
    init_blocks();
    init_charset();
    game_datas.blocks = falling_count;
    run_thread();
}

// ����Ϸ
void new_game() {
    // ������Ϸ����
    game_datas.game_level = 1;
    game_datas.player_lives = 3;
    game_datas.player_score = 0;
    game_datas.played_time = 0;
    game_datas.blocks = 0;
    // ��ʼ����Ϸ
    init_game();
}

// ��Ϸѭ���߳�
void * keep(void *pVoid) {
    srand(time(0));
    pthread_mutex_lock(&mutex);
    while (is_gaming) {
        start_count(&game_time);
        while (is_running) {
            static long long sp = 0;
            if (!((sp++) % 6)) {
                sp = 1;
                sleep(800);
                generate_block(4, 5, true);
                continue;
            }
            who_is_falling();
        }
        game_datas.played_time += stop_count(&game_time);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

// �ȴ�����ѭ������
void * await(void *pVoid) {
    while (is_gaming) {
        char key; bool flag = true;
        while (flag) {
            key = hit_key();
            switch (key) {
                case K_Space:
                    flag = false;
                    break;
                case K_Esc:
                    is_running = false;
                    if (is_quit_game()) {
                        is_gaming = false;
                        destroy();
                    } else {
                        resume();
                    }
                    is_running = true;
                    break;
                default:
                    is_running = false;
                    find_key(key);
                    is_running = true;
                    break;
            }
        }
        is_running = false;
        sleep(100);
        pause_menu();
    }
    pthread_exit(NULL);
}

// �����ӳ�
void sleep(int ms) {
    clock_t now, then;
    now = then = clock();
    while ((now - then) < ms) {
        now = clock();
    }
}

// ��ʼ��ʱ
void start_count(struct Timer *timer) {
    timer->start = time(NULL);
}

// ������ʱ
long long stop_count(struct Timer *timer) {
    timer->end = time(NULL);
    return (timer->end - timer->start);
}

// �ָ���Ϸ����
void resume() {
    game_view();
    for (int i = 1; i < 5; ++i) {
        update_info(i, &game_datas);
    }
    who_is_falling();
    sleep(1000);
    is_running = true;
}

// ��ʼ�����п�
void init_blocks() {
    for (int i = 0; i < 999; ++i) {
        blocks[i].pos.col = 0;
        blocks[i].pos.line = 0;
        blocks[i].ch = '\0';
        blocks[i].is_visible = false;
        is_falling[i] = -1;
    }
    falling_count = 0;
}

// ��ʼ�������ַ���
void init_charset() {
    char numeric[12] = {'\0'};
    for (int i = 0; i < 10; ++i) {
        numeric[i] = (char)(48 + i);
    }
    char bigLetter[26], smallLetter[26];
    for (int i = 0; i < 26; ++i) {
        bigLetter[i] = (char)(65 + i);
        smallLetter[i] = (char)(97 + i);
    }
    char specialCharA[32];
    char specialCharB[32];
    strcpy(specialCharA, "`-=[]\\;',./");
    strcpy(specialCharB, "~!@#$%^&*()_+{}|:\"<>?");
    strncpy(charset, numeric, 10);
    strncat(charset, smallLetter, 26);
    strncat(charset, specialCharA, 12);
    strncat(charset, bigLetter, 26);
    strncat(charset, specialCharB, 22);
}

// ���ɶ��ٸ���
void generate_block(int game_level, int count, bool unique) {
    int gc = 0;
    if (count > 32) return;
    int randlist[32] = {-1};
    for (int i = 0; i < 999; ++i) {
        if (!blocks[i].is_visible) {
            blocks[i].is_visible = true;
            blocks[i].ch = charset[random_char(game_level)];
            blocks[i].pos.line = 5;
            int key = (int)(rand() % 78 + 3);
            if (gc > 0 && unique) {
                do {
                    bool flag = false;
                    for (int j = 0; j <= gc; ++j) {
                        if (key == randlist[j]) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag) key = (int) (rand() % 78 + 3);
                    else break;
                } while (true);
            }
            randlist[gc] = key;
            blocks[i].pos.col = randlist[gc];
            move_cursor(blocks[i].pos.line, blocks[i].pos.col);
            printf("%c", blocks[i].ch);
            start_falling(i);
            gc++;
            if (gc == count) break;
        }
    }
}

// ���¿��λ��
int update_pos(int i) {
    if (blocks[i].is_visible) {
        move_cursor(blocks[i].pos.line, blocks[i].pos.col);
        printf(" ");
        blocks[i].pos.line++;
        if (blocks[i].pos.line >= 29) {
            kill_live();
            destroy_block(i);
        } else {
            move_cur(Down, 1);
            move_cur(Left, 1);
            printf("%c", blocks[i].ch);
        }
    }
    return 0;
}

// �����
void destroy_block(int i) {
    falling_count--;
    blocks[i].is_visible = false;
    blocks[i].ch = '\0';
    stop_falling(i);
}

// ��ʼ����
void start_falling(int id) {
    is_falling[falling_count++] = id;
}

// ֹͣ����
void stop_falling(int id) {
    for (int i = 0; i < falling_count; ++i) {
        if (is_falling[i] == id) {
            for (int j = i; j < falling_count + 1; ++j) {
                is_falling[j] = is_falling[j + 1];
            }
        }
    }
    is_falling[falling_count] = -1;
}

// ˭������
void who_is_falling() {
    game_datas.blocks = falling_count;
    update_info(5, &game_datas);
    int left = 0, right = falling_count;
    int mid = falling_count / 2 + (falling_count % 2);
    for (int i = 0; i < mid; ++i) {
        update_pos(is_falling[left + i]);
        update_pos(is_falling[right - i - 1]);
    }
}

// ��������ַ�
int random_char(int level) {
    int seed;
    switch (level) {
        case 1:
            return (int)(rand() % 10);
        case 2:
            return (int)(rand() % 26 + 10);
        case 3:
            seed = (int)(rand() % 52);
            if (seed < 26) {
                return 10 + seed;
            } else {
                return 21 + seed;
            }
        case 4:
            seed = (int)(rand() % 62);
            if (seed < 36) {
                return seed;
            } else {
                return 11 + seed;
            }
        case 5:
            seed = rand() % 32;
            if (seed < 12) {
                return 36 + seed;
            } else {
                return 61 + seed;
            }
        case 6:
            return (int)(rand() % 93);
        default:
            return 0;
    }
}


// ѡ��˵���Ŀ�¼�
void selected_event(int selected_id) {
    switch (selected_id) {
        case 0:
            destroy();
            break;
        case 11:
            new_game();
            break;
        case 12:
            select_datas(Load);
            break;
        case 21:
            resume();
            break;
        case 22:
            select_datas(Save);
            clear_scr();
            pause_menu();
            break;
        case 23:
            if (back_to_main_menu()) {
                is_running = false; is_gaming = false;
                main_menu();
            } else {
                pause_menu();
            }
            break;
        case 24:
            if (is_quit_game()) {
                is_running = false; is_gaming = false;
                destroy();
            } else {
                pause_menu();
            }
            break;
        default:
            break;
    }
}

// �������˵�
int back_to_main_menu() {
    struct Position pos = {9, 23};
    struct Size size = {32, 11};
    return dialogbox(pos, size, "", "\n �Ƿ񷵻����˵���\n\n ȷ�Ϻ���Ϸ���ݽ��ᶪʧ��", YES_NO);
}

// �Ƿ��˳���Ϸ
int is_quit_game() {
    struct Position pos = {9, 23};
    struct Size size = {32, 11};
    return dialogbox(pos, size, "", "\n �Ƿ��˳���Ϸ��\n\n ȷ�Ϻ���Ϸ���ݽ��ᶪʧ��", YES_NO);
}

// ��ʾ������ʱ��
void show_game_time(struct Position pos) {
    move_cursor(pos.line, pos.col);
    int hour, min, sec;
    hour = game_datas.played_time / 60 / 60;
    min = game_datas.played_time / 60 % 60;
    sec = game_datas.played_time % 3600 % 60;
    printf("Played time: %02d:%02d:%02d", hour, min, sec);
}

// ���µļ��Ƿ�Ϊ����������ַ�
void find_key(char key) {
    for (int i = 0; i < falling_count; ++i) {
        int id = is_falling[i];
        if (key == blocks[id].ch) {
            sleep(100);
            game_datas.player_score += 1;
            update_info(2, &game_datas);
            move_cursor(blocks[id].pos.line, blocks[id].pos.col);
            printf(" ");
            destroy_block(id);
            break;
        }
    }
}

// �۳�����ֱ������
void kill_live() {
    game_datas.player_lives -= 1;
    update_info(1, &game_datas);
    if (game_datas.player_lives < 1) {
        is_running = false;
        is_gaming = false;
        hero_dead();
    }
}

// ����
void hero_dead() {
    sleep(1000);
    is_running = false;
    is_gaming = false;
    struct Position pos = {9, 23};
    struct Size size = {32, 11};

    int n = dialogbox(pos, size, "������", "\n  �����������Ƿ�������\n\n  �������������¿�ʼ��Ϸ��\n  ���ܾ������������˵�", YES_NO);
    if (n) {
        new_game();
    } else {
        main_menu();
    }
}
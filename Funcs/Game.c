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

struct Difficulties {
    int falling_speed;
    int generate_count;
    int generate_speed;
    int charset_level;
    long goal_score;
};

void show_pass_info();

void generate_level(struct Difficulties *diff) {
    int level = game_datas.game_level - 1;
    int f_speed[18] = {
            1500, 1250, 1000,
            1250,1250, 1000,
            1250, 1000, 800,
            1500, 1000, 800,
            1000, 800, 500
    };
    int g_count[18] = {
            10, 15, 20,
            10, 15, 18,
            10, 15, 20,
            12, 16, 22,
            10, 15, 20,
            15, 20, 25
    };
    int g_per_sec[18] = {5, 4, 4,
                         5, 4, 4,
                         5, 4, 3,
                         5, 4, 3,
                         5, 4, 3,
                         5, 4, 3
    };
    int goal[18] = {
            150, 300, 600,
            750, 1000, 1500,
            1850, 2250, 3000,
            3150, 3550, 4500,
            4650, 5200, 6000,
            7000, 8500, 10000
    };
    diff->falling_speed = f_speed[level % 18];
    diff->generate_count = g_count[level % 18];
    diff->generate_speed = g_per_sec[level % 18];
    diff->charset_level = level % 12 / 3;
    diff->goal_score = 10000 * (level / 18) + goal[level % 18];
}

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
    game_view();
    sleep(500);
    is_gaming = true;
    is_running = true;
    init_blocks();
    init_charset();
    game_datas.block_count = falling_count;
    run_thread();
}

// ����Ϸ
void new_game() {
    // ������Ϸ����
    game_datas.game_level = 1;
    game_datas.player_lives = 3;
    game_datas.player_score = 0;
    game_datas.player_goal = 0;
    game_datas.played_time = 0;
    game_datas.block_count = 0;
    // ��ʼ����Ϸ
    init_game();
}

// ��Ϸѭ���߳�
void * keep(void *pVoid) {
    srand(time(0));
    pthread_mutex_lock(&mutex);
    struct Difficulties game_diff;
    generate_level(&game_diff);
    game_datas.player_goal = game_diff.goal_score;
    for (int i = 0; i < 6; ++i) {
        update_info(i + 1, &game_datas);
    }
    sleep(1000);
    while (is_gaming) {
        start_count(&game_time);
        while (is_running) {
            static long long sp = 0;
            if (game_diff.generate_speed <= 1) {
                generate_block(game_diff.charset_level, game_diff.generate_count, true);
            } else if (!((sp++) % game_diff.generate_speed)) {
                // sp = 1;
                generate_block(game_diff.charset_level, game_diff.generate_count, true);
                continue;
            }

            who_is_falling();
            if (game_datas.player_lives < 1) {
                break;
            }
            // ����
            if (game_datas.player_score - 1 >= game_diff.goal_score) {
                game_datas.game_level += 1;
                generate_level(&game_diff);
                game_datas.player_goal = game_diff.goal_score;
                is_running = false;
                sleep(1000);
                show_pass_info();
                game_datas.player_lives += 1;
                game_view();
                init_blocks();
                is_running = true;
                for (int i = 0; i < 6; ++i) {
                    update_info(i + 1, &game_datas);
                }
            }
            sleep(game_diff.falling_speed);
        }
        if (game_datas.player_lives < 1) {
            is_gaming = false;
            is_running = false;
            hero_dead();
        }
        game_datas.played_time += stop_count(&game_time);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

// �ȴ�����ѭ������
void * await(void *pVoid) {
    while (is_gaming) {
        char key;
        key = hit_key();
        switch (key) {
            case K_Space:
                if (is_running == true) {
                    is_running = false;
                    pause_menu();
                }
                break;
            case K_Esc:
                if (is_running == true) {
                    is_running = false;
                    // sleep(1000);
                    if (is_quit_game()) {
                        is_gaming = false;
                        destroy();
                    } else {
                        resume();
                    }
                }
                break;
            case '\0':
                break;
            default:
                if (is_running)
                    find_key(key);
                break;
        }

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
            destroy_block(i);
            bool is_dead = kill_live();
            update_info(1, &game_datas);
            if (!is_dead) {
                return 1;
            }

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
    game_datas.block_count = falling_count;
    update_info(5, &game_datas);
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
    game_datas.block_count = falling_count;
    update_info(5, &game_datas);
}

// ˭������
void who_is_falling() {
    game_datas.block_count = falling_count;
//    int left = 0, right = falling_count;
//    int mid = falling_count / 2 + (falling_count % 2);
//    for (int i = 0; i < mid; ++i) {
//        if (update_pos(is_falling[left + i])) break;
//        if (update_pos(is_falling[right - i - 1])) break;
//    }
    for (int i = 0; i < falling_count; ++i) {
        if (update_pos(is_falling[i])) break;
    }
}

// ��������ַ�
int random_char(int level) {
    int seed;
    switch (level) {
        case 0:
            return (int)(rand() % 10);
        case 1:
            return (int)(rand() % 26 + 10);
        case 2:
            seed = (int)(rand() % 52);
            if (seed < 26) {
                return 10 + seed;
            } else {
                return 21 + seed;
            }
        case 3:
            seed = (int)(rand() % 62);
            if (seed < 36) {
                return seed;
            } else {
                return 11 + seed;
            }
        case 4:
            seed = rand() % 32;
            if (seed < 12) {
                return 36 + seed;
            } else {
                return 61 + seed;
            }
        case 5:
            return (int)(rand() % 93);
        default:
            return 0;
    }
}


// ѡ��˵���Ŀ�¼�
void selected_event(int selected_id) {
    int n;
    switch (selected_id) {
        case -1:
            destroy();
            break;
        case 0:
            main_menu();
            break;
        case 1:
        case 11:
            new_game();
            break;
        case 12:
            n = select_datas(Load, &game_datas);
            while (n == -2) {
                n = select_datas(Load, &game_datas);
            }
            if (n == 1) {
                init_game();
            } else {
                main_menu();
            }
            break;
        case 21:
            resume();
            break;
        case 22:
            n = select_datas(Save, &game_datas);
            while (n == 1) {
                n = select_datas(Save, &game_datas);
            }
            clear_scr();
            pause_menu();
            break;
        case 23:
            if (back_to_main_menu()) {
                is_running = false; is_gaming = false;
                main_menu();
            } else {
                clear_scr();
                pause_menu();
            }
            break;
        case 24:
            if (is_quit_game()) {
                is_running = false; is_gaming = false;
                destroy();
            } else {
                clear_scr();
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
            move_cursor(blocks[id].pos.line, blocks[id].pos.col);
            printf(" ");
            break;
        }
    }
}

// �۳�����ֱ������
bool kill_live() {
    game_datas.player_lives -= 1;
    update_info(1, &game_datas);
    if (game_datas.player_lives < 1) {
        return 0;
    }
    return 1;
}

// ����
void hero_dead() {
    sleep(2000);
    struct Position pos = {9, 23};
    struct Size size = {32, 11};

    int n = dialogbox(pos, size, "��Ϸ����", "\n  �����������Ƿ�������\n\n  �������������¿�ʼ�µ��ֻء�", YES_NO);
    selected_event(n);
}

// ����
void show_pass_info() {
    struct Position pos = {9, 23};
    struct Size size = {32, 9};
    struct Position t_pos = {15, 26};
    // dialogbox(pos, size, "���أ�", "\n  ��ϲ�ɹ��ﵽĿ��֣�\n\n  ���»س����Լ���������һ�أ�\n\n  ", INFO);
    draw_frame(pos, size, "��ϲ���أ�", "\n ��ϲ�ɹ�����Ŀ��֣�\n ��������������� 1 �㣡\n �ȴ� 5 ���Ӻ�, \n ��Ϸ��������һ�أ�\n"  );
    sleep(5000);
}

#include "UI.h"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

struct Timer {
    time_t start, end;
};

struct Block {
    struct Position pos;
    char ch;
    bool is_visible;
};

void run_thread();
void init_game();
void new_game();
void * keep(void *pVoid);
void * await(void *pVoid);
void sleep(int ms);
void start_count(struct Timer *timer);
long long stop_count(struct Timer *timer);
void resume();
void init_blocks();
void init_charset();
void generate_block(int level, int count, bool unique);
int update_pos(int id);
void destroy_block(int index);
void start_falling(int id);
void stop_falling(int id);
void who_is_falling();
int random_char(int level);
int back_to_main_menu();
int is_quit_game();
void show_game_time(struct Position pos);
void find_key(char key);
bool kill_live();
void hero_dead();
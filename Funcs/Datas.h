
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct Files {
    int id[64];
    char name[64][255];
    int count;
};

struct GameDatas {
    long long played_time;
    int player_lives;
    long long player_score;
    long long player_goal;
    int game_level;
    int block_count;
};

int get_list(char dir_name[], struct Files *dest);
int create_datas(char dir_name[], struct Files *dest);

int load_data(char file_name[], struct GameDatas *datas);
int save_data(char file_name[], struct GameDatas *datas);

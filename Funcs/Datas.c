
#include "Datas.h"

FILE *rfile, *wfile;

// 获取存档目录下的所有文件
int get_list(char dir_name[], struct Files *dest) {
    dest->count = 0;
    if (system("dir /B saved >d.log")) {
        printf("\033[1A           ");
        return -1;  // 没有这个目录
    }
    rfile = fopen("d.log", "r");
    while (!feof(rfile)) {
        int cid = dest->count++;
        char *t_id = NULL;
        fscanf(rfile, "%s\n", dest->name[cid]);
        if (!strcmp(dest->name[cid], "")) {
            return 0;
        }
        char t_name[16] = {'\0'};
        strcpy(t_name, dest->name[cid]);
        strtok_r(t_name, "a", &t_id);
        strtok_r(NULL, "a", &t_id);
        dest->id[cid] = atoi(t_id);
    }
    fclose(rfile);
    // system("del d.log");
    return dest->count;
}

// 创建单个存档文件
int create_data(char dir_name[], struct Files *dest, int i) {
    char file_name[64];
    strcpy(file_name, dir_name);
    char num[3] = {'\0'};
    itoa(i, num, 10);
    strcat(file_name, "data");
    strcpy(dest->name[i], "data");
    strcat(file_name, num);
    strcat(dest->name[i], num);
    printf("Created: %s", file_name);
    wfile = fopen(file_name, "w");
    if (wfile != NULL) {
        printf("\tYES");
        dest->count++;
        fclose(wfile);
    } else {
        printf("\tNO");
    }
    printf("\n");

    return 1;
}

// 创建所有存档文件
int create_datas(char dir_name[], struct Files *dest) {
    dest->count = 0;
    for (int i = 0; i < 16; ++i) {
        char file_name[64];
        strcpy(file_name, dir_name);
        char num[3] = {'\0'};
        itoa(i + 1, num, 10);
        strcat(file_name, "data");
        strcpy(dest->name[i], "data");
        strcat(file_name, num);
        strcat(dest->name[i], num);
        printf("Created: %s", file_name);
        wfile = fopen(file_name, "w");
        if (wfile != NULL) {
            printf("\tYES");
            dest->count++;
            fclose(wfile);
        } else {
            printf("\tNO");
        }
        printf("\n");
    }
    return dest->count;
}

// 加载存档文件
int load_data(char file_name[], struct GameDatas *datas) {
    rfile = fopen(file_name, "r");
    if (rfile == NULL) {
        return 1;   // 文件不存在
    }
    long long useless = 0, seed;
    fscanf(rfile, "%lld:%lld:%lld:%lld:%d:%d:%d", &useless, &seed, &datas->played_time, &datas->player_score,
           &datas->player_lives, &datas->game_level, &datas->block_count);
    seed -= datas->player_score + datas->player_lives + datas->block_count;
    if (useless % seed) {
        return 2;   // 文件有篡改
    }
    return 0;   // 正常读取
}

// 保存存档文件
int save_data(char file_name[], struct GameDatas *datas) {
    srand(time(NULL));
    wfile = fopen(file_name, "w");
    if (system("dir saved >null")) {
        printf("\033[1A           ");
        system("mkdir saved 2>null");
        fclose(wfile);
        wfile = fopen(file_name, "w");
    }
    system("del null");
    if (wfile != NULL) {
        long long useless = 0;
        long long seed = rand() % 9000000 + 1000000;
        long long seeds[128];
        int c = 0;
        for (int i = 10000000; i < 99999999; ++i) {
            if (!(i % seed)) {
                seeds[c++] = i;
            }
        }
        seed += datas->player_score + datas->player_lives + datas->block_count;
        useless = seeds[rand() % c];
        fprintf(wfile, "%lld:%lld:%lld:%lld:%d:%d:%d", useless, seed, datas->played_time, datas->player_score,
               datas->player_lives, datas->game_level, datas->block_count);
        fclose(wfile);
    }
    return 0;   // 正常保存
}



#include "Datas.h"

FILE *rfile, *wfile;

// 获取存档目录下的所有文件
int get_list(char dir_name[], struct Files *dest) {
    dest->count = 0;
    for (int i = 0; i < 16; ++i) {
        char file_name[64];
        strcpy(file_name, dir_name);
        char num[3] = {'\0'};
        char dataName[8] = {'\0'};
        itoa(i + 1, num, 10);
        strcpy(dataName, "Data");
        strcat(dataName, num);
        strcat(file_name, dataName);
        strcat(dest->name[i], dataName);
        printf("Checked: %s", file_name);
        rfile = fopen(file_name, "r");
        if (rfile != NULL) {
            printf("\tYES");
            dest->count++;
            fclose(rfile);
        } else {
            printf("\tNO");
        }
        printf("\n");
    }
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
    return 0;
}

// 保存存档文件
int save_data(char file_name[], struct GameDatas *datas) {
    return 0;
}



#include "Datas.h"

FILE *rfile, *wfile;

// 获取存档目录下的所有文件
int get_list(char dir_name[], struct Files *dest) {
    dest->count = 0;
    for (int i = 0; i < 16; ++i) {
        char file_name[64];
        strcpy(file_name, dir_name);
        char num[3] = {'\0'};
        itoa(i + 1, num, 10);
        strcat(file_name, "data");
        strcat(file_name, num);
        printf("Open: %s", file_name);
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

// 加载存档文件
int load_data(char file_name[]) {
    return 0;
}

// 保存存档文件
int save_data(char file_name[]) {
    return 0;
}


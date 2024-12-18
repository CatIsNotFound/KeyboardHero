
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Files {
    char name[64][255];
    int count;
};

int get_list(char dir_name[], struct Files *dest);
int load_data(char file_name[]);
int save_data(char file_name[]);

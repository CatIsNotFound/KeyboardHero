
#include "Graphic.h"
#include "Control.h"
#include "Datas.h"
#include <stdbool.h>

struct Area {
    struct Position a_pos;
};

enum Mode {Load, Save};


int select_item(const struct Position position, const struct ButtonGroup group);
void selected_event(int selected_id);
void main_menu();
void select_datas(const enum Mode mode, struct GameDatas *gameDatas);
int select_one_data(struct Area *select_area, const int select_count);
void selected_one_data(bool is_selected, struct Position pos);
void game_view();
void update_info(int id, const struct GameDatas *DATAS);
void pause_menu();
int dialogbox(struct Position position, struct Size box_size, const char title[32], const char text[256], enum dia_type type);
void init();
void destroy();



#include "Graphic.h"
#include "Control.h"
#include "Datas.h"
#include <stdlib.h>

void init_game();
void destory_game();
void main_menu(const int HAVE_DATAS);
int select_item(const struct Position position, const struct ButtonGroup group);
void selected_event(int selected_id);
void select_datas();
void pause_menu();
void new_game(FILE);
void thread();

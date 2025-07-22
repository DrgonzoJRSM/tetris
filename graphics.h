#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "includes.h"
#include "tetris.h"

#define START_POINT_FIELD_X 1
#define START_POINT_FIELD_Y 1

void ncurses_settings(void);

void draw_game(field_settings*, figure*, figure*, block*, colors**, flags*, settings*);
void print_pause(void);
void game_over(void);

#endif

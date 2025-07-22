#include "graphics.h"

void ncurses_settings() {
	cbreak();
	noecho();
	curs_set(0);
	nodelay(stdscr, true);

	start_color();
	init_pair(RED, COLOR_BLACK, COLOR_RED);
	init_pair(GREEN, COLOR_BLACK, COLOR_GREEN);
	init_pair(YELLOW, COLOR_BLACK, COLOR_YELLOW);
	init_pair(BLUE, COLOR_BLACK, COLOR_BLUE);
	init_pair(MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(CYAN, COLOR_BLACK, COLOR_CYAN);
	init_pair(WHITE, COLOR_BLACK, COLOR_WHITE);
}

static void draw_block(block* tet_block, colors block_color, int y_coord, int x_coord) {

	attron(COLOR_PAIR(block_color));
	mvprintw(y_coord + START_POINT_FIELD_Y + 1, (x_coord + 1) * 2 + START_POINT_FIELD_X - 1, "%c", tet_block->left_part);
	mvprintw(y_coord + START_POINT_FIELD_Y + 1, (x_coord + 1) * 2 + START_POINT_FIELD_X, "%c", tet_block->right_part);
	attroff(COLOR_PAIR(block_color));
	
}

static void draw_figure(figure* tet_figure, block* tet_block) {

	for (int i = 0; i < NUMBER_OF_COORDS; i++) {

		if (tet_figure->y_coord[i] > -1) {
			draw_block(tet_block, tet_figure->block_color, tet_figure->y_coord[i], tet_figure->x_coord[i]);         
		}

	}
}

static void draw_next_figure(figure* tet_figure, block* tet_block) {
	int temp_x = 0;
	int temp_y = 0;

	for (int i = 0; i < NUMBER_OF_COORDS; i++) {
		temp_x = tet_figure->x_coord[i] + FIELD_WIDTH - 2;
		temp_y = tet_figure->y_coord[i] + FIELD_HEIGHT / 2 + 2;
		draw_block(tet_block, tet_figure->block_color, temp_y, temp_x);         
	}

}

static void draw_matrix(block* tet_block, colors** matrix) {

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {

			if (matrix[i][j] != NON) {
				draw_block(tet_block, matrix[i][j], i, j);
			}

		}
	}
	
}

static void draw_field(field_settings* field) {

	for (int i = 0; i < FIELD_WIDTH * 2; i++) {
		mvprintw(START_POINT_FIELD_Y, START_POINT_FIELD_X + 1 + i, "%c", field->floor_ceiling_symbol);
		mvprintw(START_POINT_FIELD_Y + FIELD_HEIGHT + 1, START_POINT_FIELD_X + 1 + i, "%c", field->floor_ceiling_symbol);
	}

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		mvprintw(START_POINT_FIELD_Y + 1 + i, START_POINT_FIELD_X, "%c", field->wall_symbol);
		mvprintw(START_POINT_FIELD_Y + 1 + i, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 1, "%c", field->wall_symbol);
	}

	mvprintw(START_POINT_FIELD_Y, START_POINT_FIELD_X, "%c", field->angle_symbol);
	mvprintw(START_POINT_FIELD_Y, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 1, "%c", field->angle_symbol);
	mvprintw(START_POINT_FIELD_Y + FIELD_HEIGHT + 1, START_POINT_FIELD_X, "%c", field->angle_symbol);
	mvprintw(START_POINT_FIELD_Y + FIELD_HEIGHT + 1, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 1, "%c", field->angle_symbol);
}

static void draw_shadow(figure* tet_figure) {
	char symbol = '=';

	for (int i = 0; i < NUMBER_OF_COORDS; i++) {
		mvprintw(FIELD_HEIGHT + START_POINT_FIELD_Y + 1, (tet_figure->x_coord[i] + 1) * 2 + START_POINT_FIELD_X - 1, "%c", symbol);
		mvprintw(FIELD_HEIGHT + START_POINT_FIELD_Y + 1, (tet_figure->x_coord[i] + 1) * 2 + START_POINT_FIELD_X, "%c", symbol);
	}
	
}

static void printf_statistics(settings* game_settings, bool flag_levels) {
	mvprintw(START_POINT_FIELD_Y + 1, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Score: %d", game_settings->score);
	mvprintw(START_POINT_FIELD_Y + 2, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Lines: %d", game_settings->lines);
	mvprintw(START_POINT_FIELD_Y + FIELD_HEIGHT, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Speed: %d", game_settings->speed);
	mvprintw(START_POINT_FIELD_Y + FIELD_HEIGHT + 1, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Seed:  %d", game_settings->seed);

	if (flag_levels) {
		mvprintw(START_POINT_FIELD_Y, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Level: %d", game_settings->level);		
	}

}

void draw_game(field_settings* field, figure* tet_figure, figure* next_tet_figure, block* tet_block, colors** matrix, flags* gamerule_flags, settings* game_settings) {
	erase();

	draw_matrix(tet_block, matrix);
	draw_field(field);
	draw_figure(tet_figure, tet_block);
	printf_statistics(game_settings, gamerule_flags->levels);

	if (gamerule_flags->shadow) {
		draw_shadow(tet_figure);
	}

	if (gamerule_flags->next_figure) {
		mvprintw(START_POINT_FIELD_Y + (FIELD_HEIGHT / 2 - 1), START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Next Figure:");
		draw_next_figure(next_tet_figure, tet_block);
	}

	refresh();
}

void print_pause() {
	mvprintw(START_POINT_FIELD_Y + 3, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Game pause");
	refresh();
}

void game_over() {
	mvprintw(START_POINT_FIELD_Y + 4, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Game Over!\n");
	refresh();
	sleep(2);

	mvprintw(START_POINT_FIELD_Y + 6, START_POINT_FIELD_X + FIELD_WIDTH * 2 + 3, "Press any key for end!\n");
	refresh();

	nodelay(stdscr, false);
}

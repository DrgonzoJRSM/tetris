#include "includes.h"
#include "tetris.h"
#include "graphics.h"
#include "parse.h"

int main(int argc, char *argv[]) {
	settings game_settings = {
		.seed = 0,
		.speed = 30000,
		.score = 0, 
		.lines = 0, 
		.level = 1, 
		.count_lines_in_level = 15
	};

	flags gamerule_flags = {
		.shadow = true, 
		.next_figure = true, 
		.levels = true
	};

	if (!parse_args(argc, argv, &game_settings, &gamerule_flags)) {
		return EXIT_FAILURE;
	}

	if (game_settings.seed == 0) {
		game_settings.seed = generate_seed(); 
	}

	initscr();
	ncurses_settings();

	int clock_cycle = 0;
	int count_cycles_before_fixation = 0;

	char entered_command;

	bool block_lifecycle;
	bool game = true;

	field_settings field;
	block tet_block;
	figure tet_figure;
	figure next_tet_figure;

	colors** matrix = init_matrix();

	init_field(&field);
	init_block(&tet_block);
	init_figure(&tet_figure);
	init_figure(&next_tet_figure);

	while (game) {
		block_lifecycle = true;
		clock_cycle = 0;

		count_cycles_before_fixation = 10;

		while (block_lifecycle) {
			clock_cycle++;

			entered_command = getch();
			figure_movements(&tet_figure, matrix, entered_command);

			if (!game_pause(entered_command)) {

				while (getch() == ERR) {
					print_pause();

					usleep(game_settings.speed);
				}

			}

			if ((clock_cycle % 10 == 0) && check_drop(tet_figure.y_coord, tet_figure.x_coord, matrix)) {

				move_figure_down(tet_figure.y_coord);

			}

			if (!check_drop(tet_figure.y_coord, tet_figure.x_coord, matrix)) {

				if (count_cycles_before_fixation > 0) {
					count_cycles_before_fixation -= 1;

					draw_game(&field, &tet_figure, &next_tet_figure, &tet_block, matrix, &gamerule_flags, &game_settings);
					usleep(game_settings.speed);

					continue;
				}

				add_figure_in_matrix(&tet_figure, matrix);

				int count_of_completed_rows = remove_completed_rows(matrix);
				calculate_score(&game_settings, count_of_completed_rows);

				if (gamerule_flags.levels) {
					change_level(&game_settings);
				}

				block_lifecycle = false;
			} 

			draw_game(&field, &tet_figure, &next_tet_figure, &tet_block, matrix, &gamerule_flags, &game_settings);
			usleep(game_settings.speed);
		}

		tet_figure = next_tet_figure;
		init_figure(&next_tet_figure);

		game = check_end_game(matrix);

		if (!game) {
			game_over();

			getch();
		}
	}

	endwin();

	free_matrix(matrix);

	return 0;
}


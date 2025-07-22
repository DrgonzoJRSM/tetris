#include "tetris.h"

unsigned int generate_seed() {
	unsigned int seed;

	srand(time(NULL));
	seed = rand() % 100000;
	srand(seed);

	return seed;
}

colors** init_matrix() {
	colors** matrix = (colors**) malloc(FIELD_HEIGHT * sizeof(colors*));

	if (matrix == NULL) {
		fprintf(stderr, "Memore allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		matrix[i] = (colors*) malloc(FIELD_WIDTH * sizeof(colors));

		if (matrix[i] == NULL) {
			fprintf(stderr, "Memore allocation failed\n");

			for (int j = 0; j < i; j++) {
				free(matrix[j]);
			}

			exit(EXIT_FAILURE);
		}

	}

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			matrix[i][j] = NON;
		}
	}

	return matrix;
}

void init_field(field_settings* field) {
	field->wall_symbol = '|';
	field->floor_ceiling_symbol = '-';
	field->angle_symbol = '+';
}

void init_block(block* tet_block) {
	tet_block->left_part = '[';
	tet_block->right_part = ']';
}

void init_figure(figure* tet_figure) {
	tet_figure->type = (type_figure) rand() % NUMBER_OF_FIGURE;
	// tet_figure->type = T_TYPE;
	tet_figure->rotation_state = 0;

	switch (tet_figure->type) {
		case O_TYPE:
			tet_figure->block_color = MAGENTA;
			break;
		case I_TYPE:
			tet_figure->block_color = BLUE;
			break;
		case S_TYPE:
			tet_figure->block_color = GREEN;
			break;
		case Z_TYPE:
			tet_figure->block_color = RED;
			break;
		case L_TYPE:
			tet_figure->block_color = CYAN;
			break;
		case J_TYPE:
			tet_figure->block_color = YELLOW;
			break;
		case T_TYPE:
			tet_figure->block_color = WHITE;
			break;
	}

	int coords[NUMBER_OF_FIGURE][NUMBER_OF_COORDS][2] = {
		//O
		{	{4, -2}, {5, -2}, {4, -1}, {5, -1}	}, 
		//I
		{	{4, -1}, {3, -1}, {5, -1}, {6, -1}	},
		//S 
		{	{5, -2}, {6, -2}, {4, -1}, {5, -1}	}, 
		//Z
		{	{5, -2}, {4, -2}, {5, -1}, {6, -1}	}, 
		//L
		{	{5, -2}, {4, -2}, {6, -2}, {4, -1}	}, 
		//J
		{	{5, -2}, {4, -2}, {6, -2}, {6, -1}	},
		//T
		{	{5, -2}, {4, -2}, {6, -2}, {5, -1}	}, 
	};

	for (int i = 0; i < NUMBER_OF_COORDS; i++) {
		tet_figure->x_coord[i] = coords[tet_figure->type][i][0];
		tet_figure->y_coord[i] = coords[tet_figure->type][i][1];
	}

}

bool check_drop(int* y_coord, int* x_coord, colors** matrix) {

	for (int i = 0; i < NUMBER_OF_COORDS; i++) {

		if ((y_coord[i] + 1) == FIELD_HEIGHT) { 
			return false;
		} else if ((y_coord[i] >= 0) && (matrix[y_coord[i] + 1][x_coord[i]] != NON)) {
			return false;
		}

	}

	return true;
}

static bool figure_collison(int* x_coord, int* y_coord, colors** matrix, type_figure type, commands key) {

	if (key == J_KEY) {

		for (int i = 0; i < NUMBER_OF_COORDS; i++) {

			if (x_coord[i] < 1) {
				return false;
			} else if ((y_coord[i] >= 0) && (matrix[y_coord[i]][x_coord[i] - 1] != NON)) {
				return false;
			}

		}

	} else if (key == L_KEY) {

		for (int i = 0; i < NUMBER_OF_COORDS; i++) {

			if (x_coord[i] > 8) {
				return false;
			} else if ((y_coord[i] >= 0) && (matrix[y_coord[i]][x_coord[i] + 1] != NON)) {
				return false;
			}
			
		}

	} else if (key == K_KEY) {
		// Collision with walls
		bool condition_1 = (x_coord[0] < 1) || (x_coord[0] > 8);

		//Collision with the right wall for I_TYPE
		bool condition_2 = (type == I_TYPE) && (x_coord[0] > 7);

		//Collision with the block on the left
		bool condition_3 = (x_coord[0] > 1) && (y_coord[0] >= 0) && (matrix[y_coord[0]][x_coord[0] - 1] != NON);

		//Collision with the block on the right
		bool condition_4 = (x_coord[0] < 8) && (y_coord[0] >= 0) && (matrix[y_coord[0]][x_coord[0] + 1] != NON);

		//Collision with the block on the lright for I_TYPE
		bool condition_5 = (type == I_TYPE) && (x_coord[0] < 7) && (y_coord[0] >= 0) && ((matrix[y_coord[0]][x_coord[0] + 1] != NON) || (matrix[y_coord[0]][x_coord[0] + 2] != NON));

		//Collision with the floor for L_TYPE, J_TYPE and T_TYPE
		bool condition_6 = ((type == L_TYPE) || (type == J_TYPE) || (type == T_TYPE)) && (y_coord[0] > FIELD_HEIGHT -2);

		//Collision with the block from below for L_TYPE, J_TYPE and T_TYPE
		bool condition_7 = ((type == L_TYPE) || (type == J_TYPE) || (type == T_TYPE)) && (y_coord[0] < FIELD_HEIGHT - 2) && ((matrix[y_coord[0] + 1][x_coord[0]] != NON));

		//Collision with the floor for I_TYPE
		bool condition_8 = (type == I_TYPE) && (y_coord[0] > FIELD_HEIGHT - 3);

		//Collision with the block from below for I_TYPE
		bool condition_9 = (type == I_TYPE) && (y_coord[0] < FIELD_HEIGHT - 3) && ((matrix[y_coord[0] + 1][x_coord[0]] != NON) || (matrix[y_coord[0] + 2][x_coord[0]] != NON));

		return !(condition_1 || condition_2 || condition_3 || condition_4 || condition_5 || condition_6 || condition_7 || condition_8 || condition_9);
	} 
 	    
    return true;
}

static void move_left(figure* tet_figure, colors** matrix) {

	if (figure_collison(tet_figure->x_coord, tet_figure->y_coord, matrix, tet_figure->type, J_KEY)) {

		for (int i = 0; i < NUMBER_OF_COORDS; i++) {
			tet_figure->x_coord[i]--;
		}

	}
	
}

static void move_right(figure* tet_figure, colors** matrix) {

	if (figure_collison(tet_figure->x_coord, tet_figure->y_coord, matrix, tet_figure->type, L_KEY)) {

		for (int i = 0; i < NUMBER_OF_COORDS; i++) {
			tet_figure->x_coord[i]++;
		}

	}
	
}

static void drop_figure(figure* tet_figure, colors** matrix) {

	while (check_drop(tet_figure->y_coord, tet_figure->x_coord, matrix)) {

		for (int i = 0; i < NUMBER_OF_COORDS; i++) {
			tet_figure->y_coord[i]++;
		}

	}

}

static void turn_figure(figure* tet_figure, colors** matrix, commands key) {

	if ((tet_figure->type != O_TYPE) && (tet_figure->y_coord[0] >= 0)) {
		int new_coord_x[NUMBER_OF_COORDS] = {tet_figure->x_coord[0], 0, 0, 0};
		int new_coord_y[NUMBER_OF_COORDS] = {tet_figure->y_coord[0], 0, 0, 0};
		int temp_x = 0;
		int temp_y = 0;

		for (int i = 1; i < NUMBER_OF_COORDS; i++) {
			temp_x = tet_figure->x_coord[0] - tet_figure->x_coord[i];
			temp_y = tet_figure->y_coord[0] - tet_figure->y_coord[i];

			if ((tet_figure->type == S_TYPE || tet_figure->type == Z_TYPE || tet_figure->type == I_TYPE) && (tet_figure->rotation_state == 0)) {
				new_coord_x[i] = tet_figure->x_coord[0] + temp_y;
				new_coord_y[i] = tet_figure->y_coord[0] - temp_x;			
			} else if ((tet_figure->type == S_TYPE || tet_figure->type == Z_TYPE || tet_figure->type == I_TYPE) && (tet_figure->rotation_state == 1)) {
				new_coord_x[i] = tet_figure->x_coord[0] - temp_y;
				new_coord_y[i] = tet_figure->y_coord[0] + temp_x;
			} else {
				new_coord_x[i] = tet_figure->x_coord[0] - temp_y;
				new_coord_y[i] = tet_figure->y_coord[0] + temp_x;
			}                      
		} 

		if (figure_collison(new_coord_x, new_coord_y, matrix, tet_figure->type, key)) {

			if (tet_figure->type == S_TYPE || tet_figure->type == Z_TYPE || tet_figure->type == I_TYPE) {
				tet_figure->rotation_state = 1 - tet_figure->rotation_state;
			}

			for (int i = 0; i < NUMBER_OF_COORDS; i++) {
				tet_figure->x_coord[i] = new_coord_x[i];
				tet_figure->y_coord[i] = new_coord_y[i];
			}

		}
	}
	
}

void figure_movements(figure* tet_figure, colors** matrix, char entered_command) {

	switch (entered_command) {
		case 'j': case 'J':
			move_left(tet_figure, matrix);

			break;
		case 'l': case 'L':
			move_right(tet_figure, matrix);

			break;
		case ' ':
			drop_figure(tet_figure, matrix);

			break;
		case 'k': case 'K':
			turn_figure(tet_figure, matrix,  K_KEY);

			break;
	}
	
}

void move_figure_down(int* y_coord) {

	for (int i = 0; i < NUMBER_OF_COORDS; i++) {
		y_coord[i]++;
	}
	
}


bool game_pause(char entered_command) {

	if ((entered_command == 'p') || (entered_command == 'P')) {
		return false;	
	}

	return true;
}

void add_figure_in_matrix(figure* tet_figure, colors** matrix) {

	if (!check_drop(tet_figure->y_coord, tet_figure->x_coord, matrix)) {

		for (int i = 0; i < NUMBER_OF_COORDS; i++) {

			if (tet_figure->y_coord[i] >= 0) {
				matrix[tet_figure->y_coord[i]][tet_figure->x_coord[i]] = tet_figure->block_color;	
			}

		}

	}
	
}

int remove_completed_rows(colors** matrix) {
	int count_of_completed_rows = 0;
	bool flag = true;

	for (int i = FIELD_HEIGHT - 1; i > 0; i--) {
		flag = true;

		for (int j = 0; j < FIELD_WIDTH; j++) {

			if (matrix[i][j] == NON) {
				flag = false;
				break;
			}

		}

		if (flag) {

			for (int k = i; k > 0; k--) {
				for (int j = 0; j < FIELD_WIDTH; j++) {
					matrix[k][j] = matrix[k - 1][j];
				}
			}

			for (int j = 0; j < FIELD_WIDTH; j++) {
				matrix[0][j] = NON;
			}

			count_of_completed_rows++;
			i++;
		}
			
	}

	return count_of_completed_rows;
}

void calculate_score(settings* game_settings, int count_of_completed_rows) {
	game_settings->lines += count_of_completed_rows;

	switch (count_of_completed_rows) {
		case 1:
			game_settings->score += 100 * (game_settings->level);
			break;
		case 2:
			game_settings->score += 200 * (game_settings->level);
			break;
		case 3:
			game_settings->score += 500 * (game_settings->level);
			break;
		case 4:
			game_settings->score += 1000 * (game_settings->level);
			break;
	}
	
}

void change_level(settings* game_settings) {

	if ((game_settings->speed > 5000) && (game_settings->lines >= game_settings->level * game_settings->count_lines_in_level)) { 
		game_settings->speed -= 2000; 
		game_settings->level++; 
	}

}

bool check_end_game(colors** matrix) {

	for (int i = 0; i < FIELD_WIDTH; i++) {

		if (matrix[0][i] != NON) {
			return false;
		}

	}

	return true;
}

void free_matrix(colors** matrix) {

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		free(matrix[i]);
	}

	free(matrix);
}

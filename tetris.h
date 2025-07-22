#ifndef TETRIS_H
#define TETRIS_H

#include "includes.h"

#define FIELD_WIDTH 10  
#define FIELD_HEIGHT 20
#define NUMBER_OF_FIGURE 7
#define NUMBER_OF_COORDS 4

typedef struct {
	unsigned int seed;
	unsigned int speed;
	unsigned int score;
	unsigned int lines;
	unsigned int level;
	unsigned int count_lines_in_level;
} settings;

typedef struct {
	bool shadow;
	bool next_figure;
	bool levels;	
} flags;

typedef enum {
	NON,
	RED,
	BLUE,
	YELLOW,
	GREEN,
	MAGENTA,
	CYAN,
	WHITE, 
} colors;

typedef enum {
	O_TYPE,
	I_TYPE,
	S_TYPE,
	Z_TYPE,
	L_TYPE,
	J_TYPE,
	T_TYPE
} type_figure;

typedef enum {
	J_KEY,
	L_KEY,
	K_KEY,
	SPACE_KEY
} commands;

typedef struct {
	char left_part;
	char right_part;
} block;

typedef struct {
	char wall_symbol;
	char floor_ceiling_symbol;
	char angle_symbol;
} field_settings;

typedef struct {
	int x_coord[NUMBER_OF_COORDS];
	int y_coord[NUMBER_OF_COORDS];
	type_figure type;
	int rotation_state;		//for S_TYPE and Z_TYPE and I_TYPE	
	colors block_color;
} figure;

unsigned int generate_seed();

colors** init_matrix(void);
void init_field(field_settings*);
void init_block(block*);
void init_figure(figure*);

bool check_drop(int*, int*, colors**);
void figure_movements(figure*, colors**, char);
void move_figure_down(int*);

bool game_pause(char);

void add_figure_in_matrix(figure*, colors**);
int remove_completed_rows(colors**);
void calculate_score(settings*, int);
void change_level(settings*);

bool check_end_game(colors**);

void free_matrix(colors**);

#endif 

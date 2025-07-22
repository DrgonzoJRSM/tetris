#include "parse.h"

bool parse_args(int argc, char* argv[], settings* game_settings, flags* gamerule_flags) {
    static struct option long_options[] = {
    	{"help", no_argument, NULL, 'h'},
        {"speed", required_argument, NULL, 's'},
        {"number_of_rows", required_argument, NULL, 'c'},
        {"seed", required_argument, NULL, 'S'},
        {"falling_block", no_argument, NULL, 't'},
        {"next_figure", no_argument, NULL, 'n'},
        {"levels", no_argument, NULL, 'l'},
        {NULL, 0, NULL, 0}
    };

    int opt;

	while ((opt = getopt_long(argc, argv, "hs:c:S:tnl", long_options, NULL)) != -1) {

		switch (opt) {
			case 'h':
				printf("    _____ _____ _____ ____  ___ ____                                            \n");
				printf("   |_   _| ____|_   _|  _ \\|_ _/ ___|                                          \n");
				printf("     | | |  _|   | | | |_) || |\\___ \\                                         \n");
				printf("     | | | |___  | | |  _ < | | ___) |                                          \n");
				printf("     |_| |_____| |_| |_| \\_\\___|____/                                         \n");	
				printf("\n");
				printf("          _                ____                                                 \n");
				printf("         | |__  _   _     / ___| ___  _ __  _______   ___ ___  _ __ _ __        \n");
				printf("         | '_ \\| | | |   | |  _ / _ \\| '_ \\|_  / _ \\ / __/ _ \\| '__| '_ \\ \n");
				printf("         | |_) | |_| |   | |_| | (_) | | | |/ / (_) | (_| (_) | |  | |_) |      \n");
				printf("         |_.__/ \\__, |    \\____|\\___/|_| |_/___\\___(_)___\\___/|_|  | .__/  \n");
				printf("                |___/                                              |_|          \n");
				printf("\n");
				printf("Control:\n");
				printf("	J - left\n");
				printf("	L - right\n");
				printf("	K - turn the figure\n");
				printf("	SPACE -drop the figure\n");
				printf("	P - pause the game\n");
				printf("\n");
				printf("Usage: tetris <options>\n");
				printf("	-h					Show this help\n");
				printf("	-s <microseconds>			Set the falling speed of the pieces; (default - 30000)\n");
				printf("	-c <positive number>			Set number of rows per level; (default - 15)\n");
				printf("	-S <positive number>			Set seed for game; (default - rand)\n");
				printf("	-t					Disable falling block shadow\n");
				printf("	-n 					Disable next figure\n");
				printf("	-l 					Disable levels\n");

				return false;

				break;
			case 's':
				game_settings->speed = atoi(optarg);

				if (game_settings->speed <= 0) {
					fprintf(stderr, "Error: the speed must be a positive number.\n");
					return false; 
				} else if (game_settings->speed < 5000) {
					fprintf(stderr, "Error: the speed is too fast. (Limit 5000ms)\n");
					return false;
				}

				break; 
			case 'c':
				if (gamerule_flags->levels) {		
					game_settings->count_lines_in_level = atoi(optarg);

				if (game_settings->count_lines_in_level < 1) {
					fprintf(stderr, "Error: the number of rows per level must be a positive.\n");
					return false; 
				}

				} else {
						fprintf(stderr, "Error: Disable levels is turn on\n");
						return false; 
				}

				break;
			case 'S':
				game_settings->seed = atoi(optarg);

				if (game_settings->seed <= 0) {
					fprintf(stderr, "Error: the seed must be a positive number.\n");
					return false; 
				}

				break; 
			case 't':
				gamerule_flags->shadow = false;
				break;
			case 'n':
				gamerule_flags->next_figure = false;
				break;
			case 'l':
				gamerule_flags->levels = false;
				break;       	
			default:
				fprintf(stderr, "Error: Invalid parameter.\n");
				return false;
		}

	}

	if (optind < argc) {
		fprintf(stderr, "Error: Unrecognized argument: %s\n", argv[optind]);
		return false;
	}

	return true;
}

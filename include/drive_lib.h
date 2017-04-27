#include <stdio.h>
#include <conio.h>

typedef struct block {
	unsigned char bytes_s[512];
} block;

typedef struct  sectror_array {
	block sector[60];
} sectror_array;

typedef struct track_array {
	sectror_array track[5];
} track_array;

typedef struct  fatlist_s {
	unsigned int used;
	unsigned int eof;
	unsigned int next;
} fatlist;

typedef struct struct  fatent_s {
	char file_name[100];
	unsigned int first_sector;
} fatent;


void show_menu ();

void menu_selection (int menu_option);
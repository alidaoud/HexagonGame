﻿/*

                   ██╗  ██╗███████╗██╗  ██╗ █████╗  ██████╗  ██████╗ ███╗   ██╗     ██████╗  █████╗ ███╗   ███╗███████╗
				   ██║  ██║██╔════╝╚██╗██╔╝██╔══██╗██╔════╝ ██╔═══██╗████╗  ██║    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝
				   ███████║█████╗   ╚███╔╝ ███████║██║  ███╗██║   ██║██╔██╗ ██║    ██║  ███╗███████║██╔████╔██║█████╗
				   ██╔══██║██╔══╝   ██╔██╗ ██╔══██║██║   ██║██║   ██║██║╚██╗██║    ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝
				   ██║  ██║███████╗██╔╝ ██╗██║  ██║╚██████╔╝╚██████╔╝██║ ╚████║    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗
				   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝     ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝

*/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<allegro5\allegro.h>
#include<allegro5\bitmap_io.h>
#include <allegro5\allegro_ttf.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include<allegro5\allegro_native_dialog.h>

#define screenWidth 500
#define screenHeight 600

#define originX 120
#define originY 120

#define elemX 70
#define elemY 400

#define cellWidth 30
#define cellHeight 36


typedef enum CELL_PATTERN
{
	Column,Diagonal,invDiagonal,Col_Diagonal, Col_invDiagonal,Diag_InvDiagonal

}CELL_PATTERN;

typedef struct HEXAGONAL{

	int id;
	int value;
	bool filled;
	int x, y;
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_COLOR cellClr;

}HEXAGONAL;

typedef struct ELEMNET {

	int x, y, orgX, orgY;
	int id, type;
	bool exist, badElem;
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_COLOR elmClr;

}ELEMNET;

typedef struct ACTION {

	bool state;

}ACTION;

void Home(ALLEGRO_EVENT event);
void draw();
void init();
void cell_update();
void cols_init();
void diag_init();
void inv_diag_init();
void check(ALLEGRO_EVENT event);
void is_good_col();
void is_good_diag();
void is_good_inv_diag();
void game_loop(ALLEGRO_EVENT event);
void shutdown();
void check_move();
void rand_elem();
void menu(ALLEGRO_EVENT event);
void help(ALLEGRO_EVENT event);
void select_level(ALLEGRO_EVENT event);
void game_over(ALLEGRO_EVENT event);
void check_end();
void check_level();
void single_effect(CELL_PATTERN groupType, int , int, int);
void double_effect(CELL_PATTERN groupType, int, int, int, int, int ,int);
void hover(ALLEGRO_EVENT event);

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;

ALLEGRO_COLOR randColr[6];
ALLEGRO_COLOR background_color;

ALLEGRO_FONT *font, *font2;
ALLEGRO_FONT *menuFont, *helpFont, *homeFont;

ALLEGRO_BITMAP *bitmap, *test;
ALLEGRO_BITMAP *hexagonal;
ALLEGRO_BITMAP *gameOver;
ALLEGRO_BITMAP *backGround;
ALLEGRO_BITMAP *menuBitmap;
ALLEGRO_BITMAP *animCell;
ALLEGRO_BITMAP *icon[10];
ALLEGRO_BITMAP *elmBitmap[9];
ALLEGRO_BITMAP *helpBitmap[4];
ALLEGRO_BITMAP *homeOptions[4];
ALLEGRO_BITMAP *soundIcon;

ALLEGRO_SAMPLE *bgSound;
ALLEGRO_SAMPLE *clickSound;
ALLEGRO_SAMPLE *endSound;
ALLEGRO_SAMPLE *winSound;
ALLEGRO_SAMPLE *animSound;
ALLEGRO_SAMPLE *levelUp;
ALLEGRO_SAMPLE_ID *bgSnd;

ALLEGRO_MOUSE_STATE mState;

HEXAGONAL cell[37];
HEXAGONAL*col[7][7], *diag[7][7], *inv_diag[7][7];
ELEMNET elem[4], rndElem[9];
ACTION move[3];
CELL_PATTERN groupType;

bool done;
int j, i, k, activeElm, activeCell;
static int score = 0,highScore = 0,
stars = 0, gameLevel = 1, counter[8];


int main() {

	init();
	Home(event);
	draw();
	game_loop(event);	
	shutdown();

	return 0;
}


/* -----------------------Functions Declarations ----------------------- */


void init() {

	if (!al_init()) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Coudn't initialize Allegro !", NULL, NULL);
	}
	if (!al_install_keyboard()) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Coudn't initialize the keyboard !", NULL, NULL);
	}
	if (!al_install_mouse()) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Coudn't initialize the mouse !", NULL, NULL);
	}
	if (!al_install_audio()) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Coudn't initialize audio !", NULL, NULL);
	}

	timer = al_create_timer(1.0 / 60);

	if (!timer) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Failed to create timer !", NULL, NULL);
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(screenWidth, screenHeight);
	al_set_window_title(display, "Hexagonal");

	if (!display) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Failed to create display !", NULL, NULL);
	}
	
	queue = al_create_event_queue();

	if (!queue) {

		al_show_native_message_box(NULL, "Allegro Error", "Error", "Failed to create event queue !", NULL, NULL);
	}
	
	//initialize addons
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_acodec_addon();


	//---------loading fonts
	font = al_load_ttf_font("fonts/pirulen rg.ttf", 22, 0);
	font2 = al_load_ttf_font("fonts/impact.ttf", 35, 0);
	menuFont = al_load_ttf_font("fonts/PermanentMarker.ttf", 63, 0);
	helpFont = al_load_ttf_font("fonts/IndieFlower.ttf", 30, 0);
	homeFont = al_load_ttf_font("fonts/forte.ttf", 50, 0);
	//end of loading fonts

	//colors declarations
	randColr[0] = al_map_rgb(244, 47, 200);
	randColr[1] = al_map_rgb(212, 169, 51);
	randColr[2] = al_map_rgb(58, 215, 46);
	randColr[3] = al_map_rgb(255, 12, 12);
	randColr[4] = al_map_rgb(42, 199, 250);
	randColr[5] = al_map_rgb(56, 52, 241);
	background_color = al_map_rgb(32, 32, 30);
	//end of colors declarations

	//---------loading bitmaps
	bitmap = al_load_bitmap("bitmaps/elmCell.png");
	hexagonal = al_load_bitmap("bitmaps/Hexagonal.png");
	animCell = al_load_bitmap("bitmaps/anim.png");
	test = al_load_bitmap("bitmaps/opacity.png");

	gameOver = al_load_bitmap("bitmaps/gameOver.png");
	backGround = al_load_bitmap("bitmaps/backGround.png");
	menuBitmap = al_load_bitmap("bitmaps/menu.png");

	elmBitmap[0] = al_load_bitmap("bitmaps/elmCell.png");
	elmBitmap[1] = al_load_bitmap("bitmaps/2C01.png");
	elmBitmap[2] = al_load_bitmap("bitmaps/2C02.png");
	elmBitmap[3] = al_load_bitmap("bitmaps/2C03.png");
	elmBitmap[4] = al_load_bitmap("bitmaps/3C01.png");
	elmBitmap[5] = al_load_bitmap("bitmaps/3C02.png");
	elmBitmap[6] = al_load_bitmap("bitmaps/3C03.png");
	elmBitmap[7] = al_load_bitmap("bitmaps/3C04.png");
	elmBitmap[8] = al_load_bitmap("bitmaps/3C05.png");

	helpBitmap[0] = al_load_bitmap("bitmaps/help1.png");
	helpBitmap[1] = al_load_bitmap("bitmaps/help2.png");
	helpBitmap[2] = al_load_bitmap("bitmaps/help3.png");
	helpBitmap[3] = al_load_bitmap("bitmaps/help4.png");
	
	homeOptions[0] = al_load_bitmap("bitmaps/homeHeader.png");
	homeOptions[1] = al_load_bitmap("bitmaps/clickStart.png");
	homeOptions[2] = al_load_bitmap("bitmaps/selectLevel.png");
	homeOptions[3] = al_load_bitmap("bitmaps/quit.png");

	icon[0] = al_load_bitmap("bitmaps/pause.png");
	icon[1] = al_load_bitmap("bitmaps/back.png");
	icon[2] = al_load_bitmap("bitmaps/next.png");
	icon[3] = al_load_bitmap("bitmaps/back2.png");
	icon[4] = al_load_bitmap("bitmaps/hexagonIcon.png");
	icon[5] = al_load_bitmap("bitmaps/soundOn.png");
	icon[6] = al_load_bitmap("bitmaps/soundOff.png");
	icon[7] = al_load_bitmap("bitmaps/trashGroup.png");
	icon[8] = al_load_bitmap("bitmaps/star.png");
	
	soundIcon = al_clone_bitmap(icon[5]);
	//end of loading bitmaps

	//---------loading sampales
	bgSound = al_load_sample("sounds/sound.wav");
	clickSound = al_load_sample("sounds/click.wav");
	winSound = al_load_sample("sounds/laser.wav");
	endSound = al_load_sample("sounds/end.wav");
	animSound = al_load_sample("sounds/test1.wav");
	levelUp = al_load_sample("sounds/level_up.wav");
	//end of loading sampales
	al_reserve_samples(5);

	al_set_display_icon(display, icon[4]);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());

	done = false;

	move[0].state = false;
	move[1].state = false;
	move[2].state = false;

	elem[0].exist = true;
	elem[1].exist = true;
	elem[2].exist = true;

	cols_init();
	diag_init();
	inv_diag_init();
	cell_update();
}

void cols_init() {

	//initialize the 1st and 7th columns 
	for (i = 0, j = 0; i < 4, j < 4; i++, j++) {

		col[0][i] = &cell[j];
		col[6][i] = &cell[j + 33];
	}

	//initialize the 2nd and 6th columns 
	for (i = 0, j = 4; i < 5, j < 9; i++, j++) {

		col[1][i] = &cell[j];
		col[5][i] = &cell[j + 24];
	}

	//initialize the 3rd and 5th columns 
	for (i = 0, j = 9; i < 6, j < 15; i++, j++) {

		col[2][i] = &cell[j];
		col[4][i] = &cell[j + 13];
	}

	//initialize the 6th columns
	for (i = 0, j = 15; i < 7, j < 22; i++, j++) {

		col[3][i] = &cell[j];
	}


}

void diag_init() {

	for (i = 0, j = 3; i < 4, j >= 0; i++, j--) {

		diag[0][i] = col[j][0];
		diag[1][i + 1] = col[j][1];
		diag[2][i + 2] = col[j][2];
		diag[3][i + 3] = col[j][3];
		if (j > 0)
			diag[4][i + 3] = col[j][4];
		if (i < 2)
			diag[5][i + 3] = col[i + 2][5];
	}

	diag[1][0] = col[4][0];

	for (i = 0, j = 5; i < 2, j >= 4; i++, j--) { //init. the left elements of diag[2]

		diag[2][i] = col[j][i];
	}

	for (i = 0, j = 6; i < 3, j >= 4; i++, j--) { //init. the left element diag[3] & diag[4] & diag[5]

		diag[3][i] = col[j][i];
		diag[4][i] = col[j][i + 1];
		diag[5][i] = col[j][i + 2];
	}

	for (i = 0, j = 6; i < 4, j >= 3; i++, j--) { //init. diag[6]

		diag[6][i] = col[j][i + 3];
	}

}

void inv_diag_init() {


	for (i = 0, j = 3; i < 4, j < 7; i++, j++) {

		inv_diag[0][i] = col[j][0];
		inv_diag[1][i + 1] = col[j][1];
		inv_diag[2][i + 2] = col[j][2];
		inv_diag[3][i + 3] = col[j][3];
		if (j < 6)
			inv_diag[4][i + 3] = col[j][4];
		if (j < 2)
			inv_diag[5][i + 3] = col[j][5];
	}

	inv_diag[1][0] = col[2][0];

	for (i = 0, j = 1; i < 2, j < 3; i++, j++) { //init. the left elements of inv_diag[2]

		inv_diag[2][i] = col[j][i];
	}

	for (i = 0, j = 0; i < 3, j < 4; i++, j++) { //init. the left element inv_diag[3] & inv_diag[4] & inv_diag[5]

		inv_diag[3][i] = col[j][i];
		inv_diag[4][i] = col[j][i + 1];
		inv_diag[5][i] = col[j][i + 2];
	}

	for (i = 0, j = 0; i < 4, j < 4; i++, j++) { //init. inv_diag[6]

		inv_diag[6][i] = col[j][i + 3];
	}
	inv_diag[5][3] = col[3][5];
	inv_diag[5][4] = col[4][5];

}

void cell_update() {

	static bool game_started = false;

	for (i = 0; i < 37; i++) {

		if (!game_started) {

			cell[i].filled = false;
			cell[i].id = i;
			cell[i].cellClr = al_map_rgb(100, 50, 50);
			cell[i].bitmap = al_clone_bitmap(bitmap); //i moved the clone to this section to be done once when the game started to avoid overload memory
		}
		
		if (cell[i].filled == false) {

			if (cell[i].bitmap == animCell) {

				cell[i].bitmap = al_clone_bitmap(bitmap);
			}
			cell[i].cellClr = al_map_rgb(255, 255, 255);
			al_draw_tinted_bitmap(cell[i].bitmap, cell[i].cellClr, cell[i].x, cell[i].y, NULL);
		}
		if (cell[i].filled == true) {

			al_draw_tinted_bitmap(cell[i].bitmap, cell[i].cellClr, cell[i].x, cell[i].y, NULL);
			
		}
	}
	
	game_started = true;

} //no need to be called alone, check function calls it always

void draw() {

	static int cellX, cellY, elX, elY;
	static int redraw = 1;

	cellX = originX, cellY = originY;

	al_clear_to_color(background_color);
	al_draw_bitmap(icon[0], 440, 80, NULL);
	al_draw_bitmap(soundIcon, 440, 165, NULL);
	al_draw_bitmap(icon[7], 440, 240, NULL);
	al_draw_bitmap(icon[8], 440, 10, NULL);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 5, NULL, "SCORE: %d", score);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 45, NULL, "BEST: %d", highScore);

	if(stars <= 9)
		al_draw_textf(font, al_map_rgb(255, 255, 255), 410, 20, NULL, "%d", stars);
	else if(stars <= 99)
		al_draw_textf(font, al_map_rgb(255, 255, 255), 395, 20, NULL, "%d", stars);
	else if(stars >= 100)
		al_draw_textf(font, al_map_rgb(255, 255, 255), 380, 20, NULL, "%d", stars);


	// ======================  >>>>> Drawing the hexagonal <<<<< =========================

	for (j = 0; j < 4; j++) {

		al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY + 1, NULL);
		al_draw_tinted_bitmap(cell[j + 33].bitmap, cell[j + 33].cellClr, cellX + 198, cellY + 1, NULL);

		
		//Set the coordinates for each cell in the 1st and 7th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 33].x = cellX + 210.4;
		cell[j + 33].y = cellY + 1;

		cellY += 37; //increase the height value only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The 2nd and 6th columns
	cellX = originX + 33, cellY = originY - 17;

	for (j = 4; j < 9; j++) {

		al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY, NULL);
		al_draw_tinted_bitmap(cell[j + 24].bitmap, cell[j + 24].cellClr, cellX + 132, cellY, NULL);

		//Set the coordinates for each cell in the 2nd and 6th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 24].x = cellX + 140;
		cell[j + 24].y = cellY;

		cellY += 37.2; //increase the height values only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The 3rd and 5th columns 
	cellX = originX + 66, cellY = originY - 35;

	for (j = 9; j < 15; j++) {

		al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY, NULL);
		al_draw_tinted_bitmap(cell[j + 13].bitmap, cell[j + 13].cellClr, cellX + 66, cellY, NULL);

		//Set the coordinates for each cell in the 3rd and 5th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 13].x = cellX + 70;
		cell[j + 13].y = cellY + 1;

		cellY += 37.2; //increase the height values only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The last column (the middle one)
	cellX = originX + 99, cellY = originY - 53;

	for (j = 15; j < 22; j++) {

		al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY, NULL);

		//Setting the coordinates for each cell in the 4th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cellY += 37.2; //increase the height value only in order to continue drawing from top to the bottom
	}

	// ======================  >>>>> Drawing the Elements <<<<< =========================


	//drawing for the first time and init the coordinates for each element

	if (!move[0].state && !move[1].state && !move[2].state && !elem[0].exist && !elem[1].exist && !elem[2].exist) {
		redraw = 1;
	}

	if (!move[0].state && !move[1].state && !move[2].state && redraw) { //When Start


		rand_elem();
		elX = elemX, elY = elemY;

		for (i = 0; i < 3; i++) {

			al_draw_tinted_bitmap(elem[i].bitmap, elem[i].elmClr, elX, elY, NULL);
			elem[i].exist = true;

			//Setting the coordinates for each element 
			elem[i].x = elX;
			elem[i].y = elY;
			elem[i].orgX = elX;
			elem[i].orgY = elY;
			elX += 150; //increase the distance value only in order to continue drawing from left to the right

		}
		redraw = 0;
		printf("stilhere\n");
	}

	check_move();

	//drawing the moving element and the others

	for (i = 0; i < 3; i++) {

		if (!elem[i].exist && !move[i].state) {
			continue;
		}
		al_draw_tinted_bitmap(elem[i].bitmap, elem[i].elmClr, elem[i].x, elem[i].y, NULL);

	}
	al_flip_display();
}

void rand_elem() {


	srand(time(NULL));

	int e1, e2, e3;
	int c1, c2, c3;

	if (gameLevel == 1) {

		e1 = rand() % 9;
		e2 = rand() % 9;
		e3 = rand() % 9;
	}
	else if (gameLevel == 2) {

		do
		{
			e1 = rand() % 9;
			e2 = rand() % 9;
			e3 = rand() % 9;

		} while (e1 < 2 || e2 < 2 || e3 < 2);
	}
	else if (gameLevel >= 3) {

		do
		{
			e1 = rand() % 9;
			e2 = rand() % 9;
			e3 = rand() % 9;

		} while (e1 < 4 || e2 < 4 || e3 < 4);
	}


	c1 = rand() % 6;
	c2 = rand() % 6;
	c3 = rand() % 6;

	//init how many cell for each element
	for (i = 0; i < 9; i++) {

		if (i == 0) {
			rndElem[i].type = 1;
			rndElem[i].bitmap = elmBitmap[i];
		}
		if (i > 0 && i < 4) {
			rndElem[i].type = 2;
			rndElem[i].bitmap = elmBitmap[i];
		}
		if (i >= 4 && i < 9) {
			rndElem[i].type = 3;
			rndElem[i].bitmap = elmBitmap[i];
		}
		rndElem[i].id = i;
	}

	//printf("%d - %d - %d - %d", choice, e1, e2, e3);

	elem[0] = rndElem[e1];
	elem[1] = rndElem[e2];
	elem[2] = rndElem[e3];

	elem[0].elmClr = randColr[c1];
	elem[1].elmClr = randColr[c2];
	elem[2].elmClr = randColr[c3];

}

void check(ALLEGRO_EVENT event) {

	for (i = 0; i < 37; i++) {

		/*	if ((elem[3].x >= cell[i].x) && (elem[3].x <= cell[i].x + cellWidth) && (elem[3].y >= cell[i].y) && (elem[3].y <= cell[i].y + cellHeight)) {
		cell[i].cellClr = elem[activeElm].elmClr;
		}
		else {
		cell[i].cellClr = al_map_rgb(100, 50, 50);
		}*/

		if ((event.mouse.button & 1) && (!cell[i].filled) && (event.mouse.x >= cell[i].x) && (event.mouse.x <= cell[i].x + cellWidth) && (event.mouse.y >= cell[i].y) && (event.mouse.y <= cell[i].y + cellHeight) && ((move[0].state && !move[1].state && !move[2].state) || (!move[0].state && move[1].state && !move[2].state) || (!move[0].state && !move[1].state && move[2].state))) {

			if (elem[activeElm].type == 1) {

				cell[i].filled = true;

				cell[i].cellClr = elem[activeElm].elmClr;

				score++;

				if (cell[i].filled = true) {

					elem[activeElm].exist = false;
					move[activeElm].state = false;

					printf("el[%d].x = %d -- el[%d].y = %d", i, elem[activeElm].x, i, elem[activeElm].y);
					printf("activEL is  %d\n", activeElm);
					printf("exist[%d] = %d", i, elem[i].exist);

				}
			}
			else if (elem[activeElm].type == 2) {

				switch (elem[activeElm].id)
				{
				case 1://======================  >>>>> CASE 1 <<<<< =========================

					if (cell[i].id == 3 || cell[i].id == 8 || cell[i].id == 14 || cell[i].id == 21 || cell[i].id == 27 || cell[i].id == 32 || cell[i].id == 36) {
						continue;
					}
					if (cell[i + 1].filled) {
						continue;
					}
					cell[i].filled = true;
					cell[i + 1].filled = true;

					cell[i].cellClr = elem[activeElm].elmClr;
					cell[i + 1].cellClr = elem[activeElm].elmClr;

					score += 2;

					if (cell[i].filled && cell[i + 1].filled) {

						elem[activeElm].exist = false;
						move[activeElm].state = false;
					}
					break;

				case 2://======================  >>>>> CASE 2 <<<<< =========================

					if ((cell[i].id >= 32 && cell[i].id <= 36) || cell[i].id == 21 || cell[i].id == 27) {
						continue;
					}
					if ((cell[i].id >= 0 && cell[i].id <= 3) || (cell[i].id >= 28 && cell[i].id <= 31)) {

						if (cell[i + 5].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 5].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 5].cellClr = elem[activeElm].elmClr;

						score += 2;

						if (cell[i].filled && cell[i + 5].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 2nd and 7th columns

					if ((cell[i].id >= 4 && cell[i].id <= 8) || (cell[i].id >= 22 && cell[i].id <= 27)) {

						if (cell[i + 6].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 6].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;

						score += 2;

						if (cell[i].filled && cell[i + 6].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 3rd and 6th columns

					if ((cell[i].id >= 9 && cell[i].id <= 21)) {

						if (cell[i + 7].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 7].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;

						score += 2;

						if (cell[i].filled && cell[i + 7].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 4th and 5th columns

					break;

				case 3://======================  >>>>> CASE 3 <<<<< =========================

					if ((cell[i].id >= 0 && cell[i].id <=3) || cell[i].id == 8 || cell[i].id == 14 || cell[i].id == 21) {
						continue;
					}
					if ((cell[i].id >= 4 && cell[i].id <= 7) || (cell[i].id >= 33 && cell[i].id <= 36)) {

						if (cell[i - 4].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 4].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 4].cellClr = elem[activeElm].elmClr;

						score += 2;

						if (cell[i].filled && cell[i - 4].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 2nd and 5th columns

					if ((cell[i].id >= 9 && cell[i].id <= 13) || (cell[i].id >= 28 && cell[i].id <= 32)) {

						if (cell[i - 5].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 5].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 5].cellClr = elem[activeElm].elmClr;

						score += 2;

						if (cell[i].filled && cell[i - 5].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 3rd and 6th columns

					if ((cell[i].id >= 15 && cell[i].id <= 20) || (cell[i].id >= 22 && cell[i].id <= 27)) {

						if (cell[i - 6].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 6].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 6].cellClr = elem[activeElm].elmClr;

						score += 2;

						if (cell[i].filled && cell[i - 6].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 3rd and 4th columns

					break;

				default:
					break;
				}
			}
			else if (elem[activeElm].type == 3) {

				switch (elem[activeElm].id)
				{
				case 4: //======================  >>>>> CASE 4 <<<<< =========================

					if (cell[i].id == 3 || cell[i].id == 3 || cell[i].id == 7 || cell[i].id == 8 || cell[i].id == 13 || cell[i].id == 14 || cell[i].id == 20 || cell[i].id == 21 || cell[i].id == 26 || cell[i].id == 27 || cell[i].id == 31 || cell[i].id == 32 || cell[i].id == 35 || cell[i].id == 36) {
						continue;
					}
					if (cell[i + 1].filled || cell[i + 2].filled) {
						continue;
					}
					cell[i].filled = true;
					cell[i + 1].filled = true;
					cell[i + 2].filled = true;

					cell[i].cellClr = elem[activeElm].elmClr;
					cell[i + 1].cellClr = elem[activeElm].elmClr;
					cell[i + 2].cellClr = elem[activeElm].elmClr;

					score += 3;

					if (cell[i].filled && cell[i + 1].filled && cell[i + 2].filled) {

						elem[activeElm].exist = false;
						move[activeElm].state = false;
					}
					break;

				case 5://======================  >>>>> CASE 5 <<<<< =========================

					if ((cell[i].id >= 26 && cell[i].id <= 36) || cell[i].id == 20 || cell[i].id == 21 || cell[i].id == 14) {
						continue;
					}
					if (cell[i].id >= 0 && cell[i].id <= 3) {

						if (cell[i + 5].filled || cell[i + 11].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 5].filled = true;
						cell[i + 11].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 5].cellClr = elem[activeElm].elmClr;
						cell[i + 11].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 5].filled && cell[i + 11].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end 1st column

					if (cell[i].id >= 4 && cell[i].id <= 8) {

						if (cell[i + 6].filled || cell[i + 13].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 6].filled = true;
						cell[i + 13].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;
						cell[i + 13].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 6].filled && cell[i + 13].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 2nd column

					if ((cell[i].id >= 9 && cell[i].id <= 13)) {

						if (cell[i + 7].filled || cell[i + 14].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 7].filled = true;
						cell[i + 14].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;
						cell[i + 14].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 7].filled && cell[i + 14].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 3rd column

					if ((cell[i].id >= 15 && cell[i].id <= 19)) {

						if (cell[i + 7].filled || cell[i + 13].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 7].filled = true;
						cell[i + 13].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;
						cell[i + 13].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 7].filled && cell[i + 13].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 4th column

					if ((cell[i].id >= 22 && cell[i].id <= 25)) {

						if (cell[i + 6].filled || cell[i + 11].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 6].filled = true;
						cell[i + 11].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;
						cell[i + 11].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i + 6].filled && cell[i + 11].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 5th column

					break;

				case 6: //======================  >>>>> CASE 6 <<<<< =========================

					if ((cell[i].id >= 0 && cell[i].id <= 8) || cell[i].id == 13 || cell[i].id == 14 || cell[i].id == 20 || cell[i].id == 21 || cell[i].id == 27) {
						continue;
					}
					if (cell[i].id >= 9 && cell[i].id <= 12) {

						if (cell[i - 5].filled || cell[i - 9].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 5].filled = true;
						cell[i - 9].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 5].cellClr = elem[activeElm].elmClr;
						cell[i - 9].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 5].filled && cell[i - 9].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end 3rd column

					if (cell[i].id >= 15 && cell[i].id <= 19) {

						if (cell[i - 6].filled || cell[i - 11].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 6].filled = true;
						cell[i - 11].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 6].cellClr = elem[activeElm].elmClr;
						cell[i - 11].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 6].filled && cell[i - 11].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 4th column

					if ((cell[i].id >= 22 && cell[i].id <= 26)) {

						if (cell[i - 6].filled || cell[i - 12].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 6].filled = true;
						cell[i - 12].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 6].cellClr = elem[activeElm].elmClr;
						cell[i - 12].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 6].filled && cell[i - 12].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 5th column

					if ((cell[i].id >= 28 && cell[i].id <= 32)) {

						if (cell[i - 5].filled || cell[i - 11].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 5].filled = true;
						cell[i - 11].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 5].cellClr = elem[activeElm].elmClr;
						cell[i - 11].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 5].filled && cell[i - 11].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 6th column

					if ((cell[i].id >= 33 && cell[i].id <= 36)) {

						if (cell[i - 4].filled || cell[i - 9].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 4].filled = true;
						cell[i - 9].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 4].cellClr = elem[activeElm].elmClr;
						cell[i - 9].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 4].filled && cell[i - 9].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 7th column

					break;

				case 7: //======================  >>>>> CASE 7 <<<<< =========================

					if ((cell[i].id >= 27 && cell[i].id <= 36) || cell[i].id == 15 || cell[i].id == 21 || cell[i].id == 22) {
						continue;
					}
					if (cell[i].id >= 0 && cell[i].id <= 3) {

						if (cell[i + 5].filled || cell[i + 10].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 5].filled = true;
						cell[i + 10].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 5].cellClr = elem[activeElm].elmClr;
						cell[i + 10].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 5].filled && cell[i + 10].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end 1st column

					if (cell[i].id >= 4 && cell[i].id <= 8) {

						if (cell[i + 6].filled || cell[i + 12].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 6].filled = true;
						cell[i + 12].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;
						cell[i + 12].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 6].filled && cell[i + 12].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 2nd column

					if ((cell[i].id >= 9 && cell[i].id <= 14)) {

						if (cell[i + 7].filled || cell[i + 13].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 7].filled = true;
						cell[i + 13].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;
						cell[i + 13].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 7].filled && cell[i + 13].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 3rd column

					if ((cell[i].id >= 16 && cell[i].id <= 20)) {

						if (cell[i + 7].filled || cell[i + 12].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 7].filled = true;
						cell[i + 12].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;
						cell[i + 12].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 7].filled && cell[i + 12].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 4th column

					if ((cell[i].id >= 23 && cell[i].id <= 26)) {

						if (cell[i + 6].filled || cell[i + 10].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i + 6].filled = true;
						cell[i + 10].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;
						cell[i + 10].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i + 6].filled && cell[i + 10].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 5th column

					break;

				case 8: //======================  >>>>> CASE 8 <<<<< =========================

					if ((cell[i].id >= 0 && cell[i].id <= 3) || (cell[i].id >= 30 && cell[i].id <= 36) || cell[i].id == 8 || cell[i].id == 14 || cell[i].id == 21 || cell[i].id == 27 || cell[i].id == 32) {
						continue;
					}
					if (cell[i].id >= 4 && cell[i].id <= 7) {

						if (cell[i - 4].filled || cell[i + 6].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 4].filled = true;
						cell[i + 6].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 4].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 4].filled && cell[i + 6].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end 2nd column

					if (cell[i].id >= 9 && cell[i].id <= 13) {

						if (cell[i - 5].filled || cell[i + 7].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 5].filled = true;
						cell[i + 7].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 5].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 5].filled && cell[i + 7].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 3d column

					if ((cell[i].id >= 15 && cell[i].id <= 20)) {

						if (cell[i - 6].filled || cell[i + 7].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 6].filled = true;
						cell[i + 7].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 6].cellClr = elem[activeElm].elmClr;
						cell[i + 7].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 6].filled && cell[i + 7].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 4th column

					if ((cell[i].id >= 22 && cell[i].id <= 26)) {

						if (cell[i - 6].filled || cell[i + 6].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 6].filled = true;
						cell[i + 6].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 6].cellClr = elem[activeElm].elmClr;
						cell[i + 6].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 6].filled && cell[i + 6].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 5th column

					if ((cell[i].id >= 28 && cell[i].id <= 31)) {

						if (cell[i - 5].filled || cell[i + 5].filled) {
							continue;
						}
						cell[i].filled = true;
						cell[i - 5].filled = true;
						cell[i + 5].filled = true;

						cell[i].cellClr = elem[activeElm].elmClr;
						cell[i - 5].cellClr = elem[activeElm].elmClr;
						cell[i + 5].cellClr = elem[activeElm].elmClr;

						score += 3;

						if (cell[i].filled && cell[i - 5].filled && cell[i + 5].filled) {

							elem[activeElm].exist = false;
							move[activeElm].state = false;
						}
					}//end of 6th column

					break;
				case 9:

					break;
				default:
					break;
				}

			}
			printf("cell[%d].filled = %d\n", i, cell[i].filled);
			printf("Your Score is = %d\n", score);
			cell_update();
			draw();
			is_good_col();
			is_good_diag();
			is_good_inv_diag();
			cell_update();
			//check_end();
			//check_end();
		}//
	}
	
}

void check_move() {

	//Check the first Element
	if (move[0].state && !move[1].state && !move[2].state) {

		if (elem[0].bitmap != elmBitmap[3] && elem[0].bitmap != elmBitmap[6] && elem[0].bitmap != elmBitmap[8]) {

			elem[0].x = elem[3].x - 15;
			elem[0].y = elem[3].y - 15;
		}
		else if (elem[0].bitmap == elmBitmap[3] || elem[0].bitmap == elmBitmap[8]) {

			elem[0].x = elem[3].x - 35;
			elem[0].y = elem[3].y - 15;
		}
		else if (elem[0].bitmap == elmBitmap[6]) {

			elem[0].x = elem[3].x - 70;
			elem[0].y = elem[3].y - 15;
		}

	}

	//Check the second Element
	if (move[1].state && !move[0].state && !move[2].state) {

		if (elem[1].bitmap != elmBitmap[3] && elem[1].bitmap != elmBitmap[6] && elem[1].bitmap != elmBitmap[8]) {

			elem[1].x = elem[3].x - 15;
			elem[1].y = elem[3].y - 15;
		}
		else if (elem[1].bitmap == elmBitmap[3] || elem[1].bitmap == elmBitmap[8]) {

			elem[1].x = elem[3].x - 35;
			elem[1].y = elem[3].y - 15;
		}
		else if (elem[1].bitmap == elmBitmap[6]) {

			elem[1].x = elem[3].x - 70;
			elem[1].y = elem[3].y - 15;
		}
	}

	//Check the third Element
	if (move[2].state && !move[0].state && !move[1].state) {

		if (elem[2].bitmap != elmBitmap[3] && elem[2].bitmap != elmBitmap[6] && elem[2].bitmap != elmBitmap[8]) {

			elem[2].x = elem[3].x - 15;
			elem[2].y = elem[3].y - 15;
		}
		else if (elem[2].bitmap == elmBitmap[3] || elem[2].bitmap == elmBitmap[8]) {

			elem[2].x = elem[3].x - 35;
			elem[2].y = elem[3].y - 15;
		}
		else if (elem[2].bitmap == elmBitmap[6]) {

			elem[2].x = elem[3].x - 70;
			elem[2].y = elem[3].y - 15;
		}
	}
}

void check_end() {


	for (i = 0; i < 9; i++) {
		counter[i] = 0;
	}

	//1C01 & 2C01 & 3C01 cases
	for (i = 0; i < 37; i++) {
		////----------1C01 case
		if (!cell[i].filled) {
			counter[0]++;
		}
		////----------2C01 case
		if (!cell[i].filled && !cell[i + 1].filled) {
			if (cell[i].id == 3 || cell[i].id == 8 || cell[i].id == 14 || cell[i].id == 21 || cell[i].id == 27 || cell[i].id == 32 || cell[i].id == 36)
				continue;

			counter[1]++;
		}
		////----------3C01 case
		if (!cell[i].filled && !cell[i + 1].filled && !cell[i + 2].filled) {
			if (cell[i].id == 2 || cell[i].id == 3 || cell[i].id == 7 || cell[i].id == 8 || cell[i].id == 13 || cell[i].id == 14 || cell[i].id == 20 || cell[i].id == 21 || cell[i].id == 26 || cell[i].id == 27 || cell[i].id == 31 || cell[i].id == 32 || cell[i].id == 35 || cell[i].id == 36)
				continue;

			counter[4]++;
		}
	}

	//----------2C02 case
	for (i = 0; i < 6; i++) {

		if ((i < 3) && ((!inv_diag[0][i]->filled && !inv_diag[0][i + 1]->filled) || (!inv_diag[6][i]->filled && !inv_diag[6][i + 1]->filled))) {

			counter[2]++;
		}//end of 1st and 7th inv_diagonals

		if ((i < 4) && ((!inv_diag[1][i]->filled && !inv_diag[1][i + 1]->filled) || (!inv_diag[5][i]->filled && !inv_diag[5][i + 1]->filled))) {

			counter[2]++;
		}//end of 2st and 6th inv_diagonals

		if ((i < 5) && ((!inv_diag[2][i]->filled && !inv_diag[2][i + 1]->filled) || (!inv_diag[4][i]->filled && !inv_diag[4][i + 1]->filled))) {

			counter[2]++;
		}//end of 3st and 5th inv_diagonals
		if (!inv_diag[3][i]->filled && !inv_diag[3][i + 1]->filled) {

			counter[2]++;
		}//end of 4th inv_diagonals
	}


	//----------2C03 case
	for (i = 0; i < 6; i++) {

		if ((i < 3) && ((!diag[0][i]->filled && !diag[0][i + 1]->filled) || (!diag[6][i]->filled && !diag[6][i + 1]->filled))) {

			counter[3]++;
		}//end of 1st and 7th diagonals

		if ((i < 4) && ((!diag[1][i]->filled && !diag[1][i + 1]->filled) || (!diag[5][i]->filled && !diag[5][i + 1]->filled))) {

			counter[3]++;
		}//end of 2st and 6th diagonals

		if ((i < 5) && ((!diag[2][i]->filled && !diag[2][i + 1]->filled) || (!diag[4][i]->filled && !diag[4][i + 1]->filled))) {

			counter[3]++;
		}//end of 3st and 5th diagonals
		if (!diag[3][i]->filled && !diag[3][i + 1]->filled) {

			counter[3]++;
		}//end of 4th diagonals
	}

	//----------3C02 case
	for (i = 0; i < 5; i++) {

		if ((i < 2) && ((!inv_diag[0][i]->filled && !inv_diag[0][i + 1]->filled && !inv_diag[0][i + 2]->filled) || (!inv_diag[6][i]->filled && !inv_diag[6][i + 1]->filled && !inv_diag[6][i + 2]->filled))) {

			counter[5]++;
		}//end of 1st and 7th inv_diagonals

		if ((i < 3) && ((!inv_diag[1][i]->filled && !inv_diag[1][i + 1]->filled && !inv_diag[1][i + 2]->filled) || (!inv_diag[5][i]->filled && !inv_diag[5][i + 1]->filled && !inv_diag[5][i + 2]->filled))) {

			counter[5]++;
		}//end of 2st and 6th inv_diagonals

		if ((i < 4) && ((!inv_diag[2][i]->filled && !inv_diag[2][i + 1]->filled && !inv_diag[2][i + 2]->filled) || (!inv_diag[4][i]->filled && !inv_diag[4][i + 1]->filled && !inv_diag[4][i + 2]->filled))) {

			counter[5]++;
		}//end of 3st and 5th inv_diagonals

		if (!inv_diag[3][i]->filled && !inv_diag[3][i + 1]->filled && !inv_diag[3][i + 2]->filled) {

			counter[5]++;
		}//end of 4th inv_diagonals
	}

	//----------3C03 case
	for (i = 0; i < 5; i++) {

		if ((i < 2) && ((!diag[0][i]->filled && !diag[0][i + 1]->filled && !diag[0][i + 2]->filled) || (!diag[6][i]->filled && !diag[6][i + 1]->filled && !diag[6][i + 2]->filled))) {

			counter[6]++;
		}//end of 1st and 7th diagonals

		if ((i < 3) && ((!diag[1][i]->filled && !diag[1][i + 1]->filled && !diag[1][i + 2]->filled) || (!diag[5][i]->filled && !diag[5][i + 1]->filled && !diag[5][i + 2]->filled))) {

			counter[6]++;
		}//end of 2st and 6th diagonals

		if ((i < 4) && ((!diag[2][i]->filled && !diag[2][i + 1]->filled && !diag[2][i + 2]->filled) || (!diag[4][i]->filled && !diag[4][i + 1]->filled && !diag[4][i + 2]->filled))) {

			counter[6]++;
		}//end of 3st and 5th diagonals

		if (!diag[3][i]->filled && !diag[3][i + 1]->filled && !diag[3][i + 2]->filled) {

			counter[6]++;
		}//end of 4th diagonals
	}

	//----------3C04 & 3C05 cases
	for (i = 0; i < 6; i++) {

		//---3C04
		if ((i < 4) && ((!col[0][i]->filled && !col[1][i + 1]->filled && !col[2][i + 1]->filled) || (!col[4][i + 1]->filled && !col[5][i + 1]->filled && !col[6][i]->filled))) {
			counter[7]++;
		}
		if ((i < 5) && ((!col[1][i]->filled && !col[2][i + 1]->filled && !col[3][i + 1]->filled) || (!col[3][i + 1]->filled && !col[4][i + 1]->filled && !col[5][i]->filled))) {
			counter[7]++;
		}
		if (!col[2][i]->filled && !col[3][i + 1]->filled && !col[4][i]->filled) {
			counter[7]++;
		}

		//---3C05
		if ((i < 4) && ((!col[0][i]->filled && !col[1][i]->filled && !col[2][i + 1]->filled) || (!col[4][i + 1]->filled && !col[5][i]->filled && !col[6][i]->filled))) {
			counter[8]++;
		}
		if ((i < 5) && ((!col[1][i]->filled && !col[2][i]->filled && !col[3][i + 1]->filled) || (!col[3][i + 1]->filled && !col[4][i]->filled && !col[5][i]->filled))) {
			counter[8]++;
		}
		if (!col[2][i]->filled && !col[3][i]->filled && !col[4][i]->filled) {
			counter[8]++;
		}
	}

	//rating the elements whether bad or not
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 9; j++) {

			if (elem[i].id == j && counter[j] == 0) {
				elem[i].badElem = true;
			}
		}
	}

	//for check only
	for (i = 0; i < 9; i++) {
		printf("counter[%d] = %d\n", i, counter[i]);
	}
	for (i = 0; i < 3; i++) {
		printf("elem[%d].badElem = %d\n", i, elem[i].badElem);
	}
	//

	cell_update();

	j = 0;
	if ((elem[j].badElem && elem[++j].badElem && elem[++j].badElem) || (elem[0].badElem && !elem[1].exist && !elem[2].exist) || (elem[1].badElem && !elem[0].exist && !elem[2].exist) || (elem[2].badElem && !elem[0].exist && !elem[1].exist) || (elem[0].badElem && elem[1].badElem && !elem[2].exist) || (elem[0].badElem && elem[2].badElem && !elem[1].exist) || (elem[1].badElem && elem[2].badElem && !elem[0].exist)) {
		printf("not true\n");
		game_over(event);
	}
	else {
		for (j = 0; j < 3; j++) {

			elem[j].badElem == false;
		}
		printf("all false\n");
	}
}

void check_level() {

	static int countr = 0;

	if (score >= gameLevel * 100 && gameLevel < 4) {

		gameLevel++;


		for (i = 0; i < 2; i++) {

			al_play_sample(levelUp, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

			al_draw_textf(homeFont, al_map_rgb(255, 0, 0), 125, 150, 0, "LEVEL UP !");
			al_flip_display();
			al_rest(0.5);

			al_draw_textf(homeFont, al_map_rgb(255, 240, 0), 125, 150, 0, "LEVEL UP !");
			al_flip_display();
			al_rest(0.5);
		}

		printf("game level is %d\n\n", gameLevel);
	}

}

void is_good_col() {

	bool noCompanion = true;

	//>>>> ====================== Checking The columns ========================= <<<<

	//>> -------check the 1st column and its companions ----------- <<
	i = 0;
	if (col[0][i]->filled == true && col[0][++i]->filled == true && col[0][++i]->filled == true && col[0][++i]->filled == true) {

		//single_effect(Column, 0, 0, 3);
		//double_effect();
		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		//cell_update();
		for (i = 0; i < 4; i++) {

			col[0][i]->filled = false;
			score++;
		}

		i = 0;
		//---------------check the companion diagonals for the 1st column
		//check the 1st diagonal
		if (diag[0][i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true && !diag[0][++i]->filled == true) {

			double_effect(Col_Diagonal, 0, 0, 3, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[0][i]->filled = false;
				score++;
			}
		}
		//check the 2nd diagonal
		i = 0;
		if (diag[1][i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && !diag[1][++i]->filled == true) {

			double_effect(Col_Diagonal, 0, 0, 3, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[1][i]->filled = false;
				score++;
			}
		}
		i = 0;
		//check the 3rd diagonal
		i = 0;
		if (diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && !diag[2][++i]->filled == true) {

			double_effect(Col_Diagonal, 0, 0, 3, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[2][i]->filled = false;
				score++;
			}
		}
		i = 0;
		//check the 4th diagonal
		i = 0;
		if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && !diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 0, 0, 3, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
		}

		i = 0;
		//--------------check the inverse companion diagonals for the 1st column
		//check the 4th inv_diagonal
		i = 0;
		if (!inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 0, 0, 3, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (!inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			double_effect(Col_invDiagonal, 0, 0, 3, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (!inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			double_effect(Col_invDiagonal, 0, 0, 3, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}
		//check the 7th inv_diagonal
		i = 0;
		if (!inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

			double_effect(Col_invDiagonal, 0, 0, 3, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}

		if (noCompanion) {
			single_effect(Column, 0, 0, 3);
		}

	}//end of 1st column


	 //>> -------check the 2nd column and its companions ----------- <<
	i = 0;
	if (col[1][i]->filled == true && col[1][++i]->filled == true && col[1][++i]->filled == true && col[1][++i]->filled == true && col[1][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 5; i++) {

			col[1][i]->filled = false;
			score++;
		}
		//---------------check the companion diagonals for the 2nd column
		//check the 1st diagonal
		i = 0;
		if (diag[0][i]->filled == true && diag[0][++i]->filled == true && !diag[0][++i]->filled == true && diag[0][++i]->filled == true) {

			double_effect(Col_Diagonal, 1, 0, 4, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[0][i]->filled = false;
				score++;
			}
		}
		i = 0;

		//check the 2nd diagonal
		i = 0;
		if (diag[1][i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && !diag[1][++i]->filled == true && diag[1][++i]->filled == true) {

			double_effect(Col_Diagonal, 1, 0, 4, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[1][i]->filled = false;
				score++;
			}
		}//end of 2nd diagonal


		 //check the 3rd diagonal
		i = 0;
		if (diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && !diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

			double_effect(Col_Diagonal, 1, 0, 4, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[2][i]->filled = false;
				score++;
			}
		}//end of 3rd diagonal


		 //check the 4th diagonal
		i = 0;
		if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && !diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 1, 0, 4, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
		}//end of 4th diagonal


		 //check the 5th diagonal
		i = 0;
		if (diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && !diag[4][++i]->filled == true) {

			double_effect(Col_Diagonal, 1, 0, 4, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[4][i]->filled = false;
				score++;
			}
		}//end of 5th diagonal

		//--------------check the inverse companion diagonals for the 2nd column
		//check the 3rd inv_diagonal
		i = 0;
		if (!inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			double_effect(Col_invDiagonal, 1, 0, 4, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 1, 0, 4, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			double_effect(Col_invDiagonal, 1, 0, 4, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && !inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			double_effect(Col_invDiagonal, 1, 0, 4, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (inv_diag[6][i]->filled == true && !inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

			double_effect(Col_invDiagonal, 1, 0, 4, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}

		if (noCompanion) {
			single_effect(Column, 1, 0, 4);
		}
	}//---------end of 2nd column


	 //>> -------check the 3rd column and its companions ----------- <<
	i = 0;
	if (col[2][i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 6; i++) {

			col[2][i]->filled = false;
			score++;
		}
		i = 0;
		//-------------check the companion diagonals for the 3rd column
		//check the 1st diagonal
		i = 0;
		if (diag[0][i]->filled == true && !diag[0][++i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true) {

			double_effect(Col_Diagonal, 2, 0, 5, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[0][i]->filled = false;
				score++;
			}
		}

		 //check the 2nd diagonal
		i = 0;
		if (diag[1][i]->filled == true && diag[1][++i]->filled == true && !diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true) {

			double_effect(Col_Diagonal, 2, 0, 5, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[1][i]->filled = false;
				score++;
			}
		}

		 //check the 3rd diagonal
		i = 0;
		if (diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && !diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

			double_effect(Col_Diagonal, 2, 0, 5, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[2][i]->filled = false;
				score++;
			}
		}

		 //check the 4th diagonal
		i = 0;
		if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && !diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 2, 0, 5, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
			i = 0;
		}

		 //check the 5th diagonal
		i = 0;
		if (diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && !diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

			double_effect(Col_Diagonal, 2, 0, 5, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[4][i]->filled = false;
				score++;
			}
		}

		 //check the 6th diagonal
		i = 0;
		if (diag[5][i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && !diag[5][++i]->filled == true) {

			double_effect(Col_Diagonal, 2, 0, 5, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[5][i]->filled = false;
				score++;
			}
		}//end of 6th diagonal


		i = 0;
		//------------check the inverse companion diagonals for the 3rd column
		//check the 2nd inv_diagonal
		i = 0;
		if (!inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			double_effect(Col_invDiagonal, 2, 0, 5, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			double_effect(Col_invDiagonal, 2, 0, 5, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 2, 0, 5, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			double_effect(Col_invDiagonal, 2, 0, 5, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && !inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			double_effect(Col_invDiagonal, 2, 0, 5, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && !inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

			double_effect(Col_invDiagonal, 2, 0, 5, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}
		if (noCompanion) {
			single_effect(Column, 2, 0, 5);
		}
	}//end of 3rd column


	 //>> -------check the 4th column and its companions ----------- <<
	i = 0;
	if (col[3][i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 7; i++) {

			col[3][i]->filled = false;
			score++;
		}

		//--------------check the companion diagonals for the 4th column
		//check the 1st diagonal
		i = 0;
		if (!diag[0][i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[0][i]->filled = false;
				score++;
			}
		}

		 //check the 2nd diagonal
		i = 0;
		if (diag[1][i]->filled == true && !diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[1][i]->filled = false;
				score++;
			}
		}

		 //check the 3rd diagonal
		i = 0;
		if (diag[2][i]->filled == true && diag[2][++i]->filled == true && !diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[2][i]->filled = false;
				score++;
			}
		}

		 //check the 4th diagonal
		i = 0;
		if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && !diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
		}

		 //check the 5th diagonal
		i = 0;
		if (diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && !diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[4][i]->filled = false;
				score++;
			}
		}

		 //check the 6th diagonal
		i = 0;
		if (diag[5][i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && !diag[5][++i]->filled == true && diag[5][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[5][i]->filled = false;
				score++;
			}
		}

		 //check the 7th diagonal
		i = 0;
		if (diag[6][i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true && !diag[6][++i]->filled == true) {

			double_effect(Col_Diagonal, 3, 0, 6, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[6][i]->filled = false;
				score++;
			}
		}

		//--------------check the inverse companion diagonals for the 4th column
		//check the 1st inv_diagonal
		i = 0;
		if (!inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && !inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && !inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && !inv_diag[6][++i]->filled == true) {

			double_effect(Col_invDiagonal, 3, 0, 6, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}
		if (noCompanion) {
			single_effect(Column, 3, 0, 6);
		}
	}//-------------end of 4th column


	 //>> -------check the 5th column and its companions ----------- <<
	i = 0;
	if (col[4][i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 6; i++) {

			col[4][i]->filled = false;
			score++;
		}
		i = 0;

		//--------------check the companion diagonal for the 5th column
		//check the 2nd diagonal
		i = 0;
		if (!diag[1][i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true) {

			double_effect(Col_Diagonal, 4, 0, 5, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[1][i]->filled = false;
				score++;
			}
		}

		 //check the 3rd diagonal
		i = 0;
		if (diag[2][i]->filled == true && !diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

			double_effect(Col_Diagonal, 4, 0, 5, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[2][i]->filled = false;
				score++;
			}
		}

		 //check the 4th diagonal
		i = 0;
		if (diag[3][i]->filled == true && diag[3][++i]->filled == true && !diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 4, 0, 5, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
		}

		 //check the 5th diagonal
		i = 0;
		if (diag[4][i]->filled == true && diag[4][++i]->filled == true && !diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

			double_effect(Col_Diagonal, 4, 0, 5, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[4][i]->filled = false;
				score++;
			}
		}

		 //check the 6th diagonal
		i = 0;
		if (diag[5][i]->filled == true && diag[5][++i]->filled == true && !diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true) {

			double_effect(Col_Diagonal, 4, 0, 5, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[5][i]->filled = false;
				score++;
			}
		}

		 //check the 7th diagonal
		i = 0;
		if (diag[6][i]->filled == true && diag[6][++i]->filled == true && !diag[6][++i]->filled == true && diag[6][++i]->filled == true) {

			double_effect(Col_Diagonal, 4, 0, 5, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[6][i]->filled = false;
				score++;
			}
		}

		//--------------check the inverse companion diagonal for the 5th column
		//check the 1st inv_diagonal
		i = 0;
		if (inv_diag[0][i]->filled == true && !inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {

			double_effect(Col_invDiagonal, 4, 0, 5, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && !inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			double_effect(Col_invDiagonal, 4, 0, 5, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			double_effect(Col_invDiagonal, 4, 0, 5, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 4, 0, 5, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			double_effect(Col_invDiagonal, 4, 0, 5, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && !inv_diag[5][++i]->filled == true) {

			double_effect(Col_invDiagonal, 4, 0, 5, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}
		if (noCompanion) {
			single_effect(Column, 4, 0, 5);
		}
	}//---------end of 5th column


	 //>> -------check the 6th column and its companions ----------- <<
	i = 0;
	if (col[5][i]->filled == true && col[5][++i]->filled == true && col[5][++i]->filled == true && col[5][++i]->filled == true && col[5][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 5; i++) {

			col[5][i]->filled = false;
			score++;
		}
		i = 0;
		//---------------check the companion diagonals for the 6th column
		//check the 3rd diagonal
		i = 0;
		if (!diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

			double_effect(Col_Diagonal, 5, 0, 4, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[2][i]->filled = false;
				score++;
			}
		}

		 //check the 4th diagonal
		i = 0;
		if (diag[3][i]->filled == true && !diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 5, 0, 4, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
		}

		 //check the 5th diagonal
		i = 0;
		if (diag[4][i]->filled == true && !diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

			double_effect(Col_Diagonal, 5, 0, 4, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[4][i]->filled = false;
				score++;
			}
		}

		 //check the 6th diagonal
		i = 0;
		if (diag[5][i]->filled == true && !diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true) {

			double_effect(Col_Diagonal, 5, 0, 4, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[5][i]->filled = false;
				score++;
			}
		}

		 //check the 7th diagonal
		i = 0;
		if (diag[6][i]->filled == true && !diag[6][++i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true) {

			double_effect(Col_Diagonal, 5, 0, 4, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[6][i]->filled = false;
				score++;
			}
		}

		//---------------check the inverse companion diagonals for the 6th column
		//check the 1st inv_diagonal
		i = 0;
		if (inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && !inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {

			double_effect(Col_invDiagonal, 5, 0, 4, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && !inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			double_effect(Col_invDiagonal, 5, 0, 4, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			double_effect(Col_invDiagonal, 5, 0, 4, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 5, 0, 4, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true) {

			double_effect(Col_invDiagonal, 5, 0, 4, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}
		if (noCompanion) {
			single_effect(Column, 5, 0, 4);
		}
	}//end of 6th column


	 //>> -------check the 7th column and its companions ----------- <<
	i = 0;
	if (col[6][i]->filled == true && col[6][++i]->filled == true && col[6][++i]->filled == true && col[6][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

		for (i = 0; i < 4; i++) {

			col[6][i]->filled = false;
			score++;
		}

		i = 0;
		//--------------check the companion diagonal for the 7th column
		//check the 4th diagonal
		i = 0;
		if (!diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

			double_effect(Col_Diagonal, 6, 0, 3, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				diag[3][i]->filled = false;
				score++;
			}
		}

		 //check the 5th diagonal
		i = 0;
		if (!diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

			double_effect(Col_Diagonal, 6, 0, 3, 4, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				diag[4][i]->filled = false;
				score++;
			}
		}

		 //check the 6th diagonal
		i = 0;
		if (!diag[5][i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true) {

			double_effect(Col_Diagonal, 6, 0, 3, 5, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				diag[5][i]->filled = false;
				score++;
			}
		}

		 //check the 7th diagonal
		i = 0;
		if (!diag[6][i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true) {

			double_effect(Col_Diagonal, 6, 0, 3, 6, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				diag[6][i]->filled = false;
				score++;
			}
		}//end of 7th diagonal

		//--------------check the inverse companion diagonal for the 7th column
		//check the 1st inv_diagonal

		i = 0;
		if (inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && !inv_diag[0][++i]->filled == true) {

			double_effect(Col_invDiagonal, 6, 0, 3, 0, 0, 3);
			noCompanion = false;
			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && !inv_diag[1][++i]->filled == true) {

			double_effect(Col_invDiagonal, 6, 0, 3, 1, 0, 4);
			noCompanion = false;
			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true) {

			double_effect(Col_invDiagonal, 6, 0, 3, 2, 0, 5);
			noCompanion = false;
			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true) {

			double_effect(Col_invDiagonal, 6, 0, 3, 3, 0, 6);
			noCompanion = false;
			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}
		if (noCompanion) {
			single_effect(Column, 6, 0, 3);
		}
	}//end of 7th column
	cell_update();
	check_end();

}

void is_good_diag() {

	//>>>> ====================== Checking The diagonals ========================= <<<<

	//>> -------check the 1st diagonal and its companions ----------- <<

	i = 0;
	if (diag[0][i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true) {
		
		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 4; i++) {

			diag[0][i]->filled = false;
			score++;
		}
		i = 0;
		//------------check the inverse companion diagonals for the 1st diagonal
		//check the 1st inv_diagonal
		i = 0;
		if (!inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (!inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (!inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (!inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}
	}//end of 1st diagonal
	

	 //>> -------check the 2nd diagonal and its companions ----------- <<

	i = 0;
	if (diag[1][i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 5; i++) {

			diag[1][i]->filled = false;
			score++;
		}

		//----------check the inverse companion diagonals for the 2nd diagonal
		//check the 1st inv_diagonal
		i = 0;
		if (inv_diag[0][i]->filled == true && !inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && !inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (!inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}
	}//end of 2nd diagonal
	

	 //>> -------check the 3rd diagonal and its companions ----------- <<
	i = 0;
	if (diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 6; i++) {

			diag[2][i]->filled = false;
			score++;
		}
		i = 0;
		//--------check the inverse companion diagonals for the 3rd diagonal
		//check the 1st inv_diagonal
		i = 0;
		if (inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && !inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && !inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (!inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}
	}//end of 3rd diagonal


	 //>> -------check the 4th diagonal and its companions ----------- <<
	i = 0;
	if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 7; i++) {

			diag[3][i]->filled = false;
			score++;
		}
		i = 0;
		//--------check the inverse companion diagonals for the 4th diagonal
		//check the 1st inv_diagonal
		i = 0;
		if (inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && !inv_diag[0][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[0][i]->filled = false;
				score++;
			}
		}

		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && !inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && !inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (!inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}
	}//end of 4th diagonal


	 //>> -------check the 5th diagonal and its companions ----------- <<
	i = 0;
	if (diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 6; i++) {

			diag[4][i]->filled = false;
			score++;
		}
		i = 0;
		//---------check the inverse companion diagonals for the 5th diagonal
		//check the 2nd inv_diagonal
		i = 0;
		if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && !inv_diag[1][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[1][i]->filled = false;
				score++;
			}
		}

		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && !inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (inv_diag[6][i]->filled == true && !inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}
	}//end of 5th diagonal


	 //>> -------check the 6th diagonal and its companions ----------- <<
	i = 0;
	if (diag[5][i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true) {

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 5; i++) {

			diag[5][i]->filled = false;
			score++;
		}
		i = 0;
		//-------check the inverse companion diagonals for the 6th diagonal
		//check the 3rd inv_diagonal
		i = 0;
		if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && !inv_diag[2][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[2][i]->filled = false;
				score++;
			}
		}

		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && !inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && !inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}
	}//end of 6th diagonal


	 //>> -------check the 7th diagonal and its companions ----------- <<
	i = 0;
	if (diag[6][i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true) {

		for (i = 0; i < 4; i++) {

			printf("WOW ! \n");
			al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			diag[6][i]->filled = false;
			score++;
		}
		i = 0;
		//-----------check the inverse companion diagonals for the 7th diagonal
		//check the 4th inv_diagonal
		i = 0;
		if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && !inv_diag[3][++i]->filled == true) {

			for (i = 0; i < 7; i++) {

				inv_diag[3][i]->filled = false;
				score++;
			}
		}

		//check the 5th inv_diagonal
		i = 0;
		if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && !inv_diag[4][++i]->filled == true) {

			for (i = 0; i < 6; i++) {

				inv_diag[4][i]->filled = false;
				score++;
			}
		}

		//check the 6th inv_diagonal
		i = 0;
		if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && !inv_diag[5][++i]->filled == true) {

			for (i = 0; i < 5; i++) {

				inv_diag[5][i]->filled = false;
				score++;
			}
		}

		//check the 7th inv_diagonal
		i = 0;
		if (inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && !inv_diag[6][++i]->filled == true) {

			for (i = 0; i < 4; i++) {

				inv_diag[6][i]->filled = false;
				score++;
			}
		}
	}//end of 7th diagonal
	check_end();

}

void is_good_inv_diag() {

	//>> ----------------------- Checking The inv_diagonals ----------------------- <<

	//check the 1st inv_diagonal
	i = 0;
	if (inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {
		
		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			inv_diag[0][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 2nd inv_diagonal
	i = 0;
	if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			inv_diag[1][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 3rd inv_diagonal
	i = 0;
	if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			inv_diag[2][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 4th inv_diagonal
	i = 0;
	if (inv_diag[3][i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true && inv_diag[3][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 7; i++) {

			inv_diag[3][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 5th inv_diagonal
	i = 0;
	if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			inv_diag[4][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 6th inv_diagonal
	i = 0;
	if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			inv_diag[5][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 7th inv_diagonal
	i = 0;
	if (inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			inv_diag[6][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	check_end();

}

void game_loop(ALLEGRO_EVENT event) {

	static bool soundOn = true;

	al_start_timer(timer);

	if (soundOn)
		//al_play_sample(bgSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, bgSnd);

	while (!done) {

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			//done = true;
			shutdown();
			exit(-1);
		}

		/*if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			//done = true;
			shutdown();
			exit(-1);
		}*/
		
		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {

			elem[3].x = event.mouse.x;
			elem[3].y = event.mouse.y;
			//hover(event);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {


			 if ((event.mouse.button & 1) && (event.mouse.x >= 440) && (event.mouse.x <= 490) && (event.mouse.y >= 80) && (event.mouse.y <= 120)) {
			
				 al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				 printf("pause!\n");
				 menu(event);
			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 440) && (event.mouse.x <= 490) && (event.mouse.y >= 165) && (event.mouse.y <= 215)) {

				al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				if (!soundOn) {

					soundOn = true;
					soundIcon = al_clone_bitmap(icon[5]);
					//al_play_sample(bgSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, bgSnd);
				}
				else if (soundOn){
					
					soundOn = false;
					soundIcon = al_clone_bitmap(icon[6]);
					al_play_sample(bgSound, 0, 0.0, 0, ALLEGRO_PLAYMODE_ONCE, bgSnd);
					//al_stop_sample(bgSnd);
				}
			}
			else if ((event.mouse.button & 1) && (stars >= 20) && (event.mouse.x >= 440) && (event.mouse.x <= 490) && (event.mouse.y >= 240) && (event.mouse.y <= 290)) {

				al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				rand_elem();
				stars -= 20;
			}

			for (i = 0; i < 3; i++) {

				if ((event.mouse.button & 1) && (elem[i].bitmap != elmBitmap[3]) && (elem[i].bitmap != elmBitmap[6]) && (elem[i].bitmap != elmBitmap[8]) && (event.mouse.x >= elem[i].x) && (event.mouse.x <= elem[i].x + cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight)) {

					move[i].state = true;
					al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

					//check_move();
					if (move[i].state) {

						activeElm = i;
					}
					printf("1stMove[%d].state =  %d\n", i, move[i].state);
					printf("click %d\n", i);
					printf("activEL is  %d\n", activeElm);
					printf("exist[%d] = %d\n", i, elem[i].exist);
				}
				else if ((event.mouse.button & 1) && (elem[i].bitmap == elmBitmap[3] || elem[i].bitmap == elmBitmap[8]) && (event.mouse.x >= elem[i].x + 30) && (event.mouse.x <= elem[i].x + 2 * cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight)) {

					move[i].state = true;
					al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

					//check_move();
					if (move[i].state) {

						activeElm = i;
					}
				}
				else if ((event.mouse.button & 1) && (elem[i].bitmap == elmBitmap[6]) && (event.mouse.x >= elem[i].x + 60) && (event.mouse.x <= elem[i].x + 3 * cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight)) {

					move[i].state = true;
					al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

					//check_move();
					if (move[i].state) {

						activeElm = i;
					}
				}
				else if (event.mouse.button & 2) // Add Here more cond. in order to avoid problem that happens when two element become over each other
				{
					move[i].state = false;
					elem[i].x = elem[i].orgX;
					elem[i].y = elem[i].orgY;
				}
			}
			check_end();
			check(event); //the order is very important here check function must be before the check_end function definetly
			check_end();
			//check(event); 
			

		}
		draw();
		hover(event);
		check_level();
	}
	printf("done!");
}

void Home(ALLEGRO_EVENT event) {

	bool started = false;

	while (!started)
	{
		al_wait_for_event(queue, &event);

		//al_clear_to_color(al_map_rgb(175, 219, 238));
		//al_draw_bitmap(hexagonal, originX, originY - 50, NULL);

		al_draw_bitmap(backGround, 0, 0, 0);
		al_draw_tinted_bitmap(homeOptions[0], al_map_rgb(255, 255, 255), 5, 75, NULL);
		al_draw_tinted_bitmap(homeOptions[1], al_map_rgb(169, 169, 169), 120, 250, NULL);
		al_draw_tinted_bitmap(homeOptions[2], al_map_rgb(169, 169, 169), 120, 350, NULL);
		al_draw_tinted_bitmap(homeOptions[3], al_map_rgb(169, 169, 169), 120, 450, NULL);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			shutdown();
			exit(-1);
		}

		if ((event.mouse.x >= 120) && (event.mouse.x <= 380) && (event.mouse.y >= 250) && (event.mouse.y <= 285)) {

			al_draw_tinted_bitmap(homeOptions[1], al_map_rgb(108, 0, 255), 120, 250, NULL);
			
			if (event.mouse.button & 1) {
				al_play_sample(clickSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				started = true;
			}
		}
		if ((event.mouse.x >= 140) && (event.mouse.x <= 360) && (event.mouse.y >= 350) && (event.mouse.y <= 385)) {

			al_draw_tinted_bitmap(homeOptions[2], al_map_rgb(108, 0, 255), 120, 350, NULL);
			
			if (event.mouse.button & 1) {
				al_play_sample(clickSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				printf("level\n");
				select_level(event);
			}
		}
		if ((event.mouse.x >= 200) && (event.mouse.x <= 290) && (event.mouse.y >= 450) && (event.mouse.y <= 485)) {

			al_draw_tinted_bitmap(homeOptions[3], al_map_rgb(108, 0, 255), 120, 450, NULL);
			
			if (event.mouse.button & 1) {
				al_play_sample(clickSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				shutdown();
				exit(-1);
			}
		}
		al_flip_display();
	}

}

void select_level(ALLEGRO_EVENT event) {

	bool done = false;

	while (!done)
	{
		al_wait_for_event(queue, &event);

		al_draw_bitmap(backGround, 0, 0, 0);
		al_draw_textf(homeFont, al_map_rgb(169, 169, 169), 200, 190, 0, "Easy");
		al_draw_textf(homeFont, al_map_rgb(169, 169, 169), 180, 290, 0, "Normal");
		al_draw_textf(homeFont, al_map_rgb(169, 169, 169), 200, 390, 0, "Hard");


		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			shutdown();
			exit(-1);
		}
		if ((event.mouse.x >= 200) && (event.mouse.x <= 340) && (event.mouse.y >= 190) && (event.mouse.y <= 240)) {

			al_draw_textf(homeFont, al_map_rgb(108, 0, 255), 200, 190, 0, "Easy");
			
			if (event.mouse.button & 1) {
				al_play_sample(clickSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				gameLevel = 1;
				done = true;
			}
		}
		if ((event.mouse.x >= 180) && (event.mouse.x <= 360) && (event.mouse.y >= 290) && (event.mouse.y <= 340)) {

			al_draw_textf(homeFont, al_map_rgb(108, 0, 255), 180, 290, 0, "Normal");
			
			if (event.mouse.button & 1) {
				al_play_sample(clickSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				gameLevel = 2;
				done = true;
			}
		}
		if ((event.mouse.x >= 200) && (event.mouse.x <= 340) && (event.mouse.y >= 390) && (event.mouse.y <= 440)) {

			al_draw_textf(homeFont, al_map_rgb(108, 0, 255), 200, 390, 0, "Hard");
			
			if (event.mouse.button & 1) {
				al_play_sample(clickSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				gameLevel = 3;
				done = true;
				
			}
		}
		al_flip_display();
	}
	printf("level = %d", gameLevel);
}

void game_over(ALLEGRO_EVENT event) {

	static bool lost = false;

	al_rest(1.0);
	al_play_sample(endSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	al_draw_bitmap(gameOver, 0, 0, NULL);
	al_draw_textf(font2, al_map_rgb(106, 222, 103), 335, 278, NULL, "%d", score);
	
	lost = true;

	while (lost)
	{

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			shutdown();
			exit(-1);
		}

		if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			//done = true;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {

			//al_draw_tinted_bitmap(startClick, al_map_rgb(31, 251, 165), buttonX, buttonY, NULL);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if ((event.mouse.button & 1) && (event.mouse.x >= 204) && (event.mouse.x <= 303) && (event.mouse.y >= 370) && (event.mouse.y <= 408)) {
				score = 0;

				elem[0].exist = false;
				elem[1].exist = false;
				elem[2].exist = false;

				for (i = 0; i < 37; i++) {
					cell[i].filled = false;
				}
				cell_update();
				printf("retry\n");
				game_loop(event);

			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 218) && (event.mouse.x <= 290) && (event.mouse.y >= 450) && (event.mouse.y <= 485)) {

				printf("Quit\n");
				shutdown();
				exit(-1);
			}
		}
		stars = score / 10;
		printf("stars is %d\n", stars);
		al_flip_display();
	}

}

void menu(ALLEGRO_EVENT event) {

	bool pause = true;

	al_draw_bitmap(menuBitmap, 0, 0, NULL);

	while (pause)
	{

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			shutdown();
			exit(-1);
		}

		if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			//done = true;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if ((event.mouse.button & 1) && (event.mouse.x >= 125) && (event.mouse.x <= 360) && (event.mouse.y >= 125) && (event.mouse.y <= 170)) {

				printf("resume\n");
				pause = false;
			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 125) && (event.mouse.x <= 360) && (event.mouse.y >= 235) && (event.mouse.y <= 273)) {

				printf("setting\n");
			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 175) && (event.mouse.x <= 318) && (event.mouse.y >= 330) && (event.mouse.y <= 370)) {

				printf("Help\n");
				pause = false;
				help(event);
			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 185) && (event.mouse.x <= 314) && (event.mouse.y >= 425) && (event.mouse.y <= 465)) {

				printf("Exit\n");
				pause = false;
				shutdown();
				exit(-1);
				
			}
		}
		al_flip_display();
	}


 }

void help(ALLEGRO_EVENT event) {

	bool help = true;
	int page = 1;

	al_clear_to_color(al_map_rgb(53, 58, 53));
	al_draw_bitmap(icon[1], 10, 540, 0); //next arrow
	al_draw_bitmap(icon[2], 440, 540, 0); //back arrow
	al_draw_bitmap(icon[3], 440, 10, 0); //back to main menu

	while (help)
	{
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			shutdown();
			exit(-1);
		}

		if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			//done = true;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if ((event.mouse.button & 1) && (event.mouse.x >= 10) && (event.mouse.x <= 60) && (event.mouse.y >= 540) && (event.mouse.y <= 590)) {

				if (page > 1)
					page--;
				else
					continue;

			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 440) && (event.mouse.x <= 490) && (event.mouse.y >= 540) && (event.mouse.y <= 590)) {

				if (page < 4)
					page++;
				else
					continue;
				
			}else if ((event.mouse.button & 1) && (event.mouse.x >= 440) && (event.mouse.x <= 490) && (event.mouse.y >= 10) && (event.mouse.y <= 60)) {

				game_loop(event);
			}
		}
		
		if (page == 1) {
			al_clear_to_color(al_map_rgb(53, 58, 53));
			al_draw_bitmap(helpBitmap[0], 53, 65, 0);

			al_draw_bitmap(icon[2], 440, 540, 0); //next arrow
			al_draw_bitmap(icon[3], 440, 10, 0); //back to main menu

			al_draw_textf(font2, al_map_rgb(139, 153, 53), 10, 10, 0, "Case 1:");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 340, 0, "1- Try to fill any column as shown ");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 370, 0, "above");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 410, 0, "2- No metter if the colors are ");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 440, 0, "different");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 480, 0, "3- One element can destroy more than");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 510, 0, "one column");
		}
		else if (page == 2) {

			al_clear_to_color(al_map_rgb(53, 58, 53));
			al_draw_bitmap(helpBitmap[1], 53, 66, 0);

			al_draw_bitmap(icon[1], 10, 540, 0); //back arrow
			al_draw_bitmap(icon[2], 440, 540, 0); //next arrow
			al_draw_bitmap(icon[3], 440, 10, 0); //back to main menu

			al_draw_textf(font2, al_map_rgb(139, 153, 53), 10, 10, 0, "Case 2:");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 340, 0, "1- Try to fill any diagonal as shown ");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 370, 0, "above");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 410, 0, "2- No metter if the colors are ");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 440, 0, "different");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 480, 0, "3- One element can destroy more than");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 510, 0, "one diagonal");
		}
		else if (page == 3) {

			al_clear_to_color(al_map_rgb(53, 58, 53));
			al_draw_bitmap(helpBitmap[2], 53, 64, 0);

			al_draw_bitmap(icon[1], 10, 540, 0); //back arrow
			al_draw_bitmap(icon[2], 440, 540, 0); //next arrow
			al_draw_bitmap(icon[3], 440, 10, 0); //back to main menu

			al_draw_textf(font2, al_map_rgb(139, 153, 53), 10, 10, 0, "Case 3:");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 340, 0, "1- Try to fill any inverse diagonal ");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 370, 0, "as shown above");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 410, 0, "2- No metter if the colors are ");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 440, 0, "different");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 480, 0, "3- One element can destroy more than");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 40, 510, 0, "one inverse diagonal");
		}
		else if (page == 4) {

			al_clear_to_color(al_map_rgb(53, 58, 53));
			al_draw_bitmap(helpBitmap[3], 53, 65, 0);

			al_draw_bitmap(icon[1], 10, 540, 0); //back arrow
			al_draw_bitmap(icon[3], 440, 10, 0); //back to main menu

			al_draw_textf(font2, al_map_rgb(139, 153, 53), 10, 10, 0, "Special cases:");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 340, 0, "Don't forget to try the special cases");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 25, 370, 0, "to destroy more cells");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 410, 0, "You can try:");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 450, 0, "* column and diagonal");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 480, 0, "* column and inverse diagonal");
			al_draw_textf(helpFont, al_map_rgb(163, 157, 155), 10, 510, 0, "* column, diagonal, and inverse diagonal");
		}

		al_flip_display();
	}



}
		
void setting(ALLEGRO_EVENT event) {

	//waiting for solving stop samples


}

void single_effect(CELL_PATTERN groupType, int rowNumber, int START, int END) {


	if (groupType == Column) {

		for (i = START; i <= END; i++) {

			col[rowNumber][i]->bitmap = al_clone_bitmap(animCell);
			al_draw_tinted_bitmap(col[rowNumber][i]->bitmap, col[rowNumber][i]->cellClr, col[rowNumber][i]->x - 10, col[rowNumber][i]->y - 10, NULL);
			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			al_rest(0.18);	
			al_flip_display();
		}
		for (i = START; i <= END; i++) {
			//I used this to avoid over load in memory, so i get the original bitmap only for the effected cells instead of 36 cells every single time
			col[rowNumber][i]->bitmap = al_clone_bitmap(bitmap);
		}
	}
	else if (groupType == Diagonal) {

		for (i = START; i <= END; i++) {

			diag[rowNumber][i]->bitmap = al_clone_bitmap(animCell);
			al_draw_tinted_bitmap(diag[rowNumber][i]->bitmap, diag[rowNumber][i]->cellClr, diag[rowNumber][i]->x - 10, diag[rowNumber][i]->y - 10, NULL);
			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			al_rest(0.18);	
			al_flip_display();
		}
		for (i = START; i <= END; i++) {
			//I used this to avoid over load in memory, so i get the original bitmap only for the effected cells instead of 36 cells every single time
			diag[rowNumber][i]->bitmap = al_clone_bitmap(bitmap);
		}
	}
	else if (groupType == invDiagonal) {

		for (i = START; i <= END; i++) {

			diag[rowNumber][i]->bitmap = al_clone_bitmap(animCell);
			al_draw_tinted_bitmap(inv_diag[rowNumber][i]->bitmap, inv_diag[rowNumber][i]->cellClr, inv_diag[rowNumber][i]->x - 10, inv_diag[rowNumber][i]->y - 10, NULL);
			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			al_rest(0.18);
			al_flip_display();
		}
		for (i = START; i <= END; i++) { 
			//I used this to avoid over load in memory, so i get the original bitmap only for the effected cells instead of 36 cells every single time
			diag[rowNumber][i]->bitmap = al_clone_bitmap(bitmap);
		}
	}
	
	al_flip_display();
}

void double_effect(CELL_PATTERN groupType, int rowNum1, int START1, int END1, int rowNum2, int START2, int END2) {

	//-----column and diagonal
	if (groupType == Col_Diagonal) {

		for (i = START1; i <= END1; i++) {

			col[rowNum1][i]->bitmap = al_clone_bitmap(animCell);

			al_draw_tinted_bitmap(col[rowNum1][i]->bitmap, col[rowNum1][i]->cellClr, col[rowNum1][i]->x - 10, col[rowNum1][i]->y - 10, NULL);

			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		for (i = START2; i <= END2; i++) {

			diag[rowNum2][i]->bitmap = al_clone_bitmap(animCell);

			al_draw_tinted_bitmap(diag[rowNum2][i]->bitmap, diag[rowNum2][i]->cellClr, diag[rowNum2][i]->x - 10, diag[rowNum2][i]->y - 10, NULL);

			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		al_flip_display();
		al_rest(0.5);

		//I used these loops to avoid overload in memory, so i get the original bitmap only for the effected cells instead of 36 cells every single time
		for (i = START1; i <= END1; i++) {
			col[rowNum1][i]->bitmap = al_clone_bitmap(bitmap);
		}

		for (i = START2; i <= END2; i++) {
			diag[rowNum2][i]->bitmap = al_clone_bitmap(bitmap);
		}
	}

	//-----column and inverse diagonal
	if (groupType == Col_invDiagonal) {

		for (i = START1; i <= END1; i++) {

			col[rowNum1][i]->bitmap = al_clone_bitmap(animCell);

			al_draw_tinted_bitmap(col[rowNum1][i]->bitmap, col[rowNum1][i]->cellClr, col[rowNum1][i]->x - 10, col[rowNum1][i]->y - 10, NULL);

			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		for (i = START2; i <= END2; i++) {

			inv_diag[rowNum2][i]->bitmap = al_clone_bitmap(animCell);

			al_draw_tinted_bitmap(inv_diag[rowNum2][i]->bitmap, inv_diag[rowNum2][i]->cellClr, inv_diag[rowNum2][i]->x - 10, inv_diag[rowNum2][i]->y - 10, NULL);

			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		al_flip_display();
		al_rest(0.5);

		//I used these loops to avoid overload in memory, so i get the original bitmap only for the effected cells instead of 36 cells every single time
		for (i = START1; i <= END1; i++) {
			col[rowNum1][i]->bitmap = al_clone_bitmap(bitmap);
		}

		for (i = START2; i <= END2; i++) {
			inv_diag[rowNum2][i]->bitmap = al_clone_bitmap(bitmap);
		}
	}

	//-----diagonal and inverse diagonal
	if (groupType == Diag_InvDiagonal) {

		for (i = START1; i <= END1; i++) {

			diag[rowNum1][i]->bitmap = al_clone_bitmap(animCell);

			al_draw_tinted_bitmap(diag[rowNum1][i]->bitmap, diag[rowNum1][i]->cellClr, diag[rowNum1][i]->x - 10, diag[rowNum1][i]->y - 10, NULL);

			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		for (i = START2; i <= END2; i++) {

			inv_diag[rowNum2][i]->bitmap = al_clone_bitmap(animCell);

			al_draw_tinted_bitmap(inv_diag[rowNum2][i]->bitmap, inv_diag[rowNum2][i]->cellClr, inv_diag[rowNum2][i]->x - 10, inv_diag[rowNum2][i]->y - 10, NULL);

			al_play_sample(animSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

		}
		al_flip_display();
		al_rest(0.5);

		//I used these loops to avoid overload in memory, so i get the original bitmap only for the effected cells instead of 36 cells every single time
		for (i = START1; i <= END1; i++) {
			diag[rowNum1][i]->bitmap = al_clone_bitmap(bitmap);
		}
		for (i = START2; i <= END2; i++) {
			inv_diag[rowNum2][i]->bitmap = al_clone_bitmap(bitmap);
		}

	}
}

void hover(ALLEGRO_EVENT event) {


	for (i = 0; i < 37; i++) {

		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {

			if (((event.mouse.x >= cell[i].x) && (event.mouse.x <= cell[i].x + cellWidth) && (event.mouse.y >= cell[i].y) && (event.mouse.y <= cell[i].y + cellHeight)) && (move[0].state || move[1].state || move[2].state)) {

				if (elem[activeElm].id == 0 && !cell[i].filled) {

				cell[i].cellClr = elem[activeElm].elmClr;
				printf("hover!\n");
				}
				if ((elem[activeElm].id == 1) && (!cell[i].filled && !cell[i + 1].filled)) {

					cell[i].cellClr = elem[activeElm].elmClr;
					cell[i + 1].cellClr = elem[activeElm].elmClr;
				}
			}
			else if (!cell[i].filled) {

				cell[i].cellClr = al_map_rgb(255, 255, 255);
			}
		}
	}
}

void shutdown(void) {

	if (done) {

		if (queue)
			al_destroy_event_queue(queue);

		if (timer)
			al_destroy_timer(timer);

		if (display)
			al_destroy_display(display);

		//destroy all bitmaps
		if (bitmap)
			al_destroy_bitmap(bitmap);

		if (hexagonal)
			al_destroy_bitmap(hexagonal);

		if (gameOver)
			al_destroy_bitmap(gameOver);

		if (menuBitmap)
			al_destroy_bitmap(menuBitmap);

		//destroy all samples
		if (animCell)
			al_destroy_bitmap(animCell);

		if (bgSound)
			al_destroy_sample(bgSound);

		if (endSound)
			al_destroy_sample(endSound);

		if (winSound)
			al_destroy_sample(winSound);

		if (endSound)
			al_destroy_sample(endSound);

		//destroy all fonts
		if (font)
			al_destroy_font(font);

		if (font2)
			al_destroy_font(font2);

		if (menuFont)
		al_destroy_font(menuFont);

		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_uninstall_audio();
	}

}

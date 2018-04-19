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

#define buttonX 120
#define buttonY 390

typedef struct {

	int id;
	int value;
	bool filled;
	int x, y;
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_COLOR cellClr;

}HEXAGONAL;

typedef struct {

	int x, y, orgX, orgY;
	int id, type;
	bool exist;
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_COLOR elmClr;

}ELEMNET;

typedef struct {

	bool state;

}ACTION;

typedef struct {
	
	int x, y;
	ALLEGRO_BITMAP *bitmap;

}SHAPES;

void Home(ALLEGRO_EVENT event);
void draw();
void init();
void cell_update();
void test(ALLEGRO_EVENT event);
void cols_init();
void diag_init();
void inv_diag_init();
void fill_all();
void check();
void check_cols();
void check_diagons();
void check_inv_diagons();
void is_good_col();
void is_good_diag();
void is_good_inv_diag();
void user_input(ALLEGRO_EVENT event);
void game_loop(ALLEGRO_EVENT event);
void redraw_elements();
void shutdown();
void check_move();
void rand_elem();
int getColor(int);
void colr_generator();
void game_over(ALLEGRO_EVENT event);

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER* timer;
ALLEGRO_TIMER* timer2;
ALLEGRO_EVENT event;
ALLEGRO_COLOR randColr[6];
ALLEGRO_COLOR tempColr;
ALLEGRO_COLOR background_color;
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *bitmap;
ALLEGRO_BITMAP *bitmap2;
ALLEGRO_BITMAP *hexagonal;
ALLEGRO_BITMAP *startClick;
ALLEGRO_BITMAP *gameOver;
ALLEGRO_BITMAP *tempBitmap;
ALLEGRO_BITMAP *elmBitmap[9];
ALLEGRO_SAMPLE *bgSound;
ALLEGRO_SAMPLE *clickSound;
ALLEGRO_SAMPLE *endSound;
ALLEGRO_SAMPLE *winSound;
ALLEGRO_MENU *menu;
ALLEGRO_MOUSE_STATE mState;
HEXAGONAL cell[37];
HEXAGONAL*col[7][7], *diag[7][7], *inv_diag[7][7];
static ELEMNET elem[4], rndElem[9], testEl[3];
ACTION move[3];

int j, i, k, colr, activeElm, activeCell;
int rnd[6];
bool done, game_started, lost;
bool move1, move2, move3;
static int cellX = originX, cellY = originY, elX = elemX, elY = elemY;
static int elemNum = 0, redraw = 1, score = 0;
int c1, c2, c3;


int main() {

	
	init();
	Home(event);

	rand_elem();
	cell_update();
	cols_init();
	diag_init();
	inv_diag_init();
	
	draw();
	//game_over(event);
	game_loop(event);
	//game_over();
	shutdown();


	system("pause>null");
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
	timer2 = al_create_timer(1.0 / 1);

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

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_acodec_addon();

	font = al_load_ttf_font("pirulen rg.ttf", 22, 0);

	randColr[0] = al_map_rgb(244, 47, 200);
	randColr[1] = al_map_rgb(212, 169, 51);
	randColr[2] = al_map_rgb(58, 215, 46);
	randColr[3] = al_map_rgb(255, 12, 12);
	randColr[4] = al_map_rgb(42, 199, 250);
	randColr[5] = al_map_rgb(56, 52, 241);
	//background_color = al_map_rgb(88, 230, 182);
	background_color = al_map_rgb(32, 32, 30);

	
	//bitmap = al_load_bitmap("emptyCell.png");
	//bitmap = al_load_bitmap("elmCellNewMod.png");
	bitmap = al_load_bitmap("elmCell.png");
	bitmap2 = al_load_bitmap("redCell.png");
	hexagonal = al_load_bitmap("Hexagonal.png");
	//hexagonal = al_load_bitmap("original.gif");

	startClick = al_load_bitmap("clickStart.png");
	//gameOver = al_load_bitmap("gameOver.png");
	gameOver = al_load_bitmap("gameOver2.png");
	tempBitmap = al_load_bitmap("clrCell01.png");

	elmBitmap[0] = al_load_bitmap("elmCell.png");

	elmBitmap[1] = al_load_bitmap("2C01.png");
	elmBitmap[2] = al_load_bitmap("2C02.png");
	elmBitmap[3] = al_load_bitmap("2C03.png");

	elmBitmap[4] = al_load_bitmap("3C01.png");
	elmBitmap[5] = al_load_bitmap("3C02.png");
	elmBitmap[6] = al_load_bitmap("3C03.png");
	elmBitmap[7] = al_load_bitmap("3C04.png");
	elmBitmap[8] = al_load_bitmap("3C05.png");

	bgSound = al_load_sample("sound.wav");
	clickSound = al_load_sample("click.wav");
	winSound = al_load_sample("win.wav");
	endSound = al_load_sample("end.wav");
	al_reserve_samples(4);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());

	done = false;
	game_started = false;

	move[0].state = false;
	move[1].state = false;
	move[2].state = false;

	elem[0].exist = true;
	elem[1].exist = true;
	elem[2].exist = true;

}

void cell_update() {

	//int c[3] = { c1,c2,c3 };

	for (i = 0; i < 37; i++) {

		if (!game_started) {

			//cell[i].filled = false;
			cell[i].id = i;
			//cell[i].cellClr = al_map_rgb(214, 208, 208);
			cell[i].cellClr = al_map_rgb(100, 50, 50);
		}

		if (cell[i].filled == false) {
			//cell[i].bitmap = al_load_bitmap("emptyCell.png");
			cell[i].cellClr = al_map_rgb(255, 255, 255);
			cell[i].bitmap = al_clone_bitmap(bitmap);
			al_draw_tinted_bitmap(cell[i].bitmap, cell[i].cellClr, cell[i].x, cell[i].y, NULL);
			//al_draw_bitmap(cell[i].bitmap, cell[i].x, cell[i].y, NULL);
		}
		if (cell[i].filled == true) {
			//cell[i].bitmap = al_load_bitmap("redCell.png");
			//cell[i].bitmap = al_clone_bitmap(tempBitmap); //I canceled it in order to test new feature
			//cell[i].bitmap = al_clone_bitmap(bitmap2); //Orginal One
			//cell[i].cellClr = elem[activeElm].elmClr;
			//al_draw_bitmap(cell[i].bitmap, cell[i].x, cell[i].y, NULL);
			//cell[i].bitmap = elmBitmap[activeElm];
			al_draw_tinted_bitmap(cell[i].bitmap, cell[i].cellClr, cell[i].x, cell[i].y, NULL);
			
		}
	}
	
	game_started = true;

} //no need to be called alone, check function calls it usually

void draw() {

	int i, j, s[3], n[3], c;

	cellX = originX, cellY = originY;

	al_clear_to_color(background_color);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 5, NULL, "SCORE: %d", score);
	//al_draw_bitmap(bitmap3, originX, originY - 55, NULL);


	// ======================  >>>>> Drawing the hexagonal <<<<< =========================

	for (j = 0; j < 4; j++) {

		al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY + 1, NULL);
		al_draw_tinted_bitmap(cell[j + 33].bitmap, cell[j + 33].cellClr, cellX + 198, cellY + 1, NULL);
		//al_draw_bitmap(cell[j].bitmap, cellX, cellY + 1, NULL); //drawing the 1st column
		//al_draw_bitmap(cell[j + 33].bitmap, cellX + 198, cellY + 1, NULL); //drawing the 7th column


		//printf("cell[%d] = X - %d Y - %d\n", j, cell[j].x, cell[j].y);
		
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
		//al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);
		//al_draw_bitmap(cell[j + 24].bitmap, cellX + 132, cellY, NULL);

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
		//al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);
		//al_draw_bitmap(cell[j + 13].bitmap, cellX + 66, cellY, NULL);

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
		//al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);

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


			//elem[i].bitmap = al_clone_bitmap(bitmap2);
			//elem[i] = rndElem[2];
			//al_draw_bitmap(elem[i].bitmap, elX, elY, NULL);
			al_draw_tinted_bitmap(elem[i].bitmap, elem[i].elmClr, elX, elY, NULL);
			//al_draw_tinted_bitmap(elem[i].bitmap, randColr[0], elX, elY, NULL);
			printf("rndClr\n");
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

		//colr_generator();
		//elem[i].exist = true;
		//elem[i] = testEl[n[i]];
		//elem[i].bitmap = al_clone_bitmap(testEl[s[i]].bitmap);
		//elem[i].bitmap = al_clone_bitmap(bitmap3);


		//al_draw_tinted_bitmap(elmBitmap[1], elem[i].elmClr, elem[i].x, elem[i].y, NULL);
		//al_draw_bitmap(elem[i].bitmap, elem[i].x, elem[i].y, NULL);
		//al_draw_tinted_bitmap(elem[i].bitmap, randColr[0], elem[i].x, elem[i].y, NULL);
	}



	//rand_elem();
	//redraw_elements();
	//check_move();
	//al_draw_bitmap(gameOver, 0, 0, NULL);
	al_flip_display();
}

void check_move() {

	//Check the first Element
	if (move[0].state && !move[1].state && !move[2].state) {

		if (elem[0].bitmap != elmBitmap[3] && elem[0].bitmap != elmBitmap[6] && elem[0].bitmap != elmBitmap[8]) {

			elem[0].x = elem[3].x - 15;
			elem[0].y = elem[3].y - 15;
		}
		else if (elem[0].bitmap == elmBitmap[3] || elem[0].bitmap == elmBitmap[8]){

			elem[0].x= elem[3].x - 35;
			elem[0].y= elem[3].y - 15;
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

void redraw_elements() {

	if (move1) {
		elem[0].bitmap = al_clone_bitmap(bitmap2);
		al_draw_bitmap(elem[0].bitmap, elem[0].x, elem[0].y, NULL);
	}

	elX = elemX, elY - elemY;

	//for (i = 0; i < 3; i++) {

		for (j = 0; j < 2; j++) {

			//dualCell[0].bitmap = al_clone_bitmap(bitmap);
			//al_draw_bitmap(dualCell[0].bitmap, elX, elY + 1, NULL); //drawing the 1st column


			 //printf("cell[%d] = X - %d Y - %d\n", j, cell[j].x, cell[j].y);

			 //Set the coordinates for each cell in the 1st and 7th column
			//dualCell[j].x = elX;
			//dualCell[j].y = elY + 1;


			elY += 35; //increase the height value only in order to continue drawing from top to the bottom
		}
//	}
	

	al_flip_display();


	//Not active now !
}
 
void test(ALLEGRO_EVENT event) {

	/*for (i = 0; i < 37; i++) {

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if ((event.mouse.button & 1) && (event.mouse.x >= cell[1].x) && (event.mouse.x <= cell[1].x + 50) && (event.mouse.y >= cell[1].y) && (event.mouse.y <= cell[1].y + 50)) {
				printf("Great !\n");
			}
		}

	}*/


	/*
	if ((event.mouse.button & 1) && (event.mouse.x >= cell[0].x) && (event.mouse.x <= cell[0].x + cellWidth) && (event.mouse.y >= cell[0].y) && (event.mouse.y <= cell[0].y + cellHeight) && ((move[0].state && !move[1].state && !move[2].state) || (!move[0].state && move[1].state && !move[2].state) || (!move[0].state && !move[1].state && move[2].state))) {

		//move[0].state = true;
		elem[0].x = cell[0].x;
		elem[0].y = cell[0].y;
	}
	*/

	//if ((event.mouse.button & 1) && (event.mouse.x >= dualCell[0].x) && (event.mouse.x <= dualCell[0].x + cellWidth) && (event.mouse.y >= dualCell[0].y) && (event.mouse.y <= dualCell[0].y + cellHeight) && ((move[0].state && !move[1].state && !move[2].state) || (!move[0].state && move[1].state && !move[2].state) || (!move[0].state && !move[1].state && move[2].state))) {


	//}

}

void cols_init() {
	
	//init for many 1D pointers 

	/*for (i = 0, j = 0; i < 4, j < 4; i++, j++) {

		col17[i] = &cell[j];
		col17[i + 4] = &cell[j + 33];

	//	col[0][i]->id = j;
	//	col[6][i]->id = j + 33;

	//	printf("col[0][%d] = %d -- col[6][%d] = %d\n", i, col[0][i]->id, i, col[6][i]->id);
	//	printf("cell[%d] = %d -- cell[%d] = %d\n", j, cell[j].id, j, cell[j + 33].id);
	}
	
	for (i = 0, j = 4; i < 5, j < 9; i++, j++) {

		col26[i] = &cell[j];
		col26[i + 5] = &cell[j + 24];
	}

	for (i = 0, j = 9; i < 6, j < 15; i++, j++) {

		col35[i] = &cell[j];
		col35[i + 6] = &cell[j + 13];
	}

	for (i = 0, j = 15; i < 7, j < 22; i++, j++) {

		col40[i] = &cell[j];
	}
	*/
	
	//init for one 2D pointer 

	for (i = 0, j = 0; i < 4, j < 4; i++, j++) {

		col[0][i] = &cell[j];
		col[6][i] = &cell[j + 33];
	}
	for (i = 0, j = 4; i < 5, j < 9; i++, j++) {

		col[1][i] = &cell[j];
		col[5][i] = &cell[j + 24];
	}

	for (i = 0, j = 9; i < 6, j < 15; i++, j++) {

		col[2][i] = &cell[j];
		col[4][i] = &cell[j + 13];
	}

	for (i = 0, j = 15; i < 7, j < 22; i++, j++) {

		col[3][i] = &cell[j];
	}
	
	
}

void diag_init() {

	for (i = 0, j = 3; i < 4, j >= 0; i++,j--) {
		
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
	
	inv_diag[1][0] = col[1][0];

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

void check() {

	//the check part that exist in the gameLoop will be here
	
}

void check_cols() {

	if ((event.mouse.button & 1) && (!cell[i].filled) && (event.mouse.x >= cell[i].x) && (event.mouse.x <= cell[i].x + cellWidth) && (event.mouse.y >= cell[i].y) && (event.mouse.y <= cell[i].y + cellHeight) && ((move[0].state && !move[1].state && !move[2].state) || (!move[0].state && move[1].state && !move[2].state) || (!move[0].state && !move[1].state && move[2].state))) {

		if (elem[activeElm].type == 2) {
			
			switch (elem[activeElm].id)
			{
			case 1:

			break;

			case 2:
			break;

			case 3:
			break;

			default:
			break;
			}
		}

	}
}

void check_diagons() {

	

}

void check_inv_diagons() {


	

}

void is_good_col() {

	//>> ----------------------- Checking The columns ----------------------- <<

	//check the 1st and 7th columns

	i = 0;
	if(col[0][i]->filled == true && col[0][++i]->filled == true && col[0][++i]->filled == true && col[0][++i]->filled == true){

		printf("WOW ! \n");
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		for (i = 0; i < 4; i++) {

			col[0][i]->filled = false;
			score++;
		}
		
	}
	i = 0;
	if (col[6][i]->filled == true && col[6][++i]->filled == true && col[6][++i]->filled == true && col[6][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			col[6][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 2nd and 6th columns
	i = 0;
	if (col[1][i]->filled == true && col[1][++i]->filled == true && col[1][++i]->filled == true && col[1][++i]->filled == true && col[1][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			col[1][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (col[5][i]->filled == true && col[5][++i]->filled == true && col[5][++i]->filled == true && col[5][++i]->filled == true && col[5][++i]->filled == true) {

		printf("WOW ! \n");

		for (i = 0; i < 5; i++) {

			col[5][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 3rd and 5th columns
	i = 0;
	if (col[2][i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true && col[2][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			col[2][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (col[4][i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true && col[4][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			col[4][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 4th columns
	i = 0;
	if (col[3][i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true && col[3][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 7; i++) {

			col[3][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

}

void is_good_diag() {

	//>> ----------------------- Checking The diagonals ----------------------- <<

	//check the 1st and 7th diagonals

	i = 0;
	if (diag[0][i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true && diag[0][++i]->filled == true) {
		
		printf("WOW ! \n");
		
		for (i = 0; i < 4; i++) {

			diag[0][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (diag[6][i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true && diag[6][++i]->filled == true) {

		for (i = 0; i < 4; i++) {

			printf("WOW ! \n");
			diag[6][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 2nd and 6th diagonals
	i = 0;
	if (diag[1][i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			diag[1][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (diag[5][i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true) {
		
		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			diag[5][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 3rd and 5th diagonals
	i = 0;
	if (diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			diag[2][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			diag[4][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 4th diagonal
	i = 0;
	if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 7; i++) {

			diag[3][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

}

void is_good_inv_diag() {

	//>> ----------------------- Checking The inv_diagonals ----------------------- <<

	//check the 1st and 7th inv_diagonals

	i = 0;
	if (inv_diag[0][i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true && inv_diag[0][++i]->filled == true) {
		
		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			inv_diag[0][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (inv_diag[6][i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true && inv_diag[6][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {
			
			inv_diag[6][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 2nd and 6th inv_diagonals
	i = 0;
	if (inv_diag[1][i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true && inv_diag[1][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			inv_diag[1][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (inv_diag[5][i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true && inv_diag[5][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {
			inv_diag[5][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}

	//check the 3rd and 5th inv_diagonals
	i = 0;
	if (inv_diag[2][i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true && inv_diag[2][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			inv_diag[2][i]->filled = false;
			score++;
		}
		al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
	i = 0;
	if (inv_diag[4][i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true && inv_diag[4][++i]->filled == true) {

		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			inv_diag[4][i]->filled = false;
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

}

void fill_all() {

	//For test Only

	al_get_mouse_state(&mState);
	al_wait_for_event(queue, &event);

	for (i = 0; i < 37; i++) {


		//if ((al_mouse_button_down(&mState, 1)) && (mState.x >= cell[i].x) && (mState.x <= cell[i].x + cellWidth) && (mState.y >= cell[i].y) && (mState.y <= cell[i].y + cellHeight) && move[0].state || move[1].state || move[2].state) {
		if ((al_mouse_button_down(&mState, 1)) && (mState.x >= cell[i].x) && (mState.x <= cell[i].x + cellWidth) && (mState.y >= cell[i].y) && (mState.y <= cell[i].y + cellHeight)) {
			cell[i].filled = true;

			cell[i].bitmap = al_clone_bitmap(bitmap2);
			al_draw_bitmap(cell[i].bitmap, cell[i].x, cell[i].y, NULL);

			if (move[0].state && cell[i].filled) {

				move[0].state = false;
			}
			if (move[1].state && cell[i].filled) {

				move[1].state = false;
			}
			if (move[2].state && cell[i].filled) {

				move[2].state = false;
			}

			printf("Great %d !\n", i);
		}
	}
	al_flip_display();
	check();
}

void user_input(ALLEGRO_EVENT event) {

	//al_get_mouse_state(&mState);
	al_wait_for_event(queue, &event);

	for (i = 0; i < 3; i++) {

	if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		elem[3].x = event.mouse.x;
		elem[3].y = event.mouse.y;

	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

		if ((event.mouse.button & 1) && (event.mouse.x >= elem[i].x) && (event.mouse.x <= elem[i].x + cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight)) {
			move[i].state = true;
			printf("klkl %d\n", i);
		}
		else if (event.mouse.button & 2)
		{
			move[0].state = false;
			move[1].state = false;
			move[2].state = false;
		}
	}
		//cell_update();
	}
	draw();
}

void game_loop(ALLEGRO_EVENT event) {

	al_start_timer(timer);


	while (!done) {

		//al_get_mouse_state(&mState);
		al_wait_for_event(queue, &event);
		//al_play_sample(bgSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		//	done = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {


		}

		if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			//done = true;
		}
		
		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {

			elem[3].x = event.mouse.x;
			elem[3].y = event.mouse.y;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			for (i = 0; i < 3; i++) {

				if ((event.mouse.button & 1) && (elem[i].bitmap != elmBitmap[3]) && (elem[i].bitmap != elmBitmap[6]) && (elem[i].bitmap != elmBitmap[8]) && (event.mouse.x >= elem[i].x) && (event.mouse.x <= elem[i].x + cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight )) {

					move[i].state = true;
					al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

					check_move();
					if (move[i].state) {

						activeElm = i;
					}
					printf("1stMove[%d].state =  %d\n", i, move[i].state);
					printf("click %d\n", i);
					printf("activEL is  %d\n", activeElm);
					printf("exist[%d] = %d", i, elem[i].exist);
				}
				else if ((event.mouse.button & 1) && (elem[i].bitmap == elmBitmap[3] || elem[i].bitmap == elmBitmap[8]) && (event.mouse.x >= elem[i].x + 30) && (event.mouse.x <= elem[i].x + 2 * cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight)) {

					move[i].state = true;
					al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

					check_move();
					if (move[i].state) {

						activeElm = i;
					}
				}
				else if ((event.mouse.button & 1) && (elem[i].bitmap == elmBitmap[6]) && (event.mouse.x >= elem[i].x + 60) && (event.mouse.x <= elem[i].x + 3 * cellWidth) && (event.mouse.y >= elem[i].y) && (event.mouse.y <= elem[i].y + cellHeight)) {

					move[i].state = true;
					al_play_sample(clickSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

					check_move();
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
							printf("inside CFT %d\n", i);
							printf("activEL is  %d\n", activeElm);
							printf("exist[%d] = %d", i, elem[i].exist);
							printf("2ndMove[%d].state =  %d\n", i, move[i].state);

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

							if ((cell[i].id >=32 && cell[i].id <= 36) || cell[i].id == 21 || cell[i].id == 27) {
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

							if ((cell[i].id >= 4 && cell[i].id <= 8) || (cell[i].id >= 23 && cell[i].id <= 27)) {

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

							if (cell[i].id == 15 || cell[i].id == 22 || cell[i].id == 28 || cell[i].id == 33 || cell[i].id == 34 || cell[i].id == 35 || cell[i].id == 36) {
								continue;
							}
							if ((cell[i].id >= 0 && cell[i].id <= 3) || (cell[i].id >= 29 && cell[i].id <= 32)) {

								if (cell[i + 4].filled) {
									continue;
								}
								cell[i].filled = true;
								cell[i + 4].filled = true;

								cell[i].cellClr = elem[activeElm].elmClr;
								cell[i + 4].cellClr = elem[activeElm].elmClr;

								score += 2;

								if (cell[i].filled && cell[i + 4].filled) {

									elem[activeElm].exist = false;
									move[activeElm].state = false;
								}
							}//end of 1st and 6th columns

							if ((cell[i].id >= 4 && cell[i].id <= 8) || (cell[i].id >= 23 && cell[i].id <= 27)) {
							
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
							}//end of 2nd and 5th columns

							if ((cell[i].id >= 9 && cell[i].id <= 14) || (cell[i].id >= 16 && cell[i].id <= 21)) {

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

							if ((cell[i].id >= 28 && cell[i].id <= 36) || cell[i].id == 9 || cell[i].id == 15 || cell[i].id == 16 || cell[i].id == 22 || cell[i].id == 23) {
								continue;
							}
							if (cell[i].id >= 0 && cell[i].id <= 3) {

								if (cell[i + 4].filled || cell[i + 9].filled) {
									continue;
								}
								cell[i].filled = true;
								cell[i + 4].filled = true;
								cell[i + 9].filled = true;

								cell[i].cellClr = elem[activeElm].elmClr;
								cell[i + 4].cellClr = elem[activeElm].elmClr;
								cell[i + 9].cellClr = elem[activeElm].elmClr;

								score += 3;

								if (cell[i].filled && cell[i + 4].filled && cell[i + 9].filled) {

									elem[activeElm].exist = false;
									move[activeElm].state = false;
								}
							}//end 1st column

							if (cell[i].id >= 4 && cell[i].id <= 8) {

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
							}//end of 2nd column

							if ((cell[i].id >= 10 && cell[i].id <= 14)) {

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
							}//end of 3rd column

							if ((cell[i].id >= 17 && cell[i].id <= 21)) {

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

								if (cell[i].filled && cell[i + 6].filled && cell[i + 11].filled) {

									elem[activeElm].exist = false;
									move[activeElm].state = false;
								}
							}//end of 4th column

							if ((cell[i].id >= 24 && cell[i].id <= 27)) {

								if (cell[i + 5].filled || cell[i + 9].filled) {
									continue;
								}
								cell[i].filled = true;
								cell[i + 5].filled = true;
								cell[i + 9].filled = true;

								cell[i].cellClr = elem[activeElm].elmClr;
								cell[i + 5].cellClr = elem[activeElm].elmClr;
								cell[i + 9].cellClr = elem[activeElm].elmClr;

								score += 3;

								if (cell[i].filled && cell[i + 5].filled && cell[i + 9].filled) {

									elem[activeElm].exist = false;
									move[activeElm].state = false;
								}
							}//end of 5th column

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

							if ((cell[i].id >= 27 && cell[i].id <= 36) || cell[i].id == 15 || cell[i].id == 21 || cell[i].id == 22) {
								continue;
							}
							if (cell[i].id >= 0 && cell[i].id <= 3) {

								if (cell[i + 4].filled || cell[i + 10].filled) {
									continue;
								}
								cell[i].filled = true;
								cell[i + 4].filled = true;
								cell[i + 10].filled = true;

								cell[i].cellClr = elem[activeElm].elmClr;
								cell[i + 4].cellClr = elem[activeElm].elmClr;
								cell[i + 10].cellClr = elem[activeElm].elmClr;

								score += 3;

								if (cell[i].filled && cell[i + 4].filled && cell[i + 10].filled) {

									elem[activeElm].exist = false;
									move[activeElm].state = false;
								}
							}//end 1st column

							if (cell[i].id >= 4 && cell[i].id <= 8) {

								if (cell[i + 5].filled || cell[i + 12].filled) {
									continue;
								}
								cell[i].filled = true;
								cell[i + 5].filled = true;
								cell[i + 12].filled = true;

								cell[i].cellClr = elem[activeElm].elmClr;
								cell[i + 5].cellClr = elem[activeElm].elmClr;
								cell[i + 12].cellClr = elem[activeElm].elmClr;

								score += 3;

								if (cell[i].filled && cell[i + 5].filled && cell[i + 12].filled) {

									elem[activeElm].exist = false;
									move[activeElm].state = false;
								}
							}//end of 2nd column

							if ((cell[i].id >= 9 && cell[i].id <= 14)) {

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
							}//end of 3rd column

							if ((cell[i].id >= 16 && cell[i].id <= 20)) {

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
							}//end of 4th column

							if ((cell[i].id >= 23 && cell[i].id <= 26)) {

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
							}//end of 5th column

							break;
						case 9:

							break;
						default:
							break;
						}

					}
					printf("cell[%d].filled = %d\n", i, cell[i].filled);
					printf("Your Score is = %d\n", score);
					check_cols();
					is_good_col();
					is_good_diag();
					is_good_inv_diag();
					cell_update();
				}//
			}
			
		}
		//cell_update();
		//rand_elem();
		draw();
		
	}
	//game_over();
}

void shutdown(void) {

	if (done) {

		if (queue)
			al_destroy_event_queue(queue);

		if (timer) {
			al_destroy_timer(timer);
		}
		if (display)
			al_destroy_display(display);
	}
}

void Home(ALLEGRO_EVENT event) {

	bool started = false;


	while (!started)
	{
		al_wait_for_event(queue, &event);

		al_clear_to_color(al_map_rgb(175, 219, 238));
		al_draw_bitmap(hexagonal, originX, originY - 50, NULL);
		
		al_draw_tinted_bitmap(startClick, al_map_rgb(43, 96, 59), buttonX, buttonY, NULL);
		//al_rest(1);
		//al_start_timer(timer2);
		al_draw_tinted_bitmap(startClick, al_map_rgb(175, 219, 238), buttonX, buttonY, NULL);

		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {

			al_draw_tinted_bitmap(startClick, al_map_rgb(31, 251, 165), buttonX, buttonY, NULL);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if ((event.mouse.button & 1) && (event.mouse.x >= buttonX) && (event.mouse.x <= buttonX + 260) && (event.mouse.y >= buttonY) && (event.mouse.y <= buttonY + 35)) {
				printf("clicked\n");
				started = true;
			}
		}
		al_flip_display();
	}
	
}

void rand_elem() {

	//colr_generator();
	//elX = elemX, elY = elemY;
	srand(time(NULL));

	int choice, e1, e2, e3;

	choice = rand() % 3;
	e1 = rand() % 9;
	e2 = rand() % 9;
	e3 = rand() % 9;

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
	
	

	/*
	switch (choice)
	{
	case 0:
		elem[0] = rndElem[e1];
		elem[1] = rndElem[e2];
		elem[2] = rndElem[e3];

		elem[0].elmClr = randColr[c1];
		elem[1].elmClr = randColr[c2];
		elem[2].elmClr = randColr[c3];
		//printf("case 0\n");
		break;

	case 1:
		elem[0] = rndElem[e3];
		elem[1] = rndElem[e1];
		elem[2] = rndElem[e2];

		elem[0].elmClr = randColr[c1];
		elem[1].elmClr = randColr[c2];
		elem[2].elmClr = randColr[c3];
		//printf("case 1\n");
		break;

	case 2:
		elem[0] = rndElem[e2];
		elem[1] = rndElem[e3];
		elem[2] = rndElem[e1];

		elem[0].elmClr = randColr[c1];
		elem[1].elmClr = randColr[c2];
		elem[2].elmClr = randColr[c3];
		//printf("case 2\n");
		break;
	}
	
	*/
}

void colr_generator() {

	srand(time(NULL));

	for (i = 0; i < 7; i++) {

		rnd[i] = rand() % 6;
	}
	

}

void game_over(ALLEGRO_EVENT event) {


	al_play_sample(endSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	gameOver = al_clone_bitmap(gameOver);
	al_draw_bitmap(gameOver, 0, 0, NULL);
	lost = true;
	while (lost)
	{

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || event.type == ALLEGRO_EVENT_MOUSE_AXES) {

			//al_draw_tinted_bitmap(startClick, al_map_rgb(31, 251, 165), buttonX, buttonY, NULL);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if ((event.mouse.button & 1) && (event.mouse.x >= 204) && (event.mouse.x <= 303) && (event.mouse.y >= 370) && (event.mouse.y <= 408)) {
				printf("retry\n");

			}
			else if ((event.mouse.button & 1) && (event.mouse.x >= 450) && (event.mouse.x <= 485) && (event.mouse.y >= 214) && (event.mouse.y <= 291)) {

				printf("Quit\n");
			}
		}
		al_create_menu();
		al_flip_display();
	}

}

int getColor(int i) {



}
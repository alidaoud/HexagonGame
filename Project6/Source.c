#include<stdio.h>
#include<stdlib.h>
#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h>
#include<allegro5\bitmap_io.h>

#define A 30
#define B 80

struct MyStruct
{
	int po;
};

typedef struct {
	
	int id;
	int value;
	bool filled;
	int cellCoordinates[2];
	ALLEGRO_BITMAP *cell;

}CELL;

int main() {

	if (!al_init()) {
		al_show_native_message_box(NULL, "Allegro Error", "Error", "Coudn't Initilize Allegro !", NULL, NULL);
	} 

	int a = 60, b = 50, counter = 30, j, i;
	float cellX = A, cellY = B;

	bool draw = false, id = false, done = false;

	ALLEGRO_DISPLAY *display = al_create_display(400, 600);
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_EVENT ev;
	ALLEGRO_COLOR color_white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR background_color = al_map_rgb(88, 230, 182);
	ALLEGRO_BITMAP *cell = al_load_bitmap("emptyCell.png");
	ALLEGRO_BITMAP *cell2 = al_load_bitmap("redCell.png");
	ALLEGRO_MOUSE_STATE mState;
	CELL col17[8], col26[10], col35[12], col40[7]; 
	//CELL hexa[4] = { col17, col26, col35, col40 };

	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	al_set_window_position(display, 350, 75);
	al_set_window_title(display, "Hexagonal");

	al_clear_to_color(background_color);

	//al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());

	while (!id)
	{
		for (i = 0; i < 8; i++) {
			if (i < 4) {

				col17[i].id = i;
			}
			else {

				col17[i].id = i + 29;
			}
			col17[i].filled = 0;
			col17[i].cell = al_load_bitmap("emptyCell.png");
		}

		for (i = 0; i < 10; i++) {
			if (i < 5) {

				col26[i].id = i + 4;
			}
			else {

				col26[i].id = i + 23;
			}
			col26[i].filled = 0;
			col26[i].cell = al_load_bitmap("emptyCell.png");
		}

		for (i = 0; i < 12; i++) {
			if (i < 6) {

				col35[i].id = i + 9;
			}
			else {

				col35[i].id = i + 16;
			}
			col35[i].filled = 0;
			col35[i].cell = al_load_bitmap("emptyCell.png");
		}

		for (i = 0; i < 7; i++) {

			col35[i].id = i + 15;

			col35[i].filled = 0;
			col35[i].cell = al_load_bitmap("emptyCell.png");
		}

		id = true;
	}

	while (!draw)
	{
		// ======================  >>>>> Drawing the hexagnol <<<<< =========================

		for (j = 0; j < 4; j++) {

			for (i = 0; i < 8; i++) {

				//initialize the bitmap for all the col17's cells

				col17[i].cell = al_load_bitmap("emptyCell.png");
			}

			
			al_draw_bitmap(col17[j].cell, cellX, cellY + 1, NULL); //drawing the 1st column
			al_draw_bitmap(col17[j + 4].cell, cellX + 210.4, cellY + 1, NULL); //drawing the 7th column


			printf("col17[%d] = X - %d Y - %d\n", j, col17[j].cellCoordinates[0], col17[j].cellCoordinates[1]);

			col17[j].cellCoordinates[0] = cellX;
			col17[j].cellCoordinates[1] = cellY + 1;

			col17[j + 4].cellCoordinates[0] = cellX + 210.4;
			col17[j + 4].cellCoordinates[1] = cellY + 1;

			cellY += 35; //increase the height values only in order to continue drawing from top to the bottom
		}

		//The new coordinates for The 2nd and 6th columns
		cellX = A + 35, cellY = B - 17;

		for (j = 0; j < 5; j++) {

			for (i = 0; i < 10; i++) {

				//initialize the bitmap for all the col26's cells

				col26[i].cell = al_load_bitmap("emptyCell.png");
			}

			al_draw_bitmap(col26[j].cell, cellX, cellY, NULL);
			al_draw_bitmap(col26[j + 5].cell, cellX + 140, cellY, NULL);

			cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
		}

		//The new coordinates for The 3rd and 5th columns 
		cellX = A + 70, cellY = B - 35;

		for (j = 0; j < 6; j++) {

			for (i = 0; i < 12; i++) {

				//initialize the bitmap for all the col35's cells

				col35[i].cell = al_load_bitmap("emptyCell.png");
			}

			al_draw_bitmap(col35[j].cell, cellX, cellY, NULL);
			al_draw_bitmap(col35[j + 6].cell, cellX + 70, cellY, NULL);

			cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
		}

		//The new coordinates for The last column (the middle one)
		cellX = A + 105, cellY = B - 53;

		for (j = 0; j < 7; j++) {

			for (i = 0; i < 7; i++) {

				//initialize the bitmap for all the col40's cells

				col40[i].cell = al_load_bitmap("emptyCell.png");
			}

			al_draw_bitmap(col40[j].cell, cellX, cellY, NULL);

			cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
		}
		draw = true;
	}
	

	al_flip_display();

	for (j = 0; j < 8; j++) {

		printf("col17[%d] = X - %d Y - %d\n", j, col17[j].cellCoordinates[0], col17[j].cellCoordinates[1]);
	}
	int cnt1 = 0, cnt2 = 0;

	while (1)
	{
		al_get_mouse_state(&mState);

		for (j = 0; j < 8; j++) {

			if (al_mouse_button_down(&mState, 1)) {

				if (mState.x / 2 > col17[j].cellCoordinates[0] && mState.x / 2 < col17[j].cellCoordinates[0] + 30) {

					if (mState.y / 2 > col17[j].cellCoordinates[1] && mState.y / 2 < col17[j].cellCoordinates[1] + 15) {

						printf("Mouse position: (%d, %d)\n", mState.x, mState.y);

						col17[j].cell = al_load_bitmap("redCell.png");
						al_draw_bitmap(col17[j].cell, col17[j].cellCoordinates[0], col17[j].cellCoordinates[1], NULL);
						col17[j].filled = true;
						

					}
				}
			}
			
			if (mState.buttons & 1) {
				// Primary (e.g. left) mouse button is held. 
				
				
				
			}
			if (mState.buttons & 2) {
				// Tertiary (e.g. middle) mouse button is held.
				printf("Mouse ** position: (%d, %d)\n", mState.x, mState.y);
				col17[j].cell = al_load_bitmap("emptyCell.png");
				al_draw_bitmap(col17[j].cell, col17[j].cellCoordinates[0], col17[j].cellCoordinates[1], NULL);
			}
			for (i = 0; i < 4; i++) {
				if (col17[i].filled == true) {
					cnt1++;
				}
				if (col17[i + 4].filled == true) {
					cnt2++;
				}
			}
			printf("cnt 1 = %d -- cnt = %d\n" ,cnt1, cnt2);
		}

		al_flip_display();
	}
	
	
	
	system("pause>null");
	return 0;
}
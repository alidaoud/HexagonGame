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

/*int main() {

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

			al_flip_display();
		}

	}

	al_flip_display();
	
	system("pause>null");
	return 0;
}*/


/*diag[1][0] = col[4][0];

diag[2][0] = col[5][0];
diag[2][1] = col[4][1];

diag[3][0] = col[6][0];
diag[3][1] = col[5][1];
diag[3][2] = col[4][2];

diag[4][0] = col[6][1];
diag[4][1] = col[5][2];
diag[4][2] = col[4][3];

diag[5][0] = col[6][2];
diag[5][1] = col[5][3];
diag[5][2] = col[4][4];
diag[5][3] = col[5][2];//
diag[5][4] = col[5][3];//

diag[6][0] = col[6][3];
diag[6][1] = col[5][4];
diag[6][2] = col[4][5];
diag[6][3] = col[3][6];//



for (i = 0, j = 3; i < 4, j < 7; i++, j++) {

inv_diag[0][i] = col[j][0];
inv_diag[1][i + 1] = col[j][1];
inv_diag[2][i + 2] = col[j][2];
inv_diag[3][i + 3] = col[j][3];
}
inv_diag[1][0] = col[2][0];

inv_diag[2][0] = col[1][0];
inv_diag[2][1] = col[2][1];

inv_diag[3][0] = col[0][0];
inv_diag[3][1] = col[1][1];
inv_diag[3][2] = col[2][2];

inv_diag[4][0] = col[0][1];
inv_diag[4][1] = col[1][2];
inv_diag[4][2] = col[2][3];

inv_diag[5][0] = col[0][2];
inv_diag[5][1] = col[1][3];
inv_diag[5][2] = col[2][4];
inv_diag[5][3] = col[1][2];//
inv_diag[5][4] = col[1][3];//

inv_diag[6][0] = col[0][3];
inv_diag[6][1] = col[1][4];
inv_diag[6][2] = col[2][5];
inv_diag[6][3] = col[3][6];//
*/

/*void drawing_elements() {

	//colr_generator();
	int choice, s1, s2, s3;
	choice = rand() % 6;


	/*for (i = 0, j = 0, k = 0; i < 3, j < 5, k < 3; i++, j++, k++) {

	shape[i][j].elemCell[k] = al_clone_bitmap(bitmap3);
	}

	for (i = 0; i < 24; i++) {

		elemCell[i].bitmap = al_clone_bitmap(bitmap3);
	}


	for (i = 0; i < 3; i++) {

		for (j = 0; j < 3; j++) {

			al_draw_bitmap(elem[i].bitmap, elX, elY + 50, NULL);

			elem[i].x = elX;
			elem[i].y = elY + 50;

			elX += cellWidth; //increase the distance value only in order to continue drawing from left to the right
		}



		switch (choice)
		{

		case 0:
			elemCell[i].x = elemX;
			elemCell[i].bitmap = elemY;
			break;

		case 1:
			elemCell[i].x = elemX + 50;
			elemCell[i].bitmap = elemY;
			break;

		case 2:
			elemCell[i].x = elemX + 50;
			elemCell[i].bitmap = elemY;
			break;

		default:
			break;
		}
		for (i = 8; i < 10; i++) {




			elem[i].bitmap = al_clone_bitmap(bitmap3);
			al_draw_bitmap(elem[i].bitmap, elX, elY + 50, NULL);

			elem[i].x = elX;
			elem[i].y = elY + 50;

			//printf("elem[%d].x = %d -- elem[%d].y = %d\n", i, elem[i].x, i, elem[i].y);
			elX += cellWidth; //increase the distance value only in order to continue drawing from left to the right
		}





		// ======================  >>>>> Drawing the Elements <<<<< =========================

		elX = elemX, elY = elemY;

		//drawing for the first time and init the coordinates for each element

		for (i = 0; i < 3; i++) {

			if (!move[0].state && !move[1].state && !move[2].state) {

				elem[i].bitmap = al_clone_bitmap(bitmap3);
				al_draw_bitmap(elem[i].bitmap, elX, elY, NULL);

				//Setting the coordinates for each element 
				elem[i].x = elX;
				elem[i].y = elY;

				//printf("elem[%d].x = %d -- elem[%d].y = %d\n", i, elem[i].x, i, elem[i].y);
				elX += 125; //increase the distance value only in order to continue drawing from left to the right
			}
		}

		check_move();

		//drawing the moving element and the others

		for (i = 0; i < 3; i++) {
			//colr_generator();
			//elem[i].bitmap = al_clone_bitmap(bitmap2);
			elem[i].bitmap = al_clone_bitmap(bitmap3);
			al_draw_tinted_bitmap(elem[i].bitmap, randColr[i], elem[i].x, elem[i].y, NULL);
			tempBitmap = al_clone_bitmap(elem[i].bitmap);
		}


	}
}*/


/*if (diag[6][i]->filled && diag[6][++i]->filled && diag[6][++i]->filled && diag[6][++i]->filled) {
for (i = 33; i < 37; i++) {
printf("WOW ! \n");
diag[6][i]->filled = false;
}
}

//check the 2nd and 6th diagumns
i = 0;
if (diag[1][i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true && diag[1][++i]->filled == true) {
printf("WOW ! \n");
for (i = 4; i < 9; i++) {

diag[1][i]->filled = false;
}
}
i = 0;
if (diag[5][i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true && diag[5][++i]->filled == true) {
for (i = 28; i < 33; i++) {
printf("WOW ! \n");
diag[5][i]->filled = false;
}
}

//check the 3rd and 5th diagumns
i = 0;
if (diag[2][i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true && diag[2][++i]->filled == true) {
printf("WOW ! \n");
for (i = 9; i < 15; i++) {

diag[2][i]->filled = false;
}
}
i = 0;
if (diag[4][i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true && diag[4][++i]->filled == true) {
for (i = 22; i < 28; i++) {
printf("WOW ! \n");
diag[4][i]->filled = false;
}
}

//check the 4th diagumns
i = 0;
if (diag[3][i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true && diag[3][++i]->filled == true) {
printf("WOW ! \n");
for (i = 15; i < 22; i++) {

diag[3][i]->filled = false;
}
}*/


/*void is_good_col() {

	//>> ----------------------- Checking The columns ----------------------- <<

	//check the 1st and 7th columns
	for (i = 0; i < 4; i++) {
		col[0][i] = &cell[i];
	}
	i = 0;
	if (col[0][i]->filled && col[0][++i]->filled && col[0][++i]->filled && col[0][++i]->filled) {
		//if ((col17[i]->filled || diag17[3]->filled) && col17[++i]->filled  && col17[++i]->filled && col17[++i]->filled) {
		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			col17[i]->filled = false;
		}
		if (diag17[3]->filled) {
			diag17[3]->filled = false;
		}
	}
	i = 4;
	if (col17[i]->filled && col17[++i]->filled && col17[++i]->filled && col17[++i]->filled) {
		for (i = 4; i < 8; i++) {
			printf("WOW ! \n");
			col17[i]->filled = false;
		}
	}

	//check the 2nd and 6th columns
	i = 0;
	if (col26[i]->filled == true && col26[++i]->filled == true && col26[++i]->filled == true && col26[++i]->filled == true && col26[++i]->filled == true) {
		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			col26[i]->filled = false;
		}
	}
	i = 5;
	if (col26[i]->filled == true && col26[++i]->filled == true && col26[++i]->filled == true && col26[++i]->filled == true && col26[++i]->filled == true) {
		for (i = 5; i < 10; i++) {
			printf("WOW ! \n");
			col26[i]->filled = false;
		}
	}

	//check the 3rd and 5th columns
	i = 0;
	if (col35[i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true) {
		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			col35[i]->filled = false;
		}
	}
	i = 6;
	if (col35[i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true && col35[++i]->filled == true) {
		for (i = 6; i < 12; i++) {
			printf("WOW ! \n");
			col35[i]->filled = false;
		}
	}

	//check the 4th columns
	i = 0;
	if (col40[i]->filled == true && col40[++i]->filled == true && col40[++i]->filled == true && col40[++i]->filled == true && col40[++i]->filled == true && col40[++i]->filled == true && col40[++i]->filled == true) {
		printf("WOW ! \n");
		for (i = 0; i < 7; i++) {

			col40[i]->filled = false;
		}
	}

}*/


/*void diag_init() {

	i = 0;
	diag17[i] = col40[0];
	diag17[++i] = col35[0];
	diag17[++i] = col26[0];
	diag17[++i] = col17[0];
	diag17[++i] = col17[7];
	diag17[++i] = col26[9];
	diag17[++i] = col35[11];
	diag17[++i] = col40[6];

	i = 0;
	diag26[i] = col35[6];
	diag26[++i] = col40[1];
	diag26[++i] = col35[1];
	diag26[++i] = col26[1];
	diag26[++i] = col17[1];
	diag26[++i] = col17[6];
	diag26[++i] = col26[8];
	diag26[++i] = col35[10];
	diag26[++i] = col40[5];
	diag26[++i] = col35[5];

	i = 0;
	diag35[i] = col26[5];
	diag35[++i] = col35[7];
	diag35[++i] = col40[2];
	diag35[++i] = col35[2];
	diag35[++i] = col26[2];
	diag35[++i] = col17[2];
	diag35[++i] = col17[5];
	diag35[++i] = col26[7];
	diag35[++i] = col35[9];
	diag35[++i] = col40[4];
	diag35[++i] = col35[4];
	diag35[++i] = col26[4];

	i = 0;
	diag40[i] = col17[4];
	diag40[++i] = col26[6];
	diag40[++i] = col35[8];
	diag40[++i] = col40[3];
	diag40[++i] = col35[3];
	diag40[++i] = col26[3];
	diag40[++i] = col17[3];



}
*/

/*void is_good_diag() {

	//>> ----------------------- Checking The diagonals ----------------------- <<

	//check the 1st and 7th diagonlas

	i = 0;
	if (diag[0][i]->filled && diag[0][++i]->filled  && diag[0][++i]->filled && (diag[0][++i]->filled || col[0][0]->filled)) {
		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			diag17[i]->filled = false;
		}
		/*if (col17[0]->filled) {
		col17[0]->filled = false;
		}
	}
	i = 4;
	if (diag17[i]->filled && diag17[++i]->filled && diag17[++i]->filled && diag17[++i]->filled) {
		for (i = 4; i < 8; i++) {
			printf("WOW ! \n");
			diag17[i]->filled = false;
		}
	}

	//check the 2nd and 6th diagonals
	i = 0;
	if (diag26[i]->filled == true && diag26[++i]->filled == true && diag26[++i]->filled == true && diag26[++i]->filled == true && diag26[++i]->filled == true) {
		printf("WOW ! \n");
		for (i = 0; i < 5; i++) {

			diag26[i]->filled = false;
		}
	}
	i = 5;
	if (diag26[i]->filled == true && diag26[++i]->filled == true && diag26[++i]->filled == true && diag26[++i]->filled == true && diag26[++i]->filled == true) {
		for (i = 5; i < 10; i++) {
			printf("WOW ! \n");
			diag26[i]->filled = false;
		}
	}

	//check the 3rd and 5th diagonals
	i = 0;
	if (diag35[i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true) {
		printf("WOW ! \n");
		for (i = 0; i < 6; i++) {

			diag35[i]->filled = false;
		}
	}
	i = 6;
	if (diag35[i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true && diag35[++i]->filled == true) {
		for (i = 6; i < 12; i++) {
			printf("WOW ! \n");
			diag35[i]->filled = false;
		}
	}

	//check the 4th diagonal
	i = 0;
	if (diag40[i]->filled == true && diag40[++i]->filled == true && diag40[++i]->filled == true && diag40[++i]->filled == true && diag40[++i]->filled == true && diag40[++i]->filled == true && diag40[++i]->filled == true) {
		printf("WOW ! \n");
		for (i = 0; i < 7; i++) {

			diag40[i]->filled = false;
		}
	}


}
*/



/*void check() {
	//>> ----------------------- Checking The columns ----------------------- <<

	//check the 1st and 7th columns

	i = 0;
	if ((cell[i].filled  && cell[++i].filled  && cell[++i].filled && cell[++i].filled)) {
		printf("WOW ! \n");
		for (i = 0; i < 4; i++) {

			cell[i].filled = false;
		}
	}
	i = 33;
	if (cell[i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true) {
		for (i = 33; i < 37; i++) {
			printf("WOW ! \n");
			cell[i].filled = false;
		}
	}

	//check the 2nd and 6th columns
	i = 4;
	if (cell[i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true) {
		printf("WOW ! \n");
		for (i = 4; i < 9; i++) {

			cell[i].filled = false;
		}
	}
	i = 28;
	if (cell[i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true) {
		for (i = 28; i < 33; i++) {
			printf("WOW ! \n");
			cell[i].filled = false;
		}
	}

	//check the 3rd and 5th columns
	i = 9;
	if (cell[i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true) {
		printf("WOW ! \n");
		for (i = 9; i < 15; i++) {

			cell[i].filled = false;
		}
	}
	i = 22;
	if (cell[i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true) {
		for (i = 22; i < 28; i++) {
			printf("WOW ! \n");
			cell[i].filled = false;
		}
	}

	//check the 4th columns
	i = 15;
	if (cell[i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true && cell[++i].filled == true) {
		printf("WOW ! \n");
		for (i = 15; i < 22; i++) {

			cell[i].filled = false;
		}
	}


	//Check the inv_diagonals elements 



	//cell_update();

	/*for (i = 0; i < 37; i++) {
	if (!cell[i].filled) {
	cell[i].bitmap = al_clone_bitmap(bitmap);
	}
	}
}*/



/*void draw() {

	int i, j, s[3], n[3], c;

	cellX = originX, cellY = originY;

	al_clear_to_color(background_color);
	//al_draw_bitmap(bitmap3, originX, originY - 55, NULL);


	// ======================  >>>>> Drawing the hexagonal <<<<< =========================

	for (j = 0; j < 4; j++) {

		//al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY + 1, NULL);
		al_draw_bitmap(cell[j].bitmap, cellX, cellY + 1, NULL); //drawing the 1st column
		al_draw_bitmap(cell[j + 33].bitmap, cellX + 210.4, cellY + 1, NULL); //drawing the 7th column


																			 //printf("cell[%d] = X - %d Y - %d\n", j, cell[j].x, cell[j].y);

																			 //Set the coordinates for each cell in the 1st and 7th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 33].x = cellX + 210.4;
		cell[j + 33].y = cellY + 1;

		cellY += 35; //increase the height value only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The 2nd and 6th columns
	cellX = originX + 35, cellY = originY - 17;

	for (j = 4; j < 9; j++) {

		al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);
		al_draw_bitmap(cell[j + 24].bitmap, cellX + 140, cellY, NULL);

		//Set the coordinates for each cell in the 2nd and 6th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 24].x = cellX + 140;
		cell[j + 24].y = cellY;

		cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The 3rd and 5th columns 
	cellX = originX + 70, cellY = originY - 35;

	for (j = 9; j < 15; j++) {

		al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);
		al_draw_bitmap(cell[j + 13].bitmap, cellX + 70, cellY, NULL);

		//Set the coordinates for each cell in the 3rd and 5th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 13].x = cellX + 70;
		cell[j + 13].y = cellY + 1;

		cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The last column (the middle one)
	cellX = originX + 105, cellY = originY - 53;

	for (j = 15; j < 22; j++) {

		al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);

		//Setting the coordinates for each cell in the 4th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cellY += 35.2; //increase the height value only in order to continue drawing from top to the bottom
	}

	// ======================  >>>>> Drawing the Elements <<<<< =========================


	//drawing for the first time and init the coordinates for each element

	if (!move[0].state && !move[1].state && !move[2].state && !elem[0].exist && !elem[1].exist && !elem[2].exist) {
		redraw = 1;
	}

	if (!move[0].state && !move[1].state && !move[2].state && redraw) { //When Start

		srand(time(NULL));

		s[0] = rand() % 3;
		s[1] = rand() % 3;
		s[2] = rand() % 3;
		c = rand() % 6;
		rand_elem();
		elX = elemX, elY = elemY;

		for (i = 0; i < 3; i++) {


			//elem[i].bitmap = al_clone_bitmap(bitmap2);
			//elem[i] = rndElem[1];
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
			elX += 125; //increase the distance value only in order to continue drawing from left to the right

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
		//		elem[i] = testEl[n[i]];
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
}*/ //draw function with the empty cell



/*void draw() {

	int i, j, s[3], n[3], c;
	
	cellX = originX, cellY = originY;

	al_clear_to_color(background_color);
	//al_draw_bitmap(bitmap3, originX, originY - 55, NULL);
	

	// ======================  >>>>> Drawing the hexagonal <<<<< =========================

	for (j = 0; j < 4; j++) {

		//al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY + 1, NULL);
		al_draw_bitmap(cell[j].bitmap, cellX, cellY + 1, NULL); //drawing the 1st column
		al_draw_bitmap(cell[j + 33].bitmap, cellX + 198, cellY + 1, NULL); //drawing the 7th column


		//printf("cell[%d] = X - %d Y - %d\n", j, cell[j].x, cell[j].y);

		//Set the coordinates for each cell in the 1st and 7th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 33].x = cellX + 210.4;
		cell[j + 33].y = cellY + 1;

		cellY += 35; //increase the height value only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The 2nd and 6th columns
	cellX = originX + 33, cellY = originY - 17;

	for (j = 4; j < 9; j++) {

			al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);
			al_draw_bitmap(cell[j + 24].bitmap, cellX + 132, cellY, NULL);

		//Set the coordinates for each cell in the 2nd and 6th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 24].x = cellX + 140;
		cell[j + 24].y = cellY;

		cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The 3rd and 5th columns 
	cellX = originX + 66, cellY = originY - 35;

	for (j = 9; j < 15; j++) {

			al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);
			al_draw_bitmap(cell[j + 13].bitmap, cellX + 66, cellY, NULL);

		//Set the coordinates for each cell in the 3rd and 5th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cell[j + 13].x = cellX + 70;
		cell[j + 13].y = cellY + 1;

		cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
	}

	//The new coordinates for The last column (the middle one)
	cellX = originX + 99, cellY = originY - 53;

	for (j = 15; j < 22; j++) {

			al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL); 

		//Setting the coordinates for each cell in the 4th column
		cell[j].x = cellX;
		cell[j].y = cellY + 1;

		cellY += 35.2; //increase the height value only in order to continue drawing from top to the bottom
	}

	// ======================  >>>>> Drawing the Elements <<<<< =========================


	//drawing for the first time and init the coordinates for each element

	if (!move[0].state && !move[1].state && !move[2].state && !elem[0].exist && !elem[1].exist && !elem[2].exist) {
		redraw = 1;
	}

	if (!move[0].state && !move[1].state && !move[2].state && redraw) { //When Start

		srand(time(NULL));

		s[0] = rand() % 3;
		s[1] = rand() % 3;
		s[2] = rand() % 3;
		c = rand() % 6;
		rand_elem();
		elX = elemX, elY = elemY;

		for (i = 0; i < 3; i++) {

			
			//elem[i].bitmap = al_clone_bitmap(bitmap2);
			//elem[i] = rndElem[1];
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
			elX += 125; //increase the distance value only in order to continue drawing from left to the right
			
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
//		elem[i] = testEl[n[i]];
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
*/ //new draw function


/*

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

cellY += 35; //increase the height value only in order to continue drawing from top to the bottom
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

cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
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

cellY += 35.2; //increase the height values only in order to continue drawing from top to the bottom
}

//The new coordinates for The last column (the middle one)
cellX = originX + 99, cellY = originY - 53;

for (j = 15; j < 22; j++) {

al_draw_tinted_bitmap(cell[j].bitmap, cell[j].cellClr, cellX, cellY, NULL);
//al_draw_bitmap(cell[j].bitmap, cellX, cellY, NULL);

//Setting the coordinates for each cell in the 4th column
cell[j].x = cellX;
cell[j].y = cellY + 1;

cellY += 35.2; //increase the height value only in order to continue drawing from top to the bottom
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


*/ //elemCellNewMod
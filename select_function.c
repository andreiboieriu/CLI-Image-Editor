// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "select_function.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUM_OF_COORDINATES 4

// function that sets selection to the given coordinates
void select_zone(pixel_matrix *pix_mat, char *coordinates)
{
	int int_coord[NUM_OF_COORDINATES];
	size_t count = 0;
	int negative_coordinates = 0;

	// read coordinates from string
	while (coordinates && count < NUM_OF_COORDINATES) {
		for (size_t i = 0; i < strlen(coordinates); i++) {
			if ((coordinates[i] < '0' || coordinates[i] > '9') &&
				coordinates[i] != '-') {
				printf("Invalid command\n");
				return;
			}
		}

		sscanf(coordinates, "%d", &int_coord[count++]);
		if (int_coord[count - 1] < 0)
			negative_coordinates = 1;

		coordinates = strtok(NULL, " ");
	}

	// in case less than 4 coordinates were read
	if (count != NUM_OF_COORDINATES) {
		printf("Invalid command\n");
		return;
	}

	size_t coord[NUM_OF_COORDINATES];

	// make coordinates size_t to avoid comparisons between signed and unsigned
	for (size_t i = 0; i < NUM_OF_COORDINATES; i++)
		coord[i] = int_coord[i];

	if (negative_coordinates || coord[0] == coord[2] || coord[1] == coord[3] ||
		coord[0] > pix_mat->width || coord[2] > pix_mat->width ||
		coord[1] > pix_mat->height || coord[3] > pix_mat->height) {
		printf("Invalid set of coordinates\n");
		return;
	}

	// in case x1 > x2
	if (coord[0] > coord[2]) {
		int aux = coord[0];
		coord[0] = coord[2];
		coord[2] = aux;
	}

	// in case y1 > y2
	if (coord[1] > coord[3]) {
		int aux = coord[1];
		coord[1] = coord[3];
		coord[3] = aux;
	}

	// swap x and y values so that x refers to row and y refers to column
	pix_mat->selection.first.x = coord[1];
	pix_mat->selection.first.y = coord[0];
	pix_mat->selection.second.x = coord[3] - 1;
	pix_mat->selection.second.y = coord[2] - 1;

	printf("Selected %zu %zu %zu %zu\n", coord[0], coord[1], coord[2],
		   coord[3]);
}

// function that sets selection to whole pixel matrix
void select_all(pixel_matrix *pix_mat)
{
	pix_mat->selection.first.x = 0;
	pix_mat->selection.first.y = 0;
	pix_mat->selection.second.x = pix_mat->height - 1;
	pix_mat->selection.second.y = pix_mat->width - 1;
}

// function that checks if whole pixel matrix is selected
bool all_is_selected(pixel_matrix pix_mat)
{
	if (pix_mat.selection.first.x == 0 &&
		pix_mat.selection.first.y == 0 &&
		pix_mat.selection.second.x == pix_mat.height - 1 &&
		pix_mat.selection.second.y == pix_mat.width - 1)
		return true;
	else
		return false;
}

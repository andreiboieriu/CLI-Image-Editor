// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "rotate_function.h"
#include "select_function.h"

#include <stdlib.h>
#include <stdio.h>

// function that rotates a pixel matrix at a specified angle
void rotate(pixel_matrix *pix_mat, char *angle)
{
	int rotations = atoi(angle);

	if ((rotations == 0 && angle[0] != '0') || rotations > 360 ||
		rotations < -360 || rotations % 90) {
		printf("Unsupported rotation angle\n");
		return;
	}

	/*
		reduce rotations to a value from 0 to 4
		-270 degrees is equivalent to 90 degrees
		-180 degrees is equivalent to 180 degrees
		-90 degrees is equivalent to 270 degrees
		+-360 degrees is equivalent to 0 degrees
	*/
	rotations /= 90;

	if (rotations % 4 == 0)
		rotations = 0;

	if (rotations < 0)
		rotations += 4;

	int dist1 = pix_mat->selection.second.x - pix_mat->selection.first.x;
	int dist2 = pix_mat->selection.second.y - pix_mat->selection.first.y;

	if (!all_is_selected(*pix_mat) && dist1 != dist2) {
		printf("The selection must be square\n");
		return;
	}

	if (all_is_selected(*pix_mat) && dist1 != dist2) // case rectangle
		rotate_rectangle_right(&(*pix_mat), rotations);
	else // case square
		rotate_square_right(*pix_mat, rotations);

	printf("Rotated %s\n", angle);
}

/*
	function that rotates a squarae pixel matrix 90 degrees to the right
	num_of_rotations times
*/
void rotate_square_right(pixel_matrix pix_mat, size_t num_of_rotations)
{
	point first = pix_mat.selection.first;
	point second = pix_mat.selection.second;

	size_t size = second.y - first.y + 1;

	for (size_t layer = 0; layer < size / 2; layer++) {
		for (size_t i = first.y + layer; i < second.y - layer; i++) {
			point coord;
			coord.x = layer + first.x;
			coord.y = i;

			rotate_pixel(pix_mat, coord, num_of_rotations);
		}
	}
}

// function that rotates a set of 4 pixels in a square pixel matrix
void rotate_pixel(pixel_matrix pix_mat, point coord, size_t num_of_rotations)
{
	size_t layer = coord.x - pix_mat.selection.first.x;

	point coordinates[4];
	pixel pixels[4];

	for (size_t i = 0; i < 4; i++) {
		coordinates[i] = coord;
		pixels[i] = pix_mat.matrix[coord.x][coord.y];
		coord = rotate_coordinates_square(coord, pix_mat.selection, layer);
	}

	for (size_t i = 0; i < 4; i++) {
		size_t index = (4 - num_of_rotations + i) % 4;
		pix_mat.matrix[coordinates[i].x][coordinates[i].y] = pixels[index];
	}
}

/*
	function that calculates the coordinates of a pixel in a square matrix
	after a 90 degrees rotation to the right
*/
point rotate_coordinates_square(point coord, zone selection, size_t layer)
{
	point result;

	if (coord.x == selection.first.x + layer) {
		result.x = selection.first.x + coord.y - selection.first.y;
		result.y = selection.second.y - layer;
	} else if (coord.y == selection.second.y - layer) {
		result.x = selection.second.x - layer;
		result.y = selection.second.y - coord.x + selection.first.x;
	} else if (coord.x == selection.second.x - layer) {
		result.x = selection.second.x - selection.second.y + coord.y;
		result.y = selection.first.y + layer;
	} else if (coord.y == selection.first.y + layer) {
		result.x = selection.first.x + layer;
		result.y = selection.first.y + selection.second.x - coord.x;
	}

	return result;
}

/*
	function that calculates the coordinates of a pixel in a rectangular matrix
	after a 90 degrees rotation to the right
*/
point rotate_coordinates_rectangle(point coord, size_t height)
{
	point result;

	result.x = coord.y;
	result.y = height - coord.x - 1;

	return result;
}

/*
	function that rotates a rectangular pixel matrix 90 degrees to the right
	num_of_rotations times
*/
void rotate_rectangle_right(pixel_matrix *pix_mat, size_t num_of_rotations)
{
	pixel_matrix initial_matrix = duplicate_pixel_matrix(*pix_mat);

	size_t height = initial_matrix.height;
	size_t width = initial_matrix.width;

	if (num_of_rotations % 2)
		resize_pixel_matrix(&(*pix_mat), height, width);

	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			point coord;

			coord.x = i;
			coord.y = j;

			for (size_t i = 0; i < num_of_rotations; i++)
				if (i % 2)
					coord = rotate_coordinates_rectangle(coord, width);
				else
					coord = rotate_coordinates_rectangle(coord, height);

			pix_mat->matrix[coord.x][coord.y] = initial_matrix.matrix[i][j];
		}
	}

	free_pixel_matrix(&initial_matrix);
}

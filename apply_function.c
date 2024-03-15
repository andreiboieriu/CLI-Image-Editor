// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "apply_function.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define KERNEL_SIZE 3
#define NUM_OF_EFFECTS 4

// function that applies a chosen effect to a pixel matrix
void apply_effect(pixel_matrix *pix_mat, char *effect)
{
	if (!effect) {
		printf("Invalid command\n");
		return;
	}

	// supported effects
	char *effect_arr[NUM_OF_EFFECTS] = {"EDGE",
										"SHARPEN",
										"BLUR",
										"GAUSSIAN_BLUR"};

	// kernels for the supported effects
	double kernel_arr[][KERNEL_SIZE][KERNEL_SIZE] = {
		{
			{-1, -1, -1},
			{-1,  8, -1}, // edge kernel
			{-1, -1, -1}
		},

		{
			{ 0, -1,  0},
			{-1,  5, -1}, // sharpen kernel
			{ 0, -1,  0}
		},

		{
			{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
			{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, // blur kernel
			{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
		},

		{
			{1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0},
			{1.0 / 8.0, 1.0 / 4.0, 1.0 / 8.0},   // gaussian_blur kernel
			{1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0}
		}
	};

	for (size_t i = 0; i < 4; i++)
		if (!strcmp(effect, effect_arr[i])) {
			apply_kernel(*pix_mat, kernel_arr[i]);

			modify_maxval(pix_mat);

			printf("APPLY %s done\n", effect);
			return;
		}

	printf("APPLY parameter invalid\n");
}

// function that applies a kernel to a pixel matrix
void apply_kernel(pixel_matrix pix_mat, double kernel[3][3])
{
	point first = pix_mat.selection.first;
	point second = pix_mat.selection.second;

	pixel_matrix processed_pix_mat;

	processed_pix_mat.height = second.x - first.x + 1;
	processed_pix_mat.width = second.y - first.y + 1;

	alloc_pixel_matrix(&processed_pix_mat);

	// values for moving in adiacent cells in a matrix
	int dx[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int dy[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

	// compute processed pixel matrix
	for (size_t i = 0; i < processed_pix_mat.height; i++) {
		for (size_t j = 0; j < processed_pix_mat.width; j++) {
			point coord;
			coord.x = i + first.x;
			coord.y = j + first.y;

			if (coord.x == 0 || coord.x == pix_mat.height - 1 ||
				coord.y == 0 || coord.y == pix_mat.width - 1) {
				processed_pix_mat.matrix[i][j].color.red =
				pix_mat.matrix[coord.x][coord.y].color.red;

				processed_pix_mat.matrix[i][j].color.green =
				pix_mat.matrix[coord.x][coord.y].color.green;

				processed_pix_mat.matrix[i][j].color.blue =
				pix_mat.matrix[coord.x][coord.y].color.blue;

				continue;
			}

			processed_pix_mat.matrix[i][j].color.red = 0;
			processed_pix_mat.matrix[i][j].color.blue = 0;
			processed_pix_mat.matrix[i][j].color.green = 0;

			for (size_t k = 0; k < 9; k++) {
				processed_pix_mat.matrix[i][j].color.red +=
				pix_mat.matrix[coord.x + dx[k]][coord.y + dy[k]].color.red *
				kernel[1 + dx[k]][1 + dy[k]];

				processed_pix_mat.matrix[i][j].color.green +=
				pix_mat.matrix[coord.x + dx[k]][coord.y + dy[k]].color.green *
				kernel[1 + dx[k]][1 + dy[k]];

				processed_pix_mat.matrix[i][j].color.blue +=
				pix_mat.matrix[coord.x + dx[k]][coord.y + dy[k]].color.blue *
				kernel[1 + dx[k]][1 + dy[k]];
			}

			clamp(&processed_pix_mat.matrix[i][j].color.red);
			clamp(&processed_pix_mat.matrix[i][j].color.green);
			clamp(&processed_pix_mat.matrix[i][j].color.blue);
		}
	}

	// copy processed pixel matrix to original pixel matrix
	for (size_t i = first.x; i <= second.x; i++) {
		for (size_t j = first.y; j <= second.y; j++) {
			point coord;
			coord.x = i - first.x;
			coord.y = j - first.y;

			pix_mat.matrix[i][j].color.red =
			processed_pix_mat.matrix[coord.x][coord.y].color.red;

			pix_mat.matrix[i][j].color.green =
			processed_pix_mat.matrix[coord.x][coord.y].color.green;

			pix_mat.matrix[i][j].color.blue =
			processed_pix_mat.matrix[coord.x][coord.y].color.blue;
		}
	}

	free_pixel_matrix(&processed_pix_mat);
}

/*
	function that calculates the highest pixel value in a pixel matrix
	and modifies maxval accordingly
*/
void modify_maxval(pixel_matrix *pix_mat)
{
	double maxval = 0;

	for (size_t i = 0; i < pix_mat->height; i++) {
		for (size_t j = 0; j < pix_mat->width; j++) {
			if (pix_mat->matrix[i][j].color.red > maxval)
				maxval = pix_mat->matrix[i][j].color.red;

			if (pix_mat->matrix[i][j].color.green > maxval)
				maxval = pix_mat->matrix[i][j].color.green;

			if (pix_mat->matrix[i][j].color.blue > maxval)
				maxval = pix_mat->matrix[i][j].color.blue;
		}
	}

	pix_mat->maxval = (unsigned char)round(maxval);
}

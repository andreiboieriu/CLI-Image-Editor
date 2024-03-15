// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "crop_function.h"
#include "select_function.h"

#include <stdio.h>

// function that crops a pixel matrix to the selection
void crop_selection(pixel_matrix *pix_mat)
{
	if (all_is_selected(*pix_mat)) {
		printf("Image cropped\n");
		return;
	}

	size_t height = pix_mat->selection.second.x -
					pix_mat->selection.first.x + 1;
	size_t width = pix_mat->selection.second.y -
				   pix_mat->selection.first.y + 1;

	// move selection to the upper left corner of the pixel matrix
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			point p = pix_mat->selection.first;

			p.x += i;
			p.y += j;

			pix_mat->matrix[i][j] = pix_mat->matrix[p.x][p.y];
		}
	}

	// resize pixel matrix accordingly
	resize_pixel_matrix(&(*pix_mat), width, height);

	select_all(pix_mat);

	printf("Image cropped\n");
}

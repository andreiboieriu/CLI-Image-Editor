// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "exit_function.h"

#include <stdio.h>

// function that sets quit to 1 and frees allocated memory
void exit_editor(pixel_matrix *pix_mat, int *quit, int image_loaded)
{
	if (image_loaded)
		free_pixel_matrix(&(*pix_mat));
	else
		printf("No image loaded\n");

	*quit = 1;
}

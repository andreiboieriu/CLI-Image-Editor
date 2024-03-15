// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "load_function.h"
#include "select_function.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// function that reads a pnm image from a file
void load_image(pnm_image *image, char *filename, int *image_loaded)
{
	if (*image_loaded) {
		free_pixel_matrix(&image->pix_mat);
		*image_loaded = 0;
	}

	FILE * fp = fopen(filename, "r");

	if (!fp) {
		fprintf(stderr, "fopen() failed");
		printf("Failed to load %s\n", filename);
		return;
	}

	// read magic word, dimensions, maxval; allocate memory for pixel matrix
	if (read_file_until_pixel_matrix(fp, image) == FAILURE ||
		alloc_pixel_matrix(&image->pix_mat) == FAILURE) {
		printf("Failed to load %s\n", filename);
		return;
	}

	// if pixel matrix is stored as binary data reopen file as binary read-only
	if (is_binary(image->magic_word)) {
		long pos = ftell(fp);

		fp = freopen(filename, "rb", fp);

		if (!fp) {
			fprintf(stderr, "freopen() failed");

			printf("Failed to load %s\n", filename);
			return;
		}

		// set position in file to the beginning of the pixel matrix
		fseek(fp, pos, SEEK_SET);
	}

	// read pixel matrix, free allocated memory if it fails
	if (read_pixel_matrix(fp, image->pix_mat, image->magic_word) == FAILURE) {
		free_pixel_matrix(&image->pix_mat);

		printf("Failed to load %s\n", filename);
		return;
	}

	fclose(fp);

	select_all(&image->pix_mat);

	*image_loaded = 1;

	printf("Loaded %s\n", filename);
}

// function that reads magic word, dimensions and maxval from a file
int read_file_until_pixel_matrix(FILE *fp, pnm_image *image)
{
	char *line = NULL;
	size_t buffer_size = 0;

	int read_magic_word = 0;
	int read_dimensions = 0;

	while (getline(&line, &buffer_size, fp) != -1) {
		// ignore comments
		if (line[0] != '#') {
			remove_newline(line);

			if (!read_magic_word) {
				strcpy(image->magic_word, line);

				if (!is_valid(image->magic_word))
					return FAILURE;

				read_magic_word = 1;
			} else if (!read_dimensions) {
				if (sscanf(line, "%zu%zu", &image->pix_mat.width,
						   &image->pix_mat.height) == EOF)
					return FAILURE;

				read_dimensions = 1;
			} else {
				if (sscanf(line, "%hhu", &image->pix_mat.maxval) == EOF)
					return FAILURE;

				// read all comments between maxval and pixel matrix
				while (fgetc(fp) == '#')
					getline(&line, &buffer_size, fp);

				// set position in file to the beginning of the pixel matrix
				fseek(fp, -1, SEEK_CUR);

				break;
			}
		}
	}

	// free memory allocated by getline
	free(line);

	return SUCCES;
}

// function that reads a pixel matrix from a file
int read_pixel_matrix(FILE *fp, pixel_matrix pix_mat, char *magic_word)
{
	for (size_t i = 0; i < pix_mat.height; i++)
		for (size_t j = 0; j < pix_mat.width; j++)
			if (read_pixel(fp, &pix_mat.matrix[i][j], magic_word) == FAILURE)
				return FAILURE;

	return SUCCES;
}

// function that reads a color/grayscale pixel from a file
int read_pixel(FILE *fp, pixel *pix, char *magic_word)
{
	if (is_binary(magic_word)) {
		if (is_color(magic_word)) {
			unsigned char rgb[3];
			if (fread(rgb, sizeof(unsigned char), 3, fp) != 3)
				return FAILURE;

			pix->color.red = rgb[0];
			pix->color.green = rgb[1];
			pix->color.blue = rgb[2];
		} else {
			if (fread(&pix->grayscale, sizeof(grayscale_pixel), 1, fp) != 1)
				return FAILURE;
		}
	} else {
		if (is_color(magic_word)) {
			if (fscanf(fp, "%lf", &pix->color.red) == EOF ||
				fscanf(fp, "%lf", &pix->color.green) == EOF ||
				fscanf(fp, "%lf", &pix->color.blue) == EOF)
				return FAILURE;
		} else {
			if (fscanf(fp, "%hhu", &pix->grayscale.value) == EOF)
				return FAILURE;
		}
	}

	return SUCCES;
}

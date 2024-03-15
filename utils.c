// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "select_function.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function that removes trailing newline from a string
void remove_newline(char *string)
{
	if (string[strlen(string) - 1] == '\n')
		string[strlen(string) - 1] = '\0';
}

// function that allocates memory for a pixel matrix
int alloc_pixel_matrix(pixel_matrix *pix_mat)
{
	pix_mat->matrix = malloc(pix_mat->height * sizeof(pixel *));

	if (!pix_mat->matrix) {
		fprintf(stderr, "malloc() failed");
		return FAILURE;
	}

	for (size_t i = 0; i < pix_mat->height; i++) {
		pix_mat->matrix[i] = malloc(pix_mat->width * sizeof(pixel));

		if (!pix_mat->matrix[i]) {
			for (size_t j = 0; j < i; j++)
				free(pix_mat->matrix[j]);

			fprintf(stderr, "malloc() failed");
			return FAILURE;
		}
	}

	return SUCCES;
}

// function that checks if a magic word refers to a color image
bool is_color(char *magic_word)
{
	return (!strcmp(magic_word, COLOR_ASCII) || !strcmp(magic_word,
														COLOR_BINARY));
}

// function that checks if a magic word refers to a binary image
bool is_binary(char *magic_word)
{
	return (!strcmp(magic_word, COLOR_BINARY) || !strcmp(magic_word,
														 GRAYSCALE_BINARY));
}

// function that checks if a magic word is valid
bool is_valid(char *magic_word)
{
	char valid_magic_words[][MAGIC_WORD_SIZE] = {COLOR_BINARY,
												 GRAYSCALE_BINARY,
												 COLOR_ASCII,
												 GRAYSCALE_ASCII};

	for (int i = 0; i < 4; i++)
		if (!strcmp(magic_word, valid_magic_words[i]))
			return true;

	return false;
}

// function that resizes a pixel matrix
int resize_pixel_matrix(pixel_matrix *pix_mat, size_t width, size_t height)
{
	for (size_t i = height; i < pix_mat->height; i++) {
		free(pix_mat->matrix[i]);
		pix_mat->matrix[i] = NULL;
	}

	pixel **p = realloc(pix_mat->matrix, height * sizeof(pixel *));

	if (!p) {
		fprintf(stderr, "realloc() failed");
		return FAILURE;
	}

	pix_mat->matrix = p;

	for (size_t i = 0; i < height; i++) {
		if (i < pix_mat->height) {
			pixel *p = realloc(pix_mat->matrix[i], width * sizeof(pixel));

			if (!p) {
				fprintf(stderr, "realloc() failed");
				return FAILURE;
			}

			pix_mat->matrix[i] = p;
		} else {
			pix_mat->matrix[i] = malloc(width * sizeof(pixel));

			if (!pix_mat->matrix[i]) {
				for (size_t j = pix_mat->height; j < i; j++) {
					free(pix_mat->matrix[j]);
					pix_mat->matrix[j] = NULL;
				}

				fprintf(stderr, "malloc() failed");
				return FAILURE;
			}
		}
	}

	pix_mat->width = width;
	pix_mat->height = height;

	select_all(pix_mat);

	return SUCCES;
}

// function that frees a pixel matrix
void free_pixel_matrix(pixel_matrix *pix_mat)
{
	for (size_t i = 0; i < pix_mat->height; i++) {
		free(pix_mat->matrix[i]);
		pix_mat->matrix[i] = NULL;
	}

	free(pix_mat->matrix);
	pix_mat->matrix = NULL;

	pix_mat->width = 0;
	pix_mat->height = 0;
	pix_mat->maxval = 0;
}

// function that clamps a double between 0 and 255
void clamp(double *value)
{
	if (*value < 0)
		*value = 0;
	if (*value > 255)
		*value = 255;
}

// function that rounds a double and returns it as unsigned char
unsigned char double_to_uchar(double n)
{
	return (unsigned char)round(n);
}

// function that duplicates a pixel matrix
pixel_matrix duplicate_pixel_matrix(pixel_matrix src)
{
	pixel_matrix new_matrix;

	new_matrix.width = src.width;
	new_matrix.height = src.height;
	new_matrix.maxval = src.maxval;
	new_matrix.selection = src.selection;

	alloc_pixel_matrix(&new_matrix);

	for (size_t i = 0; i < new_matrix.height; i++)
		for (size_t j = 0; j < new_matrix.width; j++)
			new_matrix.matrix[i][j] = src.matrix[i][j];

	return new_matrix;
}

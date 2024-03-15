// Copyright 2021 Boieriu Andrei-Teodor 312CA
#pragma once

#include <stdio.h>
#include <stdbool.h>

#define MAGIC_WORD_SIZE 3
#define SUCCES 0
#define FAILURE -1
#define COLOR_BINARY "P6"
#define GRAYSCALE_BINARY "P5"
#define COLOR_ASCII "P3"
#define GRAYSCALE_ASCII "P2"

typedef struct {
	size_t x;
	size_t y;
} point;

typedef struct {
	point first;
	point second;
} zone;

typedef struct {
	unsigned char value;
} grayscale_pixel;

typedef struct {
	double red;
	double green;
	double blue;
} color_pixel;

typedef union {
	color_pixel color;
	grayscale_pixel grayscale;
} pixel;

typedef struct {
	size_t width;
	size_t height;

	unsigned char maxval;

	pixel **matrix;

	zone selection;
} pixel_matrix;

typedef struct {
	char magic_word[MAGIC_WORD_SIZE];

	pixel_matrix pix_mat;
} pnm_image;

void remove_newline(char *string);

bool is_color(char *magic_word);

bool is_binary(char *magic_word);

bool is_valid(char *magic_word);

int alloc_pixel_matrix(pixel_matrix *pix_mat);

int resize_pixel_matrix(pixel_matrix *pix_mat, size_t width, size_t height);

void free_pixel_matrix(pixel_matrix *pix_mat);

void clamp(double *value);

unsigned char double_to_uchar(double n);

pixel_matrix duplicate_pixel_matrix(pixel_matrix src);

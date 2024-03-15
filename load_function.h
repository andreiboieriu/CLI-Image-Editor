// Copyright 2021 Boieriu Andrei-Teodor 312CA
#pragma once

#include "utils.h"

#include <stdio.h> // for FILE

void load_image(pnm_image * image, char *filename, int *image_loaded);

int read_file_until_pixel_matrix(FILE *fp, pnm_image *image);

int read_pixel_matrix(FILE *fp, pixel_matrix pix_mat, char *magic_word);

int read_pixel(FILE *fp, pixel *pix, char *magic_word);

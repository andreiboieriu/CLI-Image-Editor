// Copyright 2021 Boieriu Andrei-Teodor 312CA
#pragma once

#include "utils.h"

#include <stdio.h> // for FILE
#include <stdbool.h> // for bool

void save_image(pnm_image image, char *parameters);

void save_pixel_matrix(pixel_matrix pix_mat, FILE *fp, bool ascii, bool color);

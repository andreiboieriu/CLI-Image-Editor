// Copyright 2021 Boieriu Andrei-Teodor 312CA
#pragma once

#include "utils.h"

void apply_effect(pixel_matrix * pix_mat, char *effect);

void apply_kernel(pixel_matrix pix_mat, double kernel[3][3]);

void modify_maxval(pixel_matrix *pix_mat);

// Copyright 2021 Boieriu Andrei-Teodor 312CA
#pragma once

#include "utils.h"

#include <stdio.h> //for size_t

void rotate(pixel_matrix * pix_mat, char *angle);

void rotate_square_right(pixel_matrix pix_mat, size_t num_of_rotations);

void rotate_rectangle_right(pixel_matrix *pix_mat, size_t num_of_rotations);

void rotate_pixel(pixel_matrix pix_mat, point coord, size_t num_of_rotations);

point rotate_coordinates_square(point coord, zone selection, size_t layer);

point rotate_coordinates_rectangle(point coord, size_t height);

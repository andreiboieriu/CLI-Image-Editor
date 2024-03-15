// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "save_function.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// function that writes a pnm image to a file
void save_image(pnm_image image, char *parameters)
{
	parameters = strtok(NULL, " ");

	if (!parameters) {
		printf("Invalid command\n");
		return;
	}

	bool ascii = false;

	char filename[strlen(parameters) + 1];
	strcpy(filename, parameters);

	parameters = strtok(NULL, " ");
	if (parameters && !strcmp(parameters, "ascii"))
		ascii = true;

	FILE *fp = fopen(filename, "w");

	if (!fp) {
		printf("Failed to save %s\n", filename);
		fprintf(stderr, "fopen() failed");
		return;
	}

	// write the magic_word
	if (is_binary(image.magic_word) && ascii) {
		if (is_color(image.magic_word))
			fprintf(fp, "%s\n", COLOR_ASCII);
		else
			fprintf(fp, "%s\n", GRAYSCALE_ASCII);
	} else if (!is_binary(image.magic_word) && !ascii) {
		if (is_color(image.magic_word))
			fprintf(fp, "%s\n", COLOR_BINARY);
		else
			fprintf(fp, "%s\n", GRAYSCALE_BINARY);
	} else {
		fprintf(fp, "%s\n", image.magic_word);
	}

	// write dimensions and maxval
	fprintf(fp, "%zu %zu\n%d\n", image.pix_mat.width, image.pix_mat.height,
			image.pix_mat.maxval);

	if (!ascii) {
		fp = freopen(filename, "ab", fp);

		if (!fp) {
			printf("Failed to save %s\n", filename);
			fprintf(stderr, "freopen() failed");
			return;
		}
	}

	// write pixel matrix
	save_pixel_matrix(image.pix_mat, fp, ascii, is_color(image.magic_word));

	fclose(fp);

	printf("Saved %s\n", filename);
}

// function that writes a pixel matrix to a file
void save_pixel_matrix(pixel_matrix pix_mat, FILE *fp, bool ascii, bool color)
{
	for (size_t i = 0; i < pix_mat.height; i++) {
		for (size_t j = 0; j < pix_mat.width; j++) {
			if (ascii) {
				if (color) {
					// print rounded double values
					fprintf(fp, "%hhu ",
							double_to_uchar(pix_mat.matrix[i][j].color.red));

					fprintf(fp, "%hhu ",
							double_to_uchar(pix_mat.matrix[i][j].color.green));

					fprintf(fp, "%hhu ",
							double_to_uchar(pix_mat.matrix[i][j].color.blue));
				} else {
					fprintf(fp, "%hhu ", pix_mat.matrix[i][j].grayscale.value);
				}
			} else {
				if (color) {
					unsigned char rgb[3];

					// round double values
					rgb[0] = double_to_uchar(pix_mat.matrix[i][j].color.red);
					rgb[1] = double_to_uchar(pix_mat.matrix[i][j].color.green);
					rgb[2] = double_to_uchar(pix_mat.matrix[i][j].color.blue);

					fwrite(rgb, sizeof(unsigned char), 3, fp);
				} else {
					fwrite(&pix_mat.matrix[i][j].grayscale,
						   sizeof(grayscale_pixel), 1, fp);
				}
			}
		}

		if (ascii)
			fprintf(fp, "\n");
	}
}

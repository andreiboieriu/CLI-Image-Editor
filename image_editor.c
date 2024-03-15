// Copyright 2021 Boieriu Andrei-Teodor 312CA
#include "utils.h"
#include "load_function.h"
#include "select_function.h"
#include "rotate_function.h"
#include "crop_function.h"
#include "apply_function.h"
#include "save_function.h"
#include "exit_function.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	pnm_image image;

	char *command = NULL;
	size_t buffer_size = 0;

	int quit = 0;
	int image_loaded = 0;

	// command parser
	while (!quit) {
		ssize_t characters_read = getline(&command, &buffer_size, stdin);

		if (characters_read == -1) {
			fprintf(stderr, "getline() failed");
			free(command);
			exit(EXIT_FAILURE);
		}

		remove_newline(command);

		char *token = strtok(command, " ");

		if (!strcmp(token, "LOAD")) {
			token = strtok(NULL, " ");
			load_image(&image, token, &image_loaded);
		} else if (!strcmp(token, "SELECT")) {
			if (!image_loaded) {
				printf("No image loaded\n");
			} else {
				token = strtok(NULL, " ");

				if (!strcmp(token, "ALL")) {
					select_all(&image.pix_mat);
					printf("Selected ALL\n");
				} else {
					select_zone(&image.pix_mat, token);
				}
			}
		} else if (!strcmp(token, "ROTATE")) {
			if (!image_loaded) {
				printf("No image loaded\n");
			} else {
				token = strtok(NULL, " ");
				rotate(&image.pix_mat, token);
			}
		} else if (!strcmp(token, "CROP")) {
			if (!image_loaded)
				printf("No image loaded\n");
			else
				crop_selection(&image.pix_mat);
		} else if (!strcmp(token, "APPLY")) {
			token = strtok(NULL, " ");
			if (!image_loaded)
				printf("No image loaded\n");
			else if (!is_color(image.magic_word) && token)
				printf("Easy, Charlie Chaplin\n");
			else
				apply_effect(&image.pix_mat, token);
		} else if (!strcmp(token, "SAVE")) {
			if (!image_loaded)
				printf("No image loaded\n");
			else
				save_image(image, token);
		} else if (!strcmp(token, "EXIT")) {
			exit_editor(&image.pix_mat, &quit, image_loaded);
		} else {
			printf("Invalid command\n");
		}
	}

	// free memory allocated by getline
	free(command);

	return 0;
}

#include "../hdrs/includes.h"

void	exit_error(char *error) {
	fprintf(stderr, "Error: %s\n", error);
	exit(1);
}

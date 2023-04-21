#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int		main(int ac, char **av, char **env) {
	while (*env)
		printf("%s\n", *(env++));
	return (0);
}

#include "../hdrs/builtin.h"

void	do_status(void) {
	printf("STATUS");
}

void	do_start(char **cmd) {
	if (!strcmp(cmd[1], ""))
		fprintf(stderr, "start [process_name]");
	printf("%s", cmd[1]);
}

void	do_stop(char **cmd) {
	printf("%s", cmd[1]);
}

void	do_restart(char **cmd) {
	printf("%s", cmd[1]);
}

void	do_reload(char **cmd) {
	printf("%s", cmd[1]);
}

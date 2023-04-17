#include "../hdrs/builtin.h"
#include "../hdrs/tmp.h"

void	parse_cmd(char **cmd) {
	if (!strcmp(cmd[0], "status"))
		do_status();
	else if (!strcmp(cmd[0], "start"))
		do_start(cmd);
	else if (!strcmp(cmd[0], "stop"))
		do_stop(cmd);
	else if (!strcmp(cmd[0], "restart"))
		do_restart(cmd);
	else if (!strcmp(cmd[0], "reload"))
		do_reload(cmd);
	else if (!strcmp(cmd[0], "exit"))
		exit(0);
	else
		fprintf(stderr, "%s: command not found", cmd[0]);
}

#include "../hdrs/builtin.h"
#include "../hdrs/tmp.h"
#include "../hdrs/action.h"

void	parse_cmd(t_action *action) {
	if (!strcmp(action->cmd[0], "status"))
		action_status();
	else if (!strcmp(action->cmd[0], "start"))
		action_start(action);
	else if (!strcmp(action->cmd[0], "stop"))
		action_stop(action);
	else if (!strcmp(action->cmd[0], "restart"))
		action_restart(action);
	else if (!strcmp(action->cmd[0], "reload"))
		action_reload(action);
	else if (!strcmp(action->cmd[0], "exit"))
		exit(0);
	else
		fprintf(stderr, "%s: command not found", action->cmd[0]);
}

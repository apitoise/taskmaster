#include "../hdrs/builtin.h"
#include "../hdrs/action.h"

void	action_status(void) {
	printf("STATUS");
}

void	action_start(t_action *action) {
	if (action->sz == 1)
		fprintf(stderr, "start [process_name]");
	else if (is_proc(action->cmd[1])) {
		
	}
}

void	action_stop(t_action *action) {
	printf("%s", action->cmd[1]);
}

void	action_restart(t_action *action) {
	printf("%s", action->cmd[1]);
}

void	action_reload(t_action *action) {
	printf("%s", action->cmd[1]);
}

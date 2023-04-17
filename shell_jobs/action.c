#include "action.h"

void	action_status(void) {
	printf("STATUS");
}

void	action_start(action_t *action) {
	if (action->sz == 1)
		fprintf(stderr, "start [process_name]");
}

void	action_stop(action_t *action) {
	printf("%s", action->cmd[1]);
}

void	action_restart(action_t *action) {
	printf("%s", action->cmd[1]);
}

void	action_reload(action_t *action) {
	printf("%s", action->cmd[1]);
}

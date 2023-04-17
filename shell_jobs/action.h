#ifndef ACTION_H
#define ACTION_H

#include "includes.h"

typedef struct	action_s {
	char		*cmd[2];
	int			sz;
}				action_t;

void	parse_cmd(action_t *action);
void	action_status(void);
void	action_start(action_t *action);
void	action_stop(action_t *action);
void	action_restart(action_t *action);
void	action_reload(action_t *action);

#endif

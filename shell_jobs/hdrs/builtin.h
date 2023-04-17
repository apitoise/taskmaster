#ifndef BUILTIN_H
#define BUILTIN_H

#include "includes.h"
#include "action.h"

void	parse_cmd(t_action *action);
void	action_status(void);
void	action_start(t_action *action);
void	action_stop(t_action *action);
void	action_restart(t_action *action);
void	action_reload(t_action *action);

#endif

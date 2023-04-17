#ifndef BUILTIN_H
#define BUILTIN_H

#include "includes.h"

void	parse_cmd(char **cmd);
void	do_status(void);
void	do_start(char **cmd);
void	do_stop(char **cmd);
void	do_restart(char **cmd);
void	do_reload(char **cmd);

#endif

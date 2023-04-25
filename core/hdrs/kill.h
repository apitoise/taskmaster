#ifndef KILL_H
#define KILL_H

#include "includes.h"
#include "prog_dic.h"

int	kill_prog(prog_t *prog, int signal);
int kill_prog_dic(prog_dic_t *prog_dic, int signal);

#endif

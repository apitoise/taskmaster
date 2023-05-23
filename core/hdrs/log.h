#ifndef LOG_H
#define LOG_H

#include "../hdrs/includes.h"
#include "../hdrs/prog.h"
#include "../hdrs/glob.h"

void	log_error(prog_t *prog, uint64_t proc_i, char *error);
void	log_info(prog_t *prog, char *info);
void	log_state(prog_t *prog, uint64_t proc_i);

#endif

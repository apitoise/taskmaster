#ifndef TMP_H
#define TMP_H

#include "includes.h"

typedef struct	s_proc {
	char	*proc_name[3];
	char	*proc_cmd[3];
	char	*proc_args[3][3];
	pid_t	proc_pid[3];
}				t_proc;

#endif

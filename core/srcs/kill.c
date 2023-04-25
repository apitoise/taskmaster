#include "../hdrs/kill.h"

int			kill_prog(prog_t *prog, int signal) {
	uint64_t	i;
	int			ret = 0;
	proc_t		*proc;

	for (i = 0; i < prog->procs->sz; ++i) {
		proc = (proc_t *)prog->procs->data[i];
		if (proc->pid && kill(proc->pid, signal))
			ret = -1;
	}
	return (ret);
}

int			kill_prog_dic(prog_dic_t *prog_dic, int signal) {
	uint64_t	i;
	int		ret = 0;

	for (i = 0; i < prog_dic->keys->sz; ++i)
		if (kill_prog((prog_t *)prog_dic->values->data[i], signal))
			ret = -1;
	return (ret);
}

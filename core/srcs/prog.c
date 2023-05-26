/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:00:57 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/26 16:30:35 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/prog.h"

static map_t		g_map = {
	.bools = {	"false", "true" },
	.auto_r = {	"always", "never", "unexpected" },
	.sigs_s = {	"ALRM", "HUP", "INT", "PIPE",
				"TERM",	"USR1", "USR2", "QUIT" },
	.sigs_v = {	SIGALRM, SIGHUP, SIGINT, SIGPIPE,
				SIGTERM, SIGUSR1, SIGUSR2, SIGQUIT },
};

static uint64_t		map_str(uint64_t *out, char **in, char *str, uint64_t sz) {
	uint64_t		i;

	for (i = 0; i < sz; ++i)
		if (!strcmp(in[i], str))
			return (out ? out[i] : i);
	return (U_ERROR);
}

prog_t			*prog_new(char *name, dict_t *opts) {
	char 		*str;
	uint64_t	opt_nb = 0;
	vec_t		*ex_cds, *env;
	prog_t		*new;
	unw_targ_t	targs[] = {
		{ "cmd", name, DT_STR },
		{ "numprocs", (void *)1, DT_UNB },
		{ "autostart", "false", DT_STR },
		{ "autorestart", "unexpected", DT_STR },
		{ "exitcodes", NULL, DT_VEC },
		{ "starttime", (void *)0, DT_UNB },
		{ "startretries", (void *)0, DT_UNB },
		{ "stopsignal", "INT", DT_STR },
		{ "stoptime", (void *)5, DT_UNB },
		{ "stdout", "./log_out", DT_STR },
		{ "stderr", "./log_err", DT_STR },
		{ "env", NULL, DT_VEC },
		{ "workingdir", "", DT_STR },
		{ "umask", (void *)0002, DT_UNB },
	};

	if (!(new = malloc(sizeof(prog_t))))
		return (NULL);
	bzero(new, sizeof(prog_t));
	new->name = name;
	if (dic_get_unw(opts, (void **)&new->cmd, &targs[UO_CMD], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->numprocs, &targs[UO_NUMPROCS], &opt_nb)
		|| !new->numprocs
		|| dic_get_unw(opts, (void **)&str, &targs[UO_AUTOSTART], &opt_nb)
		|| (new->autostart = map_str(NULL, g_map.bools, str, 2)) == U_ERROR
		|| dic_get_unw(opts, (void **)&str, &targs[UO_AUTORESTART], &opt_nb)
		|| (new->autorestart = map_str(NULL, g_map.auto_r, str, 3)) == U_ERROR
		|| dic_get_unw(opts, (void **)&ex_cds, &targs[UO_EXITCODES], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->starttime, &targs[UO_STARTTIME], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->startretries, &targs[UO_STARTRETRIES], &opt_nb)
		|| dic_get_unw(opts, (void **)&str, &targs[UO_STOPSIGNAL], &opt_nb)
		|| (new->stopsignal = map_str(g_map.sigs_v, g_map.sigs_s, str, 8)) == U_ERROR
		|| dic_get_unw(opts, (void **)&new->stoptime, &targs[UO_STOPTIME], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->std_out, &targs[UO_STDOUT], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->std_err, &targs[UO_STDERR], &opt_nb)
		|| dic_get_unw(opts, (void **)&env, &targs[UO_ENV], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->workingdir, &targs[UO_WORKINGDIR], &opt_nb)
		|| dic_get_unw(opts, (void **)&new->umask, &targs[UO_UMASK], &opt_nb)
		|| opt_nb != opts->values->sz
		|| !(new->exitcodes = vec_unw(ex_cds, DT_UNB))
		|| !(new->env = vec_unw(env, DT_STR))
		|| !(new->procs = vec_new(new->numprocs))
		|| (!new->exitcodes->sz && vec_push_back(new->exitcodes, 0))
		|| (vec_push_back(new->env, NULL))) {
		prog_free(new);
		return (NULL);
	}
	return (new);
}

void		prog_free(prog_t *prog) {
	uint64_t		i;

	if (prog->exitcodes)
		vec_free(prog->exitcodes);
	if (prog->env)
		vec_free(prog->env);
	if (prog->procs) {
		for (i = 0; i < prog->procs->sz; ++i)
			free(prog->procs->data[i]);
		vec_free(prog->procs);
	}
	free(prog);
}

int			prog_proc_create(prog_t *prog) {
	uint64_t	i;
	proc_t		*new_proc;

	for (i = 0; i < prog->numprocs; ++i) {
		if (!(new_proc = malloc(sizeof(proc_t)))
			|| vec_push_back(prog->procs, new_proc)) {
			if (new_proc)
				free(new_proc);
			return (-1);
		}
		new_proc->state = prog->autostart
			? S_START : S_STOPPED;
		new_proc->retry = 0;
		new_proc->pid = 0;
		new_proc->restart = 0;
	}
	return (0);
}

void		prog_kill(prog_t *prog) {
	uint64_t	i;
	proc_t		*proc;

	for (i = 0; i < prog->procs->sz; ++i) {
		proc = prog->procs->data[i];
		if (proc->pid) {
			kill(proc->pid, SIGKILL);
			waitpid(proc->pid, &proc->status, 0);
		}
		proc->state = S_STOPPED;
	}
}

int			prog_status(prog_t *prog) {
	char		*state_str[] = {
		"Stopping", "Stopping...", "Stopped",
		"Starting", "Starting...", "Started",
		"Start failed", "Start retry", "Exited",
	};
	uint64_t	i;

	printf("%-10s:\n", prog->name);
	for (i = 0; i < prog->procs->sz; ++i)
		printf("  %ld. %s\n", i,
			state_str[((proc_t *)prog->procs->data[i])->state]);
	return (0);
}

static int	int_cmp(void *n1, void *n2) {
	return (n1 != n2);
}

static int	vec_cmp(vec_t *v1, vec_t *v2, uint64_t n, cmp_fn_t cmp_fn) {
	uint64_t	i;

	if (v1->sz != v2->sz)
		return (1);
	for (i = 0; i < n; ++i)
		if (cmp_fn(v1->data[i], v2->data[i]))
			return (1);
	return (0);
}

int			prog_cmp(prog_t *p1, prog_t *p2) {
	return (!!(strcmp(p1->cmd, p2->cmd)
		|| p1->numprocs != p2->numprocs
		|| p1->autostart != p2->autostart
		|| p1->autorestart != p2->autorestart
		|| vec_cmp(p1->exitcodes, p2->exitcodes, p1->exitcodes->sz, int_cmp)
		|| p1->starttime != p2->starttime
		|| p1->startretries != p2->startretries
		|| p1->stopsignal != p2->stopsignal
		|| p1->stoptime != p2->stoptime
		|| strcmp(p1->std_out, p2->std_out)
		|| strcmp(p1->std_err, p2->std_err)
		|| vec_cmp(p1->env, p2->env, (p1->exitcodes->sz - 1), (cmp_fn_t)strcmp)
		|| strcmp(p1->workingdir, p2->workingdir)
		|| p1->umask != p2->umask));
}

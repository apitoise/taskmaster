/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:00:57 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/26 10:14:11 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/prog.h"

static map_t		g_map = {
	.bools = {	"false", "true" },
	.auto_r = {	"always", "never", "unexpected" },
	.sigs_s = {	"ALRM", "HUP", "INT", "PIPE",
				"TERM",	"USR1", "USR2" },
	.sigs_v = {	SIGALRM, SIGHUP, SIGINT, SIGPIPE,
				SIGTERM, SIGUSR1, SIGUSR2 },
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
	vec_t		*ex_cds, *env;
	prog_t		*new;

	if (!(new = malloc(sizeof(prog_t))))
		return (NULL);
	bzero(new, sizeof(prog_t));
	new->name = name;
	if (dic_get_unw(opts, "cmd", (void **)&new->cmd, name, DT_STR)
		|| dic_get_unw(opts, "numprocs", (void **)&new->numprocs, (void *)1, DT_UNB)
		|| !new->numprocs
		|| dic_get_unw(opts, "autostart", (void **)&str, "false", DT_STR)
		|| (new->autostart = map_str(NULL, g_map.bools, str, 2)) == U_ERROR
		|| dic_get_unw(opts, "autorestart", (void **)&str, "unexpected", DT_STR)
		|| (new->autorestart = map_str(NULL, g_map.auto_r, str, 3)) == U_ERROR
		|| dic_get_unw(opts, "exitcodes", (void **)&ex_cds, NULL, DT_VEC)
		|| dic_get_unw(opts, "starttime", (void **)&new->starttime, (void *)5, DT_UNB)
		|| dic_get_unw(opts, "startretries", (void **)&new->startretries, NULL, DT_UNB)
		|| dic_get_unw(opts, "stopsignal", (void **)&str, "INT", DT_STR)
		|| (new->stopsignal = map_str(g_map.sigs_v, g_map.sigs_s, str, 7)) == U_ERROR
		|| dic_get_unw(opts, "stoptime", (void **)&new->stoptime, (void *)10, DT_UNB)
		|| dic_get_unw(opts, "stdout", (void **)&new->std_out, "./log_out", DT_STR)
		|| dic_get_unw(opts, "stderr", (void **)&new->std_err, "./log_err", DT_STR)
		|| dic_get_unw(opts, "env", (void **)&env, NULL, DT_VEC)
		|| dic_get_unw(opts, "workingdir", (void **)&new->workingdir, "", DT_STR)
		|| dic_get_unw(opts, "umask", (void **)&new->umask, NULL, DT_UNB)
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


void		prog_print(prog_t *prog) {
	uint64_t	i;

	printf("cmd: \"%s\"\n", prog->cmd);
	printf("autostart: \"%s\"\n", g_map.bools[prog->autostart]);
	printf("autorestart: \"%s\"\n", g_map.auto_r[prog->autorestart]);
#ifdef __APPLE__
	printf("umask: %s%llo\n", prog->umask ? "0" : "", prog->umask);
	printf("stoptime: %llu\n", prog->stoptime);
	printf("numprocs: \"%llu\"\n", prog->numprocs);
	printf("starttime: %llu\n", prog->starttime);
	printf("startretries: %llu\n", prog->startretries);
	printf("exitcodes: [ ");
	for (i = 0; i < prog->exitcodes->sz; ++i)
		printf("%llu%s", (uint64_t)prog->exitcodes->data[i],
			i + 1 < prog->exitcodes->sz ? ", " : " ");
	printf("]\n");
#else
	printf("umask: %s%lo\n", prog->umask ? "0" : "", prog->umask);
	printf("stoptime: %lu\n", prog->stoptime);
	printf("numprocs: \"%lu\"\n", prog->numprocs);
	printf("starttime: %lu\n", prog->starttime);
	printf("startretries: %lu\n", prog->startretries);
	printf("exitcodes: [ ");
	for (i = 0; i < prog->exitcodes->sz; ++i)
		printf("%lu%s", (uint64_t)prog->exitcodes->data[i],
			i + 1 < prog->exitcodes->sz ? ", " : " ");
	printf("]\n");
#endif // __APPLE__
	printf("workingdir: \"%s\"\n", prog->workingdir);
	printf("stopsignal: ");
	for (i = 0; i < 7; ++i) {
		if ((uint64_t)prog->stopsignal == g_map.sigs_v[i]) {
			printf("%s\n", g_map.sigs_s[i]);
			break;
		}
	}
	printf("stdout: \"%s\"\n", prog->std_out);
	printf("stderr: \"%s\"\n", prog->std_err);
	printf("env: {%s", prog->env->sz ? "\n" : " ");
	for (i = 0; i < prog->env->sz; ++i)
		printf("  \"%s\"\n", (char *)prog->env->data[i]);
	printf("}\n");
}

static int	io_redirect(int old_fd, char *new_path) {
	int	new_fd;

	if (*new_path
		&& (new_fd = open(new_path, O_WRONLY | O_CREAT | O_APPEND, 0666)) != -1) {
		if (dup2(new_fd, old_fd) == -1) {
			close(new_fd);
			return (-1);
		}
	}
	return (0);
}

static int	str_split(char *str, char **res, uint64_t n_res) {
	uint64_t	i, in = 0, j = 0, len = strlen(str);

	for (i = 0; i < len; ++i) {
		if (isspace(str[i])) {
			str[i] = '\0';
			in = 0;
		}
		else if (!in) {
			if (j == n_res)
				return (-1);
			res[j++] = &str[i];
			in = 1;
		}
	}
	str[i] = '\0';
	res[j] = NULL;
	return (0);
}

int				prog_update(prog_t *prog) {
	uint64_t		i;
	char			path[STD_MAX];
	proc_t			*proc;
	int				ret, status;

	for (i = 0; i < prog->procs->sz; ++i) {
		proc = prog->procs->data[i];
		if (proc->pid) {
			sprintf(path, "/proc/%d/status", proc->pid);
			if (!access(path, F_OK)) { // if exists
				if ((ret = waitpid(proc->pid, &status, WNOHANG)) < 0)
					return (-1);
				if (access(path, F_OK)) { // if not exists
					proc->pid = 0;
					proc->status = status;
				}
			}
		}
	}
	return (0);
}

void		prog_clean_procs(prog_t *prog) {
	uint64_t	i;

	for (i = 0; i < prog->procs->sz; ++i)
		free(prog->procs->data[i]);
	prog->procs->sz = 0;
}

int			prog_run(prog_t *prog) {
	uint64_t	i;
	char		*args[STD_MAX];
	static char	buff[STD_MAX * 2];
	proc_t		*new_proc;

	for (i = 0; i < prog->numprocs; ++i) {
		if (!(new_proc = malloc(sizeof(proc_t))))
			return (-1);
		if (time(&new_proc->timestamp) == -1
			|| vec_push_back(prog->procs, new_proc)) {
			free(new_proc);
			return (-1);
		}
		if ((new_proc->pid = fork()) == -1) {
			vec_pop_back(prog->procs, NULL);
			free(new_proc);
			return (-1);
		} else if (!new_proc->pid) {
			if (io_redirect(STDOUT_FILENO, prog->std_out)
				|| io_redirect(STDERR_FILENO, prog->std_err)
				|| str_split(prog->cmd, args, STD_MAX)
				|| execvpe(prog->cmd, args, (char **)prog->env->data)) {
				sprintf(buff, "Can't run %s", prog->name);
				fclose(stdout);
				fclose(stdin);
				clean_exit_child(buff, 1); // exit code number ?
			}
		}
	}
	return (0);
}

int			prog_kill(prog_t *prog, int signal) {
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

int			prog_status(prog_t *prog) {
	uint64_t	i, started = 0,
				succeed = 0,
				failed = 0;
	time_t		timer;
	proc_t		*proc;

	printf("%-10s: ", prog->name);
	for (i = 0; i < prog->procs->sz; ++i) {
		proc = (proc_t *)prog->procs->data[i];
		if (!proc->pid) {
			++succeed;
			continue ;
		}
		if (time(&timer) == -1)
			return (-1);
		if ((uint64_t)(timer)
			>= prog->starttime + proc->timestamp)
			++started;
	}
	if (!prog->procs->sz)
		printf("Stopped\n");
	else if (prog->numprocs == succeed + failed)
		printf("Terminated (%lu succeed, %lu failed)\n",
			succeed, failed);
	else
		printf("Running %lu/%lu (%lu succeed, %lu failed)\n",
			started, prog->numprocs, succeed, failed);
	return (0);
}

static int	int_cmp(void *n1, void *n2) {
	return (n1 != n2);
}

static int	vec_cmp(vec_t *v1, vec_t *v2, cmp_fn_t cmp_fn) {
	uint64_t	i;

	if (v1->sz != v2->sz)
		return (1);
	for (i = 0; i + 1 < v1->sz; ++i)
		if (cmp_fn(v1->data[i], v2->data[i]))
			return (1);
	return (0);
}

int			prog_cmp(prog_t *p1, prog_t *p2) {
	if (strcmp(p1->cmd, p2->cmd)
		|| p1->numprocs != p2->numprocs
		|| p1->autostart != p2->autostart
		|| p1->autorestart != p2->autorestart
		|| vec_cmp(p1->exitcodes, p2->exitcodes, int_cmp)
		|| p1->starttime != p2->starttime
		|| p1->startretries != p2->startretries
		|| p1->stopsignal != p2->stopsignal
		|| p1->stoptime != p2->stoptime
		|| strcmp(p1->std_out, p2->std_out)
		|| strcmp(p1->std_err, p2->std_err)
		|| vec_cmp(p1->env, p2->env, (cmp_fn_t)strcmp)
		|| strcmp(p1->workingdir, p2->workingdir)
		|| p1->umask != p2->umask)
		return (1);
	return (0);
}

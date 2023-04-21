/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:00:57 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/21 19:22:06 by fcadet           ###   ########.fr       */
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

int			prog_init(prog_t *prog, char *name, dict_t *opts) {
	char 		*str;
	vec_t		*ex_cds, *env;

	bzero(prog, sizeof(prog_t));
	prog->name = name;
	if (dic_get_unw(opts, "cmd", (void **)&prog->cmd, prog->name, DT_STR)
		|| dic_get_unw(opts, "numprocs", (void **)&prog->numprocs, (void *)1, DT_UNB)
		|| !prog->numprocs
		|| dic_get_unw(opts, "autostart", (void **)&str, "false", DT_STR)
		|| (prog->autostart = map_str(NULL, g_map.bools, str, 2)) == U_ERROR
		|| dic_get_unw(opts, "autorestart", (void **)&str, "unexpected", DT_STR)
		|| (prog->autorestart = map_str(NULL, g_map.auto_r, str, 3)) == U_ERROR
		|| dic_get_unw(opts, "exitcodes", (void **)&ex_cds, NULL, DT_VEC)
		|| dic_get_unw(opts, "starttime", (void **)&prog->starttime, (void *)5, DT_UNB)
		|| dic_get_unw(opts, "startretries", (void **)&prog->startretries, NULL, DT_UNB)
		|| dic_get_unw(opts, "stopsignal", (void **)&str, "INT", DT_STR)
		|| (prog->stopsignal = map_str(g_map.sigs_v, g_map.sigs_s, str, 7)) == U_ERROR
		|| dic_get_unw(opts, "stoptime", (void **)&prog->stoptime, (void *)10, DT_UNB)
		|| dic_get_unw(opts, "stdout", (void **)&prog->std_out, "", DT_STR)
		|| dic_get_unw(opts, "stderr", (void **)&prog->std_err, "", DT_STR)
		|| dic_get_unw(opts, "env", (void **)&env, NULL, DT_VEC)
		|| dic_get_unw(opts, "workingdir", (void **)&prog->workingdir, "", DT_STR)
		|| dic_get_unw(opts, "umask", (void **)&prog->umask, NULL, DT_UNB))
		return (-1);
	if (!(prog->exitcodes = vec_unw(ex_cds, DT_UNB))
		|| !(prog->env = vec_unw(env, DT_STR))
		|| !(prog->procs = vec_new(prog->numprocs))
		|| (!prog->exitcodes->sz && vec_push_back(prog->exitcodes, 0))
		|| (vec_push_back(prog->env, NULL))) {
		prog_free(prog);
		return (-1);
	}
	return (0);
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
}

void		prog_print(prog_t *prog) {
	uint64_t	i;

	printf("name: \"%s\"\n", prog->name);
	printf("cmd: \"%s\"\n", prog->cmd);
	printf("numprocs: \"%lu\"\n", prog->numprocs);
	printf("autostart: \"%s\"\n", g_map.bools[prog->autostart]);
	printf("autorestart: \"%s\"\n", g_map.auto_r[prog->autorestart]);
	printf("exitcodes: [ ");
	for (i = 0; i < prog->exitcodes->sz; ++i)
		printf("%lu%s", (uint64_t)prog->exitcodes->data[i],
			i + 1 < prog->exitcodes->sz ? ", " : " ");
	printf("]\n");
	printf("starttime: %lu\n", prog->starttime);
	printf("startretries: %lu\n", prog->startretries);
	printf("stopsignal: ");
	for (i = 0; i < 7; ++i) {
		if ((uint64_t)prog->stopsignal == g_map.sigs_v[i]) {
			printf("%s\n", g_map.sigs_s[i]);
			break;
		}
	}
	printf("stoptime: %lu\n", prog->stoptime);
	printf("stdout: \"%s\"\n", prog->std_out);
	printf("stderr: \"%s\"\n", prog->std_err);
	printf("env: {%s", prog->env->sz ? "\n" : " ");
	for (i = 0; i < prog->env->sz; ++i)
		printf("  \"%s\"\n", (char *)prog->env->data[i]);
	printf("}\n");
	printf("workingdir: \"%s\"\n", prog->workingdir);
	printf("umask: %s%lo\n", prog->umask ? "0" : "", prog->umask);
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

int			prog_run(prog_t *prog) {
	uint64_t	i;
	char		*args[STD_MAX];
	proc_t		*new_proc;

	for (i = 0; i < prog->numprocs; ++i) {
		if (!(new_proc = malloc(sizeof(proc_t)))
			|| vec_push_back(prog->procs, new_proc)
			|| time(&new_proc->timestamp) == -1
			|| (new_proc->pid = fork()) == -1)
			return (-1);
		else if (!new_proc->pid) {
			if (io_redirect(STDOUT_FILENO, prog->std_out)
				|| io_redirect(STDERR_FILENO, prog->std_err)
				|| str_split(prog->cmd, args, STD_MAX)
				|| execve(prog->cmd, args, (char **)prog->env->data))
				exit(1);
		}
	}
	return (0);
}

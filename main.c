/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/21 17:07:53 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "action.h"

typedef struct		map_s {
	char			*bools[2];
	char			*auto_r[3];
	char			*sigs_s[11];
	uint64_t		sigs_v[11];
}					map_t;

/*
Running x/y (x Failed, x Stopped)
Stopped
Failed
*/

static map_t		g_map = {
	.bools = { "false", "true" },
	.auto_r = { "always", "never", "unexpected" },
	.sigs_s = { "ALRM", "HUP", "INT", "PIPE",
		"TERM", "USR1", "USR2" },
	.sigs_v = { SIGALRM, SIGHUP, SIGINT, SIGPIPE,
		SIGTERM, SIGUSR1, SIGUSR2 },
};

typedef enum		restart_pol_e {
	RP_ALWAYS,
	RP_NEVER,
	RP_UNEXP,
}					restart_pol_t;

typedef struct		proc_s {
	char			*name;
	char			*cmd;
	uint64_t		numprocs;
	uint64_t		autostart;
	uint64_t		autorestart;
	vec_t			*exitcodes;
	uint64_t		starttime;
	uint64_t		startretries;
	uint64_t		stopsignal;
	uint64_t		stoptime;
	char			*std_out;
	char			*std_err;
	vec_t			*env;
	char			*workingdir;
	uint64_t		umask;
	pid_t			pid;
	time_t			timestamp;
}					proc_t;

typedef struct		proc_lst_s {
	proc_t			data[STD_MAX];
	uint64_t		sz;
}					proc_lst_t;

typedef struct		clean_up_s {
	proc_lst_t		*proc_lst;
	prompt_t		*prompt;
	conf_t			*config;
}					clean_up_t;

static clean_up_t	g_clean_up = { 0 };

int			node_unwrap(node_t *node, void **dst, data_type_t type) {
	*dst = node->data;
	return (node->type != type ? -1 : 0);
}

int			dic_get_unwrap(dict_t *dic, char *key, void **dst, void *def, data_type_t type) {
	node_t	*node;

	if (!dict_get(dic, key, (void **)&node))
		return (node_unwrap(node, dst, type));
	*dst = def;
	return (0);
}

vec_t		*vec_unwrap(vec_t *vec, data_type_t type) {
	uint64_t	i;
	vec_t		*new;
	void		*val;

	if (!(new = vec_new(1)))
		return (NULL);
	if (!vec)
		return (new);
	for (i = 0; i < vec->sz; ++i) {
		if (node_unwrap(vec->data[i], &val, type)) {
			vec_free(new);
			return (NULL);
		}
		vec_push_back(new, val);	
	}
	return (new);
}

void		proc_print(proc_t *proc) {
	uint64_t	i;

	printf("name: \"%s\"\n", proc->name);
	printf("cmd: \"%s\"\n", proc->cmd);
	printf("numprocs: \"%lu\"\n", proc->numprocs);
	printf("autostart: \"%s\"\n", g_map.bools[proc->autostart]);
	printf("autorestart: \"%s\"\n", g_map.auto_r[proc->autorestart]);
	printf("exit_code: [ ");
	for (i = 0; i < proc->exitcodes->sz; ++i)
		printf("%lu%s", (uint64_t)proc->exitcodes->data[i],
			i + 1 < proc->exitcodes->sz ? ", " : " ");
	printf("]\n");
	printf("starttime: %lu\n", proc->starttime);
	printf("startretries: %lu\n", proc->startretries);
	printf("stopsignal: ");
	for (i = 0; i < 7; ++i) {
		if ((uint64_t)proc->stopsignal == g_map.sigs_v[i]) {
			printf("%s\n", g_map.sigs_s[i]);
			break;
		}
	}
	printf("stoptime: %lu\n", proc->stoptime);
	printf("stdout: \"%s\"\n", proc->std_out);
	printf("stderr: \"%s\"\n", proc->std_err);
	printf("env: {%s", proc->env->sz ? "\n" : " ");
	for (i = 0; i < proc->env->sz; ++i)
		printf("  \"%s\"\n", (char *)proc->env->data[i]);
	printf("}\n");
	printf("workingdir: \"%s\"\n", proc->workingdir);
	printf("umask: %s%lo\n", proc->umask ? "0" : "", proc->umask);
}

uint64_t	proc_map_str(uint64_t *out, char **in, char *str, uint64_t sz) {
	uint64_t		i;

	for (i = 0; i < sz; ++i)
		if (!strcmp(in[i], str))
			return (out ? out[i] : i);
	return (U_ERROR);
}

int			proc_init(proc_t *proc, char *name, dict_t *opts) {
	char 		*str;
	vec_t		*ex_cds, *env;

	proc->name = name;
	if (dic_get_unwrap(opts, "cmd", (void **)&proc->cmd, proc->name, DT_STR) < 0
		|| dic_get_unwrap(opts, "numprocs", (void **)&proc->numprocs, (void *)1, DT_UNB) < 0
		|| dic_get_unwrap(opts, "autostart", (void **)&str, "false", DT_STR) < 0
		|| (proc->autostart = proc_map_str(NULL, g_map.bools, str, 2)) == U_ERROR
		|| dic_get_unwrap(opts, "autorestart", (void **)&str, "unexpected", DT_STR) < 0
		|| (proc->autorestart = proc_map_str(NULL, g_map.auto_r, str, 3)) == U_ERROR
		|| dic_get_unwrap(opts, "exitcodes", (void **)&ex_cds, NULL, DT_VEC) < 0
		|| dic_get_unwrap(opts, "starttime", (void **)&proc->starttime, (void *)5, DT_UNB) < 0
		|| dic_get_unwrap(opts, "startretries", (void **)&proc->startretries, NULL, DT_UNB) < 0
		|| dic_get_unwrap(opts, "stopsignal", (void **)&str, "INT", DT_STR) < 0
		|| (proc->stopsignal = proc_map_str(g_map.sigs_v, g_map.sigs_s, str, 7)) == U_ERROR
		|| dic_get_unwrap(opts, "stoptime", (void **)&proc->stoptime, (void *)10, DT_UNB) < 0
		|| dic_get_unwrap(opts, "stdout", (void **)&proc->std_out, "", DT_STR) < 0
		|| dic_get_unwrap(opts, "stderr", (void **)&proc->std_err, "", DT_STR) < 0
		|| dic_get_unwrap(opts, "env", (void **)&env, NULL, DT_VEC) < 0
		|| dic_get_unwrap(opts, "workingdir", (void **)&proc->workingdir, "", DT_STR) < 0
		|| dic_get_unwrap(opts, "umask", (void **)&proc->umask, NULL, DT_UNB)
		|| !(proc->exitcodes = vec_unwrap(ex_cds, DT_UNB)))
		return (-1);
	if (!proc->exitcodes->sz)
		vec_push_back(proc->exitcodes, 0);
	if (!(proc->env = vec_unwrap(env, DT_STR))) {
		vec_free(proc->exitcodes);
		return (-1);
	}
	vec_push_back(proc->env, NULL);
	return (0);
}

void		proc_free(proc_t *proc) {
	vec_free(proc->exitcodes);
	vec_free(proc->env);
}

int			proc_lst_init(proc_lst_t *proc_lst, conf_t *conf) {
	dict_t		*root, *opts;
	uint64_t	i;

	if (node_unwrap(conf->root, (void **)&root, DT_DIC)
		|| root->keys->sz > STD_MAX)
		return (-1);
	proc_lst->sz = 0;
	for (i = 0; i < root->keys->sz; ++i, ++proc_lst->sz) {
		if (node_unwrap(root->values->data[i], (void **)&opts, DT_DIC)
			|| proc_init(&proc_lst->data[i], (char *)root->keys->data[i], opts))
			return (-1);
	}
	return (0);
}

void		proc_lst_free(proc_lst_t *proc_lst) {
	uint64_t		i;

	for (i = 0; i < proc_lst->sz; ++i)
		proc_free(&proc_lst->data[i]);
}

void	clean_exit(char *error, int ret) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (g_clean_up.proc_lst)
		proc_lst_free(g_clean_up.proc_lst);
	if (g_clean_up.prompt)
		prompt_fini(g_clean_up.prompt);
	exit(ret);
}

int			str_split(char *str, char **res, uint64_t n_res) {
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

static int	proc_redirect(int old_fd, char *new_path) {
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

int			proc_run(proc_t *proc) {
	pid_t	pid;
	char	*args[STD_MAX];

	if ((pid = fork()) == -1)
		return (-1);
	else if (pid) {
		proc->pid = pid;
		if (time(&proc->timestamp) == -1)
			return (-1);
	} else if (!pid) {
		if (proc_redirect(STDOUT_FILENO, proc->std_out)
			|| proc_redirect(STDERR_FILENO, proc->std_err)
			|| str_split(proc->cmd, args, STD_MAX)
			|| execve(proc->cmd, args, (char **)proc->env->data))
			exit(1);
	}
	return (0);
}

int			proc_lst_run(proc_lst_t *proc_lst) {
	uint64_t	i, procs_nb;
	int			ret = 0;

	for (i = 0; i < proc_lst->sz; ++i) {
		if (proc_lst->data[i].autostart) {
			for (procs_nb = 0; procs_nb < proc_lst->data[i].numprocs; ++procs_nb) {
				if (proc_run(&proc_lst->data[i]) == -1) {
					ret = -1;
					fprintf(stderr, "Error: %s %lu/%lu can't be run.\n"
						, proc_lst->data[i].name, procs_nb, proc_lst->data[i].numprocs);
				}
			}
		}
	}
	return (ret);
}

//TODO:
//autocompletion in prompt
//liberation malloc exit_error

int			main(int ac, char **av) {
	prompt_t		prompt;
	cmd_t			cmd;
	proc_lst_t		proc_lst = { 0 };
	action_t		action;
	conf_t			conf;

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	if (config_init(&conf, av[1]))
		clean_exit("Can't load config file", 2);
	g_clean_up.config = &conf;
	if (proc_lst_init(&proc_lst, &conf))
		clean_exit("Can't init program's list", 3);
	g_clean_up.proc_lst = &proc_lst;
	if (proc_lst_run(&proc_lst) == -1)
		clean_exit("Fork failed", 4);
	if (prompt_init(&prompt, "> "))
		clean_exit("Can init prompt", 5);
	g_clean_up.prompt = &prompt;
	while (42) {
		if (prompt_query(&prompt, &cmd))
			clean_exit("Can't access terminal", 6);
		action.sz = cmd_split(&cmd, action.cmds, 2);
		if (action_call(&action) == AR_STOP)
			break;
		printf("\n");
	}
	clean_exit(NULL, 0);
}

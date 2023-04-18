/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/18 19:43:15 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_prompt/hdrs/prompt.h"
#include "shell_jobs/action.h"
#include "utils/hdrs/utils.h"
#include "config/config.h"
#include <signal.h>
#include <limits.h>

#define	PROC_MAX	128
#define MAP_ERR		((uint64_t)-1)

typedef enum		restart_pol_e {
	RP_ALWAYS,
	RP_NEVER,
	RP_UNEXP,
}					restart_pol_t;

typedef struct		proc_s {
	char			*name;
	char			*cmd;
	uint64_t		autostart;
	uint64_t		autorestart;
	vec_t			*exitcodes;
	uint64_t		starttime;
	uint64_t		startretries;
	uint64_t		stopsignal;
	uint64_t		stoptime;
	char			*std_out;
	char			*std_err;
	dict_t			*env;
	char			*workingdir;
	uint64_t		umask;
}					proc_t;

typedef struct		proc_lst_s {
	proc_t			data[PROC_MAX];
	uint64_t		sz;
}					proc_lst_t;

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

dict_t		*dic_unwrap(dict_t *dic, data_type_t type) {
	uint64_t	i;
	dict_t		*new;
	void		*val;

	if (!(new = dict_new(1)))
		return (NULL);
	if (!dic)
		return (new);
	for (i = 0; i < dic->keys->sz; ++i) {
		if (node_unwrap(dic->values->data[i], &val, type)) {
			dict_free(new);
			return (NULL);
		}
		dict_set(new, dic->keys->data[i], val);	
	}
	return (new);
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
	char		*bools[] = { "false", "true" };
	char		*auto_r[] = { "always", "never", "unexpected" };
	char		*sigs_s[] = { "INT", "QUIT", "TERM" };
	uint64_t	sigs_v[] = { SIGINT, SIGQUIT, SIGTERM };
	uint64_t	i;

	printf("name: \"%s\"\n", proc->name);
	printf("cmd: \"%s\"\n", proc->cmd);
	printf("autostart: \"%s\"\n", bools[proc->autostart]);
	printf("autorestart: \"%s\"\n", auto_r[proc->autorestart]);
	printf("exit_code: [ ");
	for (i = 0; i < proc->exitcodes->sz; ++i)
		printf("%lu%s", (uint64_t)proc->exitcodes->data[i],
			i + 1 < proc->exitcodes->sz ? ", " : " ");
	printf("]\n");
	printf("starttime: %lu\n", proc->starttime);
	printf("startretries: %lu\n", proc->startretries);
	printf("stopsignal: ");
	for (i = 0; i < 3; ++i) {
		if ((uint64_t)proc->stopsignal == sigs_v[i]) {
			printf("%s\n", sigs_s[i]);
			break;
		}
	}
	printf("stoptime: %lu\n", proc->stoptime);
	printf("stdout: \"%s\"\n", proc->std_out);
	printf("stderr: \"%s\"\n", proc->std_err);
	printf("env: {%s", proc->env->keys->sz ? "\n" : " ");
	for (i = 0; i < proc->env->keys->sz; ++i)
		printf("    %s=%s,\n", (char *)proc->env->keys->data[i],
			(char *)proc->env->values->data[i]);
	printf("}\n");
	printf("workingdir: \"%s\"\n", proc->workingdir);
	printf("umask: 0%lo\n", proc->umask);
}

uint64_t	proc_map_str(uint64_t *out, char **in, char *str, uint64_t sz) {
	uint64_t		i;

	for (i = 0; i < sz; ++i)
		if (!strcmp(in[i], str))
			return (out ? out[i] : i);
	return (MAP_ERR);
}

int			proc_init(proc_t *proc, char *name, dict_t *opts) {
	char		*bools[] = { "false", "true" };
	char		*auto_r[] = { "always", "never", "unexpected" };
	char		*sigs_in[] = { "INT", "QUIT", "TERM" };
	uint64_t	sigs_out[] = { SIGINT, SIGQUIT, SIGTERM };
	char 		*str;
	vec_t		*vec;
	dict_t		*dic;

	proc->name = name;
	if (dic_get_unwrap(opts, "cmd", (void **)&proc->cmd, proc->name, DT_STR) < 0
		|| dic_get_unwrap(opts, "autostart", (void **)&str, "false", DT_STR) < 0
		|| (proc->autostart = proc_map_str(NULL, bools, str, 2)) == MAP_ERR
		|| dic_get_unwrap(opts, "autorestart", (void **)&str, "unexpected", DT_STR) < 0
		|| (proc->autorestart = proc_map_str(NULL, auto_r, str, 3)) == MAP_ERR
		|| dic_get_unwrap(opts, "exitcodes", (void **)&vec, NULL, DT_VEC) < 0
		|| dic_get_unwrap(opts, "starttime", (void **)&proc->starttime, (void *)5, DT_UNB) < 0
		|| dic_get_unwrap(opts, "startretries", (void **)&proc->startretries, NULL, DT_UNB) < 0
		|| dic_get_unwrap(opts, "stopsignal", (void **)&str, "INT", DT_STR) < 0
		|| (proc->stopsignal = proc_map_str(sigs_out, sigs_in, str, 3)) == MAP_ERR
		|| dic_get_unwrap(opts, "stoptime", (void **)&proc->stoptime, (void *)10, DT_UNB) < 0
		|| dic_get_unwrap(opts, "stdout", (void **)&proc->std_out, "", DT_STR) < 0
		|| dic_get_unwrap(opts, "stderr", (void **)&proc->std_err, "", DT_STR) < 0
		|| dic_get_unwrap(opts, "env", (void **)&dic, NULL, DT_DIC) < 0
		|| dic_get_unwrap(opts, "workingdir", (void **)&proc->workingdir, "", DT_STR) < 0
		|| dic_get_unwrap(opts, "umask", (void **)&proc->umask, NULL, DT_UNB))
		return (-1);
	if (!(proc->exitcodes = vec_unwrap(vec, DT_UNB)))
		return (-1);
	if (!proc->exitcodes->sz)
		vec_push_back(proc->exitcodes, 0);
	if (!(proc->env = dic_unwrap(dic, DT_STR))) {
		vec_free(proc->exitcodes);
		return (-1);
	}
	return (0);
}

void		proc_free(proc_t *proc) {
	vec_free(proc->exitcodes);
	dict_free(proc->env);
}

int			proc_lst_init(proc_lst_t *proc_lst, conf_t *conf) {
	dict_t		*root, *opts;
	uint64_t	i;

	if (node_unwrap(conf->root, (void **)&root, DT_DIC)
		|| root->keys->sz > PROC_MAX)
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

//static void	cmd_proc(proc_t *proc, char **env) {
//	pid_t	pid;
//	for (int i = 0; i < 3; ++i) {
//		if ((pid = fork()) == -1)
//			fprintf(stderr, "fork fail");
//		else if (pid)
//			proc->proc_pid[i] = pid;
//		else if (!pid) {
//			if (execve(proc->proc_cmd[i], proc->proc_args[i], env) == -1)
//				fprintf(stderr, "execve fail");
//		}
//	}
//}

//TODO:
//need to parse octal numbers
//autocompletion in prompt

int			main(int ac, char **av, char **env) {
	prompt_t		prompt;
	cmd_t			cmd;
	proc_lst_t		proc_lst;
	action_t		action;
	conf_t			conf;

	(void)env;
	if (ac != 2)
		exit_error("Wrong number of arguments: ./taskmaster [conf file]");
	else if (config_init(&conf, av[1]))
		exit_error("Can't load config file");
	if (proc_lst_init(&proc_lst, &conf))
		printf("bite\n");

	proc_print(&proc_lst.data[0]);
	printf("\n");
	proc_print(&proc_lst.data[1]);
	printf("\n");
	proc_print(&proc_lst.data[2]);
	printf("\n");

	prompt_init(&prompt, "> ");
	while (42) {
		if (prompt_query(&prompt, &cmd)) {
			fprintf(stderr, "Error: Can't get command\n");
			proc_lst_free(&proc_lst);
			exit(4);
		}
		action.sz = cmd_split(&cmd, action.cmd, 2);
		parse_cmd(&action);
		printf("\n");
	}
	proc_lst_free(&proc_lst);
	return (0);
}

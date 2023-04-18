/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/18 15:53:53 by fcadet           ###   ########.fr       */
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
	char			*run;
	uint64_t		at_start;
	uint64_t		rest_pol;
	vec_t			*exit_codes;
	uint64_t		start_time;
	uint64_t		retry_nb;
	uint64_t		exit_sig;
	uint64_t		kill_delay;
	char			*f_out;
	char			*f_err;
	dict_t			*env;
	char			cwd[PATH_MAX];
	uint64_t		umask;
}					proc_t;

typedef struct		proc_lst_s {
	proc_t			proc[PROC_MAX];
	uint64_t		sz;
}					proc_lst_t;

int			dic_unwrap(dict_t *dic, char *key, void **dst, void *def, data_type_t type) {
	node_t	*node;

	if (dict_get(dic, key, (void **)&node) < 0)
		node = NULL;
	if (node && node->type != type)
		return (-1);
	*dst = node ? node->data : def;
	return (0);
}

int			vec_unwrap(vec_t *vec, uint64_t idx, void **dst, void *def, data_type_t type) {
	node_t	*node;

	printf("OK\n");
	vec_print(vec);
	printf("OK\n");
	if (vec_get(vec, idx, (void **)&node) < 0)
		node = NULL;
	if (node && node->type != type)
		return (-1);
	*dst = node ? node->data : def;
	return (0);
}

void		proc_print(proc_t *proc) {
	char		*bools[] = { "false", "true" };
	char		*auto_r[] = { "always", "never", "unexpected" };
	uint64_t	exit_code;
	uint64_t	i;
	char		*str;

	printf("name: %s\n", proc->name);
	printf("run: %s\n", proc->run);
	printf("at_start: %s\n", bools[proc->at_start]);
	printf("rest_pol: %s\n", auto_r[proc->rest_pol]);
	printf("exit_code:\n");
	for (i = 0; i < proc->exit_codes->sz; ++i) {
		vec_unwrap(proc->exit_codes, i, (void **)&exit_code, NULL, DT_UNB);
		printf("    %lu\n", exit_code);
	}
	printf("start_time: %lu\n", proc->start_time);
	printf("retry_nb: %lu\n", proc->retry_nb);
	printf("exit_sig: %lu\n", proc->exit_sig);
	printf("kill_delay: %lu\n", proc->kill_delay);
	printf("f_out: %s\n", proc->f_out);
	printf("f_err: %s\n", proc->f_err);
	printf("env:\n");
		for (i = 0; i < proc->env->keys->sz; ++i) {
		dic_unwrap(proc->env, proc->env->keys->data[i], (void **)&str, NULL, DT_STR);
		printf("    %s\n", str);
	}
	printf("cwd: %s\n", proc->cwd);
	printf("umask: %lu\n", proc->umask);
}

uint64_t	proc_map_str(uint64_t *out, char **in, char *str, uint64_t sz) {
	uint64_t		i;

	for (i = 0; i < sz; ++i)
		if (!strcmp(in[i], str))
			return (out ? out[i] : i);
	return (MAP_ERR);
}

int			procs_init(proc_lst_t *proc_lst, conf_t *conf) {
	char		*bools[] = { "false", "true" };
	char		*auto_r[] = { "always", "never", "unexpected" };
	char		*sigs_in[] = { "INT", "QUIT", "TERM" };
	uint64_t	sigs_out[] = { SIGINT, SIGQUIT, SIGTERM };
	dict_t		*dic;
	uint64_t	i;
	char		*str;

	if (conf->root->type != DT_DIC)
		return (-1);
	dic = conf->root->data;
	if (dic->keys->sz > PROC_MAX)
		return (-1);
	proc_lst->sz = 0;
	for (i = 0; i < dic->keys->sz; ++i, ++proc_lst->sz) {
		proc_lst->proc[i].name = (char *)dic->keys->data[i];
		if (dic_unwrap(dic, "cmd", (void **)&proc_lst->proc[i].run,
			proc_lst->proc[i].name, DT_STR) < 0
			|| dic_unwrap(dic, "autostart",
				(void **)&str, "true", DT_STR) < 0
			|| (proc_lst->proc[i].at_start
				= proc_map_str(NULL, bools, str, 2)) == MAP_ERR
			|| dic_unwrap(dic, "autorestart",
				(void **)&str, "unexpected", DT_STR) < 0
			|| (proc_lst->proc[i].rest_pol
				= proc_map_str(NULL, auto_r, str, 3)) == MAP_ERR
			|| dic_unwrap(dic, "exitcodes",
				(void **)&proc_lst->proc[i].exit_codes, NULL, DT_VEC) < 0
			|| dic_unwrap(dic, "starttime",
				(void **)&proc_lst->proc[i].start_time, (void *)5, DT_UNB) < 0
			|| dic_unwrap(dic, "startretries",
				(void **)&proc_lst->proc[i].retry_nb, NULL, DT_UNB) < 0
			|| dic_unwrap(dic, "stopsignal",
				(void **)&str, "INT", DT_STR) < 0
			|| (proc_lst->proc[i].exit_sig
				= proc_map_str(sigs_out, sigs_in, str, 3)) == MAP_ERR
			|| dic_unwrap(dic, "stoptime",
				(void **)&proc_lst->proc[i].kill_delay, (void *)10, DT_UNB) < 0
			|| dic_unwrap(dic, "stdout",
				(void **)&proc_lst->proc[i].f_out, NULL, DT_STR) < 0
			|| dic_unwrap(dic, "stderr",
				(void **)&proc_lst->proc[i].f_err, NULL, DT_STR) < 0
			|| dic_unwrap(dic, "env",
				(void **)&proc_lst->proc[i].env, NULL, DT_DIC) < 0
			|| dic_unwrap(dic, "workingdir",
				(void **)&proc_lst->proc[i].cwd, NULL, DT_STR) < 0
			|| dic_unwrap(dic, "umask",
				(void **)&proc_lst->proc[i].umask, NULL, DT_UNB))
			return (-1);
	}
	return (0);
}

//static void	init_proc(proc_t *proc, char **env) {
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
	if (procs_init(&proc_lst, &conf))
		printf("bite\n");
	config_print(&conf);
	proc_print(&proc_lst.proc[0]);
	prompt_init(&prompt, "> ");
	while (42) {
		if (prompt_query(&prompt, &cmd)) {
			fprintf(stderr, "Error: Can't get command\n");
			exit(4);
		}
		action.sz = cmd_split(&cmd, action.cmd, 2);
		parse_cmd(&action);
		printf("\n");
	}
	return (0);
}

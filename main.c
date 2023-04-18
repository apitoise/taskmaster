/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/17 21:49:00 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_prompt/hdrs/prompt.h"
#include "shell_jobs/action.h"
#include "utils/hdrs/utils.h"
#include "config/config.h"
#include <signal.h>
#include <limits.h>

#define	PROC_MAX	128

typedef enum		restart_pol_e {
	RP_ALWAYS,
	RP_NEVER,
	RP_UNEXP,
}					restart_pol_t;

typedef struct		proc_s {
	char			*name;
	char			*run;
	uint8_t			at_start;
	restart_pol_t	rest_pol;
	vec_t			*exit_codes;
	uint64_t		start_time;
	uint64_t		retry_nb;
	int				exit_signal;
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

int			proc_filler(dict_t *dic, char *key, void *def, data_type_t type) {
	node_t	*node;

	switch (type) {
		case (DT_STR): 
		break;
		case (DT_VEC):
		break;
		case (DT_DIC):
		break;
		case (DT_UNB):
		break;
	}
}

int			procs_init(proc_lst_t *proc_lst, conf_t *conf) {
	dict_t		*dic;
	uint64_t	i;
	char		*str;

	if (conf->root->type != DT_DIC)
		return (-1);
	dic = conf->root->data;
	if (dic->keys->sz > PROC_MAX)
		return (-1);
	for (i = 0; i < dic->keys->sz; ++i) {
		proc_lst->proc[i].name = (char *)dic->keys[i].data;
		if (dict_get(dic, "cmd", (void *)&proc_lst->proc[i].run) < 0)
			proc_lst->proc[i].run = proc_lst->proc[i].name;
		if (dict_get(dic, "autostart", (void *)&str) < 0 || !strcmp(str, "true"))
			proc_lst->proc[i].at_start = 1;
		else if (!strcmp(str, "false"))
			proc_lst->proc[i].at_start = 0;
		else
			return (-1);
		if (dict_get(dic, "autorestart", (void *)&str) < 0 || !strcmp(str, "unexpected"))
			proc_lst->proc[i].rest_pol = RP_UNEXP;
		else if (!strcmp(str, "always"))
			proc_lst->proc[i].rest_pol = RP_ALWAYS;
		else if (!strcmp(str, "never"))
			proc_lst->proc[i].rest_pol = RP_NEVER;
		else
			return (-1);
		if (dict_get(dic, "exitcodes", (void *)&proc_lst->proc[i].exit_codes) < 0)
			proc_lst->proc[i].exit_codes = NULL;
		if (dict_get(dic, "starttime", (void *)&proc_lst->proc[i].start_time) < 0)
			proc_lst->proc[i].start_time = 5;
		if (dict_get(dic, "startretries", (void *)&proc_lst->proc[i].retry_nb) < 0)
			proc_lst->proc[i].retry_nb = 0;
		if (dict_get(dic, "stopsignal", (void *)&str) < 0 || !strcmp(str, "INT"))
			proc_lst->proc[i].exit_signal = SIGINT;
		else if (!strcmp(str, "TERM"))
			proc_lst->proc[i].exit_signal = SIGTERM;
		else if (!strcmp(str, "QUIT"))
			proc_lst->proc[i].exit_signal = SIGQUIT;
		else
			return (-1);
		if (dict_get(dic, "stoptime", (void *)&proc_lst->proc[i].kill_delay) < 0)
			proc_lst->proc[i].kill_delay = 10;
		if (dict_get(dic, "stdout", (void *)&proc_lst->proc[i].f_out) < 0)
			strcpy(proc_lst->proc[i].f_out, "stdout");
		if (dict_get(dic, "stderr", (void *)&proc_lst->proc[i].f_err) < 0)	
			strcpy(proc_lst->proc[i].f_err, "stderr");
		if (dict_get(dic, "env", (void *)&proc_lst->proc[i].env) < 0)
			proc_lst->proc[i].env = NULL;
		if (dict_get(dic, "workingdir", (void *)&proc_lst->proc[i].cwd) < 0)
			if (getcwd(proc_lst->proc[i].cwd, sizeof(proc_lst->proc[i].cwd)))
				return (-1);
		if (dict_get(dic, "umask", (void *)&proc_lst->proc[i].umask) < 0)
			proc_lst->proc[i].umask = 0;
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
	procs_init(&proc_lst, &conf); // err
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

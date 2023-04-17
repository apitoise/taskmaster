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
	dict_t			*exp_exit;
	uint64_t		start_time;
	uint64_t		retry_nb;
	int				exit_signal;
	uint64_t		kill_delay;
	char			*f_out;
	char			*f_err;
	dict_t			*env;
	char			*cwd;
	uint64_t		umask;
}					proc_t;

typedef struct		proc_lst_s {
	proc_t			proc[PROC_MAX];
	uint64_t		sz;
}					proc_lst_t;

int			procs_init(proc_lst_t *proc_lst, conf_t *conf) {
	dict_t		*dic;
	uint64_t	i;
	char		*str;

	(void)proc_lst;
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

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
#include "shell_jobs/hdrs/builtin.h"
#include "shell_jobs/hdrs/tmp.h"
#include "shell_jobs/hdrs/action.h"
#include "utils/hdrs/utils.h"
#include "config/config.h"

static void	init_proc(proc_t *proc, char **env) {
	pid_t	pid;
	for (int i = 0; i < 3; ++i) {
		if ((pid = fork()) == -1)
			fprintf(stderr, "fork fail");
		else if (pid)
			proc->proc_pid[i] = pid;
		else if (!pid) {
			if (execve(proc->proc_cmd[i], proc->proc_args[i], env) == -1)
				fprintf(stderr, "execve fail");
		}
	}
}

int			main(int ac, char **av, char **env) {
	prompt_t		prompt;
	cmd_t			cmd;
	proc_t			proc;
	action_t		action;
	conf_t			conf;

	if (ac != 2)
		exit_error("Wrong number of arguments: ./taskmaster [conf file]");
	else if (config_init(&conf, av[1]))
		exit_error("Can't load config file");
	init_proc(&proc, env);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/03/28 10:04:28 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_prompt/hdrs/prompt.h"
#include "shell_jobs/hdrs/builtin.h"
#include "shell_jobs/hdrs/tmp.h"
#include "shell_jobs/hdrs/cmd.h"

static void	init_proc(t_proc *proc, char **env) {
	pid_t	pid;

	proc->proc_name[0] = "sleepdebain";
	proc->proc_name[1] = "sleepknot";
	proc->proc_name[2] = "sleep";
	proc->proc_cmd[0] = "/usr/bin/sleep";
	proc->proc_cmd[1] = "/usr/bin/sleep";
	proc->proc_cmd[2] = "/usr/bin/sleep";
	proc->proc_args[0][0] = "sleep";
	proc->proc_args[1][0] = "sleep";
	proc->proc_args[2][0] = "sleep";
	proc->proc_args[0][1] = "100000";
	proc->proc_args[1][1] = "100000";
	proc->proc_args[2][1] = "100000";
	proc->proc_args[0][2] = NULL;
	proc->proc_args[1][2] = NULL;
	proc->proc_args[2][2] = NULL;

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
	char			*res[2];
	t_proc			proc;

	(void)ac;
	(void)av;
	init_proc(&proc, env);
	prompt_init(&prompt, "> ");
	while (42) {
		if (prompt_query(&prompt, &cmd)) {
			fprintf(stderr, "Error: Can't get command\n");
			exit(4);
		}
		cmd_split(&cmd, res, 2);
		parse_cmd(res);
		printf("\n");
	}
	return (0);
}

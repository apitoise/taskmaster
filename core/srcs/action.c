/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:31:29 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 08:20:58 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"

int		action_stop(action_t *action) {
	prog_t		*prog;
	proc_t		*proc;
	uint64_t	i;

	if (action->sz != 2
		|| dict_get(glob.prog_dic, action->cmds[1],
		(void **)&prog))
		return (-1);
	for (i = 0; i < prog->procs->sz; ++i) {
		proc = prog->procs->data[i];
		proc->state = (proc->state != S_STARTED
			&& proc->state != S_START_WAIT)
			? S_STOPPED : S_STOP;
	}
	return (0);
}

int		action_start(action_t *action) {
	prog_t		*prog;
	proc_t		*proc;
	uint64_t	i;
	
	if (action->sz != 2
		|| dict_get(glob.prog_dic, action->cmds[1],
		(void **)&prog))
		return (-1);
	for (i = 0; i < prog->procs->sz; ++i) {
		proc = prog->procs->data[i];
		proc->retry = 0;
		if (proc->state == S_START_WAIT
			|| proc->state == S_STARTED)
			continue;
		else if (proc->state == S_STOP_WAIT) {
			kill(proc->pid, SIGKILL);
			waitpid(proc->pid, &proc->status, WNOHANG);
		}
		proc->state = S_START;
	}
	return (0);
}

int		action_restart(action_t *action) {
	return (action_stop(action)
		|| action_start(action)
		? -1 : 0);
}	

int		action_status(action_t *action) {
	if (action->sz != 1)
		return (-1);
	return (prog_dic_status(glob.prog_dic) ? -1 : 0);
}

int		action_exit(action_t *action) {
	if (action->sz != 1)
		return (-1);
	clean_exit(NULL, 0);
	return (0);
}

//a revoir
/*
int		action_reload(action_t *action) {
	glob_t		new = { 0 };
	
	if (action->sz != 1
		|| !(new.config = config_new(glob.config_path))
		|| !(new.prog_dic = prog_dic_reload(glob.prog_dic, new.config))) {
		if (new.config)
			config_free(new.config);
		if (new.prog_dic)
			prog_dic_free(new.prog_dic);
		return (-1);
	}
	prog_dic_free(glob.prog_dic);
	config_free(glob.config);
	glob.config = new.config;
	glob.prog_dic = new.prog_dic;
	return (prog_dic_run(glob.prog_dic, NULL));
}
*/

int		action_call(action_t *action) {
	char	*c_str[] = {
		"status", "start", "stop",
		"restart", "reload", "exit",
	};
	action_fn_t		c_fn[] = {
		action_status, action_start, action_stop,
		action_restart, action_reload, action_exit,
	};
	uint64_t		i;

	for (i = 0; i < ACT_NB; ++i) {
		if (strcmp(c_str[i], *action->cmds))
			continue;
		return (c_fn[i](action));
	}
	return (-1);
}

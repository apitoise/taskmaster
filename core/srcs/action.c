/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:31:29 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/26 12:59:33 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"

int		action_status(action_t *action) {
	(void)action;
	return (prog_dic_update(glob.prog_dic)
		|| prog_dic_status(glob.prog_dic) ? -1 : 0);
}

int		action_stop(action_t *action) {
	prog_t		*prog;
	
	if (action->sz != 2
		|| dict_get(glob.prog_dic, action->cmds[1],
		(void **)&prog)
		|| prog_kill(prog, SIGKILL) //need to be gentle
		|| prog_update(prog))
		return (-1);
	prog_clean_procs(prog);
	return (0);
}

int		action_start(action_t *action) {
	prog_t		*prog;
	uint64_t	i;
	
	if (action->sz != 2
		|| dict_get(glob.prog_dic, action->cmds[1],
		(void **)&prog))
		return (-1);
	for (i = 0; i < prog->procs->sz; ++i)
		if (((proc_t *)prog->procs->data[i])->pid)
			return (-1);
	prog_clean_procs(prog);
	return (prog_run(prog));
}

int		action_restart(action_t *action) {
	return (action_stop(action)
		|| action_start(action)
		? -1 : 0);
}

int		action_reload(action_t *action) {
	glob_t		new = { .config = 0,
						.prog_dic = 0,
						.prompt = glob.prompt };

	if (action->sz != 1
		|| !(new.config = config_new(glob.config_path))
		|| !(new.prog_dic = prog_dic_reload(glob.prog_dic, glob.config))) {
		if (new.config)
			config_free(new.config);
		if (new.prog_dic)
			prog_dic_free(new.prog_dic);
		return (-1);
	}
	prog_dic_free(glob.prog_dic);
	config_free(glob.config);
	glob = new;
	return (prog_dic_run(glob.prog_dic, NULL));
}

int		action_exit(action_t *action) {
	(void)action;
	clean_exit(NULL, 0);
	return (0);
}

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
//		fprintf(stderr, "%s: command not found", action->cmds[0]);
}

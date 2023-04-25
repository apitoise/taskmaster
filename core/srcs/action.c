/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:31:29 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/25 09:14:25 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"

int		action_status(action_t *action) {
	printf("%s", action->cmds[0]);
	return (0);
}

int		action_start(action_t *action) {
	if (action->sz == 1)
		fprintf(stderr, "start [process_name]");
	return (0);
}

int		action_stop(action_t *action) {
	printf("%s", action->cmds[1]);
	return (0);
}

int		action_restart(action_t *action) {
	printf("%s", action->cmds[1]);
	return (0);
}

int		action_reload(action_t *action) {
	printf("%s", action->cmds[1]);
	return (0);
}

int		action_exit(action_t *action) {
	printf("%s", action->cmds[0]);

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

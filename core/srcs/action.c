/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:31:29 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/22 14:03:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"

void	action_status(void) {
	printf("STATUS");
}

void	action_start(action_t *action) {
	if (action->sz == 1)
		fprintf(stderr, "start [process_name]");
}

void	action_stop(action_t *action) {
	printf("%s", action->cmds[1]);
}

void	action_restart(action_t *action) {
	printf("%s", action->cmds[1]);
}

void	action_reload(action_t *action) {
	printf("%s", action->cmds[1]);
}

act_res_t	action_call(action_t *action) {
	if (!strcmp(action->cmds[0], "status"))
		action_status();
	else if (!strcmp(action->cmds[0], "start"))
		action_start(action);
	else if (!strcmp(action->cmds[0], "stop"))
		action_stop(action);
	else if (!strcmp(action->cmds[0], "restart"))
		action_restart(action);
	else if (!strcmp(action->cmds[0], "reload"))
		action_reload(action);
	else if (!strcmp(action->cmds[0], "exit"))
		return (AR_STOP);
	else {
		fprintf(stderr, "%s: command not found", action->cmds[0]);
		return (AR_ERR);
	}
	return (AR_CONT);
}

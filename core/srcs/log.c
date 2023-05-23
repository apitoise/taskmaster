/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/23 10:32:58 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/includes.h"
#include "../hdrs/prog.h"
#include "../hdrs/glob.h"

static char		*log_time(void) {
	time_t		tm;
	char		*str;
	uint64_t	i;

	time(&tm);
	str = ctime(&tm);
	for (i = 0; str[i] && str[i] != '\n'; ++i);
	str[i] = '\0';
	return (str);

}



void	log_info(prog_t *prog, char *info) {
		fprintf(glob.log_file, "[%s][INFO] - %s: %s\n",
			log_time(), prog ? prog->name : "Taskmaster", info);
}

void	log_state(prog_t *prog, uint64_t proc_i) {
	proc_t	*proc = prog->procs->data[proc_i];
	char 	*state_str[] = {
		"Stopping", "Stopping...", "Stopped",
		"Starting", "Starting...", "Started",
		"Start failed", "Start retry", "Exited" };

	if (proc->state == S_RETRY)
		fprintf(glob.log_file, "[%s][STATE] - %s %lu/%lu: %s %lu/%lu\n",
			log_time(), prog->name, proc_i, prog->procs->sz,
			state_str[proc->state], proc->retry, prog->startretries);
	else
		fprintf(glob.log_file, "[%s][STATE] - %s %lu/%lu: %s\n",
			log_time(), prog->name, proc_i, prog->procs->sz,
			state_str[proc->state]);
}

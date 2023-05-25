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
	struct tm	*time_info;
	time_t		current;
	static char	str[STD_MAX];

	time(&current);
	time_info = localtime(&current);
	strftime(str, STD_MAX, "%Y-%m-%d %H:%M:%S", time_info);
	return (str);

}

void	log_error(prog_t *prog, uint64_t proc_i, char *err) {
	if (prog)
		fprintf(glob.log_file, "[%s][ERROR] - %s (%lu/%lu): %s\n",
			log_time(), prog->name, proc_i + 1, prog->procs->sz, err);
	else
		fprintf(glob.log_file, "[%s][ERROR] - Taskmaster: %s\n", log_time(), err);
	fflush(glob.log_file);
}

void	log_info(prog_t *prog, char *info) {
		fprintf(glob.log_file, "[%s][INFO] - %s: %s\n",
			log_time(), prog ? prog->name : "Taskmaster", info);
	fflush(glob.log_file);
}

void	log_state(prog_t *prog, uint64_t proc_i) {
	proc_t	*proc = prog->procs->data[proc_i];
	char 	*state_str[] = {
		"Stopping", "Stopping...", "Stopped",
		"Starting", "Starting...", "Started",
		"Start failed", "Start retry", "Exited" };

	if (proc->state == S_RETRY) {
		if (proc->retry == prog->startretries)
			return ;
		fprintf(glob.log_file, "[%s][STATE] - %s (%lu/%lu): %s %lu/%lu\n",
			log_time(), prog->name, proc_i + 1, prog->procs->sz,
			state_str[proc->state], proc->retry + 1, prog->startretries);
	} else
		fprintf(glob.log_file, "[%s][STATE] - %s (%lu/%lu): %s %s\n",
			log_time(), prog->name, proc_i + 1, prog->procs->sz,
			state_str[proc->state],
			(proc->state == S_EXITED && proc->bad_code)
			? "(with bad exit code)" : "");
	fflush(glob.log_file);
}

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

void	state_log(prog_t *prog, uint64_t proc_i) {
	char 	*state_str[] = {
		"Stopping", "Stopping...", "Stopped",
		"Starting", "Starting...", "Started",
		"Start failed", "Start retry", "Exited" };

	fprintf(glob.log_file, "[DATE][INFO] - %s %lu/%lu: %s.\n"
		, prog->name, proc_i, prog->procs->sz,
		state_str[((proc_t *)prog->procs->data[proc_i])->state]);
}
/*
int		main(void) {
	printf("[%s]\n", log_time());
	return (0);
}
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/26 10:16:16 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"

/*
Running x/y (x Failed, x Stopped)
Stopped
Failed
*/


// TODO
// TERM RESTORATION
// function error handling
// global accessors

typedef enum		restart_pol_e {
	RP_ALWAYS,
	RP_NEVER,
	RP_UNEXP,
}					restart_pol_t;

int			main(int ac, char **av) {
	cmd_t			cmd;
	action_t		action;

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	if (!(g_clean.config = config_new(av[1])))
		clean_exit("Can't load config file", 2);
	if (!(g_clean.prog_dic = prog_dic_new(g_clean.config)))
		clean_exit("Can't init program's dictionnary", 3);
	if (prog_dic_run(g_clean.prog_dic, NULL) == -1)
		clean_exit("Can't fork current program", 4);
	if (!(g_clean.prompt = prompt_new("> ")))
		clean_exit("Can init prompt", 5);
	while (42) {
		if (prompt_query(g_clean.prompt, &cmd))
			clean_exit("Can't access terminal", 6);
		action.sz = cmd_split(&cmd, action.cmds, 2);
		if (!action.sz)
			continue;
		if (action_call(&action))
			fprintf(stderr, "%s command failed\n", *action.cmds);
//		printf("\n");
	}
	clean_exit(NULL, 0);
}

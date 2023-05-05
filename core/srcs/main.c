/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 12:16:28 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"
#include "../hdrs/monitor.h"

/*
Running x/y (x Failed, x Stopped)
Stopped
Failed
*/

// TODO
// TERM RESTORATION
// function error handling
// global accessors

glob_t		glob = { 0 };

int			main(int ac, char **av) {
	cmd_t			cmd;
	action_t		action;

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	glob.config_path = av[1];
	if (!(glob.config = config_new(glob.config_path)))
		clean_exit("Can't load config file", 2);
	if (!(glob.prog_dic = prog_dic_new(glob.config)))
		clean_exit("Can't init program's dictionnary", 3);
	if (prog_dic_proc_create(glob.prog_dic))
		clean_exit("Can't create processes", 4);
	if (!(glob.prompt = prompt_new("> ", monitor_fn, 100000)))
		clean_exit("Can init prompt", 5);
	while (42) {
		if (prompt_query(glob.prompt, &cmd))
			clean_exit("Can't access terminal", 6);
		action.sz = cmd_split(&cmd, action.cmds, 2);
		if (!action.sz)
			continue;
		if (action_call(&action))
			fprintf(stderr, "%s command failed\n", *action.cmds);
	}
	clean_exit(NULL, 0);
}

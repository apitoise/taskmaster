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

__attribute__((constructor))
static void	create_file(void) {
	if (!access(LAUNCH_FILE, F_OK)) {
		fprintf(stderr, "Error: taskmaster already launched\n");
		exit(1);
	}
	if (open(LAUNCH_FILE, O_CREAT, 0666) < 0) {
		fprintf(stderr, "Error: Can not create launch file.\n");
		exit(1);
	}
}

static void	sighandler(int sig) {
	glob.sig = sig;
}

int			main(int ac, char **av) {
	cmd_t			cmd;
	action_t		action, act_reload = { .sz = 1,
											.cmds = { "reload", NULL }};
	int				i, sig[] = { SIGALRM, SIGHUP, SIGINT, SIGPIPE,
								SIGTERM, SIGUSR1, SIGUSR2, SIGQUIT };		

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	for (i = 0; i < 8; ++i)
		signal(sig[i], sighandler);
	signal(SIGTSTP, SIG_IGN);
	glob.config_path = av[1];
	if (!(glob.config = config_new(glob.config_path)))
		clean_exit("Can't load config file", 2);
	if (!(glob.prog_dic = prog_dic_new(glob.config)))
		clean_exit("Can't init program's dictionnary", 3);
	if (prog_dic_proc_create(glob.prog_dic))
		clean_exit("Can't create processes", 4);
	if (!(glob.prompt = prompt_new("> ", monitor_fn, 100000)))
		clean_exit("Can init prompt", 5);
	while (!glob.sig) {
		if (prompt_query(glob.prompt, &cmd)) {
			switch (glob.sig) {
				case 0:
					clean_exit("Can't access terminal", 6);
					break ;
				case SIGHUP:
					action_call(&act_reload);
					glob.sig = 0;
					printf("\n");
				__attribute__((fallthrough));
				default:
					continue ;
			}
		}
		action.sz = cmd_split(&cmd, action.cmds, 2);
		if (!action.sz)
			continue;
		if (action_call(&action))
			fprintf(stderr, "%s command failed\n", *action.cmds);
	}
	clean_exit(NULL, 0);
}

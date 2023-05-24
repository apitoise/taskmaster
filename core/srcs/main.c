/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/24 10:47:03 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"
#include "../hdrs/monitor.h"

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
	char			error[STD_MAX];
	cmd_t			cmd;
	action_t		action, act_reload = {
		.sz = 1,
		.cmds = { "reload", NULL },
	};
	int				i, sig[] = {
		SIGALRM, SIGHUP, SIGINT, SIGPIPE,
		SIGTERM, SIGUSR1, SIGUSR2, SIGQUIT,
	};		

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	for (i = 0; i < 8; ++i)
		signal(sig[i], sighandler);
	signal(SIGTSTP, SIG_IGN);
	glob.config_path = av[1];
	if (!(glob.log_file = fopen("log.txt", "a")))
		clean_exit("Can't open log.txt", 2);
	if (!(glob.config = config_new(glob.config_path)))
		clean_exit("Can't load config file", 3);
	if (!(glob.prog_dic = prog_dic_new(glob.config)))
		clean_exit("Can't init program's dictionnary", 4);
	if (prog_dic_proc_create(glob.prog_dic))
		clean_exit("Can't create processes", 5);
	if (!(glob.prompt = prompt_new("> ", monitor_fn, 100000)))
		clean_exit("Can init prompt", 6);
	log_info(NULL, "supervisor started");
	while (!glob.sig) {
		if (prompt_query(glob.prompt, &cmd)) {
			switch (glob.sig) {
				case 0:
					clean_exit("Can't access terminal", 7);
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
		action.sz = cmd_split(&cmd, action.cmds, 8);
		if (!action.sz)
			continue;
		if (action_call(&action)) {
			snprintf(error, STD_MAX, "%s: Command failed", *action.cmds);
			log_error(NULL, 0, error);
			fprintf(stderr, "%s command failed\n", *action.cmds);
		}
	}
	clean_exit(NULL, 0);
}

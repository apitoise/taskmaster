/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/22 17:39:05 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/action.h"

/*
Running x/y (x Failed, x Stopped)
Stopped
Failed
*/

typedef enum		restart_pol_e {
	RP_ALWAYS,
	RP_NEVER,
	RP_UNEXP,
}					restart_pol_t;

typedef struct		clean_up_s {
	prog_dic_t		*prog_dic;
	conf_t			*config;
	prompt_t		*prompt;
}					clean_up_t;

static clean_up_t	g_clean_up = { 0 };

void	clean_exit(char *error, int ret) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (g_clean_up.prog_dic)
		prog_dic_free(g_clean_up.prog_dic);
	if (g_clean_up.config)
		config_free(g_clean_up.config);
	if (g_clean_up.prompt)
		prompt_fini(g_clean_up.prompt);
	exit(ret);
}

//TODO:
//autocompletion in prompt
//liberation malloc exit_error

int			main(int ac, char **av) {
	cmd_t			cmd;
	action_t		action;

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	if (!(g_clean_up.config = config_new(av[1])))
		clean_exit("Can't load config file", 2);
	if (!(g_clean_up.prog_dic = prog_dic_new(g_clean_up.config)))
		clean_exit("Can't init program's dictionnary", 3);
	if (prog_dic_run(g_clean_up.prog_dic, NULL) == -1)
		clean_exit("Can't fork current program", 4);
	if (prompt_init(g_clean_up.prompt, "> "))
		clean_exit("Can init prompt", 5);
	while (42) {
		if (prompt_query(g_clean_up.prompt, &cmd))
			clean_exit("Can't access terminal", 6);
		action.sz = cmd_split(&cmd, action.cmds, 2);
		if (action_call(&action) == AR_STOP)
			break;
		printf("\n");
	}
	clean_exit(NULL, 0);
}

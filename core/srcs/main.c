/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/21 19:11:03 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "action.h"

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
	vec_t			*prog_lst;
	prompt_t		*prompt;
	conf_t			*config;
}					clean_up_t;

static clean_up_t	g_clean_up = { 0 };

void	clean_exit(char *error, int ret) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (g_clean_up.prog_lst)
		prog_lst_free(g_clean_up.prog_lst);
	if (g_clean_up.prompt)
		prompt_fini(g_clean_up.prompt);
	exit(ret);
}

//TODO:
//autocompletion in prompt
//liberation malloc exit_error

int			main(int ac, char **av) {
	prompt_t		prompt;
	cmd_t			cmd;
	vec_t		prog_lst = { 0 };
	action_t		action;
	conf_t			conf;

	if (ac != 2)
		clean_exit("Wrong number of arguments: ./taskmaster [conf file]", 1);
	if (config_init(&conf, av[1]))
		clean_exit("Can't load config file", 2);
	g_clean_up.config = &conf;
	if (prog_lst_init(&prog_lst, &conf))
		clean_exit("Can't init program's list", 3);
	g_clean_up.prog_lst = &prog_lst;
	if (prog_lst_run(&prog_lst) == -1)
		clean_exit("Fork failed", 4);
	if (prompt_init(&prompt, "> "))
		clean_exit("Can init prompt", 5);
	g_clean_up.prompt = &prompt;
	while (42) {
		if (prompt_query(&prompt, &cmd))
			clean_exit("Can't access terminal", 6);
		action.sz = cmd_split(&cmd, action.cmds, 2);
		if (action_call(&action) == AR_STOP)
			break;
		printf("\n");
	}
	clean_exit(NULL, 0);
}

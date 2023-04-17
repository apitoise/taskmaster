/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:47:06 by fcadet            #+#    #+#             */
/*   Updated: 2023/03/28 10:04:28 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/prompt.h"
#include "../hdrs/builtin.h"

int			main(void) {
	prompt_t		prompt;
	cmd_t			cmd;

	prompt_init(&prompt, "> ");
	while (42) {
		if (prompt_query(&prompt, &cmd)) {
			fprintf(stderr, "Error: Can't get command\n");
			exit(4);
		}
		parse_cmd(&cmd);
//		cmd_print(&cmd, 0, NULL);
		printf("\n");
	}
	return (0);
}

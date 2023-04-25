/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:03:51 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/25 09:19:51 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/clean.h"

clean_t		g_clean = { 0 };

static void	clean_fact(char *error, int ret, int is_child) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (g_clean.prog_dic) {
		if (!is_child) {
			prog_dic_update(g_clean.prog_dic);
			prog_dic_kill(g_clean.prog_dic, SIGKILL);
		}
		prog_dic_free(g_clean.prog_dic);
	}
	if (g_clean.config)
		config_free(g_clean.config);
	if (g_clean.prompt)
		prompt_free(g_clean.prompt);
	exit(ret);
}

void	clean_exit_child(char *error, int ret) {
	clean_fact(error, ret, 1);
}

void	clean_exit(char *error, int ret) {
	clean_fact(error, ret, 0);
}



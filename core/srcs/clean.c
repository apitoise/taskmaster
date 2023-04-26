/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:03:51 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/26 12:47:34 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/clean.h"

static void	clean_fact(char *error, int ret, int is_child) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (glob.prog_dic) {
		if (!is_child) {
			prog_dic_update(glob.prog_dic);
			prog_dic_kill(glob.prog_dic, SIGKILL);
		}
		prog_dic_free(glob.prog_dic);
	}
	if (glob.config)
		config_free(glob.config);
	if (glob.prompt)
		prompt_free(glob.prompt);
	exit(ret);
}

void	clean_exit_child(char *error, int ret) {
	clean_fact(error, ret, 1);
}

void	clean_exit(char *error, int ret) {
	clean_fact(error, ret, 0);
}



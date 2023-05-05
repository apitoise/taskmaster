/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:03:51 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 08:23:05 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/clean.h"

void	clean_glob(void) {
	if (glob.prog_dic)
		prog_dic_free(glob.prog_dic);
	if (glob.config)
		config_free(glob.config);
	if (glob.prompt)
		prompt_free(glob.prompt);
}

/*
static void	clean_fact(char *error, int ret, int is_child) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (glob.prog_dic) {
		if (!is_child) {
			// ???
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

void	clean_exit(char *error, int ret) {
	clean_fact(error, ret, 0);
}
*/

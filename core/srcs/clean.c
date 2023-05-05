/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:03:51 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 12:13:03 by fcadet           ###   ########.fr       */
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

void	clean_exit(char *error, int ret) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	prog_dic_kill(glob.prog_dic);
	clean_glob();
	exit(ret);
}

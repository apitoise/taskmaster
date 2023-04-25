/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:04:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/24 11:26:34 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
#define CLEAN_H

#include "prog_dic.h"
#include "kill.h"

typedef struct		clean_s {
	prog_dic_t		*prog_dic;
	conf_t			*config;
	prompt_t		*prompt;
}					clean_t;

extern clean_t		g_clean;

void	clean_exit(char *error, int ret);

#endif // CLEAN_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unwrap.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 19:05:55 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/26 16:28:09 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNWRAP_H
#define UNWRAP_H

#include "includes.h"

#define OPT_NB		14

typedef struct		unw_targ_s {
	char			*key;
	void			*def;
	data_type_t		type;
}					unw_targ_t;

typedef enum		unw_opts_e {
	UO_CMD,
	UO_NUMPROCS,
	UO_AUTOSTART,
	UO_AUTORESTART,
	UO_EXITCODES,
	UO_STARTTIME,
	UO_STARTRETRIES,
	UO_STOPSIGNAL,
	UO_STOPTIME,
	UO_STDOUT,
	UO_STDERR,
	UO_ENV,
	UO_WORKINGDIR,
	UO_UMASK,
}					unw_opts_t;

int			node_unw(node_t *node, void **dst, data_type_t type);
int			dic_get_unw(dict_t *dic, void **dst, unw_targ_t *targ, uint64_t *opt_nb);
vec_t		*vec_unw(vec_t *vec, data_type_t type);

#endif // UNWRAP_H

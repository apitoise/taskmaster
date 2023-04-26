/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:39:45 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/26 12:50:15 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_H
#define ACTION_H

#include "clean.h"

#define ACT_NB		6

typedef struct	action_s {
	char		*cmds[STD_MAX];
	uint64_t	sz;
}				action_t;

typedef int		(*action_fn_t)(action_t *action);

int			action_call(action_t *action);

#endif // ACTION_H

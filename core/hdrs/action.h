/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:39:45 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/24 11:12:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_H
#define ACTION_H

#include "clean.h"

typedef enum	act_res_e {
	AR_CONT,
	AR_STOP,
	AR_ERR,
}				act_res_t;

typedef struct	action_s {
	char		*cmds[STD_MAX];
	uint64_t	sz;
}				action_t;

act_res_t	action_call(action_t *action);

#endif // ACTION_H

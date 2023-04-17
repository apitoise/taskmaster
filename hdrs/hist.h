/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:01:21 by herrfalco         #+#    #+#             */
/*   Updated: 2023/03/27 11:25:58 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIST_H
#define HIST_H

#include "cmd.h"

typedef struct		hist_s {
	cmd_t			lst[MAX_SZ];
	uint64_t		head;
	uint64_t		sz;
}					hist_t;

cmd_t	*hist_get(hist_t *hist, uint64_t idx);
void	hist_save(hist_t *hist, cmd_t *cmd);

#endif // HIST_H

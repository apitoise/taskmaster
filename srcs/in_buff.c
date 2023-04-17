/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_buff.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:11:19 by herrfalco         #+#    #+#             */
/*   Updated: 2023/03/28 08:37:36 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/in_buff.h"

int				in_buff_push(in_buff_t *q, char c) {
	if (q->sz == MAX_SZ)
		return (-1);
	q->buff[(q->head + q->sz++) % MAX_SZ] = c;
	return (0);
}

int				in_buff_pop(in_buff_t *q) {
	int			c;

	if (!q->sz)
		return (-1);
	c = q->buff[q->head];
	q->head = (q->head + 1) % MAX_SZ;
	--q->sz;
	return (c);
}

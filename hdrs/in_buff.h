/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_buff.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:03:51 by herrfalco         #+#    #+#             */
/*   Updated: 2023/03/28 08:37:49 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IN_BUFF_H
#define IN_BUFF_H

#include "includes.h"

typedef struct		in_buff_s {
	char			buff[MAX_SZ];
	uint64_t		head;
	uint64_t		sz;
}					in_buff_t;

int				in_buff_push(in_buff_t *q, char c);
int				in_buff_pop(in_buff_t *q);

#endif // IN_BUFF_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:12:49 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/21 16:01:20 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERM_H
#define TERM_H

#include "includes.h"
#include "in_buff.h"

typedef enum		dir_e {
	D_FW,
	D_BW,
}					dir_t;

int		term_init(void);
int		term_fini(void);
int		term_pop(void);
int		term_push(char c);
int		term_dim(uint64_t *row, uint64_t *col);
int		term_get_cur(uint64_t *row, uint64_t *col);
int		term_set_cur(uint64_t delta, dir_t dir);

#endif // TERM_H

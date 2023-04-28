/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:12:49 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/28 18:31:29 by fcadet           ###   ########.fr       */
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

typedef struct		term_s {
	struct termios	save;
	in_buff_t		in_buff;
	void			(*fn)(void);
	uint64_t		usleep;
}					term_t;

int		term_init(void (*fn)(void), uint64_t usleep);
int		term_fini(void);
int		term_pop(void);
int		term_push(char c);
int		term_dim(uint64_t *row, uint64_t *col);
int		term_get_cur(uint64_t *row, uint64_t *col);
int		term_set_cur(uint64_t delta, dir_t dir);

#endif // TERM_H

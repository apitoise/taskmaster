/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:55:15 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/28 15:59:24 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
#define PROMPT_H

#include "includes.h"
#include "hist.h"
#include "term.h"

typedef struct		prompt_s {
	const char		*hdr;
	hist_t			hist;
	uint8_t			hist_flag;
	uint64_t		hist_idx;
	cmd_t			cur_sav;
	cmd_t			cur_cmd;
	uint64_t		cur_pos;
}					prompt_t;

prompt_t	*prompt_new(const char *hdr, void (*fn)(void), uint64_t usleep);
void		prompt_free(prompt_t *prompt);
int			prompt_query(prompt_t *prompt, cmd_t *cmd);

#endif // PROMPT_H

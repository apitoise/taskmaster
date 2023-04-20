/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 10:58:44 by herrfalco         #+#    #+#             */
/*   Updated: 2023/03/27 12:52:23 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
#define CMD_H

#include "includes.h"

#define U_ERROR	((uint64_t)-1)

typedef struct		cmd_s {
	char			buff[MAX_SZ + 1];
	uint64_t		sz;
}					cmd_t;

int			cmd_insert(cmd_t *cmd, char *str, uint64_t idx);
int			cmd_del(cmd_t *cmd, uint64_t idx);
void		cmd_cpy(cmd_t *dst, const cmd_t *src);
void		cmd_print(const cmd_t *cmd, uint64_t idx, char *bef);
uint64_t	cmd_split(cmd_t *cmd, char **res, uint64_t n_res);

#endif // CMD_H

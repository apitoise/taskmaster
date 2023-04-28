/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 10:57:45 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/28 19:29:52 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/cmd.h"

int		cmd_insert(cmd_t *cmd, char *str, uint64_t idx) {
	uint64_t		i, len = strlen(str);

	if (cmd->sz + len > MAX_SZ || idx > cmd->sz)
		return (-1);
	for (i = cmd->sz + len - 1; i > idx + len - 1; --i)
		cmd->buff[i] = cmd->buff[i - len];	
	for (i = 0; i < len; ++i)
		cmd->buff[idx + i] = str[i];
	cmd->sz += len;
	return (0);
}

int		cmd_del(cmd_t *cmd, uint64_t idx) {
	if (!cmd->sz || idx >= cmd->sz)
		return (-1);
	for (; idx + 1 < cmd->sz; ++idx)
		cmd->buff[idx] = cmd->buff[idx + 1];
	--cmd->sz;
	return (0);
}

void	cmd_cpy(cmd_t *dst, const cmd_t *src) {
	strncpy(dst->buff, src->buff, src->sz);
	dst->sz = src->sz;
}

void	cmd_print(const cmd_t *cmd, uint64_t idx, char *bef) {
	if (bef)
		printf("%s", bef);
	printf("%.*s", (int)(cmd->sz - idx), cmd->buff + idx);
	fflush(stdout);
}

uint64_t	cmd_split(cmd_t *cmd, char **res, uint64_t n_res) {
	uint64_t	i, in = 0, j = 0;

	for (i = 0; i < cmd->sz; ++i) {
		if (isspace(cmd->buff[i])) {
			cmd->buff[i] = '\0';
			in = 0;
		} else if (!in) {
			if (n_res == j)
				return (U_ERROR);
			res[j++] = &cmd->buff[i];
			in = 1;
		}
	}
	cmd->buff[cmd->sz] = '\0';
	return (j);
}

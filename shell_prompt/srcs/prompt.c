/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:58:05 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/27 23:38:48 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/prompt.h"

prompt_t	*prompt_new(const char *hdr) {
	prompt_t	*new;

	if (term_init()
		|| !(new = malloc(sizeof(prompt_t))))
		return (NULL);
	bzero(new, sizeof(prompt_t));
	new->hdr = hdr;
	new->hist_idx = SPE_VAL;
	return (new);
}

void		prompt_free(prompt_t *prompt) {
	term_fini();
	free(prompt);
}

static int		new_line(prompt_t *prompt) {
	uint64_t	term_w;

	if (term_dim(NULL, &term_w))
		return (-1);
	if (!((prompt->cur_cmd.sz + strlen(prompt->hdr)) % term_w))
		printf(" \b");
	return (0);
}

static int		handle_esc(prompt_t *prompt, char *buff) {
	uint64_t	col;
	int			ret;
	cmd_t		*cmd;

	buff[0] = '^';
	if ((ret = term_pop(NULL, 0)) < 0)
		return (-1);
	if ((buff[1] = ret) != '[')
		return (0);
	if ((ret = term_pop(NULL, 0)) < 0)
		return (-1);
	switch ((buff[2] = ret)) {
		case 'A':
			prompt->hist_flag = 1;
			if (!(cmd = hist_get(&prompt->hist, prompt->hist_idx + 1)))
				return (1);
			if (!(++prompt->hist_idx))
				cmd_cpy(&prompt->cur_sav, &prompt->cur_cmd);
			cmd_cpy(&prompt->cur_cmd, cmd);
			if (term_set_cur(prompt->cur_pos, D_BW))
				return (-1);
			cmd_print(&prompt->cur_cmd, 0, "\033[J");
			if (new_line(prompt))
				return (-1);
			prompt->cur_pos = prompt->cur_cmd.sz;
			return (1);
		case 'B':
			prompt->hist_flag = 1;
			if (!prompt->hist_idx)
				cmd_cpy(&prompt->cur_cmd, &prompt->cur_sav);
			else if (!(cmd = hist_get(&prompt->hist, prompt->hist_idx - 1)))
				return (1);
			else
				cmd_cpy(&prompt->cur_cmd, cmd);
			--prompt->hist_idx;
			if (term_set_cur(prompt->cur_pos, D_BW))
				return (-1);
			cmd_print(&prompt->cur_cmd, 0, "\033[J");
			if (new_line(prompt))
				return (-1);
			prompt->cur_pos = prompt->cur_cmd.sz;
			return (1);
		case '3':
			if ((ret = term_pop(NULL, 0)) < 0)
				return (-1);
			if ((buff[3] = ret) != '~')
				return (0);
			if (cmd_del(&prompt->cur_cmd, prompt->cur_pos))
				return (1);
			cmd_print(&prompt->cur_cmd, prompt->cur_pos, "\033[?25l\033[J");
			if (term_dim(NULL, &col))
				return (-1);
			if (term_set_cur(prompt->cur_cmd.sz - prompt->cur_pos
				- !(prompt->cur_cmd.sz == prompt->cur_pos
				|| ((prompt->cur_cmd.sz + strlen(prompt->hdr)) % col)), D_BW))
				return (-1);
			printf("\033[?25h");
			return (1);
		case 'C':
			prompt->hist_flag = 1;
			if (prompt->cur_pos != prompt->cur_cmd.sz) {
				if (term_set_cur(1, D_FW))
					return (-1);
				++prompt->cur_pos;
			}
			return (1);
		case 'D':
			prompt->hist_flag = 1;
			if (prompt->cur_pos) {
				if (term_set_cur(1, D_BW))
					return (-1);
				--prompt->cur_pos;
			}
			return (1);
		default:
			return (0);
	}
}

static int		handle_spe_char(prompt_t *prompt, char *buff) {
	uint64_t		col;

	switch (buff[0]) {
		case '\x1b':
			return (handle_esc(prompt, buff));
		case '\x1':
			if (term_set_cur(prompt->cur_pos, D_BW))
				return (-1);
			prompt->cur_pos = 0;
			return (1);
		case '\x5':
			if (term_set_cur(prompt->cur_cmd.sz - prompt->cur_pos, D_FW))
				return (-1);
			prompt->cur_pos = prompt->cur_cmd.sz;
			return (1);
		case '\x7f':
			if (cmd_del(&prompt->cur_cmd, prompt->cur_pos - 1))
				return (1);
			--prompt->cur_pos;
			if (term_set_cur(1, D_BW))
				return (-1);
			cmd_print(&prompt->cur_cmd, prompt->cur_pos, "\033[?25l\033[J");
			if (term_dim(NULL, &col))
				return (-1);
			if (term_set_cur(prompt->cur_cmd.sz - prompt->cur_pos
				- !(prompt->cur_cmd.sz == prompt->cur_pos
				|| ((prompt->cur_cmd.sz + strlen(prompt->hdr)) % col)), D_BW))
				return (-1);
			printf("\033[?25h");
			return (1);
		default:
			return (0);
	}
}

int			prompt_query(prompt_t *prompt, cmd_t *cmd, void (*fn)(void), uint64_t usleep) {
	int			ret;
	char		buff[MAX_SZ] = { 0 };

	prompt->cur_pos = 0;
	prompt->cur_cmd.sz = 0;
	prompt->hist_idx = SPE_VAL;
	printf("%s", prompt->hdr);
	for (; (ret = term_pop(fn, usleep)) != '\n'; bzero(buff, MAX_SZ)) {
		if ((buff[0] = ret) < 0)
			return (-1);
		prompt->hist_flag = 0;
		if ((ret = handle_spe_char(prompt, buff)) < 0)
			return (-1);
		if (!ret) {
			if (cmd_insert(&prompt->cur_cmd, buff, prompt->cur_pos))
				break;
			cmd_print(&prompt->cur_cmd, prompt->cur_pos, "\033[?25l");
			if (new_line(prompt))
				return (-1);
			if (term_set_cur(prompt->cur_cmd.sz - prompt->cur_pos - strlen(buff), D_BW))
				return (-1);
			printf("\033[?25h");
			prompt->cur_pos += strlen(buff);
		}
		if (!prompt->hist_flag)
			prompt->hist_idx = SPE_VAL;
	}
	hist_save(&prompt->hist, &prompt->cur_cmd);
	printf("\n");
	cmd_cpy(cmd, &prompt->cur_cmd);
	return (0);
}

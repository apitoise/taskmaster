/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:16:03 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/28 20:13:08 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/term.h"

static term_t				g_term;

static int	term_getchar() {
	int				c = 0;
	int				ret;

	while ((ret = read(STDIN_FILENO, &c, 1)) != 1) {
		if (ret < 0)
			return (-1);
		g_term.fn();
		usleep(g_term.usleep);
	}
	return (c);
}

int		term_init(void (*fn)(void), uint64_t usleep) {
	struct termios	new_term;

    if (tcgetattr(STDIN_FILENO, &g_term.save))
		return (-1);
	setvbuf(stdout, NULL, _IONBF, 0);
	bzero(&g_term.in_buff, sizeof(in_buff_t));
	g_term.fn = fn;
	g_term.usleep = usleep;
	new_term = g_term.save;
	new_term.c_cc[VMIN] = 0;
	new_term.c_cc[VTIME] = 0;
	new_term.c_lflag &= ~(ICANON | ECHO);
    return (tcsetattr(STDIN_FILENO, TCSANOW, &new_term));
}

int		 term_fini(void) {
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.save));
}

int		term_pop(void) {
	int				c;

	if ((c = in_buff_pop(&g_term.in_buff)) >= 0)
		return (c);
	return (term_getchar());
}

int		term_push(char c) {
	return (in_buff_push(&g_term.in_buff, c));
}

int		term_dim(uint64_t *row, uint64_t *col) {
	struct winsize size;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
		return (-1);
	if (row)
		*row = size.ws_row;
	if (col)
		*col = size.ws_col;
	return (0);
}

int		term_get_cur(uint64_t *row, uint64_t *col) {
	int			c;
	uint64_t	row_res, col_res;
	uint8_t		step;

	printf("\033[6n");
	fflush(stdout);
	for (step = 0, row_res = 0, col_res = 0; step < 4;) {
		if ((c = term_getchar()) == -1)
			return (-1);
		switch (step) {
			case 0:
				if (c == '\033')
					++step;
				else
					term_push(c);
				break;
			case 1:
				if (c == '[')
					++step;
				else
					step = 0;
				break;
			case 2:
				if (c == ';')
					++step;
				else if (isdigit(c))
					row_res = row_res * 10 + c - '0';
				else {
					row_res = 0;
					step = 0;
				}
				break;
			case 3:
				if (c == 'R')
					++step;
				else if (isdigit(c))	
					col_res = col_res * 10 + c - '0';
				else {
					row_res = 0;
					col_res = 0;
					step = 0;
				}
				break;
		}
	}
	if (row)
		*row = row_res - 1;
	if (col)
		*col = col_res - 1;
	return (0);
}

int		term_set_cur(uint64_t delta, dir_t dir) {
	uint64_t	term_w, cur_x, cur_y;

	if (term_get_cur(&cur_y, &cur_x)
		|| term_dim(NULL, &term_w))
		return (-1);
	switch (dir) {
		case D_FW:
			printf("\033[%lu;%luH",
				cur_y + (cur_x + delta) / term_w + 1,
				(cur_x + delta) % term_w + 1);
			fflush(stdout);
			break;
		case D_BW:
			printf("\033[%lu;%luH",
				cur_y - (delta + term_w - cur_x - 1) / term_w + 1,
				(term_w - (delta + term_w - cur_x) % term_w) % term_w + 1);
			fflush(stdout);
	}
	return (0);
}

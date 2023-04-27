/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:16:03 by herrfalco         #+#    #+#             */
/*   Updated: 2023/04/27 23:58:13 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/term.h"

static struct termios		g_term;
static in_buff_t			g_in_buff = { 0 };
/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main() {
    // get the current terminal attributes
    struct termios old_attr, new_attr;
    tcgetattr(STDIN_FILENO, &old_attr);
    
    // set the new attributes to the current attributes
    new_attr = old_attr;
    
    // set the input mode to non-blocking
    new_attr.c_cc[VMIN] = 0;
    new_attr.c_cc[VTIME] = 0;
    new_attr.c_lflag &= ~(ICANON | ECHO);
    new_attr.c_cflag |= CLOCAL | CREAD;
    new_attr.c_cflag &= ~CRTSCTS;
    new_attr.c_iflag &= ~(IXON | IXOFF | IXANY);
    new_attr.c_iflag |= IGNBRK;
    new_attr.c_oflag &= ~OPOST;
    new_attr.c_cc[VEOL] = 1;
    
    // apply the new attributes
    tcsetattr(STDIN_FILENO, TCSANOW | TCSAFLUSH, &new_attr);
    
    while (1) {
        // read input from stdin
        int c = getchar();
        if (c != EOF) {
            printf("You typed: %c\n", c);
        } else {
            printf("No input available\n");
        }
    }
    
    // restore the old terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
    
    return 0;
}
*/

int		term_init(void) {
	struct termios	new_term;
	int				std_flags;

    if (tcgetattr(STDIN_FILENO, &g_term)
		|| (std_flags = fcntl(STDIN_FILENO, F_GETFL)) == -1
		|| fcntl(STDIN_FILENO, F_SETFL,
			std_flags | O_NONBLOCK) == -1)
		return (-1);
	new_term = g_term;
	new_term.c_lflag &= ~(ICANON | ECHO);
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0;
    return (tcsetattr(STDIN_FILENO, TCSANOW, &new_term));
}

int		 term_fini(void) {
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_term));
}

int		term_pop(void (*fn)(void), uint64_t usleep) {
	int				c;
	fd_set			set;
	struct timeval	timeout = {
		.tv_sec = 0,
		.tv_usec = usleep,
	};

	while (42) {
		if ((c = in_buff_pop(&g_in_buff)) >= 0)
			return (c);
		FD_ZERO(&set);
		FD_SET(STDIN_FILENO, &set);
		if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) == -1)
			return (-1);
		if (FD_ISSET(STDIN_FILENO, &set))
			return ((c = getchar()) != EOF ? c : -1);
		if (fn)
			fn();
	}
	return (-1);
}

int		term_push(char c) {
	return (in_buff_push(&g_in_buff, c));
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
	int			c, ret;
	uint64_t	row_res, col_res;

	printf("\033[6n");
	while ((c = getchar()) != '\033')
		if (c == EOF || term_push(c))
			return (-1);
#ifdef __APPLE__
	while ((ret = scanf("[%8llu;%8lluR", &row_res, &col_res)) != 2) {
#else
	while ((ret = scanf("[%8lu;%8luR", &row_res, &col_res)) != 2) {
#endif
		if (ret == EOF)
			return (-1);
		while ((c = getchar()) != '\033')
			if (c == EOF)
				return (-1);
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
#ifdef __APPLE__
			printf("\033[%llu;%lluH",
#else
			printf("\033[%lu;%luH",
#endif
				cur_y + (cur_x + delta) / term_w + 1,
				(cur_x + delta) % term_w + 1);
			break;
		case D_BW:
#ifdef __APPLE__
			printf("\033[%llu;%lluH",
#else
			printf("\033[%lu;%luH",
#endif
				cur_y - (delta + term_w - cur_x - 1) / term_w + 1,
				(term_w - (delta + term_w - cur_x) % term_w) % term_w + 1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 16:38:37 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/26 16:38:55 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void	sighandler(int sig) {
	int	i, sig_int[] = { 
		SIGALRM, SIGHUP, SIGINT, SIGPIPE,
		SIGTERM, SIGUSR1, SIGUSR2, SIGQUIT
	};
	char *sig_str[] = {
		"ALRM", "HUP", "INT", "PIPE",
		"TERM", "USR1", "USR2", "QUIT"
	};

	for (i = 0; i < 8; ++i) {
		if (sig == sig_int[i]) {
			fprintf(stdout, "(log_out) Signal: %s\n", sig_str[i]); 
			fprintf(stderr, "(log_err) Signal: %s\n", sig_str[i]); 
			fflush(stdout);
			fflush(stderr);
			break ;
		}
	}
}

int	main(void) {
	int	i, sig[] = { 
		SIGALRM, SIGHUP, SIGINT, SIGPIPE,
		SIGTERM, SIGUSR1, SIGUSR2, SIGQUIT
	};

	for (i = 0; i < 8; ++i)
		signal(sig[i], sighandler);
	while (42);
	return (0);
}

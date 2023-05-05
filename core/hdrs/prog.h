/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:06:30 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 12:10:02 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROG_H
#define PROG_H

#include "unwrap.h"

void	clean_exit(char *error, int ret);
void	clean_exit_child(char *error, int ret);

typedef int			(*cmp_fn_t)(void *, void *);

typedef enum		state_e {
	S_STOP,
	S_STOP_WAIT,
	S_STOPPED,		// Stopped or killed if time exceeded
	S_START,
	S_START_WAIT,
	S_STARTED,
	S_START_FAIL,
	S_RETRY,
	S_EXITED,		// Exited or signaled
}					state_t;

typedef enum		restart_pol_e {
	RP_ALWAYS,
	RP_NEVER,
	RP_UNEXP,
}					restart_pol_t;

typedef struct		proc_s {
	pid_t			pid;
	int				status;
	state_t			state;
	uint64_t		retry;
}					proc_t;

typedef struct		prog_s {
	char			*name;
	char			*cmd;
	uint64_t		numprocs;
	uint64_t		autostart;
	uint64_t		autorestart;
	vec_t			*exitcodes;
	uint64_t		starttime;
	uint64_t		startretries;
	uint64_t		stopsignal;
	uint64_t		stoptime;
	char			*std_out;
	char			*std_err;
	vec_t			*env;
	char			*workingdir;
	mode_t			umask;
	vec_t			*procs;
	time_t			timestamp;
	uint64_t		retries;
}					prog_t;

typedef struct		map_s {
	char			*bools[2];
	char			*auto_r[3];
	char			*sigs_s[11];
	uint64_t		sigs_v[11];
}					map_t;

prog_t		*prog_new(char *name, dict_t *opts);
void		prog_free(prog_t *prog);
int			prog_proc_create(prog_t *prog);
void		prog_kill(prog_t *prog);
int			prog_status(prog_t *prog);
int			prog_cmp(prog_t *p1, prog_t *p2);

#endif // PROG_H

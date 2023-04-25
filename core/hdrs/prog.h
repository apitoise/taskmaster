/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:06:30 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/25 12:30:29 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROG_H
#define PROG_H

#include "unwrap.h"

void	clean_exit(char *error, int ret);
void	clean_exit_child(char *error, int ret);

typedef struct		proc_s {
	pid_t			pid;
	time_t			timestamp;
	int				status;
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
	uint64_t		umask;
	vec_t			*procs;
}					prog_t;

typedef struct		map_s {
	char			*bools[2];
	char			*auto_r[3];
	char			*sigs_s[11];
	uint64_t		sigs_v[11];
}					map_t;

prog_t		*prog_new(char *name, dict_t *opts);
void		prog_free(prog_t *prog);
void		prog_print(prog_t *prog);
void		prog_clean_procs(prog_t *prog);
int			prog_run(prog_t *prog);
int			prog_update(prog_t *prog);
int			prog_kill(prog_t *prog, int signal);
int			prog_status(prog_t *prog);

#endif // PROG_H

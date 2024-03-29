/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:09:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/23 20:27:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/monitor.h"

static int	io_redirect(int old_fd, char *new_path) {
	int	new_fd;

	if (*new_path
		&& (new_fd = open(new_path, O_WRONLY | O_CREAT | O_APPEND, 0666)) != -1) {
		if (dup2(new_fd, old_fd) == -1) {
			close(new_fd);
			return (-1);
		}
	}
	return (0);
}

static int	str_split(char *str, char **res, uint64_t n_res) {
	uint64_t	i, in = 0, j = 0, len = strlen(str);

	for (i = 0; i < len; ++i) {
		if (isspace(str[i])) {
			str[i] = '\0';
			in = 0;
		}
		else if (!in) {
			if (j == n_res)
				return (-1);
			res[j++] = &str[i];
			in = 1;
		}
	}
	str[i] = '\0';
	res[j] = NULL;
	return (0);
}

void		monitor_fn(void) {
	uint64_t		i, j;
	char			*args[STD_MAX];
	prog_t			*prog;
	proc_t			*proc;
	time_t			current;

	if (time(&current) == -1) {
		log_error(NULL, 0, "Can't get time");
		return;
	}
	for (i = 0; i < glob.prog_dic->keys->sz; ++i) {
		prog = glob.prog_dic->values->data[i];
		for (j = 0; j < prog->procs->sz; ++j) {
			proc = prog->procs->data[j];
			switch (proc->state) {
				case S_STOP:
					kill(proc->pid, prog->stopsignal);
					proc->state = S_STOP_WAIT;
					log_state(prog, j);
					proc->timestamp = current;
					__attribute__ ((fallthrough));
				case S_STOP_WAIT: 
					if (proc->timestamp + prog->stoptime < (uint64_t)current)
						kill(proc->pid, SIGKILL);
					waitpid(proc->pid, &proc->status, WNOHANG);
					if (access(proc->path, F_OK)) {
						proc->state = S_STOPPED;
						log_state(prog, j);
					}
					break;
				case S_STOPPED:
					if (!proc->restart)
						break ;
					proc->state = S_START;
					proc->restart = 0;
					__attribute__ ((fallthrough));
				case S_START: 
					if ((proc->pid = fork()) == -1) {
						log_error(prog, j, "Fork failed");
						proc->state = S_RETRY;	
						log_state(prog, j);
						break;
					} else if (!proc->pid) {
						umask(prog->umask);
						if ((*prog->workingdir && chdir(prog->workingdir))
							|| io_redirect(STDOUT_FILENO,
								prog->std_out)
							|| io_redirect(STDERR_FILENO,
								prog->std_err)
							|| str_split(prog->cmd, args, STD_MAX)
							|| execvpe(prog->cmd, args,
								(char **)prog->env->data))
							clean_exit_child();
					} else {
						sprintf(proc->path, "/proc/%d/status", proc->pid);
						proc->timestamp = current;
						proc->state = S_START_WAIT;
						log_state(prog, j);
					}
					__attribute__ ((fallthrough));
				case S_START_WAIT:
					if (proc->timestamp + prog->starttime > (uint64_t)current) {
						if (waitpid(proc->pid, &proc->status, WNOHANG) == -1
							|| access(proc->path, F_OK)) {
							proc->state = S_RETRY;
							log_state(prog, j);
						}
						break;
					}
					proc->state = S_STARTED;
					log_state(prog, j);
					__attribute__ ((fallthrough));
				case S_STARTED:
					if (waitpid(proc->pid, &proc->status, WNOHANG) == -1
						|| access(proc->path, F_OK)) {
						if (WIFEXITED(proc->status)) {
							proc->state = S_EXITED;
							proc->bad_code = !vec_is_in(prog->exitcodes,
								(void *)(uint64_t)WEXITSTATUS(proc->status));
						} else
							proc->state = S_RETRY;
						log_state(prog, j);
					}
					break ;
				case S_RETRY:
					if (proc->retry < prog->startretries) {
						++proc->retry;
						proc->state = S_START;
					} else {
						proc->state = S_START_FAIL;
						log_state(prog, j);
					}
					break;
				case S_EXITED: 
					switch (prog->autorestart) {
						case RP_UNEXP:
							if (!proc->bad_code)
								break ;
							proc->state = S_RETRY;
							log_state(prog, j);
							break ;
						case RP_ALWAYS:
							proc->retry = 0;
							proc->state = S_START;
						default:
							break;
					}
				default:
					break;
			}
		}
	}
	return;
}

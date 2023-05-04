/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:09:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/04 09:31:33 by fcadet           ###   ########.fr       */
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
	uint64_t		i, j, k;
	char			*args[STD_MAX], path[STD_MAX];
	prog_t			*prog;
	proc_t			*proc;
	pid_t			pid_ret;
	time_t			current;

	if (time(&current) == -1) //need to log error
		return;
	for (i = 0; i < glob.prog_dic->keys->sz; ++i) {
		prog = glob.prog_dic->values->data[i];
		for (j = 0; j < prog->procs->sz; ++j) {
			proc = prog->procs->data[j];
			sprintf(path, "/proc/%d/status", proc->pid);
			switch (proc->state) {
				case S_STOP: 
					break;
				case S_STOP_WAIT: 
					break;
				case S_STOPPED: 
					break;
				case S_START: 
					if ((proc->pid = fork()) == -1)
						proc->state = S_RETRY;	
					else if (!proc->pid) {
						umask(prog->umask);
						if (io_redirect(STDOUT_FILENO, prog->std_out)
							|| io_redirect(STDERR_FILENO, prog->std_err)
							|| str_split(prog->cmd, args, STD_MAX)
							|| execvpe(prog->cmd, args, (char **)prog->env->data)) {
							fclose(stdout);
							fclose(stdin);
							clean_glob();
							raise(SIGKILL);
						}
					} else {
						prog->timestamp = current;
						proc->state = S_START_WAIT;
					}
					break;
				case S_START_WAIT:
					if (access(path, F_OK)
						|| waitpid(proc->pid, &proc->status, WNOHANG) == -1)
						proc->state = S_RETRY;
					if (access(path, F_OK))
						proc->state = S_EXITED;
					else if (prog->timestamp + prog->starttime <= current)
						proc->state = S_STARTED;
					break;
				case S_STARTED: 
					if (access(path, F_OK)
						|| waitpid(proc->pid, &proc->status, WNOHANG) < 0
						|| access(path, F_OK)) {
						proc->state = S_RETRY;
					}
					break;
				case S_RETRY:
					if (proc->retry < prog->startretries) {
						++proc->retry;
						proc->state = S_START;
					} else
						proc->state = S_FATAL;
					break;
				case S_EXITED: 
					switch (prog->autorestart) {
						case RP_UNEXP:
							if (WIFEXITED(proc->status))
								for (k = 0; k < prog->exitcodes->sz; ++k)
									if (WEXITSTATUS(proc->status)
										== (uint64_t)prog->exitcodes->data[i])
										break;
						case RP_ALWAYS:
							proc->retry = 0;
							proc->state = S_START;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}
	return;
}

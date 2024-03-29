/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:03:51 by fcadet            #+#    #+#             */
/*   Updated: 2023/07/12 15:57:55 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/clean.h"

void	clean_glob(void) {
	if (glob.prog_dic)
		prog_dic_free(glob.prog_dic);
	if (glob.config)
		config_free(glob.config);
	if (glob.prompt)
		prompt_free(glob.prompt);
}

void	clean_exit_child(void) {
	fclose(stdout);
	fclose(stdin);
	fclose(stderr);
	if (glob.log_file)
		fclose(glob.log_file);
	clean_glob();
	raise(SIGKILL);
}

void	clean_exit(char *error, int ret) {
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	if (glob.prog_dic)
		prog_dic_kill(glob.prog_dic);
	if (glob.log_file) {
		if (error)
			log_error(NULL, 0, "Fatal error");
		else if (ret < 0)
			log_info(NULL, "Interrupted by signal");
		else
			log_info(NULL, "Supervisor exited");
		fclose(glob.log_file);
	}
	clean_glob();
	if (remove(LAUNCH_FILE))
		fprintf(stderr, "Error: Can not remove launch file.\n");
	exit(ret);
}

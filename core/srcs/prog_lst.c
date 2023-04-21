/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:02:50 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/21 18:52:07 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			prog_lst_init(vec_t *prog_lst, conf_t *conf) {
	dict_t		*root, *opts;
	uint64_t	i;

	if (node_unwrap(conf->root, (void **)&root, DT_DIC)
		|| root->keys->sz > STD_MAX)
		return (-1);
	prog_lst->sz = 0;
	for (i = 0; i < root->keys->sz; ++i, ++prog_lst->sz) {
		if (node_unwrap(root->values->data[i], (void **)&opts, DT_DIC)
			|| prog_init((prog_t *)&prog_lst->data[i], (char *)root->keys->data[i], opts))
			return (-1);
	}
	return (0);
}

void		prog_lst_free(vec_t *prog_lst) {
	uint64_t		i;

	for (i = 0; i < prog_lst->sz; ++i)
		prog_free((prog_t *)&prog_lst->data[i]);
}

int			prog_lst_run(vec_t *prog_lst) {
	uint64_t	i, procs_nb;
	int			ret = 0;
	prog_t		*prog;

	for (i = 0; i < prog_lst->sz; ++i) {
		prog = (prog_t *)prog_lst->data[i];
		if (prog->autostart) {
			for (procs_nb = 0; procs_nb < prog->numprocs; ++procs_nb) {
				if (proc_run(prog, (proc_t *)prog->procs->data[i])) {
					ret = -1;
					fprintf(stderr, "Error: %s %lu/%lu can't be run.\n"
						, prog->name, procs_nb, prog->numprocs);
				}
			}
		}
	}
	return (ret);
}

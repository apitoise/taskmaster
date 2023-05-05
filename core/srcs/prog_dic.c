/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog_dic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:02:50 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/03 10:42:50 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/prog_dic.h"

prog_dic_t		*prog_dic_new(conf_t *conf) {
	dict_t		*root, *opts;
	prog_dic_t	*pdic_new;
	prog_t		*p_new;
	uint64_t	i;

	if (node_unw(conf->root, (void **)&root, DT_DIC)
		|| !(pdic_new = dict_new(root->keys->sz)))
		return (NULL);
	for (i = 0; i < root->keys->sz; ++i) {
		p_new = NULL;
		if (node_unw(root->values->data[i], (void **)&opts, DT_DIC)
			|| !(p_new = prog_new((char *)root->keys->data[i], opts))
			|| dict_set(pdic_new, (char *)root->keys->data[i], p_new)) {
			if (p_new)
				prog_free(p_new);
			prog_dic_free(pdic_new);
			return (NULL);
		}
	}
	return (pdic_new);
}

void		prog_dic_free(prog_dic_t *prog_dic) {
	uint64_t		i;

	for (i = 0; i < prog_dic->keys->sz; ++i)
		prog_free((prog_t *)prog_dic->values->data[i]);
	dict_free(prog_dic);
}

int			prog_dic_proc_create(prog_dic_t *prog_dic) {
	uint64_t	i;
	prog_t		*prog;

	for (i = 0; i < prog_dic->keys->sz; ++i) {
		prog = prog_dic->values->data[i];
		if (!prog->procs->sz
			&& prog_proc_create(prog))
			return (-1);
	}
	return (0);
}

int			prog_dic_status(prog_dic_t *prog_dic) {
	uint64_t	i;
	int			ret = 0;

	for (i = 0; i < prog_dic->keys->sz; ++i)
		if (prog_status((prog_t *)prog_dic->values->data[i]))
			ret = -1;
	return (ret);
}

prog_dic_t	*prog_dic_reload(prog_dic_t *prog_dic, conf_t *conf) {
	prog_dic_t	*new;
	uint64_t	i;
	proc_t		*tmp;
	prog_t		*new_prog, *old_prog;

	if (!(new = prog_dic_new(conf)))
		return (NULL);
	for (i = 0; i < prog_dic->keys->sz; ++i) {
		old_prog = prog_dic->values->data[i];
		if (dict_get(new, old_prog->name, (void **)&new_prog)
			|| prog_cmp(new_prog, old_prog)) {
			prog_kill(old_prog);
			return (NULL);
		}
		else {
			while (old_prog->procs->sz) {
				vec_pop_back(old_prog->procs, (void **)&tmp);
				if (vec_push_back(new_prog->procs, tmp)) {
					vec_push_back(old_prog->procs, tmp);
					prog_dic_free(new);
					return (NULL);
				}
			}
		}
	}
	return (new);
}

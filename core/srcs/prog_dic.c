/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog_dic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:02:50 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/25 09:24:03 by fcadet           ###   ########.fr       */
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

int			prog_dic_run(prog_dic_t *prog_dic, char *name) {
	uint64_t	i;
	prog_t		*prog;

	if (name) {
		if (dict_get(prog_dic, name, (void **)&prog)
			|| prog_run(prog))
			return (-1);
	} else {
		for (i = 0; i < prog_dic->keys->sz; ++i) {
			prog = prog_dic->values->data[i];
			if (prog->autostart && prog_run(prog))
				return (-1);
		}
	}
	return (0);
}

int			prog_dic_update(prog_dic_t *prog_dic) {
	uint64_t		i;	

	for (i = 0; i < prog_dic->keys->sz; ++i)
		if (prog_update(prog_dic->values->data[i]))
			return (-1);
	return (0);
}

int			prog_dic_kill(prog_dic_t *prog_dic, int signal) {
	uint64_t	i;
	int		ret = 0;

	for (i = 0; i < prog_dic->keys->sz; ++i)
		if (prog_kill((prog_t *)prog_dic->values->data[i], signal))
			ret = -1;
	return (ret);
}

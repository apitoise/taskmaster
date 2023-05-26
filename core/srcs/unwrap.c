/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unwrap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 19:05:21 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/26 16:17:02 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/unwrap.h"

int			node_unw(node_t *node, void **dst, data_type_t type) {
	*dst = node->data;
	return (node->type != type ? -1 : 0);
}

int			dic_get_unw(dict_t *dic, void **dst, unw_targ_t *targ, uint64_t *opt_nb) {
	node_t	*node;

	if (!dict_get(dic, targ->key, (void **)&node)) {
		++*opt_nb;
		return (node_unw(node, dst, targ->type));
	}
	*dst = targ->def;
	return (0);
}

vec_t		*vec_unw(vec_t *vec, data_type_t type) {
	uint64_t	i;
	vec_t		*new;
	void		*val;

	if (!(new = vec_new(1)))
		return (NULL);
	if (!vec)
		return (new);
	for (i = 0; i < vec->sz; ++i) {
		if (node_unw(vec->data[i], &val, type)) {
			vec_free(new);
			return (NULL);
		}
		vec_push_back(new, val);	
	}
	return (new);
}

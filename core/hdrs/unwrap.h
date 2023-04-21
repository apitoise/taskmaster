/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unwrap.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 19:05:55 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/21 19:12:49 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNWRAP_H
#define UNWRAP_H

#include "includes.h"

int			node_unw(node_t *node, void **dst, data_type_t type);
int			dic_get_unw(dict_t *dic, char *key, void **dst,
	void *def, data_type_t type);
vec_t		*vec_unw(vec_t *vec, data_type_t type);

#endif // UNWRAP_H

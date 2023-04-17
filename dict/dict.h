/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:30:05 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/17 18:35:36 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DICT_H
#define DICT_H

#include "../vector/vector.h"

typedef int			(*cmp_fn_t)(void *, void *);

typedef struct		dict_s {
	vec_t			*keys;
	vec_t			*values;
}					dict_t;

dict_t		*dict_new(uint64_t cap);
void		dict_print(dict_t *dict);
void		dict_free(dict_t *dict);
int			dict_get(dict_t *dict, char *key, void **val);
int			dict_set(dict_t *dict, char *key, void *val);
int			dict_unset(dict_t *dict, char *key);

#endif // DICT_H

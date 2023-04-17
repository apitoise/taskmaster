/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 16:45:58 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/16 17:20:11 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct		vec_s {
	void			**data;
	uint64_t		cap;
	uint64_t		sz;
}					vec_t;

vec_t	*vec_new(uint64_t cap);
void	vec_free(vec_t *vec);
void	vec_print(vec_t *vec);
int		vec_get(vec_t *vec, uint64_t idx, void **val);
int		vec_set(vec_t *vec, uint64_t idx, void *val);
int		vec_insert(vec_t *vec, uint64_t idx, void *val);
int		vec_extract(vec_t *vec, uint64_t idx, void **val);
int		vec_push_back(vec_t *vec, void *val);
int		vec_push_front(vec_t *vec, void *val);
int		vec_pop_back(vec_t *vec, void **val);
int		vec_pop_front(vec_t *vec, void **val);

#endif // VECTOR_H

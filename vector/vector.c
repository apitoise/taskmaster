/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 16:45:51 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/23 09:26:28 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

vec_t	*vec_new(uint64_t cap) {
	vec_t		*new;

	if (!cap || !(new = malloc(sizeof(vec_t))))
		return (NULL);
	if (!(new->data = malloc(cap * sizeof(void *)))) {
		free(new);
		return (NULL);
	}
	new->cap = cap;
	new->sz = 0;
	return (new);
}

void		vec_free(vec_t *vec) {
	free(vec->data);
	free(vec);
}

void		vec_print(vec_t *vec) {
	uint64_t		i;

	printf("[");
	for (i = 0; i < vec->sz; ++i)
#ifdef __APPLE__
		printf("%s0x%llx%s",
#else
		printf("%s0x%lx%s",
#endif // __APPLE__
			!i ? " " : "",
			(uint64_t)vec->data[i],
			i + 1 < vec->sz ? ", " : " ");
	printf("]");
}

static int	vec_ext(vec_t *vec) {
	void		**new_data;

	if (!(new_data = malloc(vec->cap * 2 * sizeof(void *))))
		return (-1);
	memcpy(new_data, vec->data, sizeof(void *) * vec->sz);
	free(vec->data);
	vec->data = new_data;
	vec->cap *= 2;
	return (0);
}

int		vec_get(vec_t *vec, uint64_t idx, void **val) {
	if (idx >= vec->sz)
		return (-1);
	if (val)
		*val = vec->data[idx];
	return (0);
}

int		vec_set(vec_t *vec, uint64_t idx, void *val) {
	if (idx >= vec->sz)
		return (-1);
	vec->data[idx] = val;
	return (0);
}

int		vec_insert(vec_t *vec, uint64_t idx, void *val) {
	uint64_t		i;

	if (idx > vec->sz
		|| (vec->sz == vec->cap
		&& vec_ext(vec)))
		return (-1);
	for (i = vec->sz; i > idx; --i)
		vec->data[i] = vec->data[i - 1];	
	vec->data[idx] = val;
	++vec->sz;
	return (0);
}

int		vec_extract(vec_t *vec, uint64_t idx, void **val) {
	if (idx >= vec->sz)
		return (-1);
	if (val)
		*val = vec->data[idx];
	for (; idx + 1 < vec->sz; ++idx)
		vec->data[idx] = vec->data[idx + 1];
	--vec->sz;
	return (0);
}

int		vec_push_back(vec_t *vec, void *val) {
	return (vec_insert(vec, vec->sz, val));
}

int		vec_push_front(vec_t *vec, void *val) {
	return (vec_insert(vec, 0, val));
}

int		vec_pop_back(vec_t *vec, void **val) {
	return (vec_extract(vec, vec->sz - 1, val));
}

int		vec_pop_front(vec_t *vec, void **val) {
	return (vec_extract(vec, 0, val));
}

/*
int		main(void) {
	vec_t		*vec = vec_new(1);
	uint64_t	val;

	vec_push_front(vec, (void *)0x1);
	printf("vec_push_front(0x1)\n");
	vec_print(vec);
	printf("\n");
	vec_push_front(vec, (void *)0x0);
	printf("vec_push_front(0x0)\n");
	vec_print(vec);
	printf("\n");
	vec_push_back(vec, (void *)0x3);
	printf("vec_push_back(0x3)\n");
	vec_print(vec);
	printf("\n");
	vec_push_back(vec, (void *)0x4);
	printf("vec_push_back(0x4)\n");
	vec_print(vec);
	printf("\n");
	vec_insert(vec, 2, (void *)0x42);
	printf("vec_insert(2, 0x42)\n");
	vec_print(vec);
	printf("\n");
	vec_get(vec, 2, (void **)&val);
	printf("get(2) -> 0x%lx\n", val);
	vec_print(vec);
	printf("\n");
	vec_set(vec, 2, (void *)0x2);
	printf("set(2, 0x2)\n");
	vec_print(vec);
	printf("\n");
	vec_extract(vec, 2, (void **)&val);
	printf("extract(2) -> 0x%lx\n", val);
	vec_print(vec);
	printf("\n");
	vec_pop_front(vec, (void **)&val);
	printf("pop_front() -> 0x%lx\n", val);
	vec_print(vec);
	printf("\n");
	vec_pop_front(vec, (void **)&val);
	printf("pop_front() -> 0x%lx\n", val);
	vec_print(vec);
	printf("\n");
	vec_pop_back(vec, (void **)&val);
	printf("pop_back() -> 0x%lx\n", val);
	vec_print(vec);
	printf("\n");
	vec_pop_back(vec, (void **)&val);
	printf("pop_back() -> 0x%lx\n", val);
	vec_print(vec);
	printf("\n");
	vec_free(vec);
	return (0);
}
*/

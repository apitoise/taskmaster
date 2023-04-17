/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:30:38 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/15 23:02:22 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dict.h"

dict_t		*dict_new(uint64_t cap) {
	dict_t		*new;

	if (!(new = malloc(sizeof(dict_t))))
		return (NULL);
	if (!(new->keys = vec_new(cap))) {
		free(new);
		return (NULL);
	}
	if (!(new->values = vec_new(cap))) {
		vec_free(new->keys);
		free(new);
		return (NULL);
	}
	return (new);
}

void		dict_print(dict_t *dict) {
	uint64_t		i;

	printf("{");
	for (i = 0; i < dict->keys->sz; ++i) {
		printf("%s%s: 0x%lx%s",
			!i ? " " : "",
			(char *)dict->keys->data[i],
			(uint64_t)dict->values->data[i],
			i < dict->keys->sz - 1
			? ", " : " ");
	}
	printf("}");
}

void		dict_free(dict_t *dict) {
	vec_free(dict->keys);
	vec_free(dict->values);
	free(dict);
}

static int	get_key_idx(dict_t *dict, char *key, uint64_t *idx) {
	uint64_t		i;

	for (i = 0; i < dict->keys->sz; ++i) {
		if (!strcmp(key, dict->keys->data[i])) {
			*idx = i;
			return (0);
		}
	}
	return (-1);
}

int			dict_get(dict_t *dict, char *key, void **val) {
	uint64_t		i;

	if (get_key_idx(dict, key, &i))
		return (-1);
	if (val)
		*val = dict->values->data[i];
	return (0);
}

int			dict_set(dict_t *dict, char *key, void *val) {
	uint64_t		i;

	if (!get_key_idx(dict, key, &i)) {
		dict->values->data[i] = val;
		return (0);
	}
	if (vec_push_back(dict->keys, key))
		return (-1);
	if (vec_push_back(dict->values, val)) {
		vec_pop_back(dict->keys, NULL);
		return (-1);
	}
	return (0);
}

int			dict_unset(dict_t *dict, char *key) {
	uint64_t		i;

	if (get_key_idx(dict, key, &i))
		return (-1);
	vec_extract(dict->keys, i, NULL);
	vec_extract(dict->values, i, NULL);
	return (0);
}

/*
int			main(void) {
	dict_t		*dic = dict_new(1);
	void		*val;

	dict_set(dic, "bonjour", (void *)0x42);
	dict_set(dic, "aurevoir", (void *)0xaa);
	dict_set(dic, "salut", (void *)0xff);
	dict_set(dic, "lol", (void *)0x24);
	dict_print(dic);
	printf("\n");
	dict_set(dic, "lol", (void *)0x32);
	dict_print(dic);
	printf("\n");
	dict_unset(dic, "salut");
	dict_print(dic);
	printf("\n");
	dict_get(dic, "aurevoir", &val);
	printf("\"aurevoir\" == 0x%lx\n", (uint64_t)val);
	dict_free(dic);
	return (0);
}
*/

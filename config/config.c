/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 22:16:40 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/23 09:37:17 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

static node_getter_t	sel_node_getter(conf_map_t *map);

static int		is_ign(char c) {
	return (isspace(c) || c == ',');
}

static int		is_spe(char c) {
	return (c == '_' || c == '-');
}

static void		skip_ign(conf_map_t *map) {
	for (; map->idx < map->sz
		&& is_ign(map->data[map->idx]);
		++map->idx);
}

static void		rec_free(node_t *node) {
	uint64_t		i;
	vec_t			*vec;
	dict_t			*dic;

	switch (node->type) {
		case DT_VEC:
			vec = node->data;
			for (i = 0; i < vec->sz; ++i)
				rec_free(vec->data[i]);
			vec_free(vec);
			break;
		case DT_DIC:
			dic = node->data;
			for (i = 0; i < dic->keys->sz; ++i)
				rec_free(dic->values->data[i]);
			dict_free(dic);
			break;
		default:
			;
	}
	free(node);
}

static node_t	*get_str_node(conf_map_t *map) {
	char		*str;
	node_t		*node;

	if (++map->idx >= map->sz)
		return (NULL);
	str = &map->data[map->idx];
	for (; map->idx < map->sz
		&& (map->data[map->idx] != '"'
		|| map->data[map->idx - 1] == '\\');
		++map->idx);
	if (map->idx == map->sz)
		return (NULL);
	map->data[map->idx++] = '\0';
	if (!(node = malloc(sizeof(node_t))))
		return (NULL);
	node->data = str;
	node->type = DT_STR;
	return (node);
}

static node_t	*get_unb_node(conf_map_t *map) {
	uint64_t		nb = 0, base = 10;
	node_t			*node;

	if (map->data[map->idx] == '0') {
		++map->idx;
		base = 8;
	}
	for (; map->idx < map->sz; ++map->idx) {
		if (!isdigit(map->data[map->idx]))
			break;
		nb *= base;
		nb += map->data[map->idx] - '0';
	}
	if (!(node = malloc(sizeof(node_t))))
		return (NULL);
	node->data = (void *)nb;
	node->type = DT_UNB;
	return (node);
}

static node_t	*get_lst_node(conf_map_t *map) {
	node_getter_t	node_fn;
	vec_t			*vec;
	node_t			*node, *sub_node;

	if (map->idx++ == map->sz)
		return (NULL);
	if (!(node = malloc(sizeof(node_t))))
		return (NULL);
	if (!(vec = vec_new(DEF_SZ))) {
		free(node);
		return (NULL);
	}
	node->data = vec;
	node->type = DT_VEC;
	skip_ign(map);
	while (map->idx < map->sz) {
		if (map->data[map->idx] == ']') {
			++map->idx;
			return (node);
		}
		if (!(node_fn = sel_node_getter(map))
			|| !(sub_node = node_fn(map)))
			break;
		if (vec_push_back(vec, sub_node)) {
			rec_free(sub_node);
			break;
		}
		skip_ign(map);
	}
	rec_free(node);
	return (NULL);
}

static char		*get_label(conf_map_t *map) {
	char		*label = &map->data[map->idx];

	for (; map->idx < map->sz
		&& map->data[map->idx] != ':';
		++map->idx)
		if (!isalpha(map->data[map->idx])
			&& !isdigit(map->data[map->idx])
			&& !is_spe(map->data[map->idx]))
			return (NULL);
	if (map->idx == map->sz
		|| label == &map->data[map->idx])
		return (NULL);
	map->data[map->idx++] = '\0';
	return (label);
}

static node_t	*get_dict_node(conf_map_t *map) {
	node_getter_t	node_fn;
	dict_t			*dic;
	node_t			*node, *sub_node;
	char			*key;

	if (map->idx++ == map->sz)
		return (NULL);
	if (!(node = malloc(sizeof(node_t))))
		return (NULL);
	if (!(dic = dict_new(DEF_SZ))) {
		free(node);
		return (NULL);
	}
	node->data = dic;
	node->type = DT_DIC;
	skip_ign(map);
	while (map->idx < map->sz) {
		if (map->data[map->idx] == '}') {
			++map->idx;
			return (node);
		}
		if (!(key = get_label(map))
			|| !dict_get(dic, key, NULL)
			|| !(node_fn = sel_node_getter(map))
			|| !(sub_node = node_fn(map)))
			break;
		if (dict_set(dic, key, sub_node)) {
			rec_free(sub_node);
			break;
		}
		skip_ign(map);
	}
	rec_free(node);
	return (NULL);
}

static node_getter_t	sel_node_getter(conf_map_t *map) {
	skip_ign(map);
	switch (map->data[map->idx]) {
		case '[':
			return (get_lst_node);
		case '{':
			return (get_dict_node);
		case '"':
			return (get_str_node);
		default:
			if (isdigit(map->data[map->idx]))
				return (get_unb_node);
	}
	return (NULL);
}

static void		rec_print(node_t *node, uint64_t indent) {
	uint64_t		i, j;
	vec_t			*vec;
	dict_t			*dic;

	dic = node->data;
	switch (node->type) {
		case DT_STR:
			printf("\"%s\"", (char *)node->data);
			break;
		case DT_VEC:
			printf("[ ");
			vec = node->data;
			for (i = 0; i < vec->sz; ++i) {
				rec_print(vec->data[i], indent);
				printf("%s", i + 1 < vec->sz ? ", " : " ");
			}
			printf("]");
			break;
		case DT_DIC:
			printf("{\n");
			++indent;
			dic = node->data;
			for (i = 0; i < dic->keys->sz; ++i) {
				for (j = 0; j < indent; ++j)
					printf("    ");
				printf("%s: ", (char *)dic->keys->data[i]);
				rec_print(dic->values->data[i], indent);
				printf("\n");
			}
			for (; indent && indent - 1; --indent)
				printf("    ");
			printf("}");
			break;
		case DT_UNB:
#ifdef __APPLE__
			printf("%llu", (uint64_t)node->data);
#else
			printf("%lu", (uint64_t)node->data);
#endif // __APPLE__
	}
}

void	config_print(conf_t *conf) {
	rec_print(conf->root, 0);
	printf("\n");
}

void	config_free(conf_t *conf) {
	if (conf->map.data)
		munmap(conf->map.data, conf->map.sz);
	if (conf->root)
		rec_free(conf->root);
	free(conf);
}

conf_t	*config_new(char *path) {
	int				fd;
	struct stat		stat;
	node_getter_t	node_fn;
	conf_t			*new = NULL;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (NULL);
	if (fstat(fd, &stat) < 0
		|| !(new = malloc(sizeof(conf_t)))) {
		close(fd);
		return (NULL);
	}
	bzero(new, sizeof(conf_t));
	if ((new->map.data = mmap(NULL, stat.st_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))
		== MAP_FAILED) {
		close(fd);
		free(new);
		return (NULL);
	}
	close(fd);
	new->map.sz = stat.st_size;
	new->map.idx = 0;
	if (!(node_fn = sel_node_getter(&new->map))
		|| !(new->root = node_fn(&new->map))) {
		config_free(new);
		return (NULL);
	}
	skip_ign(&new->map);
	if (new->map.idx != new->map.sz) {
		config_free(new);
		return (NULL);
	}
	return (new);
}

/*
int		main(int argc, char **argv) {
	conf_t		conf;
//	dict_t		*dic;
//	uint64_t	i;

	if (argc != 2) {
		fprintf(stderr, "Error: need a config file as argument\n");
		return (1);
	}
	if (config_init(&conf, argv[1])) {
		fprintf(stderr, "Error: can't load config file\n");
		return (2);
	}
	config_print(&conf);
//	if (conf.root->type == DT_DIC) {
//		dic = conf.root->data;
//		for (i = 0; i < dic->keys->sz; ++i)
//			printf("%s\n", (char *)dic->keys->data[i]);
//	}
	config_free(&conf);
	return (0);
}
*/

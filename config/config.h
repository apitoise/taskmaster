/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 22:20:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/17 18:36:41 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
#define CONFIG_H

#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../dict/dict.h"

#define DEF_SZ	16

typedef enum		data_type_e {
	DT_STR,
	DT_VEC,
	DT_DIC,
	DT_UNB,
}					data_type_t;

typedef struct		node_s {
	void			*data;
	data_type_t		type;
}					node_t;

typedef struct		conf_map_s {
	char			*data;
	uint64_t		sz;
	uint64_t		idx;
}					conf_map_t;

typedef struct		conf_s {
	conf_map_t		map;
	node_t			*root;
}					conf_t;

typedef node_t		*(*node_getter_t)(conf_map_t *conf);

int		config_init(conf_t *conf, char *path);

#endif // CONFIG_H

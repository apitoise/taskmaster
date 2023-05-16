#ifndef GLOB_H
#define GLOB_H

#include "prog_dic.h"

typedef struct		glob_s {
	prog_dic_t		*prog_dic;
	conf_t			*config;
	prompt_t		*prompt;
	char			*config_path;
	int				stop;
}					glob_t;

extern glob_t		glob;

#endif // GLOB_H

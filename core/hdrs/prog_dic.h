/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog_dic.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 14:07:14 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/25 09:22:29 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROG_DIC_H
#define PROG_DIC_H

#include "prog.h"

typedef dict_t		prog_dic_t;

prog_dic_t		*prog_dic_new(conf_t *conf);
void			prog_dic_free(prog_dic_t *prog_dic);
int				prog_dic_run(prog_dic_t *prog_dic, char *name);
int				prog_dic_update(prog_dic_t *prog_dic);
int				prog_dic_kill(prog_dic_t *prog_dic, int signal);
int				prog_dic_status(prog_dic_t *prog_dic);

#endif //PROG_DIC_H

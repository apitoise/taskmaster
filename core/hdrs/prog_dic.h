/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog_dic.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 14:07:14 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 12:11:43 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROG_DIC_H
#define PROG_DIC_H

#include "prog.h"

typedef dict_t		prog_dic_t;

prog_dic_t		*prog_dic_new(conf_t *conf);
void			prog_dic_free(prog_dic_t *prog_dic);
int				prog_dic_proc_create(prog_dic_t *prog_dic);
int				prog_dic_status(prog_dic_t *prog_dic);
void			prog_dic_kill(prog_dic_t *prog_dic);
prog_dic_t		*prog_dic_reload(prog_dic_t *prog_dic, conf_t *conf);

#endif //PROG_DIC_H

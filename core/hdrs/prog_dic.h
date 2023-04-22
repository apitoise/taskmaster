/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog_dic.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 14:07:14 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/22 16:48:35 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROG_DIC_H
#define PROG_DIC_H

#include "prog.h"

typedef dict_t		prog_dic_t;

prog_dic_t			*prog_dic_new(conf_t *conf);
void				prog_dic_free(prog_dic_t *prog_dic);
int					prog_dic_run(prog_dic_t *prog_dic, char *name);

#endif //PROG_DIC_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:04:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/05 12:09:02 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
#define CLEAN_H

#include "glob.h"
#include "log.h"

void	clean_glob(void);
void	clean_exit(char *error, int ret);

#endif // CLEAN_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:04:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/04/26 12:49:40 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
#define CLEAN_H

#include "glob.h"

void	clean_exit(char *error, int ret);
void	clean_exit_child(char *error, int ret);

#endif // CLEAN_H

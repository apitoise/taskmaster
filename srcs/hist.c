/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:02:35 by herrfalco         #+#    #+#             */
/*   Updated: 2023/03/27 12:05:46 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/hist.h"

cmd_t	*hist_get(hist_t *hist, uint64_t idx) {
	return (idx < hist->sz
		? &hist->lst[(hist->head + (hist->sz - idx - 1)) % MAX_SZ]
		: NULL);
}

void	hist_save(hist_t *hist, cmd_t *cmd) {
	cmd_cpy(&hist->lst[(hist->head + hist->sz) % MAX_SZ], cmd);
	if (hist->sz == MAX_SZ)
		++hist->head;
	else
		++hist->sz;
}

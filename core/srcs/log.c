/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/05/23 10:32:58 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/includes.h"

static char		*log_time(void) {
	time_t		tm;
	char		*str;
	uint64_t	i;

	time(&tm);
	str = ctime(&tm);
	for (i = 0; str[i] && str[i] != '\n'; ++i);
	str[i] = '\0';
	return (str);

}

/*
int		main(void) {
	printf("[%s]\n", log_time());
	return (0);
}
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 17:39:18 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 22:07:13 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

static char	*gnl_ret(char *buf)
{
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 65)
		return ("up");
	if (buf[0] == 27 && buf[1] == 0 && buf[2] == 0)
		return ("echap");
	if (buf[0] == 10)
		return ("enter");
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 66)
		return ("down");
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 67)
		return ("right");
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 68)
		return ("left");
	if (buf[0] == 32 && buf[1] == 0 && buf[2] == 0)
		return ("space");
	if (buf[0] == 127 || buf[0] == 72)
		return ("del");
	if (buf[0] == '+')
		return ("s_all");
	if (buf[0] == '-')
		return ("d_all");
	return (NULL);
}

char		*gnl(void)
{
	char	*line;
	char	buf[3];

	line = NULL;
	ft_put("ks");
	read(g_data->ttyfd, buf, 3);
	ft_put("ke");
	if (!g_data->cursor->error)
		return (gnl_ret(buf));
	else
	{
		if (buf[0] == 27 && buf[1] == 0 && buf[2] == 0)
			return ("echap");
		return (gnl());
	}
	return (NULL);
}

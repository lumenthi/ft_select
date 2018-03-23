/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 13:20:29 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/23 13:33:18 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

int		my_outc(int c)
{
	write(g_data->ttyfd, &c, 1);
	return (1);
}

void	ft_put(char *str)
{
	tputs(tgetstr(str, NULL), g_data->ttyfd, my_outc);
}

void	get_winsize(void)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	g_data->w_row = w.ws_row;
	g_data->w_col = w.ws_col;
}

void	move_cursor(int x, int y)
{
	tputs(tgoto(tgetstr("cm", NULL), x, y), g_data->ttyfd, my_outc);
	g_data->cursor->x = x;
	g_data->cursor->y = y;
}

int		get_current_max(int s, t_elem **elems)
{
	int	i;
	int	current_max;

	i = 0;
	current_max = 0;
	while (elems[s + i])
	{
		if (elems[s + i]->len > current_max)
			current_max = elems[s + i]->len;
		i++;
	}
	return (current_max);
}

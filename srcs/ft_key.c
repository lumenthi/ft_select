/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 14:20:17 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 17:57:12 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void		ft_select(t_elem **elems)
{
	if (elems[g_data->current]->select == 0)
	{
		cursor_selected(elems[g_data->current]);
		elems[g_data->current]->select = 1;
	}
	else
	{
		cursor_on(elems[g_data->current]);
		elems[g_data->current]->select = 0;
	}
	ft_move('d', elems);
}

static void	ft_end(t_elem **elems, int pos)
{
	int	x;
	int	y;

	x = g_data->cursor->x;
	y = g_data->cursor->y;
	ft_put("cl");
	display_elems(g_data->elems, 0, 0);
	elems[0]->select == 0 ? nothing(elems[0]) : selected(elems[0]);
	g_data->current = pos;
	move_cursor(x, y);
	if (elems[pos]->select == 1)
		cursor_selected(elems[pos]);
	else
		cursor_on(elems[pos]);
}

void		ft_delete(t_elem **elems)
{
	int	bu;
	int	pos;

	bu = g_data->current;
	pos = bu;
	if (elem_size(elems) == 0)
		signal_handler(SIGKILL);
	free(elems[bu]->name);
	free(elems[bu]);
	while (elems[bu + 1])
	{
		elems[bu] = elems[bu + 1];
		bu++;
	}
	if (g_data->current == elem_size(elems))
	{
		ft_move('u', elems);
		pos--;
	}
	elems[bu] = NULL;
	ft_end(elems, pos);
}

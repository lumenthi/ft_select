/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 16:21:44 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 16:35:40 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

static void	ft_display(t_elem **elems, int i, int s, int x)
{
	move_cursor(x, g_data->cursor->y);
	if (s + i == 0)
	{
		if (elems[s + i]->select == 0)
			cursor_on(elems[s + i]);
		else
			cursor_selected(elems[s + i]);
	}
	else
	{
		if (elems[s + i]->select == 0)
			nothing(elems[s + i]);
		else
			selected(elems[s + i]);
	}
	g_data->cursor->y++;
}

static void	ft_repeat(t_elem **elems, int *s, int *x, int *current_max)
{
	*s = *s + g_data->w_row;
	*x = *x + g_data->max_spaces;
	*current_max = get_current_max(*s, elems);
	if (*current_max + *x > g_data->w_col)
	{
		ft_put("cl");
		ft_putstr_fd(RED, g_data->ttyfd);
		ft_putendl_fd("Screen is too small", g_data->ttyfd);
		ft_putstr_fd(BLANK, g_data->ttyfd);
		g_data->cursor->error = 1;
	}
	else if (*current_max + *x <= g_data->w_col)
		display_elems(elems, *s, *x);
}

void		display_elems(t_elem **elems, int s, int x)
{
	int	i;
	int	current_max;

	i = 0;
	current_max = 0;
	move_cursor(0, 0);
	g_data->current = 0;
	g_data->cursor->error = 0;
	while (elems[s + i])
	{
		if (i < g_data->w_row)
			ft_display(elems, i, s, x);
		else
		{
			ft_repeat(elems, &s, &x, &current_max);
			break ;
		}
		i++;
	}
	move_cursor(0, 0);
}

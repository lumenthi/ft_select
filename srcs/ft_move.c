/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_move.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 10:58:41 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 11:27:55 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

static void	ft_right(int col, int size)
{
	int	norm;

	if (g_data->cursor->x == col * g_data->max_spaces)
	{
		move_cursor(0, g_data->cursor->y);
		g_data->current = g_data->current - g_data->w_row * col;
	}
	else
	{
		if (g_data->current + g_data->w_row > size)
		{
			move_cursor(0, g_data->cursor->y);
			g_data->current = g_data->current - g_data->w_row * (col - 1);
		}
		else
		{
			norm = g_data->cursor->x + g_data->max_spaces;
			move_cursor(norm, g_data->cursor->y);
			g_data->current = g_data->current + g_data->w_row;
		}
	}
}

static void	ft_left(int col, int size)
{
	if (g_data->cursor->x == 0)
	{
		if (g_data->current + g_data->w_row * col > size)
		{
			g_data->current = g_data->current + g_data->w_row * (col - 1);
			move_cursor((col - 1) * g_data->max_spaces, g_data->cursor->y);
		}
		else
		{
			g_data->current = g_data->current + g_data->w_row * col;
			move_cursor(col * g_data->max_spaces, g_data->cursor->y);
		}
	}
	else
	{
		g_data->current = g_data->current - g_data->w_row;
		move_cursor(g_data->cursor->x - g_data->max_spaces, g_data->cursor->y);
	}
}

static void	ft_down(int size)
{
	if (g_data->current == size)
	{
		move_cursor(0, 0);
		g_data->current = 0;
	}
	else
	{
		g_data->current++;
		if (g_data->cursor->y == g_data->w_row - 1)
			move_cursor(g_data->cursor->x + g_data->max_spaces, 0);
		else
			move_cursor(g_data->cursor->x, g_data->cursor->y + 1);
	}
}

static void	ft_up(int size, int col)
{
	int	norm;
	int	last;

	last = get_last(size);
	if (g_data->current == 0)
	{
		g_data->current = size;
		move_cursor(col * g_data->max_spaces, last);
	}
	else
	{
		norm = g_data->cursor->x - g_data->max_spaces;
		g_data->current--;
		if (g_data->cursor->y == 0)
			move_cursor(norm, g_data->w_row - 1);
		else
			move_cursor(g_data->cursor->x, g_data->cursor->y - 1);
	}
}

void		ft_move(char str, t_elem **elems)
{
	int	col;
	int	size;

	size = elem_size(elems);
	col = max_col(size);
	if (elems[g_data->current]->select == 0)
		nothing(elems[g_data->current]);
	else
		selected(elems[g_data->current]);
	if (str == 'r')
		ft_right(col, size);
	if (str == 'l')
		ft_left(col, size);
	if (str == 'd')
		ft_down(size);
	if (str == 'u')
		ft_up(size, col);
	if (elems[g_data->current]->select == 1)
		cursor_selected(elems[g_data->current]);
	else
		cursor_on(elems[g_data->current]);
}

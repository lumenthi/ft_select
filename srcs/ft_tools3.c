/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 13:54:47 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 17:58:53 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	del_all(t_elem **elems)
{
	int		i;

	i = 0;
	g_data->current = 0;
	move_cursor(0, 0);
	while (elems[i])
	{
		elems[i]->select = 1;
		ft_select(g_data->elems);
		i++;
	}
}

int		elem_size(t_elem **elems)
{
	int i;

	i = 0;
	while (elems[i])
		i++;
	return (i - 1);
}

int		max_col(int size)
{
	int	max;

	if (size < g_data->w_row)
		return (0);
	max = size / g_data->w_row;
	return (max);
}

int		max_row(int size)
{
	size--;
	if (size < g_data->w_row)
		return (size);
	else
		return (size / max_col(size));
}

int		get_last(int size)
{
	int last;

	last = size % g_data->w_row;
	return (last);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 13:46:52 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 13:53:05 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	cursor_on(t_elem *elems)
{
	ft_put("sc");
	ft_put("us");
	ft_putstr_fd(elems->color, g_data->ttyfd);
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_putstr_fd(BLANK, g_data->ttyfd);
	ft_put("ue");
	ft_put("rc");
}

void	selected(t_elem *elems)
{
	ft_put("sc");
	ft_put("mr");
	ft_putstr_fd(elems->color, g_data->ttyfd);
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_putstr_fd(BLANK, g_data->ttyfd);
	ft_put("me");
	ft_put("rc");
}

void	cursor_selected(t_elem *elems)
{
	ft_put("sc");
	ft_put("mr");
	ft_put("us");
	ft_putstr_fd(elems->color, g_data->ttyfd);
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_putstr_fd(BLANK, g_data->ttyfd);
	ft_put("ue");
	ft_put("me");
	ft_put("rc");
}

void	nothing(t_elem *elems)
{
	ft_put("sc");
	ft_putstr_fd(elems->color, g_data->ttyfd);
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_putstr_fd(BLANK, g_data->ttyfd);
	ft_put("rc");
}

void	select_all(t_elem **elems)
{
	int		i;

	i = 0;
	g_data->current = 0;
	move_cursor(0, 0);
	while (elems[i])
	{
		elems[i]->select = 0;
		ft_select(g_data->elems);
		i++;
	}
}

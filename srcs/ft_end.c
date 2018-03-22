/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_end.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 14:29:50 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 17:54:50 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	free_datas(void)
{
	int i;

	i = 0;
	free(g_data->bu);
	free(g_data->cursor);
	while (g_data->elems[i])
	{
		free(g_data->elems[i]->name);
		free(g_data->elems[i]);
		i++;
	}
	free(g_data->elems);
	free(g_data);
}

void	term_reset(void)
{
	ft_put("ve");
	ft_put("te");
	tcsetattr(0, 0, g_data->bu);
	close(g_data->ttyfd);
}

void	return_values(t_elem **elems)
{
	int		i;

	i = 0;
	while (elems[i])
	{
		if (elems[i]->select == 1)
		{
			if (ft_strcmp(elems[i]->name, "no_name") == 0)
				ft_putstr("");
			else
				ft_putstr(elems[i]->name);
			ft_putstr(" ");
		}
		i++;
	}
}

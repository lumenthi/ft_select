/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/23 13:32:40 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

static int	error_main(int argc)
{
	if (argc < 2)
	{
		ft_putendl("ft_select: no file\nusage: ft_select [file ...]");
		return (1);
	}
	if (!term_init())
	{
		ft_putendl("ft_select: init error, exiting.");
		return (1);
	}
	return (0);
}

static int	ft_apply(char *line)
{
	if (line && ft_strcmp(line, "right") == 0)
		ft_move('r', g_data->elems);
	if (line && ft_strcmp(line, "left") == 0)
		ft_move('l', g_data->elems);
	if (line && ft_strcmp(line, "up") == 0)
		ft_move('u', g_data->elems);
	if (line && ft_strcmp(line, "down") == 0)
		ft_move('d', g_data->elems);
	if (line && ft_strcmp(line, "space") == 0)
		ft_select(g_data->elems);
	if (line && ft_strcmp(line, "del") == 0)
		ft_delete(g_data->elems);
	if (line && ft_strcmp(line, "d_all") == 0)
		del_all(g_data->elems);
	if (line && ft_strcmp(line, "s_all") == 0)
		select_all(g_data->elems);
	if (line && ft_strcmp(line, "enter") == 0)
		return (1);
	if (line && ft_strcmp(line, "echap") == 0)
		signal_handler(SIGKILL);
	return (0);
}

int			main(int argc, char **argv)
{
	char	*line;

	line = NULL;
	if (!(data_init()))
		return (0);
	all_signals();
	if (error_main(argc))
		return (0);
	if (!(make_elems(argc, argv)))
		return (0);
	ft_put("cl");
	display_elems(g_data->elems, 0, 0);
	while (1)
	{
		line = gnl();
		if (ft_apply(line))
			break ;
	}
	term_reset();
	return_values(g_data->elems);
	free_datas();
	return (0);
}

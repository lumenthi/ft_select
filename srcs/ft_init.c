/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 11:58:32 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/07 11:15:23 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

static char	*get_color(t_elem *elems)
{
	if (elems->name[ft_strlen(elems->name) - 1] == 'c' &&
		elems->name[ft_strlen(elems->name) - 2] == '.')
		return (BLUE);
	else if (ft_strcmp(elems->name, "") == 0)
	{
		free(elems->name);
		elems->name = ft_strdup("no_name");
		return (BLACK);
	}
	else if (elems->name[ft_strlen(elems->name) - 1] == 'h' &&
		elems->name[ft_strlen(elems->name) - 2] == '.')
		return (GREEN);
	else if (elems->name[ft_strlen(elems->name) - 1] == 'a' &&
		elems->name[ft_strlen(elems->name) - 2] == '.')
		return (YELLOW);
	else if (elems->name[ft_strlen(elems->name) - 1] == 'o' &&
		elems->name[ft_strlen(elems->name) - 2] == '.')
		return (RED);
	else
		return (GRAY);
}

int			make_elems(int argc, char **argv)
{
	int		i;

	i = 0;
	g_data->max_spaces = 0;
	if (!(g_data->elems = (t_elem **)malloc(sizeof(t_elem *) * argc)))
		return (0);
	argv++;
	while (argv[i])
	{
		if (!(g_data->elems[i] = (t_elem *)malloc(sizeof(t_elem))))
			return (0);
		g_data->elems[i]->name = ft_strdup(argv[i]);
		g_data->elems[i]->len = ft_strlen(argv[i]);
		g_data->elems[i]->select = 0;
		g_data->elems[i]->color = get_color(g_data->elems[i]);
		if (g_data->max_spaces < g_data->elems[i]->len)
			g_data->max_spaces = g_data->elems[i]->len;
		i++;
	}
	g_data->max_spaces++;
	g_data->elems[i] = NULL;
	return (1);
}

int			data_init(void)
{
	if (!(g_data = malloc(sizeof(t_properties))))
		return (0);
	if (!(g_data->bu = malloc(sizeof(struct termios))))
		return (0);
	if (!(g_data->cursor = malloc(sizeof(t_cursor))))
		return (0);
	g_data->current = 0;
	return (1);
}

static void	flag_init(struct termios *term)
{
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
}

int			term_init(void)
{
	char			*name_term;
	struct termios	*term;

	if (!(name_term = getenv("TERM")))
		return (0);
	if (!(term = malloc(sizeof(struct termios))))
		return (0);
	if (tgetent(NULL, name_term) <= 0)
		return (0);
	if (tcgetattr(0, g_data->bu) == -1)
		return (0);
	if (tcgetattr(0, term) == -1)
		return (0);
	flag_init(term);
	if ((g_data->ttyfd = open("/dev/tty", O_RDWR)) == -1)
		return (0);
	if (tcsetattr(0, TCSADRAIN, term) == -1)
		return (0);
	ft_put("ti");
	ft_put("vi");
	get_winsize();
	free(term);
	return (1);
}

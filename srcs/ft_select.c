/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/16 14:44:17 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"
#include <fcntl.h>

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

	ioctl(0, TIOCGWINSZ, &w);// ;get winsize
	g_data->w_row = w.ws_row;
	g_data->w_col = w.ws_col;
}

void	term_reset(void)
{
	ft_put("ve"); //make cursor visible again
	ft_put("te"); //fin du programme deplacement curseur
	tcsetattr(0, 0, g_data->bu);
	close(g_data->ttyfd);
	free(g_data->bu);
	free(g_data->cursor);
	free(g_data);
}

/*void	put_buf(char *buf)
{
	ft_putnbr(buf[0]);
	ft_putchar(',');
	ft_putnbr(buf[1]);
	ft_putchar(',');
	ft_putnbr(buf[2]);
}*/

char	*gnl(void)
{
	char	*line;
	char	buf[3];

	line = NULL;
	ft_put("ks"); // transmettre keypad
	read(g_data->ttyfd, buf, 3);
	ft_put("ke"); // fin reception keypad
//	put_buf(buf); // afficher valeur touches
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 65)
		return ("up");
	if ((buf[0] == 27 && buf[1] == 0 && buf[2] == 0) || buf[0] == 'q')
		return ("echap");
	if (buf[0] == 10 && buf[1] == 0 && buf[2] == 0)
		return ("enter");
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 66)
		return ("down");
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 67)
		return ("right");
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 68)
		return ("left");
	if (buf[0] == 32 && buf[1] == 0 && buf[2] == 0)
		return ("space");
	if (buf[0] == 127 && buf[1] == 0 && buf[2] == 0)
		return ("del");
	return (NULL);
}

void	make_elems(int argc, char **argv)
{
	int		i;

	i = 0;
	g_data->max_spaces = 0;
	g_data->elems = (t_elem **)malloc(sizeof(t_elem *) * argc);
	argv++;
	while (argv[i])
	{
		g_data->elems[i] = (t_elem *)malloc(sizeof(t_elem));
		g_data->elems[i]->name = ft_strdup(argv[i]);
		g_data->elems[i]->len = ft_strlen(argv[i]);
		g_data->elems[i]->select = 0;
		if (g_data->max_spaces < g_data->elems[i]->len)
			g_data->max_spaces = g_data->elems[i]->len + 2;
		i++;
	}
	g_data->elems[i] = NULL;
}

void	term_init(void)
{
	char			*name_term;
	struct	termios	*term;

	g_data = malloc(sizeof(t_properties));
	term = malloc(sizeof(struct termios));
	g_data->bu = malloc(sizeof(struct termios));
	g_data->cursor = malloc(sizeof(t_cursor));
	g_data->current = 0;
	name_term = getenv("TERM");
	tgetent(NULL, name_term);
	tcgetattr(0, g_data->bu);
	tcgetattr(0, term);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
	g_data->ttyfd = open("/dev/tty", O_RDWR);
	tcsetattr(0, TCSADRAIN, term);
	ft_put("ti"); //debut programme deplacement curseur
	ft_put("vi"); //invisible cursor
	get_winsize();
	free(term);
}

void	return_values(t_elem **elems)
{
	int		i;

	i = 0;
	while (elems[i])
	{
		if (elems[i]->select == 1)
		{
			ft_putstr(elems[i]->name);
			ft_putstr(" ");
		}
		i++;
	}
}

void	cursor_on(t_elem *elems)
{
	ft_put("sc"); // save cursor position
	ft_put("us"); // underline mode on
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_put("ue"); // underline mode off
	ft_put("rc"); // restore last saved cursor position
}

void	selected(t_elem *elems)
{
	ft_put("sc");
	ft_put("mr"); //highlight mode on
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_put("me"); //highlight mode off
	ft_put("rc");
}

void	cursor_selected(t_elem *elems)
{
	ft_put("sc");
	ft_put("mr");
	ft_put("us");
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_put("ue");
	ft_put("me");
	ft_put("rc");
}

void	nothing(t_elem *elems)
{
	ft_put("sc");
	ft_putstr_fd(elems->name, g_data->ttyfd);
	ft_put("rc");
}

void	move_cursor(int x, int y)
{
	tputs(tgoto(tgetstr("cm", NULL), x, y), g_data->ttyfd, my_outc);
	g_data->cursor->x = x;
	g_data->cursor->y = y;
}

void	display_elems(t_elem **elems, int s, int x)
{
	int	i;
	int	current_max;

	i = 0;
	current_max = 0;
	move_cursor(0, 0);
	g_data->current = 0;
	while (elems[s + i])
	{
		if (elems[s + i]->len > current_max)
			current_max = elems[s + i]->len;
		i++;
	}
	i = 0;
	while (elems[s + i])
	{
		if (i < g_data->w_row)
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
					ft_putstr_fd(elems[s + i]->name, g_data->ttyfd);
				else
					selected(elems[s + i]);
			}
			g_data->cursor->y++;
		}
		else
		{
			s = s + g_data->w_row;
			x = x + g_data->max_spaces;
			if (elems[s + 1] && (x + current_max) <= g_data->w_col)
				display_elems(elems, s, x);
			else if (elems[s + 1] && (x + current_max) >= g_data->w_col)
			{
				ft_put("cl");
				ft_putendl_fd("Screen is too small", g_data->ttyfd);
			}
			break ;
		}
		i++;
	}
	move_cursor(0, 0);
}

void	crashhandler(int sig)
{
	if (sig == 28)
	{
		ft_put("cl");
		get_winsize();
		display_elems(g_data->elems, 0, 0);
	}
	else
	{
		term_reset();
		ft_putendl("crash or exit");
		exit(1);
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
	max = g_data->w_col / g_data->max_spaces;
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

void	ft_move(char str, t_elem **elems)
{
	int	col;
	int	row;
	int	size;
	int	last;

	size = elem_size(elems);
	col = max_col(size);
	row = max_row(size);
	last = get_last(size);
	if (elems[g_data->current]->select == 0)
		nothing(elems[g_data->current]);
	else
		selected(elems[g_data->current]);
	if (str == 'r')
	{
		if (g_data->cursor->x == col * g_data->max_spaces)
		{
			move_cursor(0, g_data->cursor->y);
			g_data->current = g_data->current - g_data->w_row * col;
		}
		else
		{
			move_cursor(g_data->cursor->x + g_data->max_spaces, g_data->cursor->y);
			g_data->current = g_data->current + g_data->w_row;
		}
	}
	if (str == 'l')
	{
		if (g_data->cursor->x == 0)
		{
			g_data->current = g_data->current + g_data->w_row * col;
			move_cursor(col * g_data->max_spaces, g_data->cursor->y);
		}
		else
		{
			g_data->current = g_data->current - g_data->w_row;
			move_cursor(g_data->cursor->x - g_data->max_spaces, g_data->cursor->y);
		}
	}
	if (str == 'd')
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
	if (str == 'u')
	{
		if (g_data->current == 0)
		{
			g_data->current = size;
			move_cursor(col * g_data->max_spaces, last);
		}
		else
		{
			g_data->current--;
			if (g_data->cursor->y == 0)
				move_cursor(g_data->cursor->x - g_data->max_spaces, g_data->w_row - 1);
			else
				move_cursor(g_data->cursor->x, g_data->cursor->y - 1);
		}
	}
	if (elems[g_data->current]->select == 1)
		cursor_selected(elems[g_data->current]);
	else
		cursor_on(elems[g_data->current]);
}

void	ft_select(t_elem **elems, char key)
{
	if (key == 's')
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
	else
	{
		cursor_on(elems[g_data->current]);
		elems[g_data->current]->select = 0;
	}
}

int		main(int argc, char **argv)
{
	char	*line;

	line = NULL;
	signal(SIGINT, crashhandler);
	signal(SIGSEGV, crashhandler);
	signal(SIGWINCH, crashhandler);
	term_init();
	make_elems(argc, argv);
	ft_put("cl");
	display_elems(g_data->elems, 0, 0);
	while (1)
	{
		line = gnl();
		if (line && ft_strcmp(line, "right") == 0)
			ft_move('r', g_data->elems);
		if (line && ft_strcmp(line, "left") == 0)
			ft_move('l', g_data->elems);
		if (line && ft_strcmp(line, "up") == 0)
			ft_move('u', g_data->elems);
		if (line && ft_strcmp(line, "down") == 0)
			ft_move('d', g_data->elems);
		if (line && ft_strcmp(line, "space") == 0)
			ft_select(g_data->elems, 's');
		if (line && ft_strcmp(line, "del") == 0)
			ft_select(g_data->elems, 'd');
		if (line && ft_strcmp(line, "enter") == 0)
			break ;
		if (line && ft_strcmp(line, "echap") == 0)
			crashhandler(0);
	}
	term_reset();
	return_values(g_data->elems);
	return (0);
}

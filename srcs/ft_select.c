/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/16 10:35:31 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"
#include <fcntl.h>

int		my_outc(int c)
{
	write(data->ttyfd, &c, 1);
	return (1);
}

void	ft_put(char *str)
{
	tputs(tgetstr(str, NULL), data->ttyfd, my_outc);
}

void	get_winsize(void)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);// ;get winsize
	data->w_row = w.ws_row;
	data->w_col = w.ws_col;
}

void	term_reset(void)
{
	ft_put("ve"); //make cursor visible again
	ft_put("te"); //fin du programme deplacement curseur
	tcsetattr(0, 0, data->bu);
	close(data->ttyfd);
	printf("w_col = %d\n", data->w_col);
	printf("w_row = %d\n", data->w_row);
	free(data->bu);
	free(data->cursor);
	free(data);
}

/*void	put_buf(char *buf)
{
	ft_putnbr(buf[0]);
	ft_putnbr(buf[1]);
	ft_putnbr(buf[2]);
}*/

char	*gnl(void)
{
	char	*line;
	char	buf[3];
	int		i;

	line = NULL;
	i = 0;
	ft_put("ks"); // transmettre keypad
	read(data->ttyfd, buf, 3);
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
	return (NULL);
}

t_elem	**make_elems(int argc, char **argv)
{
	t_elem **elems;
	int		i;

	i = 0;
	data->max_spaces = 0;
	elems = (t_elem **)malloc(sizeof(t_elem *) * argc);
	argv++;
	while (argv[i])
	{
		elems[i] = (t_elem *)malloc(sizeof(t_elem));
		elems[i]->name = ft_strdup(argv[i]);
		elems[i]->len = ft_strlen(argv[i]);
		elems[i]->select = 0;
		if (data->max_spaces < elems[i]->len)
			data->max_spaces = elems[i]->len + 2;
		i++;
	}
	elems[i] = NULL;
	return (elems);
}

void	term_init(void)
{
	char			*name_term;
	struct	termios	*term;

	data = malloc(sizeof(t_properties));
	term = malloc(sizeof(struct termios));
	data->bu = malloc(sizeof(struct termios));
	data->cursor = malloc(sizeof(t_cursor));
	data->current = 0;
	name_term = getenv("TERM");
	tgetent(NULL, name_term);
	tcgetattr(0, data->bu);
	tcgetattr(0, term);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
	data->ttyfd = open("/dev/tty", O_RDWR);
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
	ft_putstr_fd(elems->name, data->ttyfd);
	ft_put("ue"); // underline mode off
	ft_put("rc"); // restore last saved cursor position
}

void	selected(t_elem *elems)
{
	ft_put("sc");
	ft_put("mr"); //highlight mode on
	ft_putstr_fd(elems->name, data->ttyfd);
	ft_put("me"); //highlight mode off
	ft_put("rc");
}

void	cursor_selected(t_elem *elems)
{
	ft_put("sc");
	ft_put("mr");
	ft_put("us");
	ft_putstr_fd(elems->name, data->ttyfd);
	ft_put("ue");
	ft_put("me");
	ft_put("rc");
}

void	nothing(t_elem *elems)
{
	ft_put("sc");
	ft_putstr_fd(elems->name, data->ttyfd);
	ft_put("rc");
}

void	move_cursor(int x, int y)
{
	tputs(tgoto(tgetstr("cm", NULL), x, y), data->ttyfd, my_outc);
	data->cursor->x = x;
	data->cursor->y = y;
}

void	display_elems(t_elem **elems, int s, int x)
{
	int	i;
	int	current_max;

	i = 0;
	current_max = 0;
	move_cursor(0, 0);
	data->current = 0;
	while (elems[s + i])
	{
		if (elems[s + i]->len > current_max)
			current_max = elems[s + i]->len - 2;
		i++;
	}
	i = 0;
	while (elems[s + i])
	{
		if (i < data->w_row)
		{
			move_cursor(x, data->cursor->y);
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
					ft_putstr_fd(elems[s + i]->name, data->ttyfd);
				else
					selected(elems[s + i]);
			}
			data->cursor->y++;
		}
		else
		{
			s = s + data->w_row;
			x = x + data->max_spaces;
			if (elems[s + 1] && (x + current_max) <= data->w_col)
				display_elems(elems, s, x);
			else if (elems[s + 1] && (x + current_max) >= data->w_col)
			{
				ft_put("cl");
				ft_putendl_fd("Screen is too small", data->ttyfd);
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
		display_elems(elems, 0, 0);
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
	return (i);
}

int		max_col(int size)
{
	int	max;
	if (size < data->w_row)
		return (0);
	max = data->w_col / data->max_spaces;
	return (max);
}

int		max_row(int size)
{
	size--;
	if (size < data->w_row)
		return (size);
	else
		return ((size / max_col(size + 1)) - 1);
}

void	ft_move(char str, t_elem **elems)
{
	if (elems[data->current]->select == 0)
		nothing(elems[data->current]);
	else
		selected(elems[data->current]);
	if (str == 'r')
	{
		if (data->cursor->x == (max_col(elem_size(elems)) -1) * data->max_spaces)
		{
			move_cursor(0, data->cursor->y);
			data->current = data->current - data->w_row * 
				(max_col(elem_size(elems)) - 1);
		}
		else
		{
			move_cursor(data->cursor->x + data->max_spaces, data->cursor->y);
			data->current = data->current + data->w_row;
		}
	}
	if (str == 'l')
	{
		if (data->cursor->x == 0)
		{
			data->current = data->current + data->w_row *
				(max_col(elem_size(elems)) - 1);
			move_cursor((max_col(elem_size(elems)) -1) * data->max_spaces,
				data->cursor->y);
		}
		else
		{
			data->current = data->current - data->w_row;
			move_cursor(data->cursor->x - data->max_spaces, data->cursor->y);
		}
	}
	if (str == 'd')
	{
		if (data->current == elem_size(elems) - 1)
		{
			move_cursor(0, 0);
			data->current = 0;
		}
		else
		{
			data->current++;
			if (data->cursor->y == data->w_row - 1)
				move_cursor(data->cursor->x + data->max_spaces, 0);
			else
				move_cursor(data->cursor->x, data->cursor->y + 1);
		}
	}
	if (str == 'u')
	{
		if (data->current == 0)
		{
			data->current = elem_size(elems) - 1;
			move_cursor((max_col(elem_size(elems)) -1) * data->max_spaces,
				max_row(elem_size(elems)));
		}
		else
		{
			data->current--;
			if (data->cursor->y == 0)
				move_cursor(data->cursor->x - data->max_spaces, data->w_row - 1);
			else
				move_cursor(data->cursor->x, data->cursor->y - 1);
		}
	}
	if (elems[data->current]->select == 1)
		cursor_selected(elems[data->current]);
	else
		cursor_on(elems[data->current]);
}

void	ft_select(t_elem **elems)
{
	if (elems[data->current]->select == 0)
	{
		cursor_selected(elems[data->current]);
		elems[data->current]->select = 1;
		ft_move('d', elems);
	}
	else
	{
		cursor_on(elems[data->current]);
		elems[data->current]->select = 0;
	}
}

int		main(int argc, char **argv)
{
	char	*line;

	line = NULL;
	elems = NULL;
	signal(SIGINT, crashhandler);
	signal(SIGSEGV, crashhandler);
	signal(SIGWINCH, crashhandler);
	term_init();
	elems = make_elems(argc, argv);
	ft_put("cl");
	display_elems(elems, 0, 0);
	while (1)
	{
		line = gnl();
		if (line && ft_strcmp(line, "right") == 0)
			ft_move('r', elems);
		if (line && ft_strcmp(line, "left") == 0)
			ft_move('l', elems);
		if (line && ft_strcmp(line, "up") == 0)
			ft_move('u', elems);
		if (line && ft_strcmp(line, "down") == 0)
			ft_move('d', elems);
		if (line && ft_strcmp(line, "space") == 0)
			ft_select(elems);
		if (line && ft_strcmp(line, "enter") == 0)
			break ;
		if (line && ft_strcmp(line, "echap") == 0)
			crashhandler(0);
	}
	term_reset();
	return_values(elems);
	return (0);
}

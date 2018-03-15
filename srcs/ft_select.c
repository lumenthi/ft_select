/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/15 12:52:37 by lumenthi         ###   ########.fr       */
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

void	term_reset(void)
{
	ft_put("ve"); //make cursor visible again
	ft_put("te"); //fin du programme deplacement curseur
	tcsetattr(0, 0, data->bu);
	close(data->ttyfd);
	free(data->bu);
	free(data->cursor);
	free(data);
}

void	crashhandler(int sig)
{
	(void)sig;
	term_reset();
	ft_putendl("crash or exit");
	exit(1);
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
	elems = (t_elem **)malloc(sizeof(t_elem *) * argc);
	argv++;
	while (argv[i])
	{
		elems[i] = (t_elem *)malloc(sizeof(t_elem));
		elems[i]->name = ft_strdup(argv[i]);
		elems[i]->len = ft_strlen(argv[i]);
		elems[i]->select = 0;
		i++;
	}
	elems[i] = NULL;
	return (elems);
}

void	get_winsize(void)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);// ;get winsize
	data->w_row = w.ws_row;
	data->w_col = w.ws_col;
}

void	term_init(void)
{
	char			*name_term;
	struct	termios	*term;

	data = malloc(sizeof(t_properties));
	term = malloc(sizeof(struct termios));
	data->bu = malloc(sizeof(struct termios));
	data->cursor = malloc(sizeof(t_cursor));
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

void	display_elems(t_elem **elems)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	ft_put("cl"); //clear screen
	move_cursor(0, 0);
	while (elems[i])
	{
		len = len + elems[i]->len;
//		len++;
//		dprintf(data->ttyfd, "len: %d", len);
//		dprintf(data->ttyfd, "max: %d\n", data->w_col);
		if (len > data->w_col)
		{
//			move_cursor(0, data->cursor->y + 1);
			len = 0;
			ft_putstr_fd("\n", data->ttyfd);
		}
		ft_putstr_fd(elems[i]->name, data->ttyfd);
		ft_putstr_fd(" ", data->ttyfd);
		i++;
	}
	move_cursor(0, 0);
	cursor_on(elems[0]);
}

void	ft_move(char str, t_elem **elems, int *i)
{
	int		len;
	
	if (elems[*i]->select == 0)
		nothing(elems[*i]);
	else
		selected(elems[*i]);
	if (str == 'r')
	{
		len = elems[*i]->len + 1; // remplacer + 1 par espace entre mots;
		move_cursor((data->cursor->x + len), data->cursor->y);
		(*i)++;
	}
	if (str == 'l')
	{
		ft_put("rc");
		(*i)--;
		len = elems[*i]->len + 1; // remplacer + 1 par espace entre mots;
		while (len--)
			move_cursor((data->cursor->x - 1), data->cursor->y);
	}
	if (elems[*i]->select == 1)
		cursor_selected(elems[*i]);
	else
		cursor_on(elems[*i]);
}

void	ft_select(t_elem *elems)
{
	if (elems->select == 0)
	{
		cursor_selected(elems);
		elems->select = 1;
	}
	else
	{
		cursor_on(elems);
		elems->select = 0;
	}
}

int		main(int argc, char **argv)
{
	char	*line;
	t_elem	**elems;
	int		i;

	i = 0;
	line = NULL;
	elems = NULL;
	signal(SIGINT, crashhandler);
	signal(SIGSEGV, crashhandler);
	term_init();
	elems = make_elems(argc, argv);
	display_elems(elems);
	while (1)
	{
		line = gnl();
		if (line && ft_strcmp(line, "right") == 0)
			ft_move('r', elems, &i);
		if (line && ft_strcmp(line, "left") == 0)
			ft_move('l', elems, &i);
		if (line && ft_strcmp(line, "space") == 0)
			ft_select(elems[i]);
		if (line && ft_strcmp(line, "enter") == 0)
			break ;
		if (line && ft_strcmp(line, "echap") == 0)
			crashhandler(0);
	}
	term_reset();
	return_values(elems);
	return (0);
}

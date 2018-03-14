/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/14 22:14:20 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"
#include <fcntl.h>

int		my_outc(int c)
{
	write(ttyfd, &c, 1);
	return (1);
}

void	ft_put(char *str)
{
	tputs(tgetstr(str, NULL), ttyfd, my_outc);
}

void	term_reset(void)
{
	ft_put("ve");
	ft_put("te");
	tcsetattr(0, 0, bu);
	close(ttyfd);
	free(bu);
}

void	crashhandler(int sig)
{
	(void)sig;
	ft_putendl("crash");
	term_reset();
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
	read(ttyfd, buf, 3);
	ft_put("ke"); // fin reception keypad
//	put_buf(buf); // afficher valeur touches
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 65)
		return ("up");
	if ((buf[0] == 27 && buf[1] == 0 && buf[2] == 0) || buf[0] == 'q')
		return ("echap");
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

void	term_init(void)
{
	char			*name_term;
	struct	termios	*term;

	term = malloc(sizeof(struct termios));
	bu = malloc(sizeof(struct termios));
	name_term = getenv("TERM");
	tgetent(NULL, name_term);
	tcgetattr(0, bu);
	tcgetattr(0, term);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
	ttyfd = open("/dev/tty", O_RDWR);
	tcsetattr(0, TCSADRAIN, term);
	ft_put("ti"); //debut programme deplacement curseur
	ft_put("vi"); //invisible cursor
	ft_put("cl"); //clear
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

/*void	cursor_on(char *str)
{
	ft_put("ce"); // clear from cursor to end line, better use 'ec' for n char

}

void	selected(char *str)
{

}

void	nothing(char *str)
{

}

void	cursor_selected(char *str)
{

}
*/
int		main(int argc, char **argv)
{
	char	*line = NULL;
	t_elem	**elems = NULL;
	int		i = 0;

	term_init();
	elems = make_elems(argc, argv);
	signal(SIGINT, crashhandler);
	signal(SIGSEGV, crashhandler);
	(void)argc;
	argv++;
	while (elems[i])
	{
		ft_putstr_fd(elems[i]->name, ttyfd);
		ft_putstr_fd("		", ttyfd);
		i++;
	}
	i = 0;
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), ttyfd, my_outc); //cursor 0;0
	while (1)
	{
		line = gnl();
		if (line && ft_strcmp(line, "up") == 0)
		{
			ft_put("cr"); //move cursor beginning line
			if (elems[i]->select == 0)
			{
				ft_put("ce"); //clear all line cursor is on
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
			}
			else
			{
				ft_put("ce");
				ft_put("mr"); //highlight mode on
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("me"); //highlight mode off
				ft_put("cr");
			}
			i--;
			tputs(tgetstr("up", NULL), ttyfd, my_outc); //move cursor up 1 line
			if (elems[i]->select == 0)
			{
				ft_put("ce");
				ft_put("us"); //underline mode on
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
				ft_put("ue"); //underline mode off
			}
			else
			{
				ft_put("ce");
				ft_put("us");
				ft_put("mr");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("me");
				ft_put("ue");
				ft_put("cr");
			}
		}
		if (line && ft_strcmp(line, "down") == 0)
		{
			ft_put("cr");
			if (elems[i]->select == 0)
			{
				ft_put("ce");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
			}
			else
			{
				ft_put("ce");
				ft_put("mr");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("me");
				ft_put("cr");
			}
			i++;
			tputs(tgetstr("do", NULL), ttyfd, my_outc);
			if (elems[i]->select == 0)
			{
				ft_put("ce");
				ft_put("us");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
				ft_put("ue");
			}
			else
			{
				ft_put("ce");
				ft_put("us");
				ft_put("mr");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("me");
				ft_put("ue");
				ft_put("cr");
			}
		}
		if (line && ft_strcmp(line, "right") == 0)
			tputs(tgetstr("nd", NULL), ttyfd, my_outc);
		if (line && ft_strcmp(line, "left") == 0)
			tputs(tgetstr("le", NULL), ttyfd, my_outc);
		if (line && ft_strcmp(line, "space") == 0)
		{
			if (elems[i]->select == 0)
			{
				tputs(tgetstr("ce", NULL), ttyfd, my_outc);
				tputs(tgetstr("mr", NULL), ttyfd, my_outc);
				ft_put("us");
				ft_putstr_fd(elems[i]->name, ttyfd);
				elems[i]->select = 1;
				ft_put("cr");
				ft_put("ue");
				tputs(tgetstr("me", NULL), ttyfd, my_outc);
			}
			else
			{
				tputs(tgetstr("ce", NULL), ttyfd, my_outc);
				elems[i]->select = 0;
				ft_put("us");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
				ft_put("ue");
			}
		}
		if (line && ft_strcmp(line, "echap") == 0)
			break ;
	}
	term_reset();
	return_values(elems);
	return (0);
}

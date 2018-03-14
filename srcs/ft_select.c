/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/12 17:35:31 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

struct termios *BU = NULL;
int				ttyfd = 0;

int		my_outc(int c)
{
	write(ttyfd, &c, 1);
	return (1);
}

void	stophandler(int sig)
{
	(void)sig;
	ft_putendl("stop");
//	tputs(tgetstr("ve", NULL), ttyfd, my_outc);
	tputs(tgetstr("te", NULL), ttyfd, my_outc);
	tcsetattr(0, 0, BU);
	close(ttyfd);
	signal(SIGTSTP, SIG_DFL);
}

void	crashhandler(int sig)
{
	(void)sig;
	ft_putendl("crash");
	tputs(tgetstr("ve", NULL), ttyfd, my_outc);
	tputs(tgetstr("te", NULL), ttyfd, my_outc);
	tcsetattr(0, 0, BU);
	close(ttyfd);
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
	tputs(tgetstr("ks", NULL), ttyfd, my_outc);
	read(ttyfd, buf, 3);
	tputs(tgetstr("ke", NULL), ttyfd, my_outc);
//	put_buf(buf);
	if (buf[0] == 27 && buf[1] == 79 && buf[2] == 65)
		return ("up");
	if (buf[0] == 27 && buf[1] == 0 && buf[2] == 0)
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

#include <fcntl.h>
void	ft_put(char *str)
{
	tputs(tgetstr(str, NULL), ttyfd, my_outc);
}

int		main(int argc, char **argv)
{
	char	*name_term;
	struct	termios *term = malloc(sizeof(struct termios));
	char	*line = NULL;
	t_elem	**elems = NULL;
	int		i = 0;

	elems = make_elems(argc, argv);
	BU = malloc(sizeof(struct termios));
	name_term = getenv("TERM");
	tgetent(NULL, name_term);
	tcgetattr(0, BU);
	tcgetattr(0, term);
	signal(SIGINT, crashhandler);
	signal(SIGSEGV, crashhandler);
	signal(SIGTSTP, stophandler);
	(void)argc;
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
	ttyfd = open("/dev/tty", O_RDWR);
	tcsetattr(0, TCSADRAIN, term);
	tputs(tgetstr("ti", NULL), ttyfd, my_outc);
	tputs(tgetstr("vi", NULL), ttyfd, my_outc);
	tputs(tgetstr("cl", NULL), ttyfd, my_outc);
//	tputs(tgetstr("db", NULL), ttyfd, my_outc);
	argv++;
//	tputs(tgetstr("us", NULL), ttyfd, my_outc);
	while (elems[i])
	{
		ft_putendl_fd(elems[i]->name, ttyfd);
		i++;
	}
	i = 0;
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), ttyfd, my_outc);
	ft_put("ce");
	ft_put("us");
	ft_putstr_fd(elems[i]->name, ttyfd);
	ft_put("cr");
	ft_put("ue");
//	tputs(tgetstr("ue", NULL), ttyfd, my_outc);
//	tputs(tgetstr("mr", NULL), ttyfd, my_outc);
//	ft_putendl("TEST");
//	tputs(tgetstr("me", NULL), ttyfd, my_outc);
	while (1)
	{
		line = gnl();
		if (line && ft_strcmp(line, "up") == 0)
		{
			ft_put("cr");
			if (elems[i]->select == 0)
			{
				ft_put("ce");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
			}
			i--;
			tputs(tgetstr("up", NULL), ttyfd, my_outc);
			if (elems[i]->select == 0)
			{
				ft_put("ce");
				ft_put("us");
				ft_putstr_fd(elems[i]->name, ttyfd);
				ft_put("cr");
				ft_put("ue");
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
				ft_putstr_fd(elems[i]->name, ttyfd);
				elems[i]->select = 1;
				ft_put("cr");
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
	tputs(tgetstr("ve", NULL), ttyfd, my_outc);
	tputs(tgetstr("te", NULL), ttyfd, my_outc);
	tcsetattr(0, 0, BU);
	close(ttyfd);
	return (0);
}

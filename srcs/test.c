/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 17:50:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/10 12:07:11 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

struct termios *BU = NULL;

void	crashhandler(int sig)
{
	(void)sig;
	ft_putendl("crash");
	tcsetattr(0, TCSANOW, BU);
	exit(1);
}

int		my_outc(int c)
{
	ft_putchar(c);
	return (0);
}

char	*gnl()
{
	char	*line;
	char	buf;
	int		i;
	char	**area = NULL;
	char	*res = tgetstr("le", area);

	line = NULL;
	i = 0;
	buf = 0;
	while ((read(0, &buf, 1)) && buf != '\n' && buf != '\0')
	{
		if (buf != 127)
		{
			line = (char*)ft_realloc(line, i + 2);
			*(line + i) = buf;
			i++;
			ft_putchar(buf);
		}
		if (line && buf == 127 && ft_strcmp(line, "") != 0)
		{
			ft_putstr("\b ");
			tputs(tgoto(res, 0, 0), 1, my_outc);
			line[ft_strlen(line) - 1] = '\0';
			i--;
		}
		if (buf == 'q')
			return ("exit");
	}
	if (i && line)
		*(line + i) = '\0';
	return (line);
}

int		main(int argc, char **argv)
{
	char	*name_term;
	struct	termios *term = malloc(sizeof(struct termios));
	char	*clear;
//	char	*res;
//	char	*kr;
	char	*line;

	BU = malloc(sizeof(struct termios));
	name_term = getenv("TERM");
	tgetent(NULL, name_term);
	tcgetattr(0, BU);
	tcgetattr(0, term);
	signal(SIGINT, crashhandler);
	signal(SIGSEGV, crashhandler);
	term->c_lflag = ~(ICANON | ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
	clear = tgetstr("cl", NULL);
	(void)argc;
	(void)argv;
//	res = tgetstr("cm", NULL);
//	kr = tgetstr("kr", NULL);
	ft_putstr("$> ");
	tcsetattr(0, TCSANOW, term);
	while (1)
	{
		ft_putstr("$> ");
		line = gnl();
		if (!line)
			;
		else if (ft_strcmp(line, "exit") == 0)
			break ;
		else if (ft_strcmp(line, "clear") == 0)
			tputs(clear, 0, my_outc);
	}
	tcsetattr(0, TCSANOW, BU);
	return (0);
}

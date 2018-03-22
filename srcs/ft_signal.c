/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 16:39:05 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 17:58:23 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

static void	signal_cont(void)
{
	term_init();
	signal(SIGTSTP, signal_handler);
	display_elems(g_data->elems, 0, 0);
	ioctl(0, TIOCSTI, "\0");
}

void		signal_handler(int sig)
{
	if (sig == SIGWINCH)
	{
		ft_put("cl");
		get_winsize();
		display_elems(g_data->elems, 0, 0);
	}
	if (sig == SIGTSTP)
	{
		term_reset();
		signal(SIGTSTP, SIG_DFL);
		ioctl(0, TIOCSTI, "\x1A");
	}
	if (sig == SIGCONT)
		signal_cont();
	if (sig == SIGABRT || sig == SIGSTOP || sig == SIGQUIT || sig == SIGKILL ||
		sig == SIGINT)
	{
		term_reset();
		free_datas();
		exit(1);
	}
}

void		all_signals(void)
{
	signal(SIGWINCH, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGSTOP, signal_handler);
	signal(SIGCONT, signal_handler);
	signal(SIGTSTP, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGQUIT, signal_handler);
}

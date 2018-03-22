/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/22 17:54:04 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# define GREEN	"\033[1;32m"
# define BLUE	"\033[1;34m"
# define BLANK	"\033[0m"
# define RED	"\033[1;31m"
# define YELLOW "\033[1;33m"
# define GRAY	"\033[1;37m"
# define BLACK	"\033[1;90m"

# include "../libft/libft.h"
# include <termios.h>
# include <term.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <fcntl.h>

typedef struct		s_cursor
{
	int	x;
	int	y;
	int	error;
}					t_cursor;

typedef struct		s_elem
{
	char	*name;
	int		len;
	int		select;
	char	*color;
}					t_elem;

typedef struct		s_properties
{
	struct termios	*bu;
	int				ttyfd;
	int				w_row;
	int				w_col;
	t_cursor		*cursor;
	int				max_spaces;
	int				current;
	t_elem			**elems;
}					t_properties;

t_properties		*g_data;

void				move_cursor(int x, int y);
int					elem_size(t_elem **elems);
int					max_col(int size);
int					max_row(int size);
int					get_last(int size);
void				ft_move(char str, t_elem **elems);
void				selected(t_elem *elems);
void				cursor_selected(t_elem *elems);
void				nothing(t_elem *elems);
void				cursor_on(t_elem *elems);
void				ft_put(char *str);
void				get_winsize(void);
void				make_elems(int argc, char **argv);
void				data_init(void);
int					term_init(void);
int					my_outc(int c);
int					get_current_max(int s, t_elem **elems);
void				select_all(t_elem **elems);
void				del_all(t_elem **elems);
void				ft_select(t_elem **elems);
void				ft_delete(t_elem **elems);
void				signal_handler(int sig);
void				display_elems(t_elem **elems, int s, int x);
void				free_datas(void);
void				term_reset(void);
void				return_values(t_elem **elems);
void				all_signals(void);
char				*gnl(void);

#endif

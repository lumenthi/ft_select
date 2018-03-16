/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/16 23:34:12 by lumenthi         ###   ########.fr       */
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

typedef struct		s_cursor
{
	int	x;
	int	y;
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

t_properties	*g_data;

# include "../libft/libft.h"
# include <termios.h>
# include <term.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>

#endif

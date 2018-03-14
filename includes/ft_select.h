/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/12 16:58:29 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

#define GREEN	"\033[1;32m"
#define BLUE	"\033[4;34m"
#define BLANK	"\033[0m"
#define RED		"\033[1;31m"

typedef struct		s_elem
{
	char	*name;
	int		len;
	int		select;
}					t_elem;

# include "../libft/libft.h"
# include <termios.h>
# include <term.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

#endif
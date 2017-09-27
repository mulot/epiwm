/*
**  EPIwm
**  Copyright (C) 1999 Julien Mulot, Thomas Poindessous, Stephane Rault
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** window_functions.h
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Mon May  3 20:45:06 1999 Poindessous Thomas
 ** Last update Mon Apr  3 22:34:53 2000 Thomas Poindessous
*/

#ifndef   __WINDOW_FUNCTIONS_H__
#define   __WINDOW_FUNCTIONS_H__

void	win_move(t_win *, int, int);
void	win_resize(t_win *, int, int);
void	win_shade(t_win *, int, int);
void	win_close(t_win *, int, int);
void	win_destroy(t_win *, int, int);
void	win_iconify(t_win *, int, int);

#endif /* __WINDOW_FUNCTIONS_H__ */

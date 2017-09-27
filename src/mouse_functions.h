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
 ** mouse_functions.h
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Sun Apr  4 16:41:35 1999 Poindessous Thomas
 ** Last update Tue Jun 15 22:07:46 1999 Thomas Poindessous
 */

#ifndef   __MOUSE_FUNCTIONS_H__
#define   __MOUSE_FUNCTIONS_H__

void	mouse_motion_notify(t_win *);
void	win_max_width(t_win *);
void	win_max_height(t_win *);
void	win_resize_all(t_win *);
void	win_resize_left(t_win *, int, int);
void	win_resize_right(t_win *, int, int);
void	win_resize_up(t_win *, int, int);
void	win_resize_down(t_win *, int, int);
void	win_resize_up_left(t_win *, int, int);
void	win_resize_up_right(t_win *, int, int);
void	win_resize_down_left(t_win *, int, int);
void	win_resize_down_right(t_win *, int, int);
void	win_opaque_resize(t_win *, int, int, int, int);

#endif /* __MOUSE_FUNCTIONS_H__ */

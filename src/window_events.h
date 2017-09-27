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
 ** window_events.h
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Mon Feb 22 15:34:07 1999 Poindessous Thomas
 ** Last update Fri Sep  3 21:01:15 1999 Thomas Poindessous
 */

#ifndef   __WINDOW_EVENTS_H__
#define   __WINDOW_EVENTS_H__
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void		dest_win();
void		resize_win();
void		configure_win();
void		map_win();
void		unmap_win();
void		prop_not();
void		conf_notify_win();
void		map_req();
void		internal_resize_son(t_win *, int, int, int, int);
void		internal_destroy(t_win *);
void		map_request(Window);
void		internal_resize_win(Window, int, int);
#endif /* __WINDOW_EVENTS_H__ */

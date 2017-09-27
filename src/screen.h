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
 ** screen.h
 ** 
 ** Made by ()
 ** Login   <mulot_j@epita.fr>
 ** 
 ** Started on  Wed Dec 30 01:15:54 1998 
 ** Last update Thu Mar 18 18:45:09 1999 Poindessous Thomas
 */

#ifndef   __SCREEN_H__
#define   __SCREEN_H__

#include <X11/Xlib.h>

typedef	struct		s_screen_info
{
   unsigned long	screen;
   int			depth;
   int			dpy_width;
   int			dpy_height;
   Window		root_win;
}			t_screen_info;

t_screen_info		gl_scr_info;
#endif /* __SCREEN_H__ */



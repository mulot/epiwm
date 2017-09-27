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
** gnome.c
** 
** Made by ()
** Login   <mulot_j@epita.fr>
** 
** Started on  Wed Jun 23 18:27:52 1999 
 ** Last update Mon Oct 23 22:53:20 2000 Julien Mulot
*/


#include "epiwm.h"


#ifdef __GNOME__
void	init_gnome()
{
  /*
     Atom	list[1];
     list[0] = None;
     */     
  gl_gnome.atom = XInternAtom(gl_dpy, "_WIN_SUPPORTING_WM_CHECK", False);
  gl_gnome.win = XCreateSimpleWindow(gl_dpy, gl_scr_info.root_win,
				     -200, -200, 5, 5, 0, 0, 0);
  gl_gnome.val = gl_gnome.win;
  XChangeProperty(gl_dpy, gl_scr_info.root_win, gl_gnome.atom,
		  XA_CARDINAL, 32, PropModeReplace, (unsigned char *)
		  &(gl_gnome.val), 1);
  XChangeProperty(gl_dpy, gl_gnome.win, gl_gnome.atom, XA_CARDINAL, 32,
		  PropModeReplace, (unsigned char *) &(gl_gnome.val), 1);
}
#endif

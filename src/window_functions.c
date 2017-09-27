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
** window_functions.c
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Mon May  3 20:06:30 1999 Poindessous Thomas
 ** Last update Wed Apr  5 01:02:09 2000 Mulot Julien
*/

#include "epiwm.h"

void	win_move(t_win	*ewin,
		 int	x,
		 int	y)
{
  XMoveWindow(gl_dpy, ewin->parent, x, y);
  clientSetState(ewin, NormalState);
  send_configure_notify(ewin);
}

void	win_resize(t_win	*ewin,
		   int		width,
		   int		height)
{
  internal_resize_win(ewin->win, width, height);
}

void	win_shade(t_win	*ewin,
		  int	dummy,
		  int	dummy2)
{
  if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
    internal_unshade(ewin);
  else
    internal_shade(ewin);
  if ((gl_epiwin.titlebar.width))
    draw_titlebar_shape(ewin);
}

void	win_close(t_win	*ewin,
		  int	dummy,
		  int	dummy2)
{
  clientsendprotocol(ewin, EPIWM_WM_DELETE_WINDOW);
}

void	win_destroy(t_win	*ewin,
		    int		dummy,
		    int		dummy2)
{
  XKillClient(gl_dpy, ewin->win);
}

void	win_iconify(t_win	*ewin,
		    int		dummy,
		    int		dummy2)
{
  if (ewin->iconify_title_win == 0)
    mouse_iconify(ewin);
  else
    mouse_uniconify(ewin);
}


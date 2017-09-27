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
** mouse_resize.c
** 
** Made by (Thomas Poindessous)
** Login   <poinde_t@epita.fr>
** 
** Started on  Mon Jun  7 17:45:19 1999 Thomas Poindessous
 ** Last update Wed Apr  5 03:07:31 2000 Mulot Julien
*/

#include "epiwm.h"


void	mouse_resize_max(t_mouse_move_win *list)
{
  if ((!(verify_mouse_vs_button(list->button->x, list->button->y))) || (!list))
    return ;
  if (list->ewin->size & MAX)
    internal_resize_unmax(list->ewin);
  else
    internal_resize_max(list->ewin); 
  if (test_shape(list->ewin->win) || gl_epiwin.titlebar.width_mode)
    {
      gl_event.xmap.window = list->ewin->win;
      map_win();
    }
}

void	mouse_shade(t_mouse_move_win *list)
{  
  if (!list)
    return ;
  if (list->ewin->is_shade == 0)
    list->ewin->is_shade = 1;
  else if (list->ewin->is_shade == 1)
    {
      if ((gl_win_move.bef == gl_win_move.win)
	  && ((list->button->time - gl_win_move.last) < TIME_TWO_CLICKS))
	internal_shade(list->ewin);
      else
	list->ewin->is_shade = 0;
    }
  else if (list->ewin->is_shade == 2)
    list->ewin->is_shade = 3;
  else if (list->ewin->is_shade == 3)
    {
      if ((gl_win_move.bef == gl_win_move.win) &&
	  ((list->button->time - gl_win_move.last) < TIME_TWO_CLICKS))
	internal_unshade(list->ewin);
      else
	list->ewin->is_shade = 2;
    }
  else
    list->ewin->is_shade = 2;
  if ((gl_epiwin.titlebar.width))
    if (list->ewin->is_shade == 2 || list->ewin->is_shade == 0)
      draw_titlebar_shape(list->ewin);
}


void	win_max_width(t_win *ewin)
{
  if (ewin->size & MAX_WIDTH)
    internal_unmax_width(ewin);
  else
    internal_max_width(ewin);
  if (gl_epiwin.titlebar.width)
    draw_titlebar_shape(ewin);
}

void	win_max_height(t_win *ewin)
{
  if (ewin->size & MAX_HEIGHT)
    internal_unmax_height(ewin);
  else
    internal_max_height(ewin);
  if (gl_epiwin.titlebar.width)
    draw_titlebar_shape(ewin);
}

int verify_mouse_vs_button(int x, int y)
{
  if ((x < 0) || (y < 0) || (x > gl_epiwin.btn_width)
      || (y > gl_epiwin.btn_height))
    return (0);
  else
    return (1);
}

void internal_resize_unmax(t_win *ewin)
{    
  if (!ewin)
    return ;
  ewin->size &= ~MAX;
  if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
    {
      if (gl_epiwin.titlebar.position == TOP
	  || gl_epiwin.titlebar.position == BOTTOM)
	{
	  if ((ewin->size & MAX_WIDTH) && (ewin->size & MAX_HEIGHT))
	    return ;
	  if (ewin->size & MAX_WIDTH)	    
	    XMoveWindow(gl_dpy, ewin->parent, 0, ewin->win_y);
	  else if (ewin->size & MAX_HEIGHT)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->win_width -
				2 * ewin->titlebar_border,
				ewin->titlebar_height);
	      XMoveResizeWindow(gl_dpy, ewin->parent, ewin->win_x, 0,
				ewin->win_width,
				ewin->titlebar_height +
				2*ewin->titlebar_border);
	    }
	  else
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->win_width -
				2 * ewin->titlebar_border,
				ewin->titlebar_height);
	      XMoveResizeWindow(gl_dpy, ewin->parent, ewin->win_x,
				ewin->win_y,
				ewin->win_width,
				ewin->titlebar_height +
				2*ewin->titlebar_border);
	    }
	}
      else if (gl_epiwin.titlebar.position == RIGHT
	       || gl_epiwin.titlebar.position == LEFT)
	{
	  if ((ewin->size & MAX_WIDTH) && (ewin->size & MAX_HEIGHT))
	    return ;
	  if (ewin->size & MAX_WIDTH)
	    {
	      
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->titlebar_height,
				ewin->win_height
				- 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent, 0, ewin->win_y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				ewin->win_height);
	    }
	  else if (ewin->size & MAX_HEIGHT)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->titlebar_height,
				gl_scr_info.dpy_height
				- 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent, ewin->win_x, 0,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				gl_scr_info.dpy_height);
	    }
	  else
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->titlebar_height,
				ewin->win_height
				- 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent, ewin->win_x,
				ewin->win_y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				ewin->win_height);
	    }
	}
      internal_replace_button(ewin);
    }
  else
    {
      if ((ewin->size & MAX_WIDTH) && (ewin->size & MAX_HEIGHT))
	return ;
      if (ewin->size & MAX_WIDTH)
	window_group_resize(ewin, 0, ewin->win_y,
			    gl_scr_info.dpy_width,
			    ewin->win_height);
      else if (ewin->size & MAX_HEIGHT)
	window_group_resize(ewin, ewin->win_x, 0,
			    ewin->win_width,
			    gl_scr_info.dpy_height);
      else
	window_group_resize(ewin, ewin->win_x,
			    ewin->win_y,
			    ewin->win_width,
			    ewin->win_height);
    }
  send_configure_notify(ewin);
}

void		internal_resize_max(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  if (!ewin)
    return ;  
  ewin->size |= MAX;
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
	       &width, &height, &dummy, &dummy);
  if (((ewin->size & MAX_WIDTH) || (ewin->size & MAX_HEIGHT)) == 0)
    {
      ewin->win_x = x;
      ewin->win_y = y;
    }
  if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
    {
      if (!(ewin->size & MAX_WIDTH))
	ewin->win_width = ewin->shade_width;
      if (!(ewin->size & MAX_HEIGHT))
	ewin->win_height = ewin->shade_height;
      if (gl_epiwin.titlebar.position == TOP)
	{
	  XResizeWindow(gl_dpy, ewin->titlebar_win,
			gl_scr_info.dpy_width -
			2 * ewin->titlebar_border,
			ewin->titlebar_height);
	  XMoveResizeWindow(gl_dpy, ewin->parent, 0, 0,		 
			    gl_scr_info.dpy_width,
			    ewin->titlebar_height +
			    2*ewin->titlebar_border);
	}
      else if (gl_epiwin.titlebar.position == BOTTOM)
	{
	  XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
			    0, 0, gl_scr_info.dpy_width -
			    2 * ewin->titlebar_border,
			    ewin->titlebar_height);
	  XMoveResizeWindow(gl_dpy, ewin->parent, 0, gl_scr_info.dpy_height
			    - ( ewin->titlebar_height +
				2 * ewin->titlebar_border),
			    gl_scr_info.dpy_width,
			    ewin->titlebar_height +
			    2*ewin->titlebar_border);
	}
      else if (gl_epiwin.titlebar.position == LEFT)
	{
	  XResizeWindow(gl_dpy, ewin->titlebar_win,
			ewin->titlebar_height,
			gl_scr_info.dpy_height -
			2 * ewin->titlebar_border);
	  XMoveResizeWindow(gl_dpy, ewin->parent, 0, 0,		 
			    ewin->titlebar_height +
			    2*ewin->titlebar_border,
			    gl_scr_info.dpy_height);
	}
      else if (gl_epiwin.titlebar.position == RIGHT)
	{
	  XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
			    0, 0,
			    ewin->titlebar_height,
			    gl_scr_info.dpy_height -
			    2 * ewin->titlebar_border);
	  XMoveResizeWindow(gl_dpy, ewin->parent, gl_scr_info.dpy_width -
			    (ewin->titlebar_height +
			     2 *(ewin->titlebar_border)),
			    0, ewin->titlebar_height +
			    2 * ewin->titlebar_border,
			    gl_scr_info.dpy_height);
	}
      internal_replace_button(ewin);
    }
  else
    {
      if (!(ewin->size & MAX_WIDTH))
	ewin->win_width = width;
      if (!(ewin->size & MAX_HEIGHT))
	ewin->win_height = height;
      window_group_resize(ewin, 0, 0,
			  gl_scr_info.dpy_width,
			  gl_scr_info.dpy_height);
      send_configure_notify(ewin);
    } 
}

void		internal_shade(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  ewin->is_shade = 2;
  if (ewin->size & MAX)
    {
      ewin->shade_width = ewin->win_width;
      ewin->shade_height = ewin->win_height;
      if (gl_epiwin.titlebar.position == TOP)
	XResizeWindow(gl_dpy, ewin->parent,
		      gl_scr_info.dpy_width,
		      ewin->titlebar_height +
		      2*ewin->titlebar_border);
      else if (gl_epiwin.titlebar.position == BOTTOM)
	{
	  XMoveResizeWindow(gl_dpy, ewin->parent,
			    0, gl_scr_info.dpy_height -
			    ewin->titlebar_height +
			    2*ewin->titlebar_border,
			    gl_scr_info.dpy_width,
			    ewin->titlebar_height +
			    2*ewin->titlebar_border);
	  XMoveWindow(gl_dpy, ewin->titlebar_win, 0, 0);
	}
      else if (gl_epiwin.titlebar.position == LEFT)
	XResizeWindow(gl_dpy, ewin->parent,
		      ewin->titlebar_height +
		      2*ewin->titlebar_border,
		      gl_scr_info.dpy_height);
      else if (gl_epiwin.titlebar.position == RIGHT)
	{
	  XMoveResizeWindow(gl_dpy, ewin->parent,
			    gl_scr_info.dpy_width -
			    ewin->titlebar_height +
			    2*ewin->titlebar_border,
			    0, ewin->titlebar_height +
			    2*ewin->titlebar_border,
			    gl_scr_info.dpy_height);
	  XMoveWindow(gl_dpy, ewin->titlebar_win, 0, 0);
	}
      internal_replace_button(ewin);
    }
  else
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      if (ewin->size & MAX_WIDTH)
	ewin->shade_width = ewin->win_width;
      else
	{
	  ewin->win_x = x;
	  ewin->shade_width = width;
	}
      if (ewin->size & MAX_HEIGHT)
	ewin->shade_height = ewin->win_height;
      else
	{
	  ewin->win_y = y;
	  ewin->shade_height = height;
	}
      if (!((ewin->size & MAX_WIDTH) || (ewin->size & MAX_HEIGHT)
	    || (ewin->size & MAX)))
	{
	  ewin->win_x = x;
	  ewin->win_y = y;
	  ewin->win_width = width;
	  ewin->win_height = height;
	}
      if (gl_epiwin.titlebar.position == TOP)
	XResizeWindow(gl_dpy, ewin->parent,
		      width,
		      ewin->titlebar_height
		      + 2*ewin->titlebar_border);
      else if (gl_epiwin.titlebar.position == BOTTOM)
	{
	  XMoveResizeWindow(gl_dpy, ewin->parent,
			    x, y + height -
			    (ewin->titlebar_height +
			     2*ewin->titlebar_border),
			    width, ewin->titlebar_height
			    + 2*ewin->titlebar_border);
	  XMoveWindow(gl_dpy, ewin->titlebar_win, 0, 0);
	}
      else if (gl_epiwin.titlebar.position == LEFT)
	XResizeWindow(gl_dpy, ewin->parent,
		      ewin->titlebar_height
		      + 2*ewin->titlebar_border,
		      height);
      else if (gl_epiwin.titlebar.position == RIGHT)
	{
	  XMoveResizeWindow(gl_dpy, ewin->parent,
			    x + width -
			    (ewin->titlebar_height
			     + 2*ewin->titlebar_border), y,
			    ewin->titlebar_height			    
			    + 2*ewin->titlebar_border, height);
	  XMoveWindow(gl_dpy, ewin->titlebar_win, 0, 0);
	}
      internal_replace_button(ewin);
    }
}

void		internal_unshade(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  ewin->is_shade = 0;
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
	       &width, &height, &dummy, &dummy);
  if (ewin->size & MAX)
    {
      window_group_resize(ewin, x, y,  gl_scr_info.dpy_width,
			  gl_scr_info.dpy_height);
      send_configure_notify(ewin);
    }
  else
    {      
      if (ewin->size & MAX_WIDTH)
	{
	  if (ewin->size & MAX_HEIGHT)
	    window_group_resize(ewin, x, y, gl_scr_info.dpy_width,
				gl_scr_info.dpy_height);
	  else
	    {
	      if (gl_epiwin.titlebar.position == BOTTOM)
		window_group_resize(ewin, x, y - ewin->shade_height
				    - 2 * ewin->titlebar_border
				    - ewin->titlebar_height,
				    gl_scr_info.dpy_width,
				    ewin->shade_height);
	      else if (gl_epiwin.titlebar.position == RIGHT)
		window_group_resize(ewin, x - gl_scr_info.dpy_width + width,
				    y, gl_scr_info.dpy_width,
				    ewin->shade_height);
	      
	      else
		window_group_resize(ewin, x, y,
				    gl_scr_info.dpy_width,
				    ewin->shade_height);
	    }
	}
      else if (ewin->size & MAX_HEIGHT)
	{
	  if ((gl_epiwin.titlebar.position == BOTTOM))
	    window_group_resize(ewin, x, y - gl_scr_info.dpy_height + height,
				ewin->shade_width,
				gl_scr_info.dpy_height);
	  else if (gl_epiwin.titlebar.position == RIGHT)
	    window_group_resize(ewin, x - ewin->shade_width + width, y,
				ewin->shade_width,
				gl_scr_info.dpy_height);
	  else
	    window_group_resize(ewin, x, y, ewin->shade_width,
				gl_scr_info.dpy_height);
	}
      else
	{
	  if ((gl_epiwin.titlebar.position == BOTTOM))
	    window_group_resize(ewin, x, y - ewin->shade_height + height,
				ewin->shade_width,
				ewin->shade_height);
	  else if (gl_epiwin.titlebar.position == RIGHT)
	    window_group_resize(ewin, x - ewin->shade_width + width, y,
				ewin->shade_width,
				ewin->shade_height);
	  else
	    window_group_resize(ewin, x, y, ewin->shade_width,
				ewin->shade_height);
	}
      send_configure_notify(ewin);
    }
}

void		internal_max_width(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  ewin->size |= MAX_WIDTH;
  if (!((ewin->size & MAX_HEIGHT) || (ewin->is_shade)
	|| (ewin->size & MAX)))
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      ewin->win_x = x;
      ewin->win_y = y;
      ewin->win_width = width;
      ewin->win_height = height;
    }
  if (!(ewin->size & MAX))
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
	{
	  if (gl_epiwin.titlebar.position == LEFT)
	    {
	      XResizeWindow(gl_dpy, ewin->titlebar_win,
			    ewin->titlebar_height,
			    height - 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent, 0, y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				height);
	    }
	  else if (gl_epiwin.titlebar.position == RIGHT)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win, 0, 0,
				ewin->titlebar_height,
				height - 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent, gl_scr_info.dpy_width -
				(ewin->titlebar_height +
				 2 * (ewin->titlebar_border)),
				y, ewin->titlebar_height +
				2 * ewin->titlebar_border,
				height);
	    }
	  else if (gl_epiwin.titlebar.position == TOP)
	    {
	      XResizeWindow(gl_dpy, ewin->titlebar_win,
			    gl_scr_info.dpy_width
			    - 2*ewin->titlebar_border,
			    ewin->titlebar_height);
	      XMoveResizeWindow(gl_dpy, ewin->parent, 0, y,
				gl_scr_info.dpy_width,
				height);
	    }
	  else if (gl_epiwin.titlebar.position == BOTTOM)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win, 0, 0,
				gl_scr_info.dpy_width
				- 2*ewin->titlebar_border,
				ewin->titlebar_height);
	      XMoveResizeWindow(gl_dpy, ewin->parent, 0, y,
				gl_scr_info.dpy_width,
				height);
	    }
	  internal_replace_button(ewin);
	}
      else
	window_group_resize(ewin, 0, y,
			    gl_scr_info.dpy_width,
			    height);
      send_configure_notify(ewin);
    }
}

void		internal_unmax_width(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  ewin->size &= ~MAX_WIDTH;
  if (!(ewin->size & MAX))
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
	{
	  if (gl_epiwin.titlebar.position == TOP)
	    {
	      XResizeWindow(gl_dpy, ewin->titlebar_win,
			    ewin->win_width -			
			    2*ewin->titlebar_border,
			    ewin->titlebar_height);
	      XMoveResizeWindow(gl_dpy, ewin->parent,
				ewin->win_x, y,
				ewin->win_width,
				height);
	    }
	  else if (gl_epiwin.titlebar.position == BOTTOM)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->win_width -
				2*ewin->titlebar_border,
				ewin->titlebar_height);
	      XMoveResizeWindow(gl_dpy, ewin->parent,
				ewin->win_x, y,
				ewin->win_width,
				height);
	    }
	  else if (gl_epiwin.titlebar.position == LEFT)
	    {
	      XResizeWindow(gl_dpy, ewin->titlebar_win,
			    ewin->titlebar_height,
			    height - 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent,		    
				ewin->win_x, y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				height);
	    }
	  else if (gl_epiwin.titlebar.position == RIGHT)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->titlebar_height,
				height - 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent,
				ewin->win_x + ewin->win_width
				- (ewin->titlebar_height +
				   2*ewin->titlebar_border), y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				height);
	    }
	  internal_replace_button(ewin);
	}
      else if (ewin->size & MAX_HEIGHT)
	window_group_resize(ewin, ewin->win_x, y,
			    ewin->win_width,
			    gl_scr_info.dpy_height);
      else	    
	window_group_resize(ewin, ewin->win_x, y,
			    ewin->win_width,
			    height);
      send_configure_notify(ewin);
    }
}

void		internal_max_height(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  ewin->size |= MAX_HEIGHT;
  if (!((ewin->size & MAX_WIDTH) || (ewin->is_shade) || (ewin->size & MAX)))
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      ewin->win_x = x;
      ewin->win_y = y;
      ewin->win_width = width;
      ewin->win_height = height;
    }
  if (!(ewin->size & MAX))
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
	{
	  if (gl_epiwin.titlebar.position == TOP)
	    XMoveWindow(gl_dpy, ewin->parent, x, 0);	    
	  else if (gl_epiwin.titlebar.position == BOTTOM)
	    XMoveWindow(gl_dpy, ewin->parent, x,
			gl_scr_info.dpy_height -
			(ewin->titlebar_height
			 + 2 * ewin->titlebar_border));
	  else if (gl_epiwin.titlebar.position == LEFT)
	    {
	      XResizeWindow(gl_dpy, ewin->titlebar_win,
			    ewin->titlebar_height,
			    gl_scr_info.dpy_height
			    - 2 * (ewin->titlebar_border));
	      XMoveResizeWindow(gl_dpy, ewin->parent,
				x, 0,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				gl_scr_info.dpy_height);
	    }
	  else if (gl_epiwin.titlebar.position == RIGHT)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->titlebar_height,
				gl_scr_info.dpy_height
				- 2 * (ewin->titlebar_border));
	      XMoveResizeWindow(gl_dpy, ewin->parent,
				x, 0,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				gl_scr_info.dpy_height);
	    }
	  internal_replace_button(ewin);
	}
      else
	window_group_resize(ewin, x, 0, width,
			    gl_scr_info.dpy_height);
      send_configure_notify(ewin);
    }
}

void		internal_unmax_height(t_win *ewin)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  ewin->size &= ~MAX_HEIGHT;
  if (!(ewin->size & MAX))
    {
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		   &width, &height, &dummy, &dummy);
      if ((ewin->is_shade == 2) || (ewin->is_shade == 3))
	{
	  if (gl_epiwin.titlebar.position == TOP)
	    XMoveWindow(gl_dpy, ewin->parent,
			x, ewin->win_y);
       	  else if (gl_epiwin.titlebar.position == BOTTOM)
	    XMoveWindow(gl_dpy, ewin->parent,
			x, ewin->win_y + ewin->shade_height);
       	  else if (gl_epiwin.titlebar.position == LEFT)
	    {
	      XResizeWindow(gl_dpy, ewin->titlebar_win,
			    ewin->titlebar_height,
			    ewin->win_height - 2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent,		    
				x, ewin->win_y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				ewin->win_height);
	    }
	  else if (gl_epiwin.titlebar.position == RIGHT)
	    {
	      XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
				0, 0, ewin->titlebar_height,
				ewin->win_height -
				2*ewin->titlebar_border);
	      XMoveResizeWindow(gl_dpy, ewin->parent,
				x + width - (ewin->titlebar_height +
					     2*ewin->titlebar_border),
				ewin->win_y,
				ewin->titlebar_height +
				2*ewin->titlebar_border,
				ewin->win_height);
	    }
	  internal_replace_button(ewin);
	}
      else if (ewin->size & MAX_WIDTH)
	window_group_resize(ewin, x, ewin->win_y,
			    gl_scr_info.dpy_width,
			    ewin->win_height);
      else	    
	window_group_resize(ewin, x, ewin->win_y,
			    width,
			    ewin->win_height);
      send_configure_notify(ewin);
    }
}



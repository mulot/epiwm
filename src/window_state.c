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
** window_state.c
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Sat Mar 20 16:42:40 1999 Poindessous Thomas
 ** Last update Wed Apr  5 03:15:02 2000 Mulot Julien
*/

#include "epiwm.h"

void			clientSetState(t_win	*ewin,
				       int	state)
{
  unsigned long		data[2];
  
  ewin->state = state;
  data[0] = (unsigned long) state;
  data[1] = (unsigned long) None; /* icon window */
  XChangeProperty(gl_dpy, ewin->win, EPIWM_WM_STATE, EPIWM_WM_STATE, 32, 
		  PropModeReplace, (unsigned char *) data, 2);
}

void			send_configure_notify(t_win	*ewin)
{
  XEvent		sevent;
  XWindowAttributes	attrib;
  
  XGetWindowAttributes(gl_dpy, ewin->win, &attrib);
  sevent.type = ConfigureNotify;
  sevent.xconfigure.display = gl_dpy;
  sevent.xconfigure.event = ewin->win;
  sevent.xconfigure.window = ewin->win;
  sevent.xconfigure.width = attrib.width;
  sevent.xconfigure.height = attrib.height;
  sevent.xconfigure.border_width = attrib.border_width;
  sevent.xconfigure.override_redirect = False;
  sevent.xconfigure.above = ewin->win;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  if (gl_epiwin.titlebar.position == TOP)
    {
      sevent.xconfigure.x = attrib.x + ewin->border_size
	+ ewin->bf_border;
      sevent.xconfigure.y = attrib.y + 2 * ewin->titlebar_border +
	ewin->titlebar_height + ewin->border_size;
    }
  else if ((gl_epiwin.titlebar.position == BOTTOM) ||
	   (gl_epiwin.titlebar.position == RIGHT))
    {
      sevent.xconfigure.x = attrib.x + ewin->border_size
	+ ewin->bf_border;
      sevent.xconfigure.y = attrib.y + ewin->border_size
	+ ewin->bf_border;
    }
  else if (gl_epiwin.titlebar.position == LEFT)
    {
      sevent.xconfigure.x = attrib.x + 2 * ewin->titlebar_border +
	ewin->titlebar_height + ewin->border_size;
      sevent.xconfigure.y = attrib.y + ewin->border_size
	+ ewin->bf_border;
    }
  XSendEvent(gl_dpy, ewin->win, False, StructureNotifyMask, &sevent);
}

void			client_message()
{
  t_win			*ewin;
  
  ewin = search_elt(gl_event.xclient.window);
  if (ewin)
    if (gl_event.xclient.message_type == EPIWM_WM_CHANGE_STATE)
      if (gl_event.xclient.data.l[0] == IconicState)
	mouse_iconify(ewin);
}

void		window_group_resize(t_win	*ewin,
				    int		parent_x,
				    int		parent_y,
				    int		parent_width,
				    int		parent_height)
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  if ((parent_width < 0) || (parent_height < 0))
    return ;
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
	       &width, &height, &dummy, &dummy);
  if ((gl_epiwin.titlebar.position == TOP)
      || (gl_epiwin.titlebar.position == BOTTOM))
    internal_verify_size_horizontal(ewin, &parent_width, &parent_height);
  else
    internal_verify_size_vertical(ewin, &parent_width, &parent_height);
  if (gl_epiwin.prop & IS_WIN_CHG)
    {
      win_opaque_resize(ewin, parent_x, parent_y,
			parent_width, parent_height);
      return ;
    }
  if (parent_width <= width && parent_height <= height)
    {
      if (x == parent_x && y == parent_y)
	XResizeWindow(gl_dpy, ewin->parent,
		      parent_width, parent_height);
      else
	XMoveResizeWindow(gl_dpy, ewin->parent,
			  parent_x, parent_y, parent_width, parent_height);
    }
  if (gl_epiwin.titlebar.position == TOP)
    internal_group_resize_top(ewin, parent_width, parent_height);
  else if (gl_epiwin.titlebar.position == BOTTOM)
    internal_group_resize_bottom(ewin, parent_width, parent_height);
  else if (gl_epiwin.titlebar.position == RIGHT)
    internal_group_resize_right(ewin, parent_width, parent_height);
  else if (gl_epiwin.titlebar.position == LEFT)
    internal_group_resize_left(ewin, parent_width, parent_height);
  if (!(parent_width <= width && parent_height <= height))
    {
      if (x == parent_x && y == parent_y)
	XResizeWindow(gl_dpy, ewin->parent,
		      parent_width, parent_height);
      else
	XMoveResizeWindow(gl_dpy, ewin->parent,
			  parent_x, parent_y, parent_width, parent_height);
    }
  if ((ewin->size & SHAPE) && !(gl_epiwin.titlebar.width))
    add_shape(ewin);
  if ((gl_epiwin.titlebar.width))
    draw_titlebar_shape(ewin);
}

void	window_group_map(t_win	*ewin)
{
  if (ewin->size & SHAPE)
    add_shape(ewin);
  XMapSubwindows(gl_dpy, ewin->parent);
  XMapSubwindows(gl_dpy, ewin->border_frame);
  if ((ewin->prop & NO_TITLE) == 0)
    XMapSubwindows(gl_dpy, ewin->titlebar_win);	     
  XMapWindow(gl_dpy, ewin->parent);
  if (gl_epiwin.titlebar.width_mode && ((ewin->prop & NO_TITLE) == 0))
    draw_titlebar_shape(ewin);
}

void		clientsendprotocol(t_win	*ewin,
				   Atom		protocol)
{
  XEvent	event;
  
  event.xclient.type = ClientMessage;
  event.xclient.message_type = EPIWM_WM_PROTOCOLS;
  event.xclient.format = 32;
  event.xclient.display = gl_dpy;
  event.xclient.window = ewin->win;
  event.xclient.data.l[0] = protocol;
  /*
    event.xclient.data.l[1] = time;
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 0;
  */
  XSendEvent(gl_dpy, ewin->win, False, NoEventMask, &event);
  XSync(gl_dpy, False);
}

void	internal_group_resize_top(t_win	*ewin,
				  int	parent_width,
				  int	parent_height)
{
  if ((ewin->prop & NO_TITLE) == 0)
    XResizeWindow(gl_dpy, ewin->titlebar_win, 
		  parent_width - 2 * ewin->titlebar_border,
		  ewin->titlebar_height);
  internal_resize_win_horizontal(ewin, parent_width, parent_height);
  internal_replace_button_horizontal(ewin);
}

void	internal_group_resize_bottom(t_win	*ewin,
				     int	parent_width,
				     int	parent_height)
{
  if ((ewin->prop & NO_TITLE) == 0)
    XMoveResizeWindow(gl_dpy, ewin->titlebar_win, 0,
		      parent_height - ewin->titlebar_height
		      - 2 * ewin->titlebar_border,
		      parent_width - 2 * ewin->titlebar_border,
		      ewin->titlebar_height);
  internal_resize_win_horizontal(ewin, parent_width, parent_height);
  internal_replace_button_horizontal(ewin);
}

void	internal_group_resize_left(t_win	*ewin,
				   int		parent_width,
				   int		parent_height)
{
  if ((ewin->prop & NO_TITLE) == 0)
    XResizeWindow(gl_dpy, ewin->titlebar_win,
		  ewin->titlebar_height,
		  parent_height - 2 * ewin->titlebar_border);
  internal_resize_win_vertical(ewin, parent_width, parent_height);
  internal_replace_button_vertical(ewin);
}

void	internal_group_resize_right(t_win	*ewin,
				    int		parent_width,
				    int		parent_height)
{
  if ((ewin->prop & NO_TITLE) == 0)
    XMoveResizeWindow(gl_dpy, ewin->titlebar_win,
		      parent_width - ewin->titlebar_height
		      - 2 * ewin->titlebar_border,
		      0,
		      ewin->titlebar_height,
		      parent_height - 2 * ewin->titlebar_border);
  internal_resize_win_vertical(ewin, parent_width, parent_height);
  internal_replace_button_vertical(ewin);
}

void	internal_replace_button(t_win	*ewin)
{
  if ((gl_epiwin.titlebar.position == TOP) ||
      (gl_epiwin.titlebar.position == BOTTOM))
    internal_replace_button_horizontal(ewin);
  else if ((gl_epiwin.titlebar.position == RIGHT) ||
	   (gl_epiwin.titlebar.position == LEFT))
    internal_replace_button_vertical(ewin);
}

void	internal_replace_button_horizontal(t_win	*ewin)
{
  if (ewin->prop & NO_TITLE)
    return ;
  if (gl_epiwin.align_btn_max)
    XMoveWindow(gl_dpy, ewin->button_max,
		set_btn_position(ewin, gl_epiwin.align_btn_max,
				 gl_epiwin.pos_btn_max),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
  if (gl_epiwin.align_btn_close)
    XMoveWindow(gl_dpy, ewin->button_close,
		set_btn_position(ewin, gl_epiwin.align_btn_close,
				 gl_epiwin.pos_btn_close),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
  if (gl_epiwin.align_btn_destroy)
    XMoveWindow(gl_dpy, ewin->button_destroy,
		set_btn_position(ewin, gl_epiwin.align_btn_destroy,
				 gl_epiwin.pos_btn_destroy),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
  if (gl_epiwin.align_btn_max_height)
    XMoveWindow(gl_dpy, ewin->button_max_height,
		set_btn_position(ewin, gl_epiwin.align_btn_max_height,
				 gl_epiwin.pos_btn_max_height),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
  if (gl_epiwin.align_btn_max_width)
    XMoveWindow(gl_dpy, ewin->button_max_width,
		set_btn_position(ewin, gl_epiwin.align_btn_max_width,
				 gl_epiwin.pos_btn_max_width),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
  if (gl_epiwin.align_btn_iconify)
    XMoveWindow(gl_dpy, ewin->button_iconify,
		set_btn_position(ewin, gl_epiwin.align_btn_iconify,
				 gl_epiwin.pos_btn_iconify),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
  if (gl_epiwin.align_btn_shade)
    XMoveWindow(gl_dpy, ewin->button_shade,
		set_btn_position(ewin, gl_epiwin.align_btn_shade,
				 gl_epiwin.pos_btn_shade),
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2);
}

void	internal_replace_button_vertical(t_win	*ewin)
{
  if (ewin->prop & NO_TITLE)
    return ;
  if (gl_epiwin.align_btn_max)
    XMoveWindow(gl_dpy, ewin->button_max,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_max,
				 gl_epiwin.pos_btn_max));
  if (gl_epiwin.align_btn_close)
    XMoveWindow(gl_dpy, ewin->button_close,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_close,
				 gl_epiwin.pos_btn_close));
  if (gl_epiwin.align_btn_destroy)
    XMoveWindow(gl_dpy, ewin->button_destroy,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_destroy,
				 gl_epiwin.pos_btn_destroy));
  if (gl_epiwin.align_btn_max_height)
    XMoveWindow(gl_dpy, ewin->button_max_height,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_max_height,
				 gl_epiwin.pos_btn_max_height));
  if (gl_epiwin.align_btn_max_width)
    XMoveWindow(gl_dpy, ewin->button_max_width,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_max_width,
				 gl_epiwin.pos_btn_max_width));
  if (gl_epiwin.align_btn_iconify)
    XMoveWindow(gl_dpy, ewin->button_iconify,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_iconify,
				 gl_epiwin.pos_btn_iconify));
  if (gl_epiwin.align_btn_shade)
    XMoveWindow(gl_dpy, ewin->button_shade,
		(ewin->titlebar_height - gl_epiwin.btn_height) / 2,
		set_btn_position(ewin, gl_epiwin.align_btn_shade,
				 gl_epiwin.pos_btn_shade));
}

void	internal_verify_size_horizontal(t_win	*ewin,
					int	*parent_width,
					int	*parent_height)
{
  if ((ewin->is_shade == 0) || (ewin->is_shade == 1))
    {      
      if (*parent_width < (ewin->hints.min_width + 2 *
			   (ewin->border_size + ewin->bf_border)))
	*parent_width = (ewin->hints.min_width + 2 *
			 (ewin->border_size + ewin->bf_border)) ;
      else if (ewin->hints.max_width != -1)
	{
	  if (*parent_width > (ewin->hints.max_width + 2 *
			       (ewin->border_size +
				ewin->bf_border)))
	    *parent_width = (ewin->hints.max_width + 2 *
			     (ewin->border_size + ewin->bf_border));
	}
      if (*parent_height < (ewin->hints.min_height + 2 *
			    (ewin->border_size
			     + ewin->titlebar_border)
			    + ewin->bf_border
			    + ewin->titlebar_height))
	*parent_height = (ewin->hints.min_height + 2 *
			  (ewin->border_size +
			   ewin->titlebar_border) + ewin->bf_border +
			  ewin->titlebar_height);
      else if (ewin->hints.max_height != -1)
	{
	  if (*parent_height > (ewin->hints.max_height + 2 *
				(ewin->border_size
				 + ewin->titlebar_border)
				+ ewin->bf_border +
				ewin->titlebar_height))
	    *parent_height = (ewin->hints.max_height + 2 *
			      (ewin->border_size +
			       ewin->titlebar_border)
			      + ewin->bf_border +
			      ewin->titlebar_height);
	}
    }
  else
    {
      if (*parent_width < (ewin->hints.min_width + 2 *
			   (ewin->border_size + ewin->bf_border)))
	*parent_width = (ewin->hints.min_width + 2 *
			 (ewin->border_size + ewin->bf_border));
      else if (ewin->hints.max_width != -1)
	{
	  if (*parent_width > (ewin->hints.max_width + 2 *
			       (ewin->border_size + ewin->bf_border)
			       ))
	    *parent_width = (ewin->hints.max_width + 2 *
			     (ewin->border_size + ewin->bf_border));
	}
    }
}

void	internal_verify_size_vertical(t_win	*ewin,
				      int	*parent_width,
				      int	*parent_height)
{
  if ((ewin->is_shade == 0) || (ewin->is_shade == 1))
    {      
      if (*parent_width < (ewin->hints.min_width + 2 *
			   (ewin->border_size +
			    ewin->titlebar_border)
			   + ewin->bf_border
			   + ewin->titlebar_height))
	*parent_width = (ewin->hints.min_width + 2 *
			 (ewin->border_size + ewin->titlebar_border)
			 + ewin->bf_border + ewin->titlebar_height);
      else if (ewin->hints.max_width != -1)
	{
	  if (*parent_width > (ewin->hints.max_width + 2 *
			       (ewin->border_size +
				ewin->titlebar_border)
			       + ewin->bf_border
			       + ewin->titlebar_height))
	    *parent_width = (ewin->hints.max_width + 2 *
			     (ewin->border_size +
			      ewin->titlebar_border)
			     + ewin->bf_border
			     + ewin->titlebar_height);
	}
      if (*parent_height < (ewin->hints.min_height + 2 *
			    (ewin->border_size +
			     + ewin->bf_border)))
	*parent_height =  (ewin->hints.min_height + 2 *
			   (ewin->border_size
			    + ewin->bf_border));
      else if (ewin->hints.max_height != -1)
	{
	  if (*parent_height > (ewin->hints.max_height + 2 *
				(ewin->border_size
				 + ewin->bf_border)))
	    *parent_height =  (ewin->hints.max_height + 2 *
			       (ewin->border_size
				+ ewin->bf_border));
	}
    }
  else
    {
      if (*parent_height < (ewin->hints.min_height + 2 *
			    (ewin->border_size + ewin->bf_border)))
	*parent_height =  (ewin->hints.min_height + 2 *
			   (ewin->border_size + ewin->bf_border));
      else if (ewin->hints.max_height != -1)
	{
	  if (*parent_height > (ewin->hints.max_height + 2 *
				(ewin->border_size
				 + ewin->bf_border)))
	    *parent_height =  (ewin->hints.max_height + 2 *
			       (ewin->border_size
				+ ewin->bf_border));
	}
    }
}

void	internal_resize_win_horizontal(t_win	*ewin,
				       int	parent_width,
				       int	parent_height)
{
  if ((ewin->is_shade == 0) || (ewin->is_shade == 1))
    {
      XResizeWindow(gl_dpy, ewin->border_frame, 
		    parent_width - 2 * ewin->bf_border,
		    parent_height - ewin->titlebar_height -
		    2 * ewin->titlebar_border - ewin->bf_border);
      XResizeWindow(gl_dpy, ewin->win, 
		    parent_width - 2*(ewin->bf_border
				      + ewin->border_size),
		    parent_height - ewin->titlebar_height -
		    2*(ewin->titlebar_border
		       + ewin->border_size) - ewin->bf_border);
    }
}

void	internal_resize_win_vertical(t_win	*ewin,
				     int	parent_width,
				     int	parent_height)
{
  if ((ewin->is_shade == 0) || (ewin->is_shade == 1))
    {
      XResizeWindow(gl_dpy, ewin->border_frame, 
		    parent_width - ewin->titlebar_height -
		    2 * (ewin->titlebar_border) - ewin->bf_border,
		    parent_height - 2 * ewin->bf_border);
      XResizeWindow(gl_dpy, ewin->win, 
		    parent_width - ewin->titlebar_height -
		    2*(ewin->titlebar_border + ewin->border_size)
		    - ewin->bf_border,
		    parent_height - 2*(ewin->bf_border
				       + ewin->border_size));
    }
}









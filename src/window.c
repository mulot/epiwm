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
** window.c
** 
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Fri Jan  8 21:13:01 1999 Julien Mulot
 ** Last update Mon Oct 23 22:18:59 2000 Julien Mulot
*/

#include "epiwm.h"
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

int		test_shape(Window	win)
{
  int		xws;
  int		yws;
  int		xbs;
  int		ybs;
  unsigned int	wws;
  unsigned int	hws;
  unsigned int	wbs;
  unsigned int	hbs;
  int		boundingShaped;
  int		clipShaped;
  
  XShapeSelectInput(gl_dpy, win, ShapeNotifyMask);
  XShapeQueryExtents(gl_dpy, win, &boundingShaped, &xws, &yws, &wws,
		     &hws, &clipShaped, &xbs, &ybs, &wbs, &hbs);
  return (boundingShaped);
}

void			add_shape(t_win	*ewin)
{
  XRectangle		rect[4];
  XWindowAttributes	attrib;
  unsigned short	nb_rect;
  
  nb_rect = 4;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  rect[0].x = 0;
  rect[0].y = 0;
  rect[0].width = attrib.width ;
  rect[0].height = ewin->border_size + ewin->titlebar_height +
    ewin->titlebar_border*2;
  rect[1].x = 0;
  rect[1].y = 0;
  rect[1].width = ewin->border_size + ewin->bf_border;
  rect[1].height = attrib.height;
  rect[2].x = attrib.width - ewin->border_size - ewin->bf_border;
  rect[2].y = 0;
  rect[2].width = ewin->border_size + ewin->bf_border;
  rect[2].height = attrib.height;
  rect[3].x = 0;
  rect[3].y = attrib.height - ewin->border_size - ewin->bf_border;
  rect[3].width = attrib.width;
  rect[3].height = ewin->border_size + ewin->bf_border;   
  XGetWindowAttributes(gl_dpy, ewin->win, &attrib);
  XShapeCombineShape(gl_dpy, ewin->parent, ShapeBounding,
		     ewin->border_size + attrib.border_width
		     + ewin->bf_border,
		     ewin->border_size + attrib.border_width
		     + ewin->titlebar_height
		     + 2*ewin->titlebar_border,
		     ewin->win, ShapeBounding, ShapeSet);
  if (ewin->border_size != 0 && ewin->titlebar_height != 0)
    XShapeCombineRectangles(gl_dpy, ewin->parent, ShapeBounding,
			    0, 0, rect, nb_rect,
			    ShapeUnion, Unsorted);     
}

t_win			*add_border(Window	win)
{     
  t_win			*ewin;
  XWindowAttributes	attrib;
  XSetWindowAttributes	attr;  
  char			*name;
  XWMHints		*wmhints;
  Window		transient;
  
  XGetWindowAttributes(gl_dpy, win, &attrib);
  if (!attrib.override_redirect)
    {
      ewin = (t_win *) xmalloc(sizeof (t_win));
      ewin->win = win;
      XSetWindowBorderWidth(gl_dpy, win, 0);
      XGetWindowAttributes(gl_dpy, win, &attrib);
      XAddToSaveSet(gl_dpy, win);
      XGrabServer(gl_dpy);
      XUnmapWindow(gl_dpy, win);
      XFetchName(gl_dpy, win, &name);
      check_app_prop(ewin, name);
      motif_check(ewin);
      check_properties(ewin);
      ewin->parent = add_parent(ewin, &attrib);
      ewin->border_frame = add_border_frame(ewin, ewin->parent);
      ewin->titlebar_win = add_titlebar_win(ewin, ewin->parent);
      XStoreName(gl_dpy, ewin->parent, name);
      if ((ewin->prop & NO_TITLE) == 0)
	XStoreName(gl_dpy, ewin->titlebar_win, name);
      free(name);
      XGetIconName(gl_dpy, win, &name);
      XSetIconName(gl_dpy, ewin->parent, name);
      free(name);
      if (gl_epiwin.align_btn_max)
	ewin->button_max = build_button(ewin,
					gl_epiwin.align_btn_max,
					gl_epiwin.pos_btn_max);
      if (gl_epiwin.align_btn_max_height)
	ewin->button_max_height = 
	  build_button(ewin, 
		       gl_epiwin.align_btn_max_height,
		       gl_epiwin.pos_btn_max_height);
      if (gl_epiwin.align_btn_max_width)
	ewin->button_max_width = build_button(ewin,
					      gl_epiwin.align_btn_max_width,
					      gl_epiwin.pos_btn_max_width);
      
      if (gl_epiwin.align_btn_iconify)
	ewin->button_iconify = build_button(ewin,
					    gl_epiwin.align_btn_iconify,
					    gl_epiwin.pos_btn_iconify);
      if (gl_epiwin.align_btn_shade)
	ewin->button_shade = build_button(ewin,
					  gl_epiwin.align_btn_shade,
					  gl_epiwin.pos_btn_shade);
      if (gl_epiwin.align_btn_close)
	ewin->button_close = build_button(ewin,
					  gl_epiwin.align_btn_close,
					  gl_epiwin.pos_btn_close);
      if (gl_epiwin.align_btn_destroy)
	ewin->button_destroy = build_button(ewin,
					    gl_epiwin.align_btn_destroy,
					    gl_epiwin.pos_btn_destroy);
      add_elt(ewin);
      attr.event_mask = PropertyChangeMask | ResizeRedirectMask
	| StructureNotifyMask | KeyPressMask | PointerMotionMask;
      attr.do_not_propagate_mask = ButtonPressMask | ButtonReleaseMask;
      XChangeWindowAttributes(gl_dpy, ewin->win,
			      CWEventMask | CWDontPropagate, &attr);
      XReparentWindow(gl_dpy, win, ewin->border_frame,
		      ewin->border_size, ewin->border_size);
      grab_all_shortcuts(win);
      wmhints = XGetWMHints(gl_dpy, win);
      if (gl_epiwin.prop & NEW_AUTO_FOCUS)
	window_state(ewin, "active");
      else
	window_state(ewin, "inactive");
      if (wmhints)
	{
	  if ((wmhints->flags & StateHint) &&
	      (wmhints->initial_state == IconicState))
	    win_iconify(ewin, 1, 0);
	  else if (attrib.map_state == IsViewable)
	    {
	      if ((gl_epiwin.prop & SMART_PLACE)
		  && (!(gl_epiwin.prop & FIRST_START)))
		placement_smart(ewin);
	      window_group_map(ewin);
	      if (gl_epiwin.prop & NEW_AUTO_FOCUS)
		window_state(ewin, "active");
	      else
		window_state(ewin, "inactive");
	      catch_win_mouse();
	    }
	  XFree(wmhints);
	}
      else if (attrib.map_state == IsViewable)
	{
	  if ((gl_epiwin.prop & SMART_PLACE)
	      && (!(gl_epiwin.prop & FIRST_START)))
	    placement_smart(ewin);
	  window_group_map(ewin);
	  if (gl_epiwin.prop & NEW_AUTO_FOCUS)
	    window_state(ewin, "active");
	  else
	    window_state(ewin, "inactive");
	  catch_win_mouse();
	}
      if (XGetTransientForHint(gl_dpy, ewin->win, &transient))
	{
	  if (transient != gl_scr_info.root_win)
	    map_request(transient);
	}
      XUngrabServer(gl_dpy);
      XSync(gl_dpy, 1);
      return (ewin);
    }
  else
    return (NULL);
}

void			handle_window(Window	win)
{
  XWindowAttributes	attrib;
  
  XGetWindowAttributes(gl_dpy, win, &attrib);
  if (attrib.map_state == IsViewable)
    add_border(win);
}

t_win		*create_win(Window	win)
{
  t_win		*ewin;
  
  ewin = add_border(win);
  return (ewin);
}

Window			add_parent(t_win		*ewin,
				   XWindowAttributes	*attrib)
{
  Window		temp;
  XSetWindowAttributes	attr;
  int			width;
  int			height;
  
  width = 0;
  height = 0;
  if (ewin->prop & NO_TITLE)
    {
      width = attrib->width + (ewin->border_size + ewin->bf_border) * 2;
      height = attrib->height + (ewin->border_size + ewin->bf_border) * 2;
    }
  else if (gl_epiwin.titlebar.position == TOP
	   || gl_epiwin.titlebar.position == BOTTOM)
    {
      width = attrib->width + (ewin->border_size + ewin->bf_border) * 2;
      height = attrib->height +	(ewin->border_size + ewin->titlebar_border)*2
	+ ewin->titlebar_height + ewin->bf_border;
    }
  else if (gl_epiwin.titlebar.position == LEFT
	   || gl_epiwin.titlebar.position == RIGHT)
    {
      width = attrib->width + ewin->titlebar_height + ewin->titlebar_border * 2
	+ ewin->border_size * 2 + ewin->bf_border;
      height = attrib->height +	(ewin->border_size + ewin->bf_border)*2;
    }
  temp = XCreateSimpleWindow(gl_dpy,
			     gl_scr_info.root_win,
			     attrib->x, attrib->y, width, height, 0,
			     BlackPixel(gl_dpy, gl_scr_info.screen),
			     WhitePixel(gl_dpy, gl_scr_info.screen));
  attr.event_mask = /*ExposureMask | */EnterWindowMask | LeaveWindowMask
    | ButtonPressMask | ButtonReleaseMask | SubstructureNotifyMask
    | PropertyChangeMask | SubstructureRedirectMask | KeyPressMask;
  XChangeWindowAttributes(gl_dpy, temp , CWEventMask, &attr);
  return (temp);
}

Window			build_button(t_win	*ewin,
				     char	align,
				     int	pos)
{
  t_window_btn		tmp;
  XSetWindowAttributes	attrib;  
  Window		w_tmp;
  
  if (ewin->prop & NO_TITLE)
    return (0);
  if ((gl_epiwin.titlebar.position == TOP) 
      || (gl_epiwin.titlebar.position == BOTTOM))
    {
      tmp.x = set_btn_position(ewin, align, pos);
      tmp.y = (ewin->titlebar_height - gl_epiwin.btn_height) / 2;
    }
  else
    {
      tmp.x = (ewin->titlebar_height - gl_epiwin.btn_width) / 2;
      tmp.y = set_btn_position(ewin, align, pos);
    }
  tmp.width = gl_epiwin.btn_width;
  tmp.height = gl_epiwin.btn_height ;
  tmp.bgcolor = BlackPixel(gl_dpy, gl_scr_info.screen);
  tmp.fgcolor = gl_epiwin.titlebar.color_active.pixel;
  tmp.parent = ewin->titlebar_win;
  tmp.cursor = gl_epiwin.normal_cursor;
  tmp.border = 0;
  w_tmp = add_btn(&tmp);
  attrib.event_mask = ButtonPressMask | ButtonReleaseMask
    /* | ExposureMask */;
  XChangeWindowAttributes(gl_dpy, w_tmp, CWEventMask, &attrib);
  return (w_tmp);
}

Window			add_border_frame(t_win	*ewin,
					 Window	parent)
{
  t_window_btn		tmp;
  XWindowAttributes	attrib;
  XSetWindowAttributes	attr;
  Window		w_tmp;
  int			width;
  int			height;
  int			x;
  int			y;
  
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  XGetWindowAttributes(gl_dpy, parent, &attrib);
  if (ewin->prop & NO_TITLE)
    {
      width = attrib.width - ewin->bf_border * 2;
      height = attrib.height - ewin->bf_border * 2;
    }
  else if (gl_epiwin.titlebar.position == TOP)
    {
      x = 0;
      y = ewin->titlebar_height + ewin->titlebar_border * 2
	- ewin->bf_border;
      width = attrib.width - ewin->bf_border * 2;
      height =  attrib.height - ewin->titlebar_height
	- (ewin->titlebar_border * 2) - ewin->bf_border;
    }
  else if (gl_epiwin.titlebar.position == LEFT)
    {
      x = ewin->titlebar_height + ewin->titlebar_border * 2
	- ewin->bf_border;
      y = 0;
      width = attrib.width - ewin->bf_border - ewin->titlebar_height
	- (ewin->titlebar_border * 2);
      height = attrib.height - ewin->bf_border * 2;
    }
  else if (gl_epiwin.titlebar.position == RIGHT)
    {
      x = 0;
      y = 0;
      width = attrib.width - ewin->bf_border - ewin->titlebar_height
	- (ewin->titlebar_border * 2);
      height = attrib.height - ewin->bf_border * 2;
    }
  else
    {
      x = 0;
      y = 0;
      width = attrib.width - ewin->bf_border * 2;
      height =  attrib.height - ewin->titlebar_height
	- (ewin->titlebar_border * 2) - ewin->bf_border;
      /*
	width = attrib.width - ewin->bf_border * 2;
	height = attrib.height - ewin->bf_border * 2;
      */
    }
  tmp.bgcolor = BlackPixel(gl_dpy, gl_scr_info.screen);
  tmp.fgcolor = gl_epiwin.color_active.pixel;
  tmp.parent = parent;
  /*#warning Changer le cursor de border frame*/
  tmp.cursor = 0;
  tmp.border = ewin->bf_border;
  tmp.x = x;
  tmp.y = y;
  tmp.width = width;
  tmp.height = height;
  w_tmp = add_btn(&tmp);
  attr.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask
    | SubstructureRedirectMask | SubstructureNotifyMask | PointerMotionMask
    | KeyPressMask;
  XChangeWindowAttributes(gl_dpy, w_tmp, CWEventMask, &attr);
  return (w_tmp);
}

Window			add_titlebar_win(t_win	*ewin,
					 Window	parent)
{
  t_window_btn		tmp;
  XWindowAttributes	attrib;
  XSetWindowAttributes	attr;
  Window		w_tmp;
  int			width;
  int			height;
  int			x;
  int			y;
  
  if (ewin->prop & NO_TITLE)
    return (0);
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  XGetWindowAttributes(gl_dpy, parent, &attrib);
  if (gl_epiwin.titlebar.position == TOP)
    {
      x = 0;
      y = 0;
      width = attrib.width - 2 * ewin->titlebar_border;
      height = ewin->titlebar_height;
    }
  else if (gl_epiwin.titlebar.position == LEFT)
    {
      x = 0;
      y = 0;
      width = ewin->titlebar_height;
      height = attrib.height - 2 * ewin->titlebar_border;
    }
  else if (gl_epiwin.titlebar.position == RIGHT)
    {
      x = attrib.width - ewin->titlebar_height - ewin->titlebar_border * 2;
      y = 0;
      width = ewin->titlebar_height;
      height = attrib.height - 2 * ewin->titlebar_border;
    }
  else
    {
      x = 0;
      y = attrib.height - ewin->titlebar_height
	- ewin->titlebar_border * 2;
      width = attrib.width - 2 * ewin->titlebar_border;
      height = ewin->titlebar_height;
    }
  tmp.x = x;
  tmp.y = y;
  tmp.bgcolor = BlackPixel(gl_dpy, gl_scr_info.screen);
  tmp.fgcolor = gl_epiwin.titlebar.color_active.pixel;
  tmp.parent = parent;
  tmp.cursor = 0;
  tmp.border = ewin->titlebar_border;
  tmp.width = width;
  tmp.height = height;
  w_tmp = add_btn(&tmp);
  attr.event_mask = ButtonPressMask | ButtonReleaseMask
    | ExposureMask | PointerMotionMask | KeyPressMask;
  XChangeWindowAttributes(gl_dpy, w_tmp, CWEventMask, &attr);
  return (w_tmp);
}


Window		add_btn(t_window_btn	*win_struct)
{
  Window	temp;
  
  temp = XCreateSimpleWindow(gl_dpy,
			     win_struct->parent,
			     win_struct->x,
			     win_struct->y,
			     win_struct->width,
			     win_struct->height,
			     win_struct->border,
			     win_struct->bgcolor,
			     win_struct->fgcolor);
  if (win_struct->cursor)
    XDefineCursor(gl_dpy, temp, win_struct->cursor);
  return (temp);
}

void	check_properties(t_win	*ewin)
{  
  check_size_hints(ewin);
  check_icon(ewin);
}

void		check_icon(t_win	*ewin)
{
  XWMHints	*wm_hints;
  
  if (gl_icon.showed)
    {
      wm_hints = XGetWMHints(gl_dpy, ewin->win);
      if (wm_hints)
	{      
	  if (wm_hints->flags & IconWindowHint)
	    ewin->iconify_win = wm_hints->icon_window;	  
	  else
	    ewin->iconify_win = 0;
	  if (wm_hints->flags & IconMaskHint)
	    create_pixmap_mask(ewin, &(wm_hints->icon_mask));
	  if (wm_hints->flags & IconPixmapHint)
	    create_icon_win_with_pixmap(ewin, &(wm_hints->icon_pixmap));
	  if (wm_hints->flags & IconPositionHint)
	    ;
	  /*	    printf("icon position\n");*/
	}
      else
	ewin->iconify_win = 0;
      XFree(wm_hints);
    }
  else
    ewin->iconify_win = 0;
}

void	create_icon_win_with_pixmap(t_win	*ewin,
				    Pixmap	*pixmap)
{
  int	width;
  int	height;
  
  if (pixmap != 0)
    {
      check_icon_size(pixmap, &width, &height);
      if (ewin->iconify_win == 0)
	ewin->iconify_win =
	  XCreateSimpleWindow(gl_dpy, gl_scr_info.root_win,
			      0, 0, width, height, 0,
			      BlackPixel(gl_dpy,
					 gl_scr_info.screen),
			      WhitePixel(gl_dpy,
					 gl_scr_info.screen));
      else
	XResizeWindow(gl_dpy, ewin->iconify_win, width, height);      
      XSetWindowBackgroundPixmap(gl_dpy, ewin->iconify_win, *pixmap);
      iconify_win_set_attrib(ewin);
    }  
}

void	create_pixmap_mask(t_win	*ewin,
			   Pixmap	*shape_pixmap)
{
  XShapeCombineMask(gl_dpy, ewin->iconify_win, ShapeBounding,
		    0, 0, *shape_pixmap, ShapeSet);
}

void			iconify_win_set_attrib(t_win	*ewin)
{
  XSetWindowAttributes	attr;
  
  attr.event_mask = ButtonPressMask | ButtonReleaseMask
    | EnterWindowMask | LeaveWindowMask | ExposureMask | PointerMotionMask;
  XChangeWindowAttributes(gl_dpy, ewin->iconify_win, CWEventMask, &attr);
}

void			check_icon_size(Pixmap	*pixmap,
					int	*width,
					int	*height)
{
  Window		dummy;
  int			dummy_int;
  
  XGetGeometry(gl_dpy, *pixmap, &dummy, &dummy_int, &dummy_int, width, 
	       height, &dummy_int, &dummy_int);
}

void		check_size_hints(t_win		*ewin)
{
  XSizeHints	size_hints;
  long		toto;
  
  XGetWMNormalHints(gl_dpy, ewin->win, &size_hints, &toto);
  if (size_hints.flags & PResizeInc)
    {
      if (size_hints.width_inc == 0)
	ewin->hints.width_inc = 1;
      else
	ewin->hints.width_inc = size_hints.width_inc;
      if (size_hints.height_inc == 0)
	ewin->hints.height_inc = 1;
      else
	ewin->hints.height_inc = size_hints.height_inc;
    }
  else
    {
      ewin->hints.width_inc = 1;
      ewin->hints.height_inc = 1;
    }
  if (size_hints.flags & PMinSize)
    {
      if (size_hints.min_width  == 0)
	ewin->hints.min_width = 1;
      else
	ewin->hints.min_width = size_hints.min_width;
      if (size_hints.min_height  == 0)
	ewin->hints.min_height = 1;
      else
	ewin->hints.min_height = size_hints.min_height;
    }
  else
    {
      ewin->hints.min_width = 1;
      ewin->hints.min_height = 1;
    }
  if (size_hints.flags & PMaxSize)
    {
      if (size_hints.max_width  == 0)
	ewin->hints.max_width = 1;
      else
	ewin->hints.max_width = size_hints.max_width;
      if (size_hints.max_height == 0)
	ewin->hints.max_height = 1;
      else
	ewin->hints.max_height = size_hints.max_height;
    }
  else
    {
      ewin->hints.max_width = -1;
      ewin->hints.max_height = -1;
    }
  verify_max_min(ewin);
}

void	verify_max_min(t_win	*ewin)
{
  int	tmp;
  
  if (ewin->hints.max_width != -1)
    if (ewin->hints.max_width < ewin->hints.min_width)
      {
	tmp = ewin->hints.max_width;
	ewin->hints.max_width = ewin->hints.min_width;
	ewin->hints.min_width = tmp;
      }
  if (ewin->hints.max_height != -1)
    if (ewin->hints.max_height < ewin->hints.min_height)
      {
	tmp = ewin->hints.max_height;
	ewin->hints.max_height = ewin->hints.min_height;
	ewin->hints.min_height = tmp;
      }
}

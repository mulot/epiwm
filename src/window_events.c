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
 ** window_events.c
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Mon Feb 22 15:30:57 1999 Poindessous Thomas
 ** Last update Tue May 23 07:55:33 2000 Julien Mulot
 */

#include "epiwm.h"

void		dest_win()
{
  Window	win;  
  t_win		*ewin;
  
  win = gl_event.xdestroywindow.window;
  ewin = search_elt(win);
  internal_destroy(ewin);   
}

void	internal_destroy(ewin)
t_win	*ewin;
{
  if (ewin)
    {
      XDestroyWindow(gl_dpy,ewin->parent);
      if (ewin->iconify_title_win)
	{
	  XDestroyWindow(gl_dpy, ewin->iconify_title_win);
	  iconify_rm_elt(ewin);
	}
      if (ewin->iconify_win)
	XDestroyWindow(gl_dpy, ewin->iconify_win);
      remove_elt(ewin->win);
      if (ewin == gl_epiwin.current)
	{
	  gl_epiwin.current = NULL;
	  XSetInputFocus(gl_dpy, PointerRoot,
			 RevertToPointerRoot, CurrentTime);
	}
    }
}

void	resize_win()
{
  internal_resize_win(gl_event.xresizerequest.window,
		      gl_event.xresizerequest.width,
		      gl_event.xresizerequest.height);  
}

void			internal_resize_win(win, width, height)
Window			win;
int			width;
int			height;
{
  t_win			*ewin;
  XWindowAttributes	attrib;
  
  ewin	= search_elt(win);
  if (ewin)
    {
      if (ewin->iconify_title_win != win &&
	  ewin->titlebar_win != win && ewin->iconify_win != win)
	{
	  verify_max_min(ewin);
	  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
	  if ((gl_epiwin.titlebar.position == TOP)
	      || (gl_epiwin.titlebar.position == BOTTOM))
	    window_group_resize(ewin,
				attrib.x,
				attrib.y,
				width + (ewin->border_size +
					 ewin->bf_border)*2,
				height + (ewin->border_size +
					  ewin->titlebar_border)*2
				+ ewin->titlebar_height
				+ ewin->bf_border);
	  else
	    window_group_resize(ewin,
				attrib.x,
				attrib.y,
				width + (ewin->border_size +
					 ewin->titlebar_border)*2
				+ ewin->titlebar_height
				+ ewin->bf_border ,
				height + (ewin->border_size +
					  ewin->bf_border)*2);
	  send_configure_notify(ewin);
	}
    }
}

void				configure_win()
{
  XConfigureRequestEvent	*stct;
  t_win				*ewin;
  XWindowChanges		win_conf;
  int				x;
  int				y;
  int				width;
  int				height;
  Window			dummy_win;
  unsigned int			dummy;
  
  stct = &gl_event.xconfigurerequest;
  ewin = search_elt(stct->window);
  /*  printf("configure request %ld \n", stct->window);*/
  if (ewin == 0)
    {
      win_conf.x = stct->x;
      win_conf.y = stct->y;
      win_conf.width = stct->width;
      win_conf.height = stct->height;
      win_conf.border_width = stct->border_width;
      win_conf.stack_mode = stct->detail;
      win_conf.sibling = stct->above;
      XConfigureWindow(gl_dpy, stct->window, stct->value_mask, &win_conf);
    }
  else if (ewin->win == stct->window)  
    if (stct->value_mask)
      {
	verify_max_min(ewin);
	XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
		     &width, &height, &dummy, &dummy);
	if ((stct->value_mask & CWWidth) || (stct->value_mask & CWHeight))
	  {
	    internal_resize_son(ewin, stct->x, stct->y,
				stct->width, stct->height);
	    send_configure_notify(ewin);
	  }
	else
	  if (((stct->value_mask & CWX) || (stct->value_mask & CWY)) &&
	      ((stct->x != ewin->border_size) ||
	       (stct->y != ewin->border_size)))
	    {
	      XMoveWindow(gl_dpy, ewin->parent,
			  stct->x - stct->border_width -
			  ewin->border_size -
			  ewin->bf_border,
			  stct->y - stct->border_width -
			  ewin->border_size -
			  ewin->titlebar_height -
			  ewin->titlebar_border*2);
	      send_configure_notify(ewin);
	    }
      }
}

void		map_win()
{
  Window	win;  
  t_win		*ewin;
  
  win	= gl_event.xmap.window;
  ewin	= search_elt(win);
  /*  printf("map win %ld\n", win);*/
  if (ewin)
    {
      if (ewin->iconify_title_win != win && ewin->iconify_win != win)
	{
	  window_group_map(ewin);
	  clientSetState(ewin, NormalState);
	}
      add_menu_task_elt(ewin);
    }
  else     
    add_menu_task_elt(create_win(win));
}

void		unmap_win()
{
  Window	parent;
  Window	win;       
  t_win		*ewin;
  XEvent	ev;
  
  parent= gl_event.xunmap.event;
  win	= gl_event.xunmap.window;
  ewin	= search_elt(win);
  /*  printf("unmap %d %p\n",gl_event.xunmap.window, ewin);*/
  if (ewin)
    if (ewin->iconify_title_win != win && ewin->iconify_win != win)
      {
	if (XCheckTypedWindowEvent(gl_dpy, ewin->win, DestroyNotify, &ev))
	  {
	    /*
	       gl_event.xdestroywindow.window = ewin->parent;
	       dest_win();*/
	    internal_destroy(ewin);
	    remove_menu_task_elt(ewin);
	    return;
	  }
	XUnmapWindow(gl_dpy,ewin->parent);
	clientSetState(ewin, WithdrawnState);
	remove_menu_task_elt(ewin);
      }
}

void			prop_not()
{   
  t_win			*ewin;
  XWMHints		*wm_hints;
  char			*old_name;
  char			*name;
  XWindowAttributes	attrib;
  Window		transient;
  
  ewin = search_elt(gl_event.xproperty.window);
  if (ewin)
    {
      if (gl_event.xproperty.atom == XA_WM_NORMAL_HINTS)
	check_size_hints(ewin);
      else if (gl_event.xproperty.atom == EPIWM_WM_NAME)
	{
	  XFetchName(gl_dpy, ewin->parent, &old_name);
	  XFetchName(gl_dpy, gl_event.xproperty.window, &name);
	  if (name)
	    {
	      if (old_name)
		{
		  if (strcmp(old_name, name) == 0)
		    {
		      free(old_name);
		      free(name);
		      return ;
		    }
		}
	      XStoreName(gl_dpy, ewin->parent, name);
	      if ((ewin->prop & NO_TITLE) == 0)
		{
		  XStoreName(gl_dpy, ewin->titlebar_win, name);
/*		  XClearWindow(gl_dpy, ewin->titlebar_win);*/
		}
	      if (gl_epiwin.current == ewin)
		window_state(ewin, "active");
	      else
		window_state(ewin, "inactive");
	      free(name);
	    }
	}
      else if (gl_event.xproperty.atom == EPIWM_WM_ICON_NAME)
	{	     
	  XFetchName(gl_dpy, gl_event.xproperty.window, &name);
	  XGetIconName(gl_dpy, ewin->win, &old_name);
	  if (name)
	    {
	      if (old_name)
		{
		  if (strcmp(old_name, name) == 0)
		    {
		      free(old_name);
		      free(name);
		      return ;
		    }
		}
	      if (ewin->iconify_title_win)
		{
		  XStoreName(gl_dpy, ewin->iconify_title_win, name);
		  XGetWindowAttributes(gl_dpy,
				       ewin->iconify_title_win, &attrib);
		  draw_icon(ewin->iconify_title_win, attrib.width, "active");
		  free(name);
		}
	      XSetIconName(gl_dpy, ewin->win, name);
	      XSetIconName(gl_dpy, ewin->parent, name);
	    }
	  
	}
      else if (gl_event.xproperty.atom == XA_WM_HINTS)
	{
	  wm_hints = XGetWMHints(gl_dpy, ewin->win);
	  if (wm_hints != NULL)
	    {
	      if (wm_hints->flags & IconPixmapHint);
	      /*printf("pixmap hints\n");*/
	      if (wm_hints->flags & IconWindowHint);
		/*printf("window hints\n");*/
	      if (wm_hints->flags & IconMaskHint);
	      /*printf("mask hints\n");*/
	      if (wm_hints->flags & IconPositionHint);
	      /*printf("position hints\n");*/
	    }
	}
      else if (gl_event.xproperty.atom == XA_WM_TRANSIENT_FOR)
	{
	  if (XGetTransientForHint(gl_dpy, ewin->win, &transient))
	    if (transient != gl_scr_info.root_win)
	      map_request(transient);
	}
    }
}

void	map_req()
{
  map_request(gl_event.xmaprequest.window);
}

void	map_request(win)
Window	win;
{
  t_win		*ewin;
  XWMHints	*wmhints;
  
  ewin	= search_elt(win);
  if (ewin == 0)
    ewin = create_win(win);
  if (ewin)
    {
      if (ewin->iconify_title_win != win && ewin->iconify_win != win)
	{
	  wmhints = XGetWMHints(gl_dpy, win);
	  if (wmhints)
	    if ((wmhints->flags & StateHint) &&
		(wmhints->initial_state == IconicState))
	      {
		XFree(wmhints);
		return ;
	      }
	  if (ewin->iconify_title_win)
	    {
	      XDestroyWindow(gl_dpy, ewin->iconify_title_win);
	      if (ewin->iconify_win)
		XUnmapWindow(gl_dpy, ewin->iconify_win);
	      iconify_rm_elt(ewin);
	    }
	  if ((gl_epiwin.prop & SMART_PLACE)
	      && (!(gl_epiwin.prop & FIRST_START)))
	    placement_smart(ewin);
	  window_group_map(ewin);
	  XRaiseWindow(gl_dpy, ewin->parent);
	  clientSetState(ewin, NormalState);
	  XFree(wmhints);
	}    
    }
}

void		internal_resize_son(ewin, son_x, son_y,
				    son_width, son_height)
t_win		*ewin;
int		son_x;
int		son_y;
int		son_width;
int		son_height;
{
  int		x;
  int		y;
  int		width;
  int		height;
  Window	dummy_win;
  unsigned int	dummy;
  
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &y,
	       &width, &height, &dummy, &dummy);
  if ((son_x != ewin->border_size) ||
      (son_y != ewin->border_size))
    {
      /* this is a piece of shit !
	 x += (son_x - ewin->border_size);
	 y += (son_y - ewin->border_size);
	 */
      x = son_x;
      y = son_y;
    }
  if ((gl_epiwin.titlebar.position == TOP) ||
      (gl_epiwin.titlebar.position == BOTTOM))
    {
      window_group_resize(ewin, x, y, son_width + 2 *
			  (ewin->bf_border + ewin->border_size),
			  son_height + ewin->bf_border +
			  2 * (ewin->border_size + ewin->titlebar_border)
			  + ewin->titlebar_height);
    }
  else if ((gl_epiwin.titlebar.position == LEFT) ||
	   (gl_epiwin.titlebar.position == RIGHT))
    {
      window_group_resize(ewin, x, y, son_width + ewin->bf_border
			  + 2 * (ewin->border_size + ewin->titlebar_border)
			  + ewin->titlebar_height,
			  son_height + 
			  2 * (ewin->border_size + ewin->bf_border));
    }
}







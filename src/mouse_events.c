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
** mouse_events.c
**
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
**
** Started on  Wed Feb 24 15:19:04 1999 Poindessous Thomas
 ** Last update Wed Apr  5 02:28:13 2000 Mulot Julien
*/

#include "epiwm.h"

void			handle_mouse()
{
  Window		child;
  Window		root;
  int			root_x;
  int			root_y;
  int			win_x;
  int			win_y;
  unsigned int		status;
  t_menu_pos		**p_menu;
  
  if (gl_menu.menu_task == gl_event.xbutton.window)
    menu_task_click(gl_event.xmotion.y);
  else if (gl_menu.menu_prop == gl_event.xbutton.window)
    menu_prop_click(gl_event.xmotion.y);
  else if (gl_menu.workspace == gl_event.xbutton.window)
    menu_workspace_click(gl_event.xmotion.y);
  else
    {
      p_menu = &menu_pos;
      while ((*p_menu) != NULL)
	{
	  if (((*p_menu)->win) == gl_event.xbutton.window)
	    break;
	  p_menu = &((*p_menu)->next);
	}
      if ((*p_menu))
	{
	  XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
			&root_x, &root_y, &win_x, &win_y, &status);
	  menu_click(*p_menu, win_y, status);
	}
      else
	menu_display(gl_event.xbutton.window, gl_event.xbutton.x, 
		     gl_event.xbutton.y, gl_event.xbutton.button);
    }
}

void		mouse_on_menu(t_menu_pos	*menu)
{
  int		i;
  int		elt_pos;
  int		text_width;
  t_menu_apps	*p_menu_apps;
  
  p_menu_apps = *(menu->pos);
  elt_pos = gl_event.xmotion.y / gl_menu.height + 1;
  for (i = 0; i < elt_pos; i++)
    {
      p_menu_apps = p_menu_apps->next_elt;
      if (!p_menu_apps)
	break;
    }
  if (p_menu_apps)
    {
      if (gl_menu.current_elt != &(p_menu_apps->elt))
	{
	  menu_app_update(menu->pos, &menu);
	  text_width = XTextWidth(gl_menu.font, p_menu_apps->elt,
				  strlen(p_menu_apps->elt));
	  XDrawString(gl_dpy, menu->win, gl_menu.font_active,
		      (gl_menu.width - text_width) / 2,
		      elt_pos * gl_menu.height, p_menu_apps->elt,
		      strlen(p_menu_apps->elt));
	  if (p_menu_apps->menu)
	    draw_triangle(menu->win, gl_menu.width - 14, 
			  elt_pos * gl_menu.height - 8, "active");
	  gl_menu.current_elt = &(p_menu_apps->elt);
	}
    }
}

void		catch_win_mouse()
{
  Window	child;
  Window	root;
  int		root_x;
  int		root_y;
  int		win_x;
  int		win_y;
  unsigned int	status;
  t_win		*ewin;
  
  XQueryPointer(gl_dpy, gl_scr_info.root_win, &root, &child,
		&root_x, &root_y, &win_x, &win_y, &status);
  ewin = search_elt(child);
  if (ewin)
    if (child == ewin->parent)
      if ((gl_epiwin.focus_mode == FOCUS_SLOPPY) 
	  || (gl_epiwin.focus_mode == FOCUS_FOLLOW_MOUSE))
	window_state(ewin, "active");
}

void		mouse_enter_win()
{
  t_win		*ewin;
  
  if (gl_event.xcrossing.window != gl_scr_info.root_win)
    {
      if (gl_event.xcrossing.window == gl_menu.menu_prop)
	{
	  if (gl_epiwin.focus_mode == FOCUS_FOLLOW_MOUSE)
	    if (gl_menu.prop_type == FUNC_WIN)
	      gl_epiwin.current = gl_epiwin.left_win;
	}
      else
	{
	  ewin = search_elt(gl_event.xcrossing.window);
	  if (ewin)
	    {
	      if (ewin->iconify_title_win == gl_event.xcrossing.window)
		mouse_on_icon(gl_event.xcrossing.window);
	      else if (ewin->parent == gl_event.xcrossing.window)
		{
		  if (gl_epiwin.prop & AUTO_RAISE)
		    XRaiseWindow(gl_dpy, ewin->parent);
		  if ((gl_epiwin.focus_mode == FOCUS_FOLLOW_MOUSE)
		      || (gl_epiwin.focus_mode == FOCUS_SLOPPY))
		    window_state(ewin, "active");
		}
	    }
	}
    }
}

void			mouse_leave_win()
{
  t_win			*ewin;
  t_menu_pos		**menu;
  
  if (gl_event.xcrossing.window != gl_scr_info.root_win)
    {
      if (gl_event.xcrossing.window == gl_menu.menu_task)
	{
	  menu_task_update();
	  gl_menu.current_task = NULL;
	}
      else if (gl_event.xcrossing.window == gl_menu.menu_prop)
	{
	  menu_prop_update();
	  gl_menu.current_prop = NULL;
	}
      else if (gl_event.xcrossing.window == gl_menu.workspace)
	{
	  menu_workspace_update();
	  gl_menu.current_workspace = NULL;
	}
      else
	{
	  ewin = search_elt(gl_event.xcrossing.window);
	  if (ewin)
	    {
	      if (ewin->iconify_title_win == gl_event.xcrossing.window)
		mouse_out_icon(gl_event.xcrossing.window);
	      else if (ewin->parent == gl_event.xcrossing.window)
		{
		  if (gl_epiwin.focus_mode == FOCUS_FOLLOW_MOUSE)
		    {
		      gl_epiwin.left_win = gl_epiwin.current;
		      window_state(ewin, "inactive");
		      gl_epiwin.current = NULL;
		    }
		}
	    }
	  else
	    {
	      menu = &menu_pos;
	      while ((*menu) != NULL)
		{
		  if ((*menu)->win == gl_event.xcrossing.window)
		    break;
		  menu = &((*menu)->next);
		}
	      if (((*menu) != NULL) && ((*menu)->here))
		{
		  gl_menu.current_elt = NULL;
		  if (!gl_menu.grad_type)
		    XClearArea(gl_dpy, (*menu)->win, gl_menu.width - 14, 0,
			       14, menu_set_height(*menu), False);
		  draw_menu((*menu)->pos, menu);
		}
	    }
	}
    }
}

void			mouse_press()
{
  XWindowAttributes	attrib;
  XButtonEvent		*button;
  t_win			*ewin;
  int			dummy_int;
  Window		dummy_win;
  
  button = &gl_event.xbutton;
  gl_win_move.bef = gl_win_move.win;
  gl_win_move.win = button->window;
  gl_win_move.mouse_x = button->x_root;
  gl_win_move.mouse_y = button->y_root;
  gl_win_move.win_x = button->x;   
  gl_win_move.win_y = button->y;
  gl_win_move.time_press = button->time;
  ewin = search_elt(button->window);
  if (ewin)
    {
      /*      if (gl_epiwin.focus_mode == FOCUS_CLICK)*/
      window_state(ewin, "active");
      XGetWindowAttributes(gl_dpy, ewin->border_frame , &attrib);
      gl_win_move.width = attrib.width;
      gl_win_move.height = attrib.height;
      XRaiseWindow(gl_dpy, ewin->parent);
      XGetWindowAttributes(gl_dpy, ewin->parent , &attrib);
      gl_epiwin.opaque_x = attrib.x;
      gl_epiwin.opaque_y = attrib.y;
      gl_epiwin.opaque_width = attrib.width;
      gl_epiwin.opaque_height  = attrib.height;
      gl_win_move.mouse_x = button->x_root - attrib.x;
      gl_win_move.mouse_y = button->y_root - attrib.y;
      gl_epiwin.prop |= IS_WIN_CHG;
      XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &dummy_int,
		   &dummy_int, &gl_epiwin.current_win_width,
		   &gl_epiwin.current_win_height,
		   &gl_epiwin.current_win_border_width, &dummy_int);
      if (gl_menu.here_prop)
	if (gl_menu.prop_type == FUNC_WIN)
	  menu_prop(0, 0, 0, 0);
    }
  if (gl_current_func)
    {
      if (ewin)
	{
	  gl_menu.prop_type = FUNC_WIN;
	  gl_current_func();
	  win_func_terminate();
	}
      else if (button->window == gl_scr_info.root_win)
	win_func_terminate();
    }
  else
    handle_mouse();
}

void		mouse_on_icon(icon)
     Window          icon;
{
  char		*name;
  int		font_direction;
  int		top_size;
  int		bottom_size;
  XCharStruct	dim;
  
#ifdef	  IMLIB
  if (gl_icon.image_active)
    draw_icon_image(icon, "active");
  else
    {
#endif
      XSetWindowBackground(gl_dpy, icon, gl_icon.color_active.pixel);
      XClearWindow(gl_dpy, icon);
#ifdef	  IMLIB
    }
#endif
  XGetIconName(gl_dpy, icon, &name);
  XTextExtents(gl_icon.font, name, strlen(name), &font_direction,
	       &top_size, &bottom_size, &dim);
  if ((dim.width + 6) > gl_icon.width)
    {
      XResizeWindow(gl_dpy, icon, dim.width + 6, gl_icon.height);
      draw_icon(icon, dim.width + 6, "active");
    }
  else
    {
      XResizeWindow(gl_dpy, icon, gl_icon.width, gl_icon.height);
      draw_icon(icon, gl_icon.width, "active");
    }
  free(name);
}

void		mouse_out_icon(Window	icon)
{
#ifdef	  IMLIB
  if (gl_icon.image_active)
    draw_icon_image(icon, "inactive");
  else
    {
#endif
      XSetWindowBackground(gl_dpy, icon, gl_icon.color_inactive.pixel);
      XClearWindow(gl_dpy, icon);
#ifdef	IMLIB
    }
#endif
  XResizeWindow(gl_dpy, icon, gl_icon.width, gl_icon.height);
  draw_icon(icon, gl_icon.width, "inactive");
}

void			mouse_release()
{
  t_mouse_move_win	list;
  
  if (gl_epiwin.opaque_mvt == 1)
    {
      gl_epiwin.opaque_mvt = 0;
      XDrawRectangle(gl_dpy, gl_scr_info.root_win, gc_xor,
		     gl_epiwin.opaque_x, gl_epiwin.opaque_y,
		     gl_epiwin.opaque_width,
		     gl_epiwin.opaque_height);
      XUngrabPointer(gl_dpy, CurrentTime);
      XUngrabServer(gl_dpy);
      XSync(gl_dpy, False);
    }
  list.button = &gl_event.xbutton;
  gl_epiwin.prop &= ~IS_WIN_CHG;
  if (!(Button1Mask == (list.button->state & Button1Mask)))
    return ;
  list.ewin = search_elt(list.button->window);
  if (!list.ewin)
    return ;
  if (list.ewin->button_close == list.button->window)
    win_close(list.ewin, 0, 0);
  else if (list.ewin->button_destroy == list.button->window)
    win_destroy(list.ewin, 0, 0);
  else if (list.ewin->button_shade == list.button->window)
    win_shade(list.ewin, 0, 0);
  else if (list.ewin->button_max_height == list.button->window)
    win_max_height(list.ewin);
  else if (list.ewin->button_max_width == list.button->window)
    win_max_width(list.ewin);
  if (list.ewin->button_max == list.button->window)
    mouse_resize_max(&list);
  else if (list.ewin->button_iconify == list.button->window)
    {
      if ((list.button->x >= 0) && (list.button->y >= 0) &&
	  (list.button->x <= gl_epiwin.btn_width)
	  && (list.button->y <= gl_epiwin.btn_height))
	mouse_iconify(list.ewin);
    }
  else if (((list.ewin->iconify_title_win == list.button->window)
	    || (list.ewin->iconify_win == list.button->window))
	   && ((list.button->time - gl_win_move.time_press) < TIME_RELEASE))
    mouse_uniconify(list.ewin);
  else if (((list.button->time - gl_win_move.time_press) < TIME_RELEASE) &&
	   list.ewin->titlebar_win == list.button->window)
    mouse_shade(&list);
  else if (list.ewin->titlebar_win == list.button->window)
    mouse_move_win(&list);
  else if (list.ewin->win == list.button->window)
    mouse_resize_l_r_d(&list);
  gl_win_move.last = list.button->time;
}

void	mouse_iconify(t_win	*ewin)
{
  char	*name;
  
  XGetIconName(gl_dpy, ewin->parent, &name);
  ewin->iconify_title_win = add_iconify_win(ewin);
  XSetIconName(gl_dpy, ewin->iconify_title_win, name);
  free(name);
  XUnmapWindow(gl_dpy, ewin->parent);
  if (gl_icon.showed)
    {
      if (ewin->iconify_win)
	{
	  XLowerWindow(gl_dpy, ewin->iconify_win);
	  XMapWindow(gl_dpy, ewin->iconify_win);
	}
      XLowerWindow(gl_dpy, ewin->iconify_title_win);
      XMapWindow(gl_dpy, ewin->iconify_title_win);
#ifdef	  IMLIB
      if (gl_icon.image_inactive)
	draw_icon_image(ewin->iconify_title_win, "inactive");
#endif
      draw_icon(ewin->iconify_title_win, gl_icon.width, "inactive");
    }
  clientSetState(ewin, IconicState);	
}

void	mouse_uniconify(t_win	*ewin)
{
  XDestroyWindow(gl_dpy, ewin->iconify_title_win);
  if (ewin->iconify_win)
    XUnmapWindow(gl_dpy, ewin->iconify_win);
  iconify_rm_elt(ewin);
  XMapWindow(gl_dpy, ewin->parent);
  clientSetState(ewin, NormalState);
}

void			mouse_move_win(t_mouse_move_win	*list)
{
  int			x_root;
  int			y_root;
  t_coord		*coord;
  
  if (gl_epiwin.prop & OPAQ_MOVE)
    {
      coord = window_on_border(list->ewin,
			       gl_epiwin.opaque_x, gl_epiwin.opaque_y);
      XMoveWindow(gl_dpy, list->ewin->parent,
		  coord->x, coord->y);
      if (coord->x == gl_epiwin.opaque_x)
	gl_epiwin.cursor_position_x = -1;
      if (coord->y == gl_epiwin.opaque_y)
	gl_epiwin.cursor_position_y = -1;
      free (coord);
    }
  else
    {
      x_root = gl_event.xmotion.x_root - gl_win_move.mouse_x;
      y_root = gl_event.xmotion.y_root - gl_win_move.mouse_y;
      coord = window_on_border(list->ewin, x_root, y_root);
      XMoveWindow(gl_dpy, list->ewin->parent, coord->x, coord->y);
      if (coord->x == x_root)
	gl_epiwin.cursor_position_x = -1;
      if (coord->y == y_root)
	gl_epiwin.cursor_position_y = -1;
      free (coord);      
    } 
  clientSetState(list->ewin, NormalState);
  send_configure_notify(list->ewin);
}

void			mouse_resize_l_r_d(t_mouse_move_win	*list)
{
  window_group_resize(list->ewin, gl_epiwin.opaque_x, gl_epiwin.opaque_y,
		      gl_epiwin.opaque_width, gl_epiwin.opaque_height);
  send_configure_notify(list->ewin);
}

void mouse_on_border_notify()
{
  if (gl_event.xmotion.x_root == gl_scr_info.dpy_width - 1)
    wait_before_change_workspace('r');
  else if (gl_event.xmotion.x_root == 0)
    wait_before_change_workspace('l');
  else
    if ((gl_epiwin.current_cursor != gl_epiwin.normal_cursor)
	&& (!gl_current_func))
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.normal_cursor);
}

t_coord		*window_on_border(t_win		*ewin,
				  int		x,
				  int		y)
{
  int		rgt;
  int		dwn;
  t_coord	*coord;
  t_bicoord	*bicoord;
  
  rgt = x + gl_epiwin.current_win_width +
    2 * gl_epiwin.current_win_border_width + gl_epiwin.virtual_x;
  dwn = y + gl_epiwin.current_win_height +
    2 * gl_epiwin.current_win_border_width;
  coord = (t_coord *) xmalloc(sizeof (t_coord));
  coord->x = x;
  coord->y = y;
  if ((rgt >= (gl_epiwin.virtual_x + gl_scr_info.dpy_width)) &&
      (rgt <= (gl_epiwin.virtual_x + gl_scr_info.dpy_width +
	       gl_workspace_time_change)))
    {
      bicoord = set_cursor_position();
      if (((bicoord->x - gl_epiwin.cursor_position_x) <
	   gl_workspace_time_change) &&
	  (bicoord->x >= gl_epiwin.cursor_position_x) &&
	  (bicoord->win_x < x))
	coord->x = gl_scr_info.dpy_width -
	  gl_epiwin.current_win_width -
	  2 * gl_epiwin.current_win_border_width;
      free (bicoord);
    }
  else if (((x + gl_epiwin.virtual_x) <= gl_epiwin.virtual_x) &&
	   ((x + gl_epiwin.virtual_x) >= (gl_epiwin.virtual_x -
					  gl_workspace_time_change)))
    {
      bicoord = set_cursor_position();
      if (((gl_epiwin.cursor_position_x - bicoord->x) <
	   gl_workspace_time_change) &&
	  (bicoord->x <= gl_epiwin.cursor_position_x) &&
	  (bicoord->win_x > x))
	coord->x = 0;
      free (bicoord);
    }
  if ((dwn >= gl_scr_info.dpy_height) &&
      (dwn <= gl_scr_info.dpy_height + gl_workspace_time_change))
    {
      bicoord = set_cursor_position();
      if (((bicoord->y - gl_epiwin.cursor_position_y) <
	   gl_workspace_time_change) &&
	  (bicoord->y >= gl_epiwin.cursor_position_y) &&
	  (bicoord->win_y < y))
	coord->y = gl_scr_info.dpy_height -
	  gl_epiwin.current_win_height -
	  2 * gl_epiwin.current_win_border_width;
      free (bicoord);
    }
  else if ((y <= 0) &&
	   (y >= -gl_workspace_time_change))
    {
      bicoord = set_cursor_position();
      if (((gl_epiwin.cursor_position_y - bicoord->y) <
	   gl_workspace_time_change) &&
	  (bicoord->y <= gl_epiwin.cursor_position_y) &&
	  (bicoord->win_y > y))
	coord->y = 0;
      free (bicoord);
    }
  return (coord);
}

t_bicoord		*set_cursor_position()
{
  Window	dummy_win;
  int		dummy_int;
  int		cur_win_x;
  int		cur_win_y;
  t_bicoord	*bicoord;
  
  bicoord = (t_bicoord *) xmalloc(sizeof (t_bicoord));
  XQueryPointer(gl_dpy, gl_event.xbutton.window, &dummy_win, &dummy_win,
		&(bicoord->x), &(bicoord->y), &cur_win_x, &cur_win_y,
		&dummy_int);
  bicoord->win_x = bicoord->x - cur_win_x;
  bicoord->win_y = bicoord->y - cur_win_y;
  if (gl_epiwin.cursor_position_x == -1)
    gl_epiwin.cursor_position_x = bicoord->x;
  if (gl_epiwin.cursor_position_y == -1)
    gl_epiwin.cursor_position_y = bicoord->y;
  return (bicoord);
}

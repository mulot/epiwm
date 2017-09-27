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
** menu_events.c
** 
** Made by ()
** Login   <mulot_j@epita.fr>
** 
** Started on  Sun Jun 27 02:49:04 1999 
 ** Last update Mon Oct 23 23:38:34 2000 Julien Mulot
*/


#include "epiwm.h"


int	menu_set_height(t_menu_pos *menu_list)
{ 
  if (menu_list->nb_elt)
    return (menu_list->nb_elt * gl_menu.height + gl_menu.font_head);
  else
    return (1);
}


void			menu_workspace(int pos_y, char *type)
{
  XSetWindowAttributes	attrib;
  XWindowAttributes	attrib_menu_prop;
  t_workspace		*p_workspace;
  int			height;
  int			width;
  int			x;
  int			y;
  int			nb;
  
  if (!gl_menu.workspace_type)
    {
      nb = 0;
      p_workspace = gl_epiwin.first_workspace;
      if (gl_menu.prop_type == FUNC_ROOT)
	width = gl_menu.prop_width_root;
      else
	width = gl_menu.prop_width_win;
      XGetWindowAttributes(gl_dpy, gl_menu.menu_prop, &attrib_menu_prop);
      x = attrib_menu_prop.x + width + 1;
      y = attrib_menu_prop.y + pos_y;
      while (p_workspace)
	{
	  nb++;
	  p_workspace = p_workspace->next;
	}
      height = nb * gl_menu.height + gl_menu.font_head;
      width = gl_menu.workspace_width;
      gl_menu.workspace =
	XCreateSimpleWindow(gl_dpy,
			    gl_scr_info.root_win, x, y, width, height, 1,
			    BlackPixel(gl_dpy, gl_scr_info.screen),
			    gl_menu.color.pixel);
      attrib.override_redirect = 1;
      XChangeWindowAttributes(gl_dpy, gl_menu.workspace,
			      CWOverrideRedirect, &attrib);
      XSelectInput(gl_dpy, gl_menu.workspace, ButtonPressMask | ExposureMask
		   | PointerMotionMask | LeaveWindowMask | EnterWindowMask);
#ifdef	  IMLIB
      if (gl_menu.image_name)
	draw_image_menu(gl_menu.workspace);
      else
#endif
	if (gl_menu.grad_type)
	  draw_gradient(NULL, gl_menu.workspace, "menu", "");
      XMapWindow(gl_dpy, gl_menu.workspace);
      if (!strcmp("GoTo", type))
	gl_menu.workspace_type = GOTO;
      else
	gl_menu.workspace_type = SENDTO;
      draw_menu_workspace();
    }
  else
    {
      XDestroyWindow(gl_dpy, gl_menu.workspace);
      gl_menu.workspace_type = NONE;
    }
}


void		destroy_other_menus(t_menu_apps *p_menu, char *name)
{
  t_menu_pos	**p_menu_pos;
  
  while (p_menu)
    {
      if (p_menu->menu)
	{
	  p_menu_pos = &menu_pos;
	  while (strcmp(((*p_menu_pos)->menu_name), p_menu->elt))
	    p_menu_pos = &((*p_menu_pos)->next);
	  if (strcmp((*p_menu_pos)->menu_name, name))
	    destroy_menu(p_menu_pos, name);
	}
      p_menu = p_menu->next_elt;
    }
}


void	destroy_menu(t_menu_pos **p_menu_pos, char *keep_menu)
{
  if (keep_menu)
    while ((*p_menu_pos) != NULL)
      {
	if (((*p_menu_pos)->here) && (!(*p_menu_pos)->stick) &&
	    (strcmp((*p_menu_pos)->menu_name, keep_menu)))
	  {
	    XDestroyWindow(gl_dpy, (*p_menu_pos)->win);
	    (*p_menu_pos)->here = 0;
	  }
	p_menu_pos = &(*p_menu_pos)->next;
      }
  else
    while ((*p_menu_pos) != NULL)
      {
	if (((*p_menu_pos)->here) && (!(*p_menu_pos)->stick))
	  {
	    XDestroyWindow(gl_dpy, (*p_menu_pos)->win);
	    (*p_menu_pos)->here = 0;
	  }
	p_menu_pos = &(*p_menu_pos)->next;
      }
}


void			add_menu_task_elt(t_win *ewin)
{
  char			*name;
  int			nb_elt;
  t_menu_task		**p_menu_task;
  XWindowAttributes	attrib;
  
  if (ewin)
    {
      if (gl_menu.here_task)
	{
	  nb_elt = 1;
	  p_menu_task = &gl_menu_task;
	  while (*p_menu_task)
	    {
	      nb_elt++;
	      if ((*p_menu_task)->elt->parent == ewin->parent)
		return;
	      p_menu_task = &((*p_menu_task)->next_elt);
	    }
	  XFetchName(gl_dpy, ewin->parent, &name);
	  if (name)
	    {
	      if ((XTextWidth(gl_menu.font, name, strlen(name)) 
		   + MENU_WIDTH_SPACE)
		  > gl_menu.task_width)
		gl_menu.task_width = 
		  XTextWidth(gl_menu.font, name,
			     strlen(name)) + MENU_WIDTH_SPACE;
	      free(name);
	    }
	  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
	  if (attrib.map_state == IsViewable)
	    {
	      if (!((ewin->prop) & WINDOW_LIST_SKIP))
		{
		  *p_menu_task = (t_menu_task *) xmalloc(sizeof (t_menu_task));
		  (*p_menu_task)->elt = ewin;
		  (*p_menu_task)->next_elt = NULL;
		  XResizeWindow(gl_dpy, gl_menu.menu_task, gl_menu.task_width,
				nb_elt * gl_menu.height + gl_menu.font_head);
		}
	    }
	}
    }
}


void		remove_menu_task_elt(t_win *ewin)
{
  char		*name;
  int		nb_elt;
  t_menu_task	**p_menu_task;
  t_menu_task	*tmp_menu_task;
  t_menu_task	**tmp2_menu_task;
  
  if (ewin)
    {
      if (gl_menu.here_task)
	{
	  if (!(ewin->iconify_title_win))
	    {
	      nb_elt = 0;
	      gl_menu.task_width = 0;
	      p_menu_task = &gl_menu_task;
	      tmp2_menu_task = p_menu_task;
	      while ((*p_menu_task))
		{
		  nb_elt++;
		  if ((*p_menu_task)->elt->parent == ewin->parent)
		    {
		      nb_elt--;
		      tmp_menu_task = (*p_menu_task)->next_elt;
		      /*
			(*p_menu_task)->elt = NULL;
			(*p_menu_task)->next_elt = NULL;
			free((*p_menu_task));
		      */
		      (*tmp2_menu_task)->next_elt = tmp_menu_task;
		      p_menu_task = &(*tmp2_menu_task);
		    }
		  else
		    {
		      XFetchName(gl_dpy, (*p_menu_task)->elt->parent, &name);
		      if (name)
			{
			  if ((XTextWidth(gl_menu.font, name, strlen(name))
			       > gl_menu.task_width))
			    gl_menu.task_width = XTextWidth(gl_menu.font, name,
							    strlen(name));
			  free(name);
			}
		    }
		  tmp2_menu_task = p_menu_task;
		  p_menu_task = &((*p_menu_task)->next_elt);
		}
	      gl_menu.task_width += MENU_WIDTH_SPACE;
	      if (gl_menu.task_width < MENU_MINI_WIDTH)
		gl_menu.task_width = MENU_MINI_WIDTH;
	      XResizeWindow(gl_dpy, gl_menu.menu_task, gl_menu.task_width,
			    nb_elt * gl_menu.height + gl_menu.font_head);
	    }
	}
    }
}


void			menu_click(t_menu_pos	*menu,
				   int		pos_y,
				   unsigned int	button_state)
{
  int			elt_pos;
  int			i;
  t_menu_apps		*p_menu_apps;
  XWindowAttributes	attrib;
  
  p_menu_apps = (*menu->pos)->next_elt;
  elt_pos = pos_y / gl_menu.height + 1;
  for (i = 0; i < elt_pos - 1; i++)
    {
      p_menu_apps = p_menu_apps->next_elt;
      if (!p_menu_apps)
	break;
    }
  if (p_menu_apps)
    {
      if (!p_menu_apps->menu)
	{
	  if (p_menu_apps->cmd)
	    system(p_menu_apps->cmd);
	  else
	    {
	      if (p_menu_apps->func)
		p_menu_apps->func();
	    }
	  destroy_menu(&menu_pos, NULL);
	}
      else
	{
	  destroy_other_menus((*(menu->pos))->next_elt, p_menu_apps->elt);
	  XGetWindowAttributes(gl_dpy, menu->win, &attrib);
	  menu_apps(p_menu_apps->elt, attrib.x + gl_menu.width + 1,
		    attrib.y + pos_y);
	}
    }
}


void		menu_workspace_click(int pos_y)
{
  t_workspace	*p_workspace;
  int		elt_pos;
  int		i;
  
  elt_pos = pos_y / gl_menu.height;
  p_workspace = gl_epiwin.first_workspace;
  for (i = 0; i < elt_pos; i++)
    {
      if (!p_workspace->next)
	break;
      p_workspace = p_workspace->next;
    }
  if (gl_menu.workspace_type == SENDTO)
    {
      gl_workspace_to_send = i + 1;
      func_sendto();
    }
  else
    go_to_nth_workspace(i + 1);
  menu_prop(NULL, 0, 0, 0);
}


void	menu_prop_click(int pos_y)
{
  int	elt_pos;
  
  elt_pos = pos_y / gl_menu.height;
  if (elt_pos < 
      ((gl_menu.prop_type == FUNC_ROOT) ? WM_FUNCTIONS_NB : FUNC_WIN_NB))
    {
      if (func_tab[elt_pos].func)
	func_tab[elt_pos].func();
    }
  menu_prop(NULL, 0, 0, 0);
}


void		menu_task_click(int pos_y)
{
  int		i;
  int		elt_pos;
  t_menu_task	*apps_list;
  
  if (gl_menu_task)
    {
      apps_list = gl_menu_task;
      elt_pos = pos_y / gl_menu.height;
      for (i = 0; i < elt_pos; i++)
	{
	  if (!apps_list->next_elt)
	    break;
	  apps_list = apps_list->next_elt;
	}
      if (apps_list->elt->iconify_title_win)
	mouse_uniconify(apps_list->elt);
      go_to_nth_workspace(window_workspace(apps_list->elt));
      XRaiseWindow(gl_dpy, apps_list->elt->parent);
      window_state(apps_list->elt, "active");
      if (gl_epiwin.focus_mode == FOCUS_FOLLOW_MOUSE)
	XWarpPointer(gl_dpy, None, apps_list->elt->parent, 0, 0, 0, 0, 
		     gl_epiwin.border_size + 1, gl_epiwin.titlebar.height + 1);
      menu_task(0, 0);
      /*      draw_menu_task();*/
    }
}


void			menu_move(t_menu_pos *menu, int pos_y)
{
  int			elt_pos;
  int			i;
  t_menu_apps		*p_menu_apps;
  XWindowAttributes	attrib;
  
  p_menu_apps = (*menu->pos)->next_elt;
  elt_pos = pos_y / gl_menu.height + 1;
  for (i = 0; i < elt_pos - 1; i++)
    {
      p_menu_apps = p_menu_apps->next_elt;
      if (!p_menu_apps)
	break;
    }
  if (p_menu_apps)
    {
      if (gl_menu.current_menu != &(p_menu_apps->elt))
	{
	  if (p_menu_apps->menu)
	    {
	      destroy_other_menus((*(menu->pos))->next_elt, p_menu_apps->elt);
	      XGetWindowAttributes(gl_dpy, menu->win, &attrib);
	      menu_apps(p_menu_apps->elt, attrib.x + gl_menu.width + 1,
			attrib.y + pos_y);
	      gl_menu.current_menu = &(p_menu_apps->elt);
	    }
	  else
	    {
	      destroy_other_menus((*(menu->pos))->next_elt, menu->menu_name);
	      gl_menu.current_menu = NULL;
	    }
	}
    }
}


void		menu_workspace_move(int pos_y)
{
  int		elt_pos;
  int		text_width;
  int		i;
  t_workspace	*p_workspace;
  
  elt_pos = pos_y / gl_menu.height;
  p_workspace = gl_epiwin.first_workspace;
  for (i = 0; i < elt_pos; i++)
    {
      if (!p_workspace->next)
	break;
      p_workspace = p_workspace->next;
    }
  if (gl_menu.current_workspace != p_workspace->name)
    {
      menu_workspace_update();
      text_width = XTextWidth(gl_menu.font, p_workspace->name,
			      strlen(p_workspace->name));
      XDrawString(gl_dpy, gl_menu.workspace, gl_menu.font_active,
		  (gl_menu.workspace_width - text_width) / 2,
		  (elt_pos + 1) * gl_menu.height,
		  p_workspace->name, strlen(p_workspace->name));
      gl_menu.current_workspace = p_workspace->name;
    }
}  


void	menu_prop_move(int pos_y)
{
  int	elt_pos;
  int	text_width;
  int	width;
  char	*name;
  
  elt_pos = pos_y / gl_menu.height;
  if (gl_menu.prop_type == FUNC_ROOT)
    width = gl_menu.prop_width_root;
  else
    width = gl_menu.prop_width_win;
  if (elt_pos < 
      ((gl_menu.prop_type == FUNC_ROOT) ? WM_FUNCTIONS_NB : FUNC_WIN_NB))
    {
      name = func_tab[elt_pos].name;
      if (gl_menu.current_prop != name)
	{
	  if (gl_menu.workspace_type)
	    menu_workspace(0, NULL);
	  menu_prop_update();
	  text_width = XTextWidth(gl_menu.font, name, strlen(name));
	  XDrawString(gl_dpy, gl_menu.menu_prop, gl_menu.font_active,
		      (width - text_width) / 2,
		      (elt_pos + 1) * gl_menu.height, name, strlen(name));
	  if ((!strcmp("GoTo", name)) || ((!strcmp("SendTo", name))))
	    menu_workspace(pos_y, name);
	  gl_menu.current_prop = name;
	}
    }  
}


void		menu_task_move(int pos_y)
{
  int		i;
  int		elt_pos;
  int		text_width;
  char		*name;
  t_menu_task	*apps_list;
  
  apps_list = gl_menu_task;
  elt_pos = pos_y / gl_menu.height;
  for (i = 0; i < elt_pos; i++)
    {
      if (!apps_list)
	break;
      apps_list = apps_list->next_elt;
    }
  if (apps_list)
    {
      if (gl_menu.current_task != &(apps_list->elt->parent))
	{
	  menu_task_update();
	  XFetchName(gl_dpy, apps_list->elt->parent, &name);
	  if (name)
	    {
	      text_width = XTextWidth(gl_menu.font, name, strlen(name));
	      XDrawString(gl_dpy, gl_menu.menu_task, gl_menu.font_active,
			  (gl_menu.task_width - text_width) / 2,
			  (elt_pos + 1) * gl_menu.height, name, strlen(name));
	      free(name);
	    }
	  gl_menu.current_task = &(apps_list->elt->parent);
	}
    }   
}


void			menu_apps(char *title, int pos_x, int pos_y)
{
  XSetWindowAttributes	attrib;
  t_menu_pos		**p_menu_pos;
  
  p_menu_pos = &menu_pos;
  while (strcmp(((*p_menu_pos)->menu_name), title))
    p_menu_pos = &((*p_menu_pos)->next);
  if (!((*p_menu_pos)->here))
    {
      if ((pos_x + gl_menu.width) > gl_scr_info.dpy_width)
	pos_x = gl_scr_info.dpy_width - gl_menu.width;
      if ((pos_y + menu_set_height(*p_menu_pos)) > gl_scr_info.dpy_height)
	pos_y = gl_scr_info.dpy_height - menu_set_height(*p_menu_pos);
      (*p_menu_pos)->win =
	XCreateSimpleWindow(gl_dpy,
			    gl_scr_info.root_win,
			    pos_x, pos_y,
			    gl_menu.width, menu_set_height(*p_menu_pos),
			    1, BlackPixel(gl_dpy, gl_scr_info.screen),
			    gl_menu.color.pixel);
      attrib.override_redirect = 1;
      XChangeWindowAttributes(gl_dpy, (*p_menu_pos)->win,
			      CWOverrideRedirect, &attrib);
      XSelectInput(gl_dpy, (*p_menu_pos)->win, ButtonPressMask | ExposureMask 
		   | PointerMotionMask | EnterWindowMask | LeaveWindowMask);
#ifdef	  IMLIB
      if (gl_menu.image_name)
	draw_image_menu((*p_menu_pos)->win);
      else
#endif
	if (gl_menu.grad_type)
	  draw_gradient(NULL, (*p_menu_pos)->win, "menu", "");
      XMapWindow(gl_dpy, (*p_menu_pos)->win);
      ((*p_menu_pos)->here) = 1;
      draw_menu((*p_menu_pos)->pos, p_menu_pos);
    }
  else
    destroy_menu(p_menu_pos, NULL);
}


void			menu_task(int pos_x, int pos_y)
{
  XSetWindowAttributes	attrib_set;
  XWindowAttributes	attrib;
  char			*name;
  int			text_width;
  int			nb_elt;
  int			x;
  int			y;
  t_win_list		*apps_list; 
  t_menu_task		**p_menu_task;
  t_menu_task		*tmp;
  
  if (!gl_menu.here_task)
    {
      nb_elt = 0;
      text_width = 0;
      apps_list = gl_epiwin.first_win_group_list;
      p_menu_task = &gl_menu_task;
      while (apps_list)
	{
	  XGetWindowAttributes(gl_dpy, apps_list->p_t_win->parent, &attrib);
	  XFetchName(gl_dpy, apps_list->p_t_win->parent, &name);
	  if (name)
	    {
	      if (XTextWidth(gl_menu.font, name, strlen(name)) > text_width)
		text_width = XTextWidth(gl_menu.font, name, strlen(name));
	      free(name);
	    }
	  if (((attrib.map_state == IsViewable)
	       || (apps_list->p_t_win->iconify_title_win)))
	    {
	      if (!((apps_list->p_t_win->prop) & WINDOW_LIST_SKIP))
		{
		  nb_elt++;
		  while (*p_menu_task)
		    p_menu_task = &((*p_menu_task)->next_elt);
		  *p_menu_task = (t_menu_task *) xmalloc(sizeof (t_menu_task));
		  (*p_menu_task)->elt = apps_list->p_t_win;
		  (*p_menu_task)->next_elt = NULL;
		}
	    }
	  apps_list = apps_list->next_elt;
	}
      if (gl_menu_task)
	{
	  gl_menu.task_width = text_width + MENU_WIDTH_SPACE;
	  if (gl_menu.task_width < MENU_MINI_WIDTH)
	    gl_menu.task_width = MENU_MINI_WIDTH;
	  if ((pos_x + gl_menu.task_width + 2)> gl_scr_info.dpy_width)
	    x = gl_scr_info.dpy_width - (gl_menu.task_width + 2);
	  else
	    x = pos_x;
	  if ((pos_y + (nb_elt + 1) * gl_menu.height + 2)> gl_scr_info.dpy_height)
	    y = gl_scr_info.dpy_height - ((nb_elt + 1) * gl_menu.height + 2);
	  else
	    y = pos_y;
	  gl_menu.menu_task = 
	    XCreateSimpleWindow(gl_dpy,
				gl_scr_info.root_win,
				x, y, gl_menu.task_width,
				nb_elt * gl_menu.height + gl_menu.font_head,
				1, BlackPixel(gl_dpy, gl_scr_info.screen),
				gl_menu.color.pixel);
	  attrib_set.override_redirect = 1;
	  XChangeWindowAttributes(gl_dpy, gl_menu.menu_task,
				  CWOverrideRedirect, &attrib_set);
	  XSelectInput(gl_dpy, gl_menu.menu_task, ButtonPressMask 
		       | ExposureMask | PointerMotionMask | LeaveWindowMask);
#ifdef	  IMLIB
	  if (gl_menu.image_name)
	    draw_image_menu(gl_menu.menu_task);
	  else
#endif
	    if (gl_menu.grad_type)
	      draw_gradient(NULL, gl_menu.menu_task, "menu", "");
	  XMapWindow(gl_dpy, gl_menu.menu_task);
	  draw_menu_task();
	  gl_menu.here_task = 1;
	}
    }
  else
    {
      nb_elt = 0;
      XDestroyWindow(gl_dpy, gl_menu.menu_task);
      for (tmp = gl_menu_task; tmp != NULL; tmp = gl_menu_task)
	{
	  nb_elt++;
	  gl_menu_task = tmp->next_elt;
	  free(tmp);
	}
      gl_menu.current_task = NULL;
      gl_menu.here_task = 0;
    }
}


void			menu_prop(t_win	*ewin,
				  int	pos_x,
				  int	pos_y,
				  char	target)
{
  XSetWindowAttributes	attrib;
  XWindowAttributes	attr_win;
  int			height;
  int			width;
  int			x;
  int			y;
  
  x = y = 0;
  if (!gl_menu.here_prop)
    {
      gl_menu.prop_type = target;
      if (target == FUNC_ROOT)
	{
	  height = WM_FUNCTIONS_NB * gl_menu.height + gl_menu.font_head;
	  width = gl_menu.prop_width_root;
	}
      else
	{
	  height = FUNC_WIN_NB * gl_menu.height + gl_menu.font_head;
	  width = gl_menu.prop_width_win;
	}
      if (ewin)
	{
	  XGetWindowAttributes(gl_dpy, ewin->parent, &attr_win);
	  if ((gl_epiwin.titlebar.position == TOP) || 
	      (gl_epiwin.titlebar.position == BOTTOM))
	    {
	      if ((gl_epiwin.titlebar.width_mode) && 
		  (gl_epiwin.titlebar.align_shape == ALIGN_RIGHT))
		{
		  if ((pos_x + width + 2) > attr_win.width)
		    x = attr_win.x + attr_win.width - (width + 2);
		  else
		    x = attr_win.x + pos_x;
		}
	      else
		{
		  if ((pos_x + width + 2 + 1) > get_titlebar_width(ewin))
		    x = attr_win.x + get_titlebar_width(ewin) - (width + 2) + 1;
		  else
		    x = attr_win.x + pos_x;
		}
	      if (gl_epiwin.titlebar.position == TOP)
		y = attr_win.y + gl_epiwin.titlebar.height 
		  + ewin->titlebar_border;
	      else if (gl_epiwin.titlebar.position == BOTTOM)
		y = attr_win.y + attr_win.height - 1;
	    }
	  else
	    {
	      if ((gl_epiwin.titlebar.width_mode) && 
		  (gl_epiwin.titlebar.align_shape == ALIGN_RIGHT))
		y = attr_win.y + attr_win.height - get_titlebar_width(ewin);
	      else
		y = attr_win.y;
	      if (gl_epiwin.titlebar.position == LEFT)
		x = attr_win.x + gl_epiwin.titlebar.height 
		  + ewin->titlebar_border;
	      else if (gl_epiwin.titlebar.position == RIGHT)
		x = attr_win.x + attr_win.width - 1;
	    }
	}
      else
	{
	  if ((pos_x + width + 2) > gl_scr_info.dpy_width)
	    x = gl_scr_info.dpy_width - (width + 2);
	  else
	    x = pos_x;
	  if ((pos_y + height + 2) > gl_scr_info.dpy_height)
	    y = gl_scr_info.dpy_height - (height + 2);
	  else
	    y = pos_y;
	}
      gl_menu.menu_prop =
	XCreateSimpleWindow(gl_dpy,
			    gl_scr_info.root_win, x, y, width, height, 1,
			    BlackPixel(gl_dpy, gl_scr_info.screen),
			    gl_menu.color.pixel);
      attrib.override_redirect = 1;
      XChangeWindowAttributes(gl_dpy, gl_menu.menu_prop,
			      CWOverrideRedirect, &attrib);
      XSelectInput(gl_dpy, gl_menu.menu_prop, ButtonPressMask | ExposureMask
		   | PointerMotionMask | LeaveWindowMask | EnterWindowMask);
#ifdef	  IMLIB
      if (gl_menu.image_name)
	draw_image_menu(gl_menu.menu_prop);
      else
#endif
	if (gl_menu.grad_type)
	  draw_gradient(NULL, gl_menu.menu_prop, "menu", "");
      XMapWindow(gl_dpy, gl_menu.menu_prop);
      draw_menu_prop(gl_menu.prop_type);
      gl_menu.here_prop = 1;
    }
  else
    {
      if (gl_menu.workspace_type)
	menu_workspace(0, NULL);
      XDestroyWindow(gl_dpy, gl_menu.menu_prop);
      gl_menu.here_prop = 0;
    }
}


void	menu_display(Window		win,
		     int		pos_x,
		     int		pos_y,
		     unsigned int	button_state)
{
  t_win	*ewin;
  
  if (win == gl_scr_info.root_win)
    {
      if (button_state == 1)
	{
	  gl_menu.current_elt = NULL;
	  if (gl_menu_apps)
	    menu_apps(gl_menu_apps->elt, pos_x, pos_y);
	}
      else if (button_state == 2)
	menu_task(pos_x, pos_y);
      else if (button_state == 3)
	menu_prop(NULL, pos_x, pos_y, FUNC_ROOT);
    } 
  else if (button_state == 3)
    {
      if ((ewin = search_elt(win)))
	menu_prop(ewin, pos_x, pos_y, FUNC_WIN);
    }
}

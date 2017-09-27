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
** menu_x.c
**
** Made by (mulot)
** Login   <mulot_j@epita.fr>
**
** Started on  Tue Mar  2 20:52:55 1999 mulot
 ** Last update Mon Oct 30 22:55:36 2000 Julien Mulot
*/


#include "epiwm.h"


#ifdef	  IMLIB
void		draw_image_menu(Window menu)
{
  int		width;
  int		height;
  Pixmap	pixmap;
  
  if (!(gl_menu.image = Imlib_load_image(gl_imlib_data, gl_menu.image_name)))
    {
      fprintf(stderr, "cannot load menu image\n");
      exit(ERROR);
    }
  width = gl_menu.image->rgb_width;
  height = gl_menu.image->rgb_height;
  Imlib_render(gl_imlib_data, gl_menu.image, width, height);  
  pixmap = Imlib_move_image(gl_imlib_data, gl_menu.image);
  XSetWindowBackgroundPixmap(gl_dpy, menu, pixmap);  
  XClearWindow(gl_dpy, menu);
}
#endif


void	draw_menu(t_menu_apps **p_menu_apps, t_menu_pos **p_menu_pos)
{
  int	h_space;
  int	text_width;
  
  h_space = 0;
  p_menu_apps = &((*p_menu_apps)->next_elt);
  if (gl_menu.grad_type)
    draw_gradient(NULL, (*p_menu_pos)->win, "menu", "");
  while ((*p_menu_apps) != NULL)
    {
      h_space += gl_menu.height;
      (*p_menu_pos)->win_pos = h_space;
      text_width = XTextWidth(gl_menu.font, (*p_menu_apps)->elt,
			      strlen((*p_menu_apps)->elt));
      XDrawString(gl_dpy, (*p_menu_pos)->win, gl_menu.font_inactive,
		  (gl_menu.width - text_width) / 2,
		  (*p_menu_pos)->win_pos, (*p_menu_apps)->elt,
		  strlen((*p_menu_apps)->elt));
      if (((*p_menu_apps)->menu) != NULL)
	draw_triangle((*p_menu_pos)->win, gl_menu.width - 14, 
		      (*p_menu_pos)->win_pos - 8, "inactive");
      p_menu_apps = &((*p_menu_apps)->next_elt);
    }
}


void	draw_menu_prop(char target)
{
  int	h_space;
  int	text_width;
  int	width;
  int	i;
  
  h_space = 0;
  if (target == FUNC_ROOT)
    width = gl_menu.prop_width_root;
  else
    width = gl_menu.prop_width_win;
  if (gl_menu.grad_type)
    draw_gradient(NULL, gl_menu.menu_prop, "menu", "");
  for (i = 0; i < WM_FUNCTIONS_NB; i++)
    {
      if ((func_tab[i].type == target) || (target == FUNC_ROOT))
	{
	  h_space += gl_menu.height;
	  text_width = XTextWidth(gl_menu.font, func_tab[i].name, 
				  strlen(func_tab[i].name));
	  XDrawString(gl_dpy, gl_menu.menu_prop, gl_menu.font_inactive,
		      (width - text_width) / 2, h_space,
		      func_tab[i].name, strlen(func_tab[i].name));
	  if (!strcmp("SendTo", func_tab[i].name))
	    draw_triangle(gl_menu.menu_prop, width - 14, 
			  (i + 1) * gl_menu.height - 8, "inactive");
	  else if (!strcmp("GoTo", func_tab[i].name))
	    draw_triangle(gl_menu.menu_prop, width - 14, 
			  (i + 1) * gl_menu.height - 8, "inactive");
	}
    }
}


void		draw_menu_workspace()
{
  t_workspace	*p_workspace;
  int		h_space;
  int		text_width;
  
  h_space = 0;
  p_workspace = gl_epiwin.first_workspace;
  if (gl_menu.grad_type)
    draw_gradient(NULL, gl_menu.workspace, "menu", "");
  else
    XClearWindow(gl_dpy, gl_menu.workspace);
  while (p_workspace)
    {
      h_space += gl_menu.height;
      text_width = XTextWidth(gl_menu.font, p_workspace->name,
			      strlen(p_workspace->name));
      XDrawString(gl_dpy, gl_menu.workspace, gl_menu.font_inactive,
		  (gl_menu.workspace_width - text_width) / 2, h_space,
		  p_workspace->name, strlen(p_workspace->name));
      /*
	if (p_workspace->elt == gl_epiwin.current)
	draw_triangle(gl_menu.workspace, 2, h_space - 8, "active");
      */
      p_workspace = p_workspace->next;
    }
}


void		draw_menu_task()
{
  int		h_space;
  char		*name;
  int		text_width;
  t_menu_task	*apps_list;
  
  h_space = 0;
  apps_list = gl_menu_task;
  if (gl_menu.grad_type)
    draw_gradient(NULL, gl_menu.menu_task, "menu", "");
  else
    XClearWindow(gl_dpy, gl_menu.menu_task);
  while (apps_list)
    {
      h_space += gl_menu.height;
      XFetchName(gl_dpy, apps_list->elt->parent, &name);
      if (name)
	{
	  text_width = XTextWidth(gl_menu.font, name, strlen(name));
	  XDrawString(gl_dpy, gl_menu.menu_task, gl_menu.font_inactive,
		      (gl_menu.task_width - text_width) / 2, h_space,
		      name, strlen(name));
	  free(name);
	}
      if (apps_list->elt == gl_epiwin.current)
	draw_triangle(gl_menu.menu_task, 2, h_space - 8, "active");
      apps_list = apps_list->next_elt;
    }
}


void		draw_triangle(Window win, int x, int y, char *state)
{
  XPoint	points[4];
  
  points[0].x = x;
  points[0].y = y;
  points[1].x = x + 9;
  points[1].y = y + 5;
  points[2].x = x;
  points[2].y = y + 10;
  points[3].x = x;
  points[3].y = y;
  if (strcmp(state, "active"))
    XDrawLines(gl_dpy, win, gl_menu.font_inactive, points, 4,
	       CoordModeOrigin);  
  else
    /*
      XFillPolygon(gl_dpy, win, gl_menu.font_active, points, 4,
      Nonconvex, CoordModeOrigin);
    */
    XDrawLines(gl_dpy, win, gl_menu.font_active, points, 4,
	       CoordModeOrigin);  
}


void	menu_app_update(t_menu_apps **p_menu_apps, t_menu_pos **p_menu_pos)
{
  int	text_width;
  int	i;
  
  i = 0;
  p_menu_apps = &((*p_menu_apps)->next_elt);
  while ((*p_menu_apps) != NULL)
    {
      i++;
      if (gl_menu.current_elt == &((*p_menu_apps)->elt))
	break;
      p_menu_apps = &((*p_menu_apps)->next_elt);
    }
  if ((*p_menu_apps) != NULL)
    {
      if ((*p_menu_apps)->menu)
	{
	  draw_triangle((*p_menu_pos)->win, gl_menu.width - 14, 
			i * gl_menu.height - 8, "inactive");
	}
      text_width = XTextWidth(gl_menu.font, (*p_menu_apps)->elt,
			      strlen((*p_menu_apps)->elt));
      XDrawString(gl_dpy, (*p_menu_pos)->win, gl_menu.font_inactive,
		  (gl_menu.width - text_width) / 2, i * gl_menu.height,
		  (*p_menu_apps)->elt, strlen((*p_menu_apps)->elt));
    }
}


void		menu_workspace_update()
{
  int		i;
  int		text_width;
  t_workspace	*p_workspace;
  
  if (gl_menu.workspace_type)
    {
      i = 0;
      p_workspace = gl_epiwin.first_workspace;
      while (p_workspace)
	{
	  i++;
	  if (gl_menu.current_workspace == p_workspace->name)
	    break;
	  p_workspace = p_workspace->next;
	}
      if (p_workspace)
	{
	  text_width = XTextWidth(gl_menu.font, p_workspace->name,
				  strlen(p_workspace->name));
	  XDrawString(gl_dpy, gl_menu.workspace, gl_menu.font_inactive,
		      (gl_menu.workspace_width - text_width) / 2,
		      i * gl_menu.height, p_workspace->name,
		      strlen(p_workspace->name));
	}
    }
}


void	menu_prop_update()
{
  int	i;
  char	*name;
  int	text_width;
  int	width;
  
  if (gl_menu.here_prop)
    if (gl_menu.current_prop)
      {
	if (gl_menu.prop_type == FUNC_ROOT)
	  width = gl_menu.prop_width_root;
	else
	  width = gl_menu.prop_width_win;
	for (i = 0; gl_menu.current_prop != func_tab[i].name; i++);
	name = func_tab[i].name;
	text_width = XTextWidth(gl_menu.font, name, strlen(name));
	XDrawString(gl_dpy, gl_menu.menu_prop, gl_menu.font_inactive,
		    (width - text_width) / 2,
		    (i + 1) * gl_menu.height, name, strlen(name));
      }
}


void		menu_task_update()
{
  int		i;
  int		text_width;
  char		*name;
  t_menu_task	*apps_list;
  
  if (gl_menu.here_task)
    {
      i = 0;
      apps_list = gl_menu_task;
      while (apps_list)
	{
	  i++;
	  if (gl_menu.current_task == &(apps_list->elt->parent))
	    break;
	  apps_list = apps_list->next_elt;
	}
      if (apps_list)
	{
	  XFetchName(gl_dpy, apps_list->elt->parent, &name);
	  if (name)
	    {
	      text_width = XTextWidth(gl_menu.font, name, strlen(name));
	      XDrawString(gl_dpy, gl_menu.menu_task, gl_menu.font_inactive,
			  (gl_menu.task_width - text_width) / 2,
			  i * gl_menu.height, name, strlen(name));
	      free(name);
	    }
	}
    }
}

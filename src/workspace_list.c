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
 ** workspace_list.c
 ** 
 ** Made by (Stephane RAULT)
 ** Login   <rault_s@epita.fr>
 ** 
 ** Started on  Wed Mar 31 17:51:34 1999 Stephane RAULT
 ** Last update Wed Apr  5 01:31:27 2000 Mulot Julien
 */

#include "epiwm.h"

void		workspace_new(name)
char		*name;
{
  t_workspace	*new_workspace;
  t_workspace	*last;
  
  new_workspace = (t_workspace *) xmalloc(sizeof (t_workspace));
  new_workspace->next = NULL;
  new_workspace->prev = NULL;
  new_workspace->name = (char *) xmalloc(sizeof (char) * (strlen(name) + 1));
  strcpy(new_workspace->name, name);
  new_workspace->first_icon = NULL;
  new_workspace->last_icon = NULL;
  if (!(gl_epiwin.first_workspace))
    {
      gl_epiwin.first_workspace = new_workspace;
      gl_epiwin.current_workspace = gl_epiwin.first_workspace;
    }
  else
    {
      last = gl_epiwin.first_workspace;
      while (last->next)
	last = last->next;
      last->next = new_workspace;
      new_workspace->prev = last;
    }
  gl_epiwin.virtual_width = gl_epiwin.virtual_width + gl_scr_info.dpy_width;
  set_workspace_menu_width(name);
}

void		workspace_remove()
{
  t_workspace	*tmp;
  t_win		*ewin;
  t_win_list	*apps_list;
  int		nb;
  
  nb = 1;
  apps_list = gl_epiwin.first_win_group_list;
  tmp = gl_epiwin.first_workspace;
  while(tmp->next)
    {
      nb++;
      tmp = tmp->next;
    }
  if (apps_list)
    while(apps_list->next_elt)
      {
	ewin = apps_list->p_t_win;
	if (ewin->win_x > (nb * gl_scr_info.dpy_width))
	  {
	    if (!(gl_workspace_cycle))
	      {
		/* remove window */
	      }
	    else
	      if ((ewin->win_x + ewin->win_width) < (nb * gl_scr_info.dpy_width))
		{
		  /* remove window */
		}
	  }
	apps_list = apps_list->next_elt;
      }
  tmp->prev->next = NULL;
  free (tmp->name);
  free(tmp);
  gl_epiwin.virtual_width = gl_epiwin.virtual_width - gl_scr_info.dpy_width;
}








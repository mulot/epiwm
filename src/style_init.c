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
 ** style_init.c
 ** 
 ** Made by ()
 ** Login   <mulot_j@epita.fr>
 ** 
 ** Started on  Mon May 31 02:03:29 1999 
 ** Last update Mon Oct 23 21:58:03 2000 Julien Mulot
 */


#include "epiwm.h"


int		search_app_prop(char *name_app)
{
  t_apps_prop	*p_apps_prop;
  
  p_apps_prop = gl_apps_prop;
  while (p_apps_prop)
    {
      if (name_app)
	if (!strcmp(p_apps_prop->name, name_app))
	  return (p_apps_prop->prop);
      p_apps_prop = p_apps_prop->next;
    } 
  return (0);
}


void	check_app_prop(t_win *ewin, char *name_app)
{
  int	app_prop;
  
  app_prop = search_app_prop(name_app);
  ewin->prop = app_prop;
  if (app_prop)
    {
      if (app_prop & NO_TITLE)
	{
	  ewin->titlebar_height = 0;
	  ewin->titlebar_border = 0;
	}
      if (app_prop & NO_BORDER)
	{
	  ewin->border_size = 0;
	  ewin->bf_border = 0;
	}
    }
}


char		*read_app_prop(char *line)
{
  char		*tokens[1];
  static char	tmpstring[LINE_LEN];
  
  strncpy(tmpstring, line, LINE_LEN);
  tokens[0] = strtok(tmpstring, "\"");
  return (tokens[0]);
}


void	init_properties(char *prop_name, char *prop)
{
  if (!strcmp(prop_name, "NoTitle"))
    *prop |= NO_TITLE;
  else if (!strcmp(prop_name, "NoBorder"))
    *prop |= NO_BORDER;
  else if (!strcmp(prop_name, "Sticky"))
    *prop |= STICKY;
  else if (!strcmp(prop_name, "WindowListSkip"))
    *prop |= WINDOW_LIST_SKIP;
  else if (!strcmp(prop_name, "StayOnTop"))
    *prop |= STAY_ON_TOP;
}


void		style_build(FILE *file)
{
  char		line[LINE_LEN];
  char		*app_return;
  t_apps_prop	**p_apps_prop;
  
  while (fgets(line, LINE_LEN, file) != NULL)
    {
      if ((*line != '#') && (*line != '\t') && (*line != '\n')
	  && (*line != ' '))
	{
	  if (strlen(line) != 1)
	    line[strlen(line) - 1] = ' ';
	  if ((app_return = read_app_prop(line)))
	    {
	      p_apps_prop = &gl_apps_prop;
	      while (*p_apps_prop)
		p_apps_prop = &((*p_apps_prop)->next);
	      *p_apps_prop = (t_apps_prop *) xmalloc(sizeof (t_apps_prop));
	      (*p_apps_prop)->next = NULL;
	      (*p_apps_prop)->name =
		(char *) xmalloc(sizeof (char) * (strlen(app_return) + 1));
	      strcpy((*p_apps_prop)->name, app_return);
	      (*p_apps_prop)->prop = 0;
	      if ((app_return = index_file(line, 1)))
		init_properties(app_return, &(*p_apps_prop)->prop);
	      if ((app_return = index_file(line, 2)))
		init_properties(app_return, &(*p_apps_prop)->prop);
	      if ((app_return = index_file(line, 3)))
		init_properties(app_return, &(*p_apps_prop)->prop);
	      if ((app_return = index_file(line, 4)))
		init_properties(app_return, &(*p_apps_prop)->prop);
	      if ((app_return = index_file(line, 5)))
		init_properties(app_return, &(*p_apps_prop)->prop);
	      if ((app_return = index_file(line, 6)))
		init_properties(app_return, &(*p_apps_prop)->prop);
	    }
	}
    }
}




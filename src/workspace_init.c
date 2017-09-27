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
** workspace_init.c
** 
** Made by ()
** Login   <mulot_j@epita.fr>
** 
** Started on  Mon May 31 21:38:57 1999 
 ** Last update Mon Oct 23 21:54:34 2000 Julien Mulot
*/

#include "epiwm.h"

void	set_workspace_menu_width(char *name)
{
  int	text_width;
  
  if (name)
    {
      text_width = XTextWidth(gl_menu.font, name, strlen(name)); 
      if (text_width > gl_menu.workspace_width)
	gl_menu.workspace_width = text_width;
    }
}


void	workspace_build(FILE *file)
{
  char	line[LINE_LEN];
  char	*tmp;
  
  workspace_new("Main");
  while (fgets(line, LINE_LEN, file) != NULL)
    {
      if ((*line != '#') && (*line != '\t') && (*line != '\n')
	  && (*line != ' '))
	{
	  if (strlen(line) != 1)
	    line[strlen(line) - 1] = ' ';
	  tmp = index_file(line, 0);
	  if (!strcasecmp(tmp, "Workspace"))
	    workspace_new(index_file(line, 1));
	  else if (!strcasecmp(tmp, "WorkspaceChangePercent"))
	    gl_workspace_change_percent = atoi(index_file(line, 1));
	  else if (!strcasecmp(tmp, "WorkspaceResistance"))
	    gl_workspace_time_change = atoi(index_file(line, 1));
	  else if (!strcasecmp(tmp, "WorkspaceCycle"))
	    gl_workspace_cycle = 1;
	}
    }
}


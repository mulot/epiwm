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
** menu.c
**
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
**
** Started on  Thu Feb 18 16:38:48 1999 Julien Mulot
 ** Last update Thu Oct 26 23:50:31 2000 Julien Mulot
*/


#include "epiwm.h"


void		set_dim_menu()
{
  int		font_direction;
  int		top_size;
  int		bottom_size;
  XCharStruct	dim;
  
  if (gl_menu.lngst_str)
    {
      gl_menu.width = (MENU_WIDTH_SPACE
		       + XTextWidth(gl_menu.font, gl_menu.lngst_str,
				    strlen(gl_menu.lngst_str)));
      if (gl_menu.width < MENU_MINI_WIDTH)
	gl_menu.width = MENU_MINI_WIDTH;
    }
  gl_menu.prop_width_root = (MENU_WIDTH_SPACE + 
			     XTextWidth(gl_menu.font, "GoPagerStepRight",
					strlen("GoPagerStepRight")));
  gl_menu.prop_width_win = (MENU_WIDTH_SPACE + 
			    XTextWidth(gl_menu.font, "MaxHeight",
				       strlen("MaxHeight")));
  if (gl_menu.prop_width_root < MENU_MINI_WIDTH)
    gl_menu.prop_width_root = MENU_MINI_WIDTH;
  if (gl_menu.prop_width_win < MENU_MINI_WIDTH)
    gl_menu.prop_width_win = MENU_MINI_WIDTH;
  XTextExtents(gl_menu.font, NULL, 0, &font_direction, 
	       &top_size, &bottom_size, &dim);
  gl_menu.height = top_size + bottom_size + MENU_HEIGHT_SPACE;
  gl_menu.font_head = top_size;
}


char	*extract_str(char *str, char *word)
{
  while (strncasecmp(str, word, strlen(word)))
    str++;
  return (cut_str(str, word));
}


char		*extract_name(char *str)
{
  char		*tokens[2];
  static char	tmpstring[LINE_LEN];
  
  strncpy(tmpstring, str, LINE_LEN);
  tokens[0] = strtok(tmpstring, "\"");
  if (tokens[0] != NULL) 
	  tokens[1] = strtok(NULL, "\"");
  else
	  tokens[1] = NULL;
  return (tokens[1]);
}


void		add_menu_elt(t_menu_pos	**menu_list,
			     char	*elt,
			     char	*cmd,
			     char	type)
{
  t_menu_apps	**p_apps_list;
  int		i;
  
  p_apps_list = (*menu_list)->pos;
  while ((*p_apps_list) != NULL)
    p_apps_list = &((*p_apps_list)->next_elt);
  (*p_apps_list) = xmalloc(sizeof (t_menu_apps));
  (*p_apps_list)->elt = xmalloc(sizeof (char) * (strlen(elt) + 1));
  strcpy((*p_apps_list)->elt, elt);
  if (type == CMD)
    {
      (*p_apps_list)->cmd = xmalloc(sizeof (char) * (strlen(cmd) + 1));
      strcpy((*p_apps_list)->cmd, cmd);
      (*p_apps_list)->func = NULL;
    }
  else
    {
      (*p_apps_list)->cmd = NULL;
      (*p_apps_list)->func = NULL;
      for (i = 0; i < WM_FUNCTIONS_NB; i++)
	{
	  if (!(strcmp(cmd, func_tab[i].name)))
	    (*p_apps_list)->func = func_tab[i].func;
	}
    }
  (*p_apps_list)->next_elt = NULL;
  (*p_apps_list)->menu = NULL;
  (*menu_list)->nb_elt++;
}


void	add_menu_sub(t_menu_apps	**p_apps_list,
		     t_menu_pos	**menu_address,
		     char		*name)
{
  while ((*p_apps_list) != NULL)
    p_apps_list = &((*p_apps_list)->next_elt);
  (*p_apps_list) = xmalloc(sizeof (t_menu_apps));
  (*p_apps_list)->elt = xmalloc(sizeof (char) * (strlen(name) + 1));
  strcpy((*p_apps_list)->elt, name);
  (*p_apps_list)->next_elt = NULL;
  (*p_apps_list)->menu = xmalloc(sizeof (t_menu_apps));
  (*p_apps_list)->menu->elt = xmalloc(sizeof (char) * (strlen(name) + 1));
  strcpy((*p_apps_list)->menu->elt, name);
  (*p_apps_list)->menu->next_elt = NULL;
  (*p_apps_list)->menu->menu = ((*p_apps_list)->menu);
  while ((*menu_address) != NULL)
    menu_address = &((*menu_address)->next);
  (*menu_address) = xmalloc(sizeof (t_menu_pos));
  (*menu_address)->menu_name = xmalloc(sizeof (char) * (strlen(name) + 1));
  strcpy((*menu_address)->menu_name, name);
  (*menu_address)->pos = &((*p_apps_list)->menu);
  (*menu_address)->here = 0;
  (*menu_address)->stick = 0;
  (*menu_address)->win = 0;
  (*menu_address)->nb_elt = 0;
  (*menu_address)->next = 0;
}


void		sub_menu_detect(char *menu_name, char *line)
{
  t_menu_pos	**p_menu_pos;
  
  p_menu_pos = &menu_pos;
  while (((strcmp(((*p_menu_pos)->menu_name), menu_name))
	  != 0) && (((*p_menu_pos)->next) != NULL))
    p_menu_pos = &((*p_menu_pos)->next);
  if (!(strcmp(((*p_menu_pos)->menu_name), menu_name)))
    {
      (*p_menu_pos)->nb_elt++;
      add_menu_sub((*p_menu_pos)->pos, &menu_pos,
		   extract_name(line));
      if (gl_menu.lngst_str)
	{
	  if (strlen(extract_name(line)) > strlen(gl_menu.lngst_str))
	    {
	      free(gl_menu.lngst_str);
	      gl_menu.lngst_str =
		xmalloc(sizeof (char) * (strlen(extract_name(line)) + 1));
	      strcpy(gl_menu.lngst_str, extract_name(line));
	    }
	}
      else
	{
	  gl_menu.lngst_str =
	    xmalloc(sizeof (char) * (strlen(extract_name(line)) + 1));
	  strcpy(gl_menu.lngst_str, extract_name(line));
	}
    }
}


char		*elt_type(char *str)
{
  char          *tokens[3]; 
  static char   tmpstring[LINE_LEN]; 
  int           i; 
  
  i = 0; 
  strncpy(tmpstring, str, LINE_LEN); 
  tokens[i++] = strtok(tmpstring, "\""); 
  while ((tokens[i - 1] != NULL) && i < 3) 
	  tokens[i++] = strtok(NULL, "\""); 
  return (index_file((tokens[2])++, 0)); 
}


void		elt_detect(char *menu_name, char *line)
{
  char		*elt;
  t_menu_pos	**p_menu_pos;
  
  p_menu_pos = &menu_pos;
  while (((strcmp(((*p_menu_pos)->menu_name), menu_name)) != 0)
	 && (((*p_menu_pos)->next) != NULL))
    p_menu_pos = &((*p_menu_pos)->next);
  if (!(strcmp(((*p_menu_pos)->menu_name), menu_name)))
    {
      if (!(strcmp("Exec", elt_type(line))))
	add_menu_elt(p_menu_pos, extract_name(line),
		     extract_str(line, "Exec "), CMD);
      else
	{
	  if (!(strcmp("Func", elt_type(line))))
	    {
	      elt = index_file(extract_str(line, "Func "), 0);
	      add_menu_elt(p_menu_pos, extract_name(line), elt, FUNC);
	    }
	  else
	    {
	      fprintf(stderr, "error in menu file in %s\n", line);
	      exit(ERROR);
	    }
	}
      if (gl_menu.lngst_str)
	{
	  if (strlen(extract_name(line)) > strlen(gl_menu.lngst_str))
	    {
	      free(gl_menu.lngst_str);
	      gl_menu.lngst_str =
		xmalloc(sizeof (char) * (strlen(extract_name(line)) + 1));
	      strcpy(gl_menu.lngst_str, extract_name(line));
	    }
	}
      else
	{
	  gl_menu.lngst_str =
	    xmalloc(sizeof (char) * (strlen(extract_name(line)) + 1));
	  strcpy(gl_menu.lngst_str, extract_name(line));
	}
    }
}


void	menu_detect(char **menu_name, char *line, char *is_menu)
{
  char	*tmp;
  
  tmp = 0;
  if (!strcasecmp(index_file(line, 1), "End"))
    *is_menu = 0;
  else if (strcasecmp(index_file(line, 1), "Main"))
    {
      *is_menu = 1;
      tmp = extract_name(line);
      *menu_name = xmalloc(sizeof (char) * (strlen(tmp) + 1));
      strcpy(*menu_name, tmp);
    }
  else
    {
      *is_menu = 1;
      tmp = extract_name(line);
      *menu_name = xmalloc(sizeof (char) * (strlen(tmp) + 1));
      strcpy(*menu_name, tmp);
      add_menu_sub(&gl_menu_apps, &menu_pos, tmp);
      if (gl_menu.lngst_str)
	{
	  if (strlen(tmp) > strlen(gl_menu.lngst_str))
	    {
	      free(gl_menu.lngst_str);
	      gl_menu.lngst_str =
		xmalloc(sizeof (char) * (strlen(*menu_name) + 1));
	      strcpy(gl_menu.lngst_str, *menu_name);
	    }
	}
      else
	{
	  gl_menu.lngst_str =
	    xmalloc(sizeof (char) * (strlen(*menu_name) + 1));
	  strcpy(gl_menu.lngst_str, *menu_name);
	}
    }
}


void	menu_build(FILE *file)
{
  char	line[LINE_LEN];
  char	*tmp;
  char	*menu_name;
  char	is_menu;
  
  is_menu = 0;
  menu_name = NULL;
  while (fgets(line, LINE_LEN, file) != NULL)
    {
      if ((*line != '#') && (*line != '\t') && (*line != '\n')
	  && (*line != ' '))
	{
	  if (strlen(line) != 1)
	    line[strlen(line) - 1] = ' ';
	  tmp = index_file(line, 0);
	  if ((!strcasecmp(tmp, "-")) && is_menu)
	    elt_detect(menu_name, line);
	  else if ((!strcasecmp(tmp, "+")) && is_menu)
	    sub_menu_detect(menu_name, line);
	  else if (!strcasecmp(tmp, "Menu"))
	    {
	      free(menu_name);
	      menu_detect(&menu_name, line, &is_menu);
	    }
	  else if (!strcasecmp(tmp, "MenuColor"))
	    {
	      if (!strcmp(index_file(line, 1),"H")
		  ||!strcmp(index_file(line, 1),"V"))
		gradient(index_file(line, 2), index_file(line, 3),
			 index_file(line, 1), "menu", "active");
	      else
		init_color(index_file(line, 1), &gl_menu.color);
	    }
	  else if (!strcasecmp(tmp, "MenuFontInactiveColor"))
	    init_font_color(&gl_menu.font_inactive, index_file(line, 1));
	  else if (!strcasecmp(tmp, "MenuFontActiveColor"))
	    init_font_color(&gl_menu.font_active, index_file(line, 1));
#ifdef	IMLIB
	  else if (!strcasecmp(tmp, "MenuImage"))
	    init_image(index_file(line, 1), &gl_menu.image_name);
#endif
	  else if (!strcasecmp(tmp, "MenuAuto"))
	    gl_menu.auto_menu = 1;
	  else if (!strcasecmp(tmp, "MenuFont"))
	    {
	      gl_menu.font = XLoadQueryFont(gl_dpy, index_file(line, 1));
	      XSetFont(gl_dpy, gl_menu.font_active, gl_menu.font->fid);
	      XSetFont(gl_dpy, gl_menu.font_inactive, gl_menu.font->fid);
	    }
	}
    }
  set_dim_menu();
  free(menu_name);
}

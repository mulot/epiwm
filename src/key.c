/*
** key.c
** 
** Made by (Stephane RAULT)
** Login   <rault_s@epita.fr>
** 
** Started on  Fri Apr  9 21:29:44 1999 Stephane RAULT
 ** Last update Mon Oct 23 23:17:11 2000 Julien Mulot
*/


#include "epiwm.h"
#include <ctype.h>


void			handle_keyboard()
{
  char			hit[3];
  KeySym		key;
  XComposeStatus	compose;
  t_shortcut		*shortcut;
  
  XLookupString(&gl_event.xkey, hit, 3, &key, &compose);
  shortcut = (t_shortcut *)xmalloc(sizeof (t_shortcut));
  if (gl_event.xkey.state & ControlMask)
    shortcut->control = 1;
  else
    shortcut->control = 0;
  if (gl_event.xkey.state & Mod1Mask)
    shortcut->alt = 1;
  else
    shortcut->alt = 0;
  if (gl_event.xkey.state & ShiftMask)
    shortcut->shift = 1;
  else
    shortcut->shift = 0;
  shortcut->key = key;
  compare_key_to_list(shortcut);
  free (shortcut);
}


char		*read_modifier_keys(char *line, int index)
{
  char		*tokens[4];
  static char	tmpstring[LINE_LEN];
  int		i;
  
  i = 0;
  strncpy(tmpstring, line, LINE_LEN);
  tokens[i++] = strtok(tmpstring, "+");
  while ((tokens[i - 1] != NULL) && i < 4)
	  tokens[i++] = strtok(NULL, "+");
  if (tokens[i - 1] != NULL)
	  tokens[i - 1] = NULL;
  return (tokens[index]);
}


int	get_key_name(char *str, t_shortcut *key)
     
{
  key->key = XStringToKeysym(str);
  if (key->key == NoSymbol)
    return (0);
  else
    return (1);
}


void			shortcut_list_add(t_shortcut	*shortcut,
					  char		*command,
					  void		(*p_func)())
{
  t_shortcut_list	*tmp;
  t_shortcut_list	*list;
  
  tmp = (t_shortcut_list *)xmalloc(sizeof (t_shortcut_list));
  tmp->shortcut = shortcut;
  tmp->cmd = NULL;
  tmp->p_func = NULL;
  if (command)
    {
      tmp->cmd = (char *)xmalloc(sizeof (char)*(strlen(command)+1));
      strcpy(tmp->cmd, command);
    }
  else if (p_func)
    tmp->p_func = p_func;
  tmp->next = NULL;
  if (gl_key_list == NULL)
    gl_key_list = tmp;
  else
    {
      list = gl_key_list;
      while (list->next)
	list = list->next;
      list->next = tmp;
    }
}


t_shortcut	*create_shortcut(char	control,
				 char	alt,
				 char	shift,
				 KeySym	key)
{
  t_shortcut	*shortcut;
  
  shortcut = (t_shortcut *)xmalloc(sizeof (t_shortcut));
  shortcut->control = control;
  shortcut->alt = alt;
  shortcut->shift = shift;
  shortcut->key = key;
  return (shortcut);
}


void			compare_key_to_list(t_shortcut *shortcut)
{
  int			i;
  t_shortcut_list	*list;
  
  list = gl_key_list;
  while (list)
    {
      if ((list->shortcut->control == shortcut->control) &&
	  (list->shortcut->alt == shortcut->alt) &&
	  (list->shortcut->shift == shortcut->shift) &&
	  (list->shortcut->key == shortcut->key))
	{
	  if (list->cmd)
	    system(list->cmd);
	  else if (list->p_func)
	    {
	      gl_menu.prop_type = FUNC_ROOT;
	      for (i = 0; i < FUNC_WIN_NB; i++)
		if (*(list->p_func) == *(func_tab[i].func))
		  gl_menu.prop_type = FUNC_WIN;
	      if (gl_menu.prop_type == FUNC_ROOT)
		list->p_func();
	      else if (gl_epiwin.current)
		list->p_func();
	    }
	}
      list = list->next;
    }
}


void	check_keys(char	*alt,
		   char	*ctrl,
		   char *shift,
		   char **key,
		   char *key_return)
{
  if (!strcmp("Control", key_return))
    *ctrl = 1;
  else if (!strcmp("Alt", key_return))
    *alt = 1;
  else if (!strcmp("Shift", key_return))
    *shift = 1;
  else if (!(*key))
    (*key) = index_file(key_return, 0);
}


void		key_build(FILE *file)
{
  char		tmp[LINE_LEN];
  char		*key;
  char		*key_comb;
  char		*key_return;
  char		ctrl;
  char		alt;
  char		shift;
  int		i;
  t_shortcut	shortcut;
  
  while (fgets(tmp, LINE_LEN, file) != NULL)
    {
      if ((*tmp != '#') && (*tmp != '\t') && (*tmp != '\n') && (*tmp != ' '))
	{
	  ctrl = 0;
	  alt = 0;
	  shift = 0;
	  key = NULL;
	  if (strlen(tmp) != 1)
	    tmp[strlen(tmp) - 1] = ' ';
	  key_comb = index_file(tmp, 0);
	  if ((key_return = read_modifier_keys(key_comb, 0)))
	    check_keys(&alt, &ctrl, &shift, &key, key_return);
	  if ((key_return = read_modifier_keys(key_comb, 1)))
	    check_keys(&alt, &ctrl, &shift, &key, key_return);
	  if ((key_return = read_modifier_keys(key_comb, 2)))
	    check_keys(&alt, &ctrl, &shift, &key, key_return);
	  if ((key_return = read_modifier_keys(key_comb, 3)))
	    {
	      if (!key)
		key = index_file(key_return, 0);
	    }
	  if (!get_key_name(key, &shortcut))
	    {
	      fprintf(stderr, "bad shortcut key : %s\n", key);
	      exit(ERROR);
	    }
	  if (!strcmp(index_file(tmp, 1), "Exec"))
	    shortcut_list_add(create_shortcut(ctrl, alt, shift, shortcut.key),
			      extract_str(tmp, "Exec "), NULL);
	  else if (!strcmp(index_file(tmp, 1), "Func"))
	    {
	      for (i = 0; i < WM_FUNCTIONS_NB; i++)
		{
		  if (!(strcmp(func_tab[i].name, index_file(tmp, 2))))
		    {
		      shortcut_list_add(create_shortcut(ctrl, alt, shift, 
							shortcut.key), NULL,
					func_tab[i].func);
		    }
		}
	    }
	  else
	    {
	      fprintf(stderr, "error in key reading : %s\n", 
		      index_file(tmp, 1));
	      exit(ERROR);
	    }
	}
      
    }
}


void	add_func_to_list(char *name, void (*p_func)(), int index)
{
  func_tab[index].name = xmalloc(sizeof (char) * (strlen(name) + 1));
  strcpy(func_tab[index].name, name);
  func_tab[index].func = p_func;
}


void		go_to_pager_left_keyb()
{
  Window	child;
  Window	root;
  int		root_x;
  int		root_y;
  int		win_x;
  int		win_y;
  unsigned int	status;
  
  XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
		&root_x, &root_y, &win_x, &win_y, &status);
  go_to_pager('l', root_x, root_y, 100);
}


void		go_to_pager_right_keyb()
{
  Window	child;
  Window	root;
  int		root_x;
  int		root_y;
  int		win_x;
  int		win_y;
  unsigned int	status;
  
  XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
		&root_x, &root_y, &win_x, &win_y, &status);
  go_to_pager('r', root_x, root_y, 100);
}


void		go_to_pager_step_left_keyb()
{
  Window	child;
  Window	root;
  int		root_x;
  int		root_y;
  int		win_x;
  int		win_y;
  unsigned int	status;
  
  XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
		&root_x, &root_y, &win_x, &win_y, &status);
  go_to_pager('l', root_x, root_y, gl_workspace_change_percent);
}


void		go_to_pager_step_right_keyb()
{
  Window	child;
  Window	root;
  int		root_x;
  int		root_y;
  int		win_x;
  int		win_y;
  unsigned int	status;
  
  XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
		&root_x, &root_y, &win_x, &win_y, &status);
  go_to_pager('r', root_x, root_y, gl_workspace_change_percent);
}


void			grab_all_shortcuts(Window win)
{
  t_shortcut_list	*list;
  unsigned int		modifiers;
  int			keycode;
  
  list = gl_key_list;
  while (list)
    {
      modifiers = (((list->shortcut->shift)<<0) |
		   ((list->shortcut->control)<<2) |
		   ((list->shortcut->alt)<<3));
      keycode = XKeysymToKeycode(gl_dpy, list->shortcut->key);
      XGrabKey(gl_dpy, keycode, modifiers, win, True,
	       GrabModeAsync, GrabModeAsync);
      list = list->next;
    }
}

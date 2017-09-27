/*
 ** key.h
 ** 
 ** Made by (Stephane RAULT)
 ** Login   <rault_s@epita.fr>
 ** 
 ** Started on  Fri Apr  9 21:32:02 1999 Stephane RAULT
 ** Last update Sun Sep  5 23:04:35 1999 Thomas Poindessous
 */

#ifndef   __KEY_H__
#define   __KEY_H__

#include "epiwm.h"

typedef struct	s_shortcut
{
  char		control;
  char		alt;
  char		shift;
  KeySym	key;
}		t_shortcut;

typedef struct			s_shortcut_list
{
  t_shortcut			*shortcut;
  char				*cmd;
  void				(*p_func)();
  struct s_shortcut_list	*next;
}				t_shortcut_list;		

void		check_keys(char *, char *, char *, char **, char *);
void		handle_keyboard();
void		init_shortcut(FILE *);
void		parse_shortcut(char *);
void		key_build(FILE *);
char		*read_modifier_keys(char *, int);
int		get_key_name(char *, t_shortcut *);
void		shortcut_list_add(t_shortcut *, char *, void (*)());
t_shortcut	*create_shortcut(char, char, char, KeySym);
void		compare_key_to_list(t_shortcut *);
void		print_left();
void		print_right();
void		print_up();
void		print_down();
void		add_func_to_list(char *, void (*)(), int);
void		go_to_pager_left_keyb();
void		go_to_pager_right_keyb();
void		go_to_pager_step_left_keyb();
void		go_to_pager_step_right_keyb();
void		grab_all_shortcuts(Window);

t_shortcut_list *gl_key_list;

#endif /* __KEY_H__ */



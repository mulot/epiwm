/*
 ** epiwm_functions.h
 ** 
 ** Made by (Julien Mulot)
 ** Login   <mulot_j@epita.fr>
 ** 
 ** Started on  Sun May  9 02:37:52 1999 Julien Mulot
 ** Last update Wed Apr  5 18:52:57 2000 Mulot Julien
 */

#ifndef   __EPIWM_FUNCTIONS_H__
#define   __EPIWM_FUNCTIONS_H__

#define WM_FUNCTIONS_NB 18
#define	FUNC_ROOT 1
#define FUNC_WIN 2
#define FUNC_WIN_NB 10
#define	FUNC_ROOT_NB 8

typedef struct	s_epiwin_functions
{
  char		*name;
  char		type;
  void		(*func)();
}		t_epiwin_functions;

void	init_tab_function();
void	func_close();
void	func_destroy();
void	func_iconify();
void	func_max();
void	func_max_fit();
void	func_max_height();
void	func_max_width();
void	func_resize();
void	func_move();
void	func_sendto();
void	func_shade();
void	quit();
void	restart();
void	win_func_terminate();
void	win_switch();

t_epiwin_functions	func_tab[WM_FUNCTIONS_NB];
void			(*gl_current_func)();
#endif /* __EPIWM_FUNCTIONS_H__ */

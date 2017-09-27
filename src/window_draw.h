/*
** window_draw.h
** 
** Made by (mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Wed Apr 14 01:41:22 1999 mulot
 ** Last update Tue Jan 25 22:38:24 2000 Julien Mulot
*/

#ifndef   __WINDOW_DRAW_H__
#define   __WINDOW_DRAW_H__

#define	GRAD_NB_PIXEL 128

typedef unsigned long	Pixel;

typedef struct		s_color
{
  int			allocated;
  unsigned char		red;
  unsigned char		green;
  unsigned char		blue;
  unsigned long		pixel;
}			t_color;

typedef struct	s_rotchar
{
  int		width;
  int		count;
  XPoint	points[200];
}		t_rotchar;


Pixel	*alloc_linear_gradient(char *, char *, int);
Pixel	*alloc_nonlinear_gradient(char **, int *, int, int);
int	gradient(char *, char *, char *, char *, char *);
void	draw_gradient(t_win *, Window, char *, char *);
void	draw_titlebar(t_win *, Window, char *);
void	draw_titlebar_image(t_win *, char *);
void	draw_titlebar_shape(t_win *);
void	set_background(Window, char *);
void	titlebar_button_state(t_win *, char *);
void	window_state(t_win *, char *);

t_color		from;
t_color		to;
t_rotchar	gl_fn[96];
#endif /* __WINDOW_DRAW_H__ */

/*
 ** button_draw.h
 ** 
 ** Made by (Julien Mulot)
 ** Login   <mulot_j@epita.fr>
 ** 
 ** Started on  Sun Apr 11 17:23:20 1999 Julien Mulot
 ** Last update Fri Aug 13 18:36:47 1999 Julien Mulot
 */

#ifndef   __ICON_DRAW_H__
#define   __ICON_DRAW_H__

void	draw_button_close(Window, char *);
void	draw_button_destroy(Window, char *);
void	draw_button_iconify(Window, char *);
void	draw_button_max(Window, char *);
void	draw_button_max_height(Window, char *);
void	draw_button_max_width(Window, char *);
void	draw_button_resize(Window, char *);
void	draw_button_shade(Window, char *);
void	draw_button_image(Window, char *);
int	set_btn_position(t_win *, char, int);

#endif /* __ICON_DRAW_H__ */

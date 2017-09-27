/*
** gnome.h
** 
** Made by ()
** Login   <mulot_j@epita.fr>
** 
** Started on  Wed Jun 23 18:23:33 1999 
 ** Last update Wed Jun 23 18:29:11 1999 
*/

#ifndef   __GNOME_H__
#define   __GNOME_H__
#ifdef	  __GNOME__

typedef struct	s_gnome
{
  Window	win;
  Atom		atom;
  CARD32	val;
}		t_gnome;

void	init_gnome();

t_gnome	gl_gnome;

#endif __GNOME__
#endif /* __GNOME_H__ */

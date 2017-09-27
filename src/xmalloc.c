/*
** xmalloc.c
** 
** Made by (Mulot Julien)
** Login   <mulot_j@epita.fr>
** 
** Started on  Wed Apr  5 01:07:52 2000 Mulot Julien
 ** Last update Mon Oct 23 23:15:11 2000 Julien Mulot
*/


#include "epiwm.h"


void	*xmalloc(int size)
{
  void	*p;
  
  if ((p = malloc(size)))
    return (p);
  fprintf(stderr, "Error malloc\n");
  exit(ERROR);
}

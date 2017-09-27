/*
** rev_str.c
** 
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Wed Jan 26 03:25:07 2000 Julien Mulot
 ** Last update Wed Jan 26 03:26:02 2000 Julien Mulot
*/

#include "epiwm.h"

int	rev_str(char *str)
{
  int	i;
  int   len_str;
  char	tmp_char;
  
  len_str = strlen(str);
  for (i = 0; i < (len_str / 2); i++)
    {
      tmp_char = *(str + i);
      *(str + i) = *(str + len_str - 1 - i);
      *(str + len_str - 1 - i) = tmp_char;
    }
  return (len_str);
}



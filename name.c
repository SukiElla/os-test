/* name.c*/
#include <stdio.h>
#include <string.h>

#include "filesys.h"
// find the d_ino by name
unsigned int namei(name) /* namei */
char *name;
{
  int i, notfound = 1;
  for (i = 0; ((i < dir.size) && (notfound)); i++) {
    if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[1].d_ino != 0))
      return dir.direct[i].d_ino; /* find */
  }
  /* notfind */
  return 0;
};

unsigned short iname(name) /* iname */
char *name;
{
  int i, notfound = 1;
  for (i = 0; ((i < DIRNUM) && (notfound)); i++)
    if (dir.direct[i].d_ino == 0) {
      notfound = 0;
      break;
    }

  if (notfound) {
    printf("\nThe current directory is full! !\n");
    return 0;
  } else {
    strcpy(dir.direct[i].d_name, name);
    dir.direct[i].d_ino = 1;
    return i;
  }
}

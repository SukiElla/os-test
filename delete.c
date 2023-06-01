/*删除文件函数delete( )(文件名delete.c)*/
#include <stdio.h>
#include <string.h>

#include "filesys.h"

delete (char *filename) {
  unsigned int dinodeid, i;
  struct inode *inode;
  dinodeid = namei(filename);
  if (dinodeid != NULL) inode = iget(dinodeid);
  inode->di_number--;
  for (i = 0; i < dir.size; i++)
  {
      if (dir.direct[i].d_ino == dinodeid)
          break;
  }
  i++;
    while (dir.direct[i].d_ino != 0)
    {
        strcpy(dir.direct[i-1].d_name, dir.direct[i].d_name);
        dir.direct[i-1].d_ino = dir.direct[i].d_ino;
        i++;
    }
    dir.direct[i-1].d_ino = 0;
    dir.size = i - 1;
  iput(inode);
}

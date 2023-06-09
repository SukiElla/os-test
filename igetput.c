/*获取释放i节点内容程序iget()/iput()*/
#include <stdio.h>
#include <stdlib.h>

#include "filesys.h"

struct inode *iget(unsigned int dinodeid) /* iget( ) */
{
  int existed = 0, inodeid;
  long addr;
  struct inode *temp, *newinode;
  inodeid = dinodeid % NHINO;
  if (hinode[inodeid].i_forw == NULL)
    existed = 0;
  else {
    temp = hinode[inodeid].i_forw;
    while (temp) {
      if (temp->i_ino == inodeid)
      /*existed */
      {
        existed = 1;
        temp->i_count++;
        return temp;
      } else /*not existed */
        temp = temp->i_forw;
    };
  }
  /*	1. calculate the addr of the dinode in the file sys column */
  addr = DINODESTART + dinodeid * DINODESIZ;
  /*	2. malloc the new mode */
  newinode = (struct inode *)malloc(sizeof(struct inode));
  /*	3.read the dinode to the mode */
  if (fd == NULL) {
    printf("error");
  }
  fseek(fd, addr, SEEK_SET);
  fread(&(newinode->di_number), DINODESIZ, 1, fd);
  /* 4.put it into hinode[inodeid] queue */
  if (hinode[inodeid].i_forw == NULL) {
    /* hinode[inodeid].i_forw is empty */
    newinode->i_forw = newinode;
    newinode->i_back = newinode;
    hinode[inodeid].i_forw = newinode;
  } else {
    /* hinode[inodeid].i_forw is not empty */
    newinode->i_forw = hinode[inodeid].i_forw;
    newinode->i_back = newinode;
    newinode->i_forw->i_back = newinode;
    hinode[inodeid].i_forw = newinode;
  }
  // /* 5.initialize the mode */
  newinode->i_count = 1;
  newinode->i_flag = 0; /* flag for not update */
  newinode->i_ino = dinodeid;

  newinode->di_size = 3 * (sizeof(struct direct));
  if (dinodeid == 3) {
    newinode->di_size = BLOCKSIZ;
  }

  return newinode;
}

iput(pinode) /* iput ( ) */
    struct inode *pinode;
{
  long addr;
  unsigned int block_num;
  int i;
  if (pinode->i_count > 1) {
    pinode->i_count--;
    return 0;
  } else {
    if (pinode->di_number != 0) {
      /*write back the mode */
      addr = DINODESTART + pinode->i_ino * DINODESIZ;
      fseek(fd, addr, SEEK_SET);
      fwrite(&pinode->di_number, DINODESIZ, 1, fd);
    } else {
      /*	rm the mode & the block of the file in the disk */
      block_num = pinode->di_size / BLOCKSIZ;
      for (i = 0; i < block_num; i++) {
        bfree(pinode->di_addr[i]);
      }
      ifree(pinode->i_ino);
    };

    /*	free the mode in the memory */
    if (pinode->i_forw == NULL)
      pinode->i_back->i_forw = NULL;
    else {
      pinode->i_forw->i_back = pinode->i_back;
      pinode->i_back->i_forw = pinode->i_forw;
    };
    ifree(pinode);
  }
}

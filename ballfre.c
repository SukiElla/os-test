/* ���̿�������ͷź���*/
#include <stdio.h>

#include "filesys.h"

static unsigned int block_buf[BLOCKSIZ];
unsigned int balloc() {
  unsigned int free_block, free_block_num;
  unsigned int i;
  if (filsys.s_nfree == 0) {
    printf("\nDisk Full!!! \n");
    return DISKFULL;
  };
  free_block = filsys.s_free[filsys.s_pfree];
  if (filsys.s_pfree == NICFREE - 1) {
      fseek(fd, DATASTART + (562 - filsys.s_nfree) * BLOCKSIZ, SEEK_SET);
    fread(block_buf, 1, BLOCKSIZ, fd);
    free_block_num = block_buf[NICFREE];
    for (i = 0; i < free_block_num; i++) {
      filsys.s_free[NICFREE - 1 - i] = block_buf[i];
    }
    filsys.s_pfree = NICFREE - free_block_num;
  } else
    filsys.s_pfree++;
  filsys.s_nfree--;
  filsys.s_fmod = SUPDATE;
  return free_block;
}

bfree(unsigned int block_num) {
  int i;
  if (filsys.s_pfree == 0) /* if s-free full */
  {
    block_buf[NICFREE] = NICFREE;
    for (i = 0; i < NICFREE; i++) {
      block_buf[i] = filsys.s_free[NICFREE - 1 - i];
    }
    filsys.s_pfree = NICFREE - 1;
  }
  fwrite(block_buf, 1, BLOCKSIZ, fd);
  filsys.s_nfree++;
  filsys.s_fmod = SUPDATE;
}

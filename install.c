/*install.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesys.h"

int install() {
  int i, j;
  /*0.open the file column */
  fd = fopen("filesystem", "r+w+b");
  if (fd == NULL) {
    printf("\nfilesys can not be loaded\n");
    exit(0);
  }
  /*	1. read the filsys from the superblock */
  fseek(fd, BLOCKSIZ, SEEK_SET);
  fread(&filsys, 1, sizeof(struct filsys), fd);
  /*	2. initialize the mode hash chain */
  for (i = 0; i < NHINO; i++) {
    hinode[i].i_forw = NULL;
  }
  /*	3. initjalize the sys-ofile */
  for (i = 0; i < SYSOPENFILE; i++) {
    sys_ofile[i].f_count = 0;
    sys_ofile[i].f_inode = NULL;
  }
  /*4. initialize the user */
  for (i = 0; i < USERNUM; i++) {
    user[i].u_uid = 0;
    user[i].u_gid = 0;
    for (j = 0; j < NOFILE; j++) {
      user[i].u_ofile[j] = SYSOPENFILE + 1;
    }
  }
  /* 5. read the main directory to initialize the dir */
  cur_path_inode = iget(1);
  dir.size = cur_path_inode->di_size / (sizeof(struct direct));
  for (i = 0; i < DIRNUM; i++) {
    strcpy(dir.direct[i].d_name, "             ");
    dir.direct[i].d_ino = 0;
  }

  for (i = 0; i < dir.size / (BLOCKSIZ / (sizeof(struct direct))); i++) {
    fseek(fd, DATASTART + BLOCKSIZ * cur_path_inode->di_addr[i], SEEK_SET);
    fread(&dir.direct[(BLOCKSIZ / (sizeof(struct direct))) * i], 1, BLOCKSIZ, fd);
  }
  fseek(fd, DATASTART + BLOCKSIZ * cur_path_inode->di_addr[i], SEEK_SET);
  fread(&dir.direct[(BLOCKSIZ / (sizeof(struct direct))) * i], 1,
        cur_path_inode->di_size % BLOCKSIZ, fd);



  /* 6. read the pwd directory to initialize the pwd */
  fseek(fd, DATASTART + BLOCKSIZ * 2, SEEK_SET);
  fread(&pwd, 1, BLOCKSIZ, fd);

  /* 7. read the filsys to initial the filsys */
  fseek(fd, BLOCKSIZ, SEEK_SET);
  fread(&filsys, 1, sizeof(struct filsys), fd);

  return 0;
}

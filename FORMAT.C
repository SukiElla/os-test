/*初始化磁盘格式化程序format.c*/  // #0縮uper
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesys.h"
format() {
  struct inode* inode;
  struct direct dir_buf[BLOCKSIZ / (sizeof(struct direct))];  // one block can contain
                                                   // direct struct number
  struct pwd passwd[BLOCKSIZ / (PWDSIZ + 4)];

  struct filsys filesystem;
  unsigned int block_buf[BLOCKSIZ / sizeof(int)];
  char* buf;
  int i, j;
  /*	creat the file system file */
  fd = fopen("filesystem", "rwb+");
  buf = (char*)malloc((DINODEBLK + FILEBLK + 2) * BLOCKSIZ *
                      sizeof(char));  // #0縮uper
  if (buf == NULL) {
    printf("\nfile system file creat failed! \n");
    exit(0);
  }
  fseek(fd, 0, SEEK_SET);
  fwrite(buf, 1, (DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char), fd);
  // create the disk finish
  /*0.initialize the passwd */
  passwd[0].p_uid = 2116;
  passwd[0].p_gid = 03;
  strcpy(passwd[0].password, "dddd");
  passwd[1].p_uid = 2117;
  passwd[1].p_gid = 03;
  strcpy(passwd[1].password, "bbbb");
  passwd[2].p_uid = 2118;
  passwd[2].p_gid = 04;
  strcpy(passwd[2].password, "abcd");
  passwd[3].p_uid = 2119;
  passwd[3].p_gid = 04;
  strcpy(passwd[3].password, "cccc");
  passwd[4].p_uid = 2220;
  passwd[4].p_gid = 05;
  strcpy(passwd[4].password, "eeee");
  /*	1.creat the main directory and its sub dir etc and the file password */
  inode = iget(0); /* 0 empty dinode id */
  inode->di_mode = DIEMPTY;
  iput(inode);
  inode = iget(1); /* 1 main dir id */
  inode->di_number = 1;
  inode->di_mode = DEFAULTMODE | DIDIR;
  inode->di_size = 3 * (sizeof(struct direct));
  inode->di_addr[0] = 0; /* block 0tfl is used by the main directory */
  strcpy(dir_buf[0].d_name, "..");
  dir_buf[0].d_ino = 1;
  strcpy(dir_buf[1].d_name, ".");
  dir_buf[1].d_ino = 1;
  strcpy(dir_buf[2].d_name, "etc");
  dir_buf[2].d_ino = 2;
  printf("the size is %d\n", sizeof(struct direct));
  fseek(fd, DATASTART, SEEK_SET);
  fwrite(dir_buf, 1, 3 * (sizeof(struct direct)), fd);
  // printf("format.c: the name is %s %d", dir_buf[2].d_name, dir_buf[2].d_ino);



  iput(inode);
  inode = iget(2); /* 2 etc dir id */
  inode->di_number = 1;
  inode->di_mode = DEFAULTMODE | DIDIR;
  inode->di_size = 3 * (sizeof(struct direct));
  inode->di_addr[0] = 0; /* block 0# is used by the etc */
  strcpy(dir_buf[0].d_name, "..");
  dir_buf[0].d_ino = 1;
  strcpy(dir_buf[1].d_name, ".");
  dir_buf[1].d_ino = 2;
  strcpy(dir_buf[2].d_name, "password");
  dir_buf[2].d_ino = 3;
  fseek(fd, DATASTART + BLOCKSIZ * 1, SEEK_SET);
  fwrite(dir_buf, 1, 3 * (sizeof(struct direct)), fd);

  iput(inode);
  inode = iget(3); /* 3 password id */
  inode->di_number = 1;
  inode->di_mode = DEFAULTMODE | DIFILE;
  inode->di_size = BLOCKSIZ;
  inode->di_addr[0] = 2;
  fseek(fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET);
  fwrite(passwd, 1, BLOCKSIZ, fd);

  iput(inode);

  /*	2. initialize the superblock */
  filesystem.s_isize = DINODEBLK;
  filesystem.s_fsize = FILEBLK;
  filesystem.s_ninode = DINODEBLK * BLOCKSIZ / DINODESIZ - 4;
  filesystem.s_nfree = FILEBLK - 3;
  for (i = 0; i < NICINOD; i++) {
    /*	begin with 4, 0,1.2,3, is used by main, etc, password */
    filesystem.s_inode[i] = 4 + i;
  }
  filesystem.s_pinode = 0;
  filesystem.s_rinode = NICINOD + 4;
  /*FILEBLK+1 is a flag of end */
  block_buf[NICFREE - 1] = FILEBLK + 1;
  for (i = 0; i < NICFREE - 1; i++) block_buf[NICFREE - 2 - i] = FILEBLK - i;
  fseek(fd, DATASTART + BLOCKSIZ * (FILEBLK - NICFREE - 1), SEEK_SET);
  fwrite(block_buf, 1, BLOCKSIZ, fd);

  for (i = FILEBLK - NICFREE - 1; i > 2; i -= NICFREE) {
    for (j = 0; j < NICFREE; j++) {
      block_buf[j] = i - j;
    }
    fseek(fd, DATASTART + BLOCKSIZ * (i - 1), SEEK_SET);
    fwrite(block_buf, 1, BLOCKSIZ, fd);
  }
  j = 1;
  for (i = i; i > 2; i--) {
    filesystem.s_free[NICFREE + i - j] = i;
  }
  filesystem.s_pfree = NICFREE - j;
  filesystem.s_pinode = 0;
  fseek(fd, BLOCKSIZ, SEEK_SET);
  fwrite(&filesystem, 1, sizeof(struct filsys), fd);

  fclose(fd);

  printf("format success\n");
}


// #include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "filesys.h"

struct hinode hinode[NHINO];
struct dir dir;
struct file sys_ofile[SYSOPENFILE];
struct filsys filsys;
struct pwd pwd[PWDNUM];
struct user user[USERNUM];
FILE* fd;
struct inode* cur_path_inode;
int user_id;
int file_block;

int main() {
  unsigned short ab_fd1, ab_fd2, ab_fd3, ab_fd4;
  unsigned short bhy_fd1;
  char* buf;

  format();
  install();
  fclose(fd);

  fd = fopen("filesystem", "r+w+x");
  login(2118, "abcd");
  user_id = 0;
  mkdir("a2118");
  _dir();
  chdir("a2118");

  ab_fd1 = creat(2118, "ab_file0.c", 01777);
  _dir();
  printf("\nthe size is %d\n", dir.size);
  buf = (char*)malloc(BLOCKSIZ * 6 + 5);
  write(ab_fd1, buf, BLOCKSIZ * 6 + 5);
  close(user_id, ab_fd1);
  free(buf);

  mkdir("subdir");
  _dir();
  chdir("subdir");
  _dir();
  // ab_fd2 = creat(2118, "file1.c", 01777);
  // buf = (char*)malloc(BLOCKSIZ * 4 + 20);
  // write(ab_fd2, buf, BLOCKSIZ * 4 + 20);
  // close(user_id, ab_fd2);
  // free(buf);
  //
  // chdir("..");
  // ab_fd3 = creat(2118, "_file2.c", 01777);
  // buf = (char*)malloc(BLOCKSIZ * 10 + 255);
  // awrite(ab_fd3, buf, BLOCKSIZ * 3 + 255);
  // close(ab_fd3);
  // free(buf);
  //
  // delete ("ab_file0.c");
  // ab_fd4 = creat(2118, "ab_file3.c", 01777);
  // buf = (char*)malloc(BLOCKSIZ * 8 + 300);
  // write(ab_fd4, buf, BLOCKSIZ * 8 + 300);
  // close(ab_fd4);
  // free(buf);
  //
  // ab_fd3 = aopen(2118, "ab_file2.c", FAPPEND);
  // buf = (char*)malloc(BLOCKSIZ * 3 + 100);
  // awrite(ab_fd3, buf, BLOCKSIZ * 3 + 100);
  // close(ab_fd3);
  // free(buf);
  //
  // _dir();
  // chdir("..");
  // // logout();
  // halt();
}

// open file but not close ,会发生什么

#include <stdio.h>
#include <string.h>

#include "filesys.h"

int login(unsigned short uid, char *passwd) {
  // check the user and put it into the user array
  int i, j;
  for (i = 0; i < PWDNUM; i++) {
    if ((uid == pwd[i].p_uid) && (!strcmp(passwd, pwd[i].password))) {
      for (j = 0; j < USERNUM; j++)
        if (user[j].u_uid == 0) break;
      if (j == USERNUM) {
        printf("\nToo much user in the System, waited to login\n");
        return 0;
      } else {
        user[j].u_uid = uid;
        user[j].u_gid = pwd[i].p_gid;
        user[j].u_default_mode = DEFAULTMODE;
        printf("login success: the uid: %d\n", j);
      }
      break;
    }
  }

  // 0: login failed, 1: login success
  if (i == PWDNUM) {
    printf("\n incorrect password\n");
    return 0;
  } else
    return 1;
}
int logout(unsigned short uid) {
  int i, j, sys_no;
  struct inode *inode;
  for (i = 0; i < USERNUM; i++)
    if (uid == user[i].u_uid) break;
  if (i == USERNUM) {
    printf("\nno such a file\n");
    return 0;
  }
  for (j = 0; j < NOFILE; j++) {
    if (user[i].u_ofile[j] != SYSOPENFILE + 1) {
      /* iput the mode free the sys_ofile and clear the user-ofile */
      sys_no = user[i].u_ofile[j];
      inode = sys_ofile[sys_no].f_inode;
      iput(inode);
      sys_ofile[sys_no].f_count--;
      user[i].u_ofile[j] = SYSOPENFILE + 1;
    }
  }
  return 1;
}

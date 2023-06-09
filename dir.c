/* dir.c*/

#include <stdio.h>
#include <string.h>

#include "filesys.h"
// ls
_dir() /* _dir */
{
  unsigned int di_mode;
  int i, j, k, one;
  struct inode *temp_inode;
  char a[2] = {'.'};
  printf("\nCURRENT DIRECTORY is %d\n", dir.size);
  // list the dir
  for (i = 0; i < dir.size; i++) {
    if (dir.direct[i].d_ino != DIEMPTY) {
        if (i == 0 && strcmp(dir.direct[i].d_name, a) != 0)
        {
            printf(".<dir>\n");
        }
      printf("%s", dir.direct[i].d_name);
      temp_inode = iget(dir.direct[i].d_ino);  // get the inode by ino
      di_mode = temp_inode->di_mode;
      // for (i = 0; i < 9; i++) {
      //   one = di_mode % 2;
      //   di_mode = di_mode / 2;
      //   if (one)
      //     printf("x");
      //   else
      //     printf("-");
      // }
      if ((temp_inode->di_mode & DIFILE)) {  // if file, print the size and the block chain
        //printf("%d\n", temp_inode->di_size);
        //printf("block chain:");
        // for (i = 0; i < temp_inode->di_size / BLOCKSIZ + 1; i++)
        //   printf("%4d", temp_inode->di_addr[i]);
        printf("\n");
      } else
        printf("<dir>\n");
      iput(temp_inode);
    }
  }
}

mkdir(char *dirname) /* mkdir */
{
  int dirid, dirpos;

  struct inode *inode;

  struct direct buf[BLOCKSIZ / (sizeof(struct direct))];
  //memset(buf, 0, sizeof(buf));
  unsigned int block;

  dirid = namei(dirname);  // get the ino by name
  if (dirid != 0) {
    inode = iget(dirid);  // get the inode by ino
    if (inode->di_mode & DIDIR)
      printf("\n%s directory already existed! ! \n");
    else
      printf("\n%s is a file name, &can't create a dir the same name", dirname);

    iput(inode);
    return 0;
  }
  dirpos = iname(dirname);
  inode = ialloc();  // alloc the new inode
  dirid = inode->i_ino;

  dir.direct[dirpos].d_ino = inode->i_ino;
  dir.size++;

  /*	fill the new dir buf */
  strcpy(buf[0].d_name, ".");  // for dir, the fisrt is '.', the second is '..'
  buf[0].d_ino = dirid;
  strcpy(buf[1].d_name, "..");
  buf[1].d_ino = cur_path_inode->i_ino;
  buf[2].d_ino = 0;
  block = balloc();  // alloc the block
  fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
  fwrite(buf, 1, BLOCKSIZ, fd);

  inode->di_size = 2 * (sizeof(struct direct));
  inode->di_number = 1;
  inode->di_mode = user[user_id].u_default_mode | DIDIR;
  inode->di_uid = user[user_id].u_uid;
  inode->di_gid = user[user_id].u_gid;
  inode->di_addr[0] = block;
  iput(inode);

  return 0;
}

chdir(char *dirname) /* chdir */
{
  unsigned int dirid;
  struct inode *inode;
  unsigned short block;
  int i, j, low = 0, high = 0;
  dirid = namei(dirname);
  //  printf("dirid %d\n", dirid);
  if (dirid == NULL) {
    printf("\n%s does not existed\n", dirname);
    return 0;
  }
  inode = iget(dirid);
//    printf("cur: %d %d %d %d  next: %d %d %d %d\n",
//           cur_path_inode->di_addr[0], cur_path_inode->i_ino,
//           cur_path_inode->di_size, cur_path_inode->di_uid,
//           inode->di_addr[0], inode->i_ino,
//           inode->di_size, inode->di_uid);
  if (!access(user_id, inode, user[user_id].u_default_mode)) {
    printf("\nhas not access to the directory %s", dirname);
    iput(inode);
    return 0;
  }

  /* pack the current directory */
  // incorrect?
  // must?
  for (i = 0; i < dir.size; i++)
  {
    for (j = 0; j < DIRNUM; j++)
        if (dir.direct[j].d_ino == 0)
            break;
    memcpy(&dir.direct[j], &dir.direct[i], sizeof(struct direct));
    dir.direct[i].d_ino = 0;
  }
  int o;

  /*	write back the current directory */

  for (i = 0; i < cur_path_inode->di_size / BLOCKSIZ + 1; i++) {
    bfree(cur_path_inode->di_addr[i]);
  }

  for (i = 0; i < dir.size; i += BLOCKSIZ / (sizeof(struct direct))) {
    block = balloc();
    //  printf("%d\n", block);
    cur_path_inode->di_addr[i] = block;
    fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
    fwrite(&dir.direct[0], 1, BLOCKSIZ, fd);//?
  }



//    for (o = 0; o < dir.size; o++)
//    {
//        printf("time 1 %d %s\n", o, dir.direct[o].d_name);
//    }



  cur_path_inode->di_size = dir.size * (sizeof(struct direct));
  iput(cur_path_inode);
  cur_path_inode = inode;
  dir.size = inode->di_size / (sizeof(struct direct));


  /*	read the change dir from disk */
  j = 0;

  for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++) {
    fseek(fd, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
    fread(&dir.direct[0], 1, BLOCKSIZ, fd);
    j += BLOCKSIZ / (sizeof(struct direct));
  }




  return 0;
}

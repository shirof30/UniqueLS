#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>

DIR * dir;
struct dirent * ptr;
char flags[100];
char dct[100];


int main(int argc,char *argv[])
{
  void longlist(char *dct);
  void inode(char *dct);
  void longlisti(char *dct);
  void noarg();
  void noarguments(char*dct);
  void getperms(int mode, char *str);
  int checking = 0;
  if (argc == 1)
  {
      noarg();
  }
  else
  {
    int done = 0;
    memset(dct, 0, sizeof(dct));
    strcpy(flags, argv[1]);
    strcpy(dct, ".");
    if(argc == 2)
    {
      if(flags[0] != '-')
      {
        strcpy(dct,argv[1]);
        noarguments(dct);
      }
    }
    if (argc > 2)
    {
        strcpy(dct, argv[2]);
        strcpy(flags, argv[1]);
    }
    if (argc == 4)
    {
      strcpy(dct, argv[3]);
      strcpy(flags, argv[1]);
      checking == 1;
      longlisti(dct);
      done = 1;
    }
    if ((strcmp(dct, "-l")) == 0 && (strcmp(flags, "-i")) == 0 )
    {
      strcpy(dct,".");
      longlisti(dct);
      done = 1;
    }
    if ((strcmp(dct, "-i")) == 0 && (strcmp(flags, "-l")) == 0 )
    {
      strcpy(dct,".");
      longlisti(dct);
      done = 1;
    }
    if ((strcmp(flags, "-l")) == 0 && done == 0)
    {
        longlist(dct);
    }
    if (strcmp(flags, "-i") == 0&& done ==0)
    {
      inode(dct);
    }
    if (strcmp(flags, "-li") == 0 || strcmp(flags, "-il") == 0 && done ==0)
    {
      longlisti(dct);
    }

  }
}
void noarg()
{
    dir = opendir(".");
    if (dir != NULL)
    {
        while ((ptr = readdir(dir)) != NULL)
        {
          if(ptr -> d_name[0] == '.')
            continue;
          printf("%s  ", ptr->d_name);
        }
        printf("\n");
        closedir(dir);
    }
}
void noarguments(char *dct)
{
    dir = opendir(dct);
    if (dir != NULL)
    {
        while ((ptr = readdir(dir)) != NULL)
        {
          if(ptr -> d_name[0] == '.')
            continue;
          printf("%s\t", ptr->d_name);
        }
        printf("\n");
        closedir(dir);
    }
    else
    {
      dir = opendir(".");
      int found = 0;
      while ((ptr = readdir(dir)) != NULL)
      {
        if(strcmp(dct,ptr->d_name) != 0)
          continue;
        printf("%s",dct);
        printf("\n");
        found = 1;
      }
      if(found == 0)
      {
        printf("File/directory not found");
        printf("\n");
      }
      closedir(dir);
    }
}
void longlist(char *dct)
{
  void getperms(int mode, char *str);
  struct stat status;
  struct tm * mytime = (struct tm *) malloc(sizeof(struct tm));
  char str[10];
  dir = opendir(dct);
  char concat[100];
  char months[12][12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

  if (dir != NULL)
  {
    while((ptr = readdir(dir)) != NULL)
    {
      if(ptr->d_name[0] == '.')
        continue;
      strcpy(concat,dct);
      strcat(concat,"/");
      strcat(concat,ptr->d_name);
      lstat(concat, &status);
      // permission
      getperms(status.st_mode, str);
      // file type and permission
      printf("%s", str);
      // file hard links
      printf(" %ld", status.st_nlink);
      // file owner & owner group
      printf(" %s", getpwuid(status.st_uid)->pw_name);
      printf(" %s", getgrgid(status.st_gid)->gr_name);
      // file size
      printf(" %ld", (long)status.st_size);
      // file time
      mytime = localtime(&status.st_mtime);
      printf(" %s %02d %d %02d:%02d", months[mytime->tm_mon],mytime->tm_mday,mytime->tm_year + 1900,   mytime->tm_hour, mytime->tm_min);
      // file name
      printf(" %s", ptr->d_name);
      printf("\n");
    }
    closedir(dir);
  }
  else
  {
    int found = 0;
    dir = opendir(".");
    while((ptr = readdir(dir)) != NULL)
    {
      if(strcmp(dct,ptr->d_name) != 0)
        continue;
      strcpy(concat,".");
      strcat(concat,"/");
      strcat(concat,ptr->d_name);
      lstat(concat, &status);
      // permission
      getperms(status.st_mode, str);
      // file type and permission
      printf("%s", str);
      // file hard links
      printf(" %ld", status.st_nlink);
      // file owner & owner group
      printf(" %s", getpwuid(status.st_uid)->pw_name);
      printf(" %s", getgrgid(status.st_gid)->gr_name);
      // file size
      printf(" %ld", (long)status.st_size);
      // file time
      mytime = localtime(&status.st_mtime);
      printf(" %s %02d %d %02d:%02d", months[mytime->tm_mon],mytime->tm_mday,mytime->tm_year + 1900,   mytime->tm_hour, mytime->tm_min);
      // file name
      printf(" %s", ptr->d_name);
      printf("\n");
      found = 1;
    }
    if(found == 0)
      printf("directory not found");
    closedir(dir);
  }

}

void getperms(int mode, char *str)
{
  str[0] = '-';
  if(S_ISDIR(mode))
    str[0] = 'd';
  if(S_ISCHR(mode))
    str[0] = 'c';
  if(S_ISBLK(mode))
    str[0] = 'b';

  // IRUSR - IWUSR - IXUSR - IRGRP - IWGRP - IXGRP urutan
  if(mode & S_IRUSR)
    str[1] = 'r';
  else str[1] = '-';

  if(mode & S_IWUSR)
    str[2] = 'w';
  else
    str[2] = '-';

  if(mode & S_IXUSR)
    str[3] = 'x';
  else
    str[3] = '-';

  if(mode & S_IRGRP)
    str[4] = 'r';
  else
    str[4] = '-';
  if(mode & S_IWGRP)
    str[5] = 'w';
  else
    str[5] = '-';
  if(mode & S_IXGRP)
    str[6] = 'x';
  else
    str[6] = '-';


  //urutan IROTH - IWOTH - IXOTH
  if(mode & S_IROTH)
    str[7] = 'r';
  else
    str[7] = '-';

  if(mode & S_IWOTH)
    str[8] = 'w';
  else
    str[8] = '-';
  if(mode & S_IXOTH)
    str[9] = 'x';
  else
    str[9] = '-';

  str[10] = '\0';
}

void inode(char *dct)
{
    dir = opendir(dct);
    if(dir == NULL)
    {
      dir = opendir(".");
      int found =0;
      while((ptr = readdir(dir)) != NULL)
      {
          if(strcmp(dct,ptr->d_name) != 0)
            continue;
          printf("%ld ", (long)ptr->d_ino);
          printf("%s\t", ptr->d_name);
          found = 1;
      }
      if(found == 0)
        printf("file /directory not found");
        printf("\n");
      return;
    }
    while((ptr = readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
          continue;
        printf("%ld ", (long)ptr->d_ino);
        printf("%s\t", ptr->d_name);
    }
    printf("\n");
    closedir(dir);
}
void longlisti(char *dct)
{
  struct stat status;
  struct tm * mytime = (struct tm *) malloc(sizeof(struct tm));
  char str[12];
  char months[12][12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  dir = opendir(dct);
  char concat[100];
  if (dir != NULL)
  {
    while((ptr = readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
          continue;
        strcpy(concat,dct);
        strcat(concat,"/");
        strcat(concat,ptr->d_name);
        lstat(concat, &status);
        // permission information
        getperms(status.st_mode, str);
        // file type and permission
        printf("%ld", (long)ptr->d_ino);
        printf(" %s", str);
        // file hard links
        printf(" %ld", status.st_nlink);
        // file owner & owner group
        printf(" %s", getpwuid(status.st_uid)->pw_name);
        printf(" %s", getgrgid(status.st_gid)->gr_name);
        // file size
        printf(" %ld", (long)status.st_size);
        // file time
        mytime = localtime(&status.st_mtime);
        printf(" %s %02d %d %02d:%02d", months[mytime->tm_mon],mytime->tm_mday,mytime->tm_year + 1900,   mytime->tm_hour, mytime->tm_min);
        // file name
        printf(" %s", ptr->d_name);
        printf("\n");
      }
      closedir(dir);
  }
  else
    {
      int found = 0;
      dir = opendir(".");
      while((ptr = readdir(dir)) != NULL)
      {
        if(strcmp(dct,ptr->d_name) != 0)
          continue;
        strcpy(concat,".");
        strcat(concat,"/");
        strcat(concat,ptr->d_name);
        lstat(concat, &status);
        // permission
        getperms(status.st_mode, str);
        // file type and permission
        printf("%ld", (long)ptr->d_ino);
        printf("%s", str);
        // file hard links
        printf(" %ld", status.st_nlink);
        // file owner & owner group
        printf(" %s", getpwuid(status.st_uid)->pw_name);
        printf(" %s", getgrgid(status.st_gid)->gr_name);
        // file size
        printf(" %ld", (long)status.st_size);
        // file time
        mytime = localtime(&status.st_mtime);
        printf(" %s %02d %d %02d:%02d", months[mytime->tm_mon],mytime->tm_mday,mytime->tm_year + 1900,   mytime->tm_hour, mytime->tm_min);
        // file name
        printf(" %s", ptr->d_name);
        printf("\n");
        found = 1;
      }
      if(found == 0)
      {
        printf("directory not found");
        printf("\n");
      }  
      closedir(dir);
    }
}

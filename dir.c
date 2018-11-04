#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>


void dirinfo(char * path){
  DIR *d;
  d = opendir(path);
  int i = 0;

  //finding the number of items in directory
  //HAD SOME HELP FROM STACK OVERFLOW AT THIS PART ONLY
  struct dirent * cpy;

  while (cpy = readdir(d)){
    i += 1;
  }
  
  printf("%d\n", i);
  closedir(d);

  d = opendir(path);
  
  struct dirent *entry = calloc(i, sizeof(struct dirent *));
  entry = readdir(d);

  //print names of files + directories
  while ((entry = readdir(d)) != NULL){
    printf("%s\n", entry->d_name);  
  } 

  closedir(d);

}



int main(){
  dirinfo("/home/bot/Documents/dir");
  return 0;
}

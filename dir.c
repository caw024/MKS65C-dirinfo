#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>


void dirinfo(char * path){
  DIR *d;
  d = opendir(path);
  int numfiles = 0;
  int numdir = 0;

  //finding the number of items in directory
  //HAD SOME HELP FROM STACK OVERFLOW AT THIS PART ONLY (clarified man pg)
  struct dirent * cpy;

  while (cpy = readdir(d)){
    if (cpy->d_type == DT_DIR)
      numdir += 1;
    if (cpy->d_type == DT_REG)
      numfiles += 1;
    
  }
    printf("Total number of files: %d\n", numfiles);
    printf("Total number of directories: %d\n", numdir);
    printf("Total number of things: %d\n", numfiles + numdir);
    closedir(d);

  d = opendir(path);
  
  struct dirent *entry = calloc(numfiles + numdir, sizeof(struct dirent *));
  entry = readdir(d);

  //print names of files + directories
  while (entry != NULL){
    if (entry->d_type == DT_DIR)
          printf("Directory: %s\n", entry->d_name);  

    if (entry->d_type == DT_REG)
          printf("File: %s\n", entry->d_name);
    entry = readdir(d);

  } 

  closedir(d);

}



int main(){
  dirinfo("/home/bot/Documents/dir");
  return 0;
}

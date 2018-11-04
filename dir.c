#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>


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
  char * arrdir[numdir];
  char * arrfile[numfiles];

  int dirc = 0;
  int filec = 0;
  //print names of files + directories
  while (entry != NULL){
    if (entry->d_type == DT_DIR){
	  arrdir[dirc] = entry->d_name;
	  dirc++;
    }
    if (entry->d_type == DT_REG){
         arrfile[filec] = entry->d_name;
	 filec++;
    }
    entry = readdir(d);

  }
  printf("Directories:\n");
  while (dirc--){
    printf("\t %s \n", arrdir[dirc]);
  }

  printf("\nFiles:\n");
  while (filec--){
    printf("\t %s \n", arrfile[filec]);
  }

  closedir(d);

}



int main(){
  //get cwd is needed
  char arr[256];
  char * cwd = getcwd( arr, 100);
  dirinfo(cwd);
  return 0;
}

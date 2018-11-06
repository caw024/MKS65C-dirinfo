#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

char ** sort_strarr(char ** strarr, int l) {
  int i = 1;
  int j;
  char tmp[256];
  while(i < l) {
    j = i;
    while(j && strcasecmp(strarr[j-1], strarr[j]) < 0) {
      strcpy(tmp, strarr[j]);
      strcpy(strarr[j], strarr[j-1]);
      strcpy(strarr[j-1], tmp);
      j--;
    }
    i++;
  }
}

unsigned long getDirSize(char * path){
  char currP[256];
  DIR *d;
  unsigned long size = 0;
  d = opendir(path);
  struct stat * statbuf = malloc(sizeof(struct stat));
  struct dirent * cpy;
  while(cpy = readdir(d)) {
    if(strcmp(cpy->d_name, ".") & strcmp(cpy->d_name, "..")) {
      strcpy(currP, path);
      strcat(currP, "/");
      strcat(currP, cpy->d_name);
      if(cpy->d_type == DT_DIR) {
        size += getDirSize(currP);
      }
      if(cpy->d_type == DT_REG) {
        stat(currP, statbuf); 
        size += statbuf->st_size;
      }
    }
  }
  free(statbuf);
  return size;
}

char * getFileInfo(char * path) {
  char * permStr[8];
  permStr[0] ="---";
  permStr[1] ="--x";
  permStr[2] ="-w-";
  permStr[3] ="-wx";
  permStr[4] ="r--";
  permStr[5] ="r-x";
  permStr[6] ="rw-";
  permStr[7] ="rwx";
  struct stat * statbuf = malloc(sizeof(struct stat));
  char * retStr = malloc(512);
  stat(path, statbuf);
  int i = 2;
  int mode = statbuf->st_mode;
  strcpy(retStr, permStr[mode%8]);
  mode /= 8;
  while(i--) {
    strcat(retStr, permStr[mode%8]);
    mode /= 8;
  }
  strcat(retStr, " ");
  struct passwd *pwd;
  pwd = getpwuid(statbuf->st_uid);
  strcat(retStr, pwd->pw_name);
  strcat(retStr, " ");
  struct group * grp;
  grp = getgrgid(statbuf->st_gid);
  strcat(retStr, grp->gr_name);
  free(statbuf);
  return retStr;
}

void printTree(char * path, char * pre) {
  DIR *d;
  d = opendir(path);
  struct dirent * cpy;
  int size = 0;
  while(cpy = readdir(d)) {
    if(strncmp(cpy->d_name, ".", 1)) {
      size += 1;
    }
  }
  closedir(d);
  d = opendir(path);
  char prenext[256];
  char npath[512];
  strcpy(prenext, pre);
  strcat(prenext, "|  ");
  while(size && (cpy = readdir(d))) {
    if(strncmp(cpy->d_name, ".", 1)) {
      if(cpy->d_type == DT_DIR) {
        printf("%s+--%s\n",pre, cpy->d_name);
        strcpy(npath, path);
        strcat(npath, "/");
        strcat(npath, cpy->d_name);
        printTree(npath, prenext);
      }
      if(cpy->d_type == DT_REG) {
        printf("%s+--%s\n", pre, cpy->d_name);
      }
      size--;
    }
  }
}

void dirinfo(char * path){
  DIR *d;
  d = opendir(path);
  if (d == NULL){
    printf("Something went wrong: %s\n", strerror(errno));
  }
  else{
    int numfiles = 0;
    int numdirs = 0;

    //finding the number of items in directory
    //HAD SOME HELP FROM STACK OVERFLOW AT THIS PART ONLY (clarified man pg)
    struct dirent * cpy;

    while (cpy = readdir(d)){
      if (cpy->d_type == DT_DIR)
	numdirs += 1;
      if (cpy->d_type == DT_REG)
	numfiles += 1;
    
    }
    printf("Total number of files: %d\n", numfiles);
    printf("Total number of directories: %d\n", numdirs);
    printf("Total number of things: %d\n", numfiles + numdirs);
    unsigned long dirBytes = getDirSize(path);
    char ext[16];
    if(dirBytes <= 1024) {
      strcpy(ext, "B");
    }
    if(dirBytes > 1024) {
      strcpy(ext, "KB");
      dirBytes /= 1024;
    }
    if (dirBytes > 1024) {
      strcpy(ext, "MB");
      dirBytes /= 1024;
    }
    if (dirBytes > 1024) {
      strcpy(ext, "GB");
      dirBytes /= 1024;
    }

    printf("Size of directory: %ld%s\n", dirBytes, ext);
    closedir(d);

    d = opendir(path);
  
    struct dirent *entry = calloc(numfiles + numdirs, sizeof(struct dirent *));
    entry = readdir(d);
    char * arrdir[numdirs];
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
    sort_strarr(arrdir, numdirs);
    sort_strarr(arrfile, numfiles);

    printf("Directories:\n");
    while (dirc--){
      printf("\t%s\n", arrdir[dirc]);
    }

    char tmpPath[1024];
    char tmpInf[512];
    printf("\nFiles:\n");
    while (filec--){
      strcpy(tmpPath, path);
      strcat(tmpPath, "/");
      strcat(tmpPath, arrfile[filec]);
      strcpy(tmpInf, getFileInfo(tmpPath));
      printf("\t%s", tmpInf); 
      printf("\t%s\n", arrfile[filec]);
    }
    printf("\nDir Tree:\n");
    printTree(path, "");

    closedir(d);
  }
}



int main(int argc, char * argv[]){
  printf("Provide a path: ");
  scanf("%s",argv[0]);
  dirinfo(argv[0]);
  /* if(argc > 1) { */
  /*   for(int i = 1; i < argc; i++) { */
  /*     dirinfo(argv[i]); */
  /*   } */
  /* } */
  /* else { */
  /*   printf("not a legit path name, showing current directory\n"); */
  /*   dirinfo("."); */
  /* } */
  return 0;
}

/*******************************
 * Headers for dirinfo systems assignment
 *
 *******************************/
#include <sys/types.h>

void listDir(const char * path);

unsigned char isDir(struct dirent * de);

unsigned long dirSize(DIR * dir);

char ** sort_list(char ** strarr, int l);

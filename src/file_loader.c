#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void print_file_open_error(){
  printf("file cannot be opened.\n");
}

char* process_input(char* filename){
  FILE* fp;
  char* input_buf;
  struct stat stat_buf;

  if(stat(filename, &stat_buf) != 0)
    return NULL;
  
  if((fp = fopen(filename, "r")) == NULL)
    return NULL;

  input_buf = (char*)malloc(stat_buf.st_size + 1);
  fread(input_buf, stat_buf.st_size, 1, fp);
  input_buf[stat_buf.st_size] = '\0';

  fclose(fp);

  return input_buf;
}

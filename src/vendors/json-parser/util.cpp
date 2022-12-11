#include <sys/stat.h>
#include <json-parser/util.h>

json_value * json_parse_file(const char * file_name)
{

  json_value *value; 
  json_char* json;
  struct stat filestatus;
  char* file_contents;
  FILE *fp;
  int file_size; //???
  // TODO exception
  if (stat(file_name, &filestatus) != 0) {
    fprintf(stderr, "File %s not found\n", file_name);
  }

  file_size = filestatus.st_size;
  file_contents = (char*)malloc(filestatus.st_size);
  if ( file_contents == NULL) {
    fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
  }

  fp = fopen(file_name, "rt");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open %s\n", file_name);
    fclose(fp);
    free(file_contents);
  }

  if ( fread(file_contents, file_size, 1, fp) != 1 ) {
    fprintf(stderr, "Unable to read content of %s\n", file_name);
    fclose(fp);
    free(file_contents);
  }
  fclose(fp);

  printf("%s\n", file_contents);

  printf("--------------------------------\n\n");


  json  = (json_char*)file_contents;
  value = json_parse(json, file_size);
  free(file_contents);
  return value;
}

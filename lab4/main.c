#include <stdio.h>
#include <dlfcn.h>

int main() {
  void *handle;
  void(*my_str_cat)(char *, char *);
  char *error;

  handle = dlopen("my_strcat.so", RTLD_LAZY);
  if(!handle) {
    printf("%s\n", dlerror());
    exit(1);
  }
  dlerror();
  my_str_cat = dlsym(handle, "my_strcat");
  if((error = dlerror()) != NULL) {
    printf("%s\n", dlerror());
    exit(1);
  }

  


  //Let's test it
  char dest[]= "Hello World";
  char src[] = " again!";
  
  my_str_cat(dest, src);

  printf("%s\n", dest);
  dlclose(handle);
  return 0;
}

#include <stdio.h>
#include <pthread.h>


void *do_stuff(void *p) {
	while(1){
	  printf("hello from thread %d - A\n", *(int *)p);
	  pthread_yield();
	  printf("hello from thread %d - B\n", *(int *)p);
	}	
}

int main() {
        pthread_t thread;
        int id, arg1, arg2;

        arg1 = 1;

        id = pthread_create(&thread, NULL, do_stuff, (void *)&arg1);

        arg2= 2;

        //pthread_yield()   //if any other threads... RUN THEM. pthread_yield()- SUGGESTION for the scheduler to run. NO guarantee.

        do_stuff((void *)&arg2);

        //**pthread_join(thread, NULL);**   Can be seen as a merge. Block caller thread, until specified thread exists(completes). No guarantee

        return 0;
}


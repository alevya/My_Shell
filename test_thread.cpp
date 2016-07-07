#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int i;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
char pathresult[] = "/home/alevya/main.pid";

void* thread(void *args)
{
    bool done = false;
    int a = *(static_cast<int*> (args));
   
    //pthread_t pid = pthread_self();
    //printf("thread %lu", pid);
    int value = 0;
    while(1)
    {
        pthread_mutex_lock(&mtx);
        done = i >= a;
        pthread_mutex_unlock(&mtx); 
        
        if(done)
            break;
        
        pthread_mutex_lock(&mtx);
        printf("thread2 i = %d\n", i);
        value = ++i;
        printf("thread2 value = %d\n", value);
        pthread_mutex_unlock(&mtx); 
        
        sleep(1);
    }
    return  (void *)value;
}

int main(int argc, char** argv) 
{
    FILE * fd;
    fd = fopen(pathresult, "w");
    fprintf(fd, "%d", getpid() );
    fclose(fd);
    
    pthread_t pid;
    void* status;
    
    bool done = false;
    int v = 3;
    pthread_create(&pid, NULL, thread, (void *)&v);
    while(1)
    {
        pthread_mutex_lock(&mtx);
        done = i >=5;
        pthread_mutex_unlock(&mtx);
        
        if(done)
            break;
        
        pthread_mutex_lock(&mtx);
        printf("thread1 i = %d\n", ++i);
        pthread_mutex_unlock(&mtx);
        
        sleep(2);
    }
    
    pthread_join(pid, &status);
    int* s = static_cast<int*>(status);
    printf("code complete thread2 = %d", s);
    
    return 0;
}

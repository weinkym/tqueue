#include "tqueue.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>

const int T_ALPHANUMERIC_COUNT = 62;

char g_alphanumeric_array[T_ALPHANUMERIC_COUNT];
const int T_MAX_LENGTH = 12;
TQueue<char> *p;
int g_finished_count = 0;

char createAlphanumeric()
{
    int index = rand() % T_ALPHANUMERIC_COUNT;
    return g_alphanumeric_array[index];
}

void sord(char *a,int m)
{
    int n = m-1;
    for(int i=0 ; i < n; i++)
    {
        for(int j=0;j< m-1;j++)
        {
            if(a[j]>a[j+1])
            {
                char temp;
                temp = a[j];
                a[j] = a[j+1];
                a[j+1]=temp;
            }
        }
        m--;
    }
}


void product()
{
    /*异步取消， 线程接到取消信号后，立即退出*/
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(true)
    {
        p->produceData(createAlphanumeric());
    }
}

void custom()
{
    /*异步取消， 线程接到取消信号后，立即退出*/
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char *datas = new char[T_MAX_LENGTH];
    while(true)
    {
        for(int i = 0; i < T_MAX_LENGTH;++i)
        {
            datas[i] = p->consumeData();
        }
        sord(datas,T_MAX_LENGTH);
        g_finished_count++;
    }
}

void testCharSort()
{
    /*异步取消， 线程接到取消信号后，立即退出*/
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char *datas = new char[T_MAX_LENGTH];
    while(true)
    {
        for(int i = 0; i < T_MAX_LENGTH;++i)
        {
            datas[i] = createAlphanumeric();
        }
        sord(datas,T_MAX_LENGTH);
        g_finished_count++;
    }
}

int main(int argc, char *argv[])
{
    int index = 0;
    for(int i = 0; i < 10; ++i)
    {
        g_alphanumeric_array[index++] = '0'+ i;
    }
    for(int i = 0; i < 26; ++i)
    {
        g_alphanumeric_array[index++] = 'a'+ i;
    }
    for(int i = 0; i < 26; ++i)
    {
        g_alphanumeric_array[index++] = 'A'+ i;
    }
    int test_sces = 1;
    if(argc > 1)
    {
        test_sces = atoi(argv[1]);
    }
    if(test_sces <= 0)
    {
        test_sces = 1;
    }
//    printf("test_sces=%d\n",test_sces);
    srand((unsigned)time(NULL));
    p = new TQueue<char>(12);

#ifdef USE_QUEUE
    pthread_t product_thread;
    pthread_t custom_thread;
    pthread_create(&product_thread, NULL, (void *(*)(void*))product,NULL);
    pthread_create(&custom_thread, NULL, (void *(*)(void*))custom,NULL);
#else
    pthread_t test_thread;
    pthread_create(&test_thread, NULL, (void *(*)(void*))testCharSort,NULL);
#endif

//    pthread_join(custom_thread,NULL);

    sleep(test_sces);
#ifdef USE_QUEUE
    pthread_cancel(custom_thread);
    pthread_cancel(product_thread);
#else
    pthread_cancel(test_thread);
#endif
//    printf("test_sces=%ds,finished_count=%d\n",test_sces,g_finished_count);
    printf("finished_count=%d\n",g_finished_count);
    return 0;
}

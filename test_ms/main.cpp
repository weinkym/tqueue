#include "mainwindow.h"
#include <QApplication>
#include "tqueue.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>

const int T_ALPHANUMERIC_COUNT = 62;

char g_alphanumeric_array[T_ALPHANUMERIC_COUNT];
const int T_MAX_LENGTH = 12;
TQueue<char> *p;
int g_finished_count = 0;

static void sleep_ms(unsigned int secs)

{

    struct timeval tval;

    tval.tv_sec=secs/1000;

    tval.tv_usec=(secs*1000)%1000000;

    select(0,NULL,NULL,NULL,&tval);

}

char createAlphanumeric()
{
    int index = rand() % T_ALPHANUMERIC_COUNT;
    return g_alphanumeric_array[index];
}

void sord(char *a,int m)
{
//    int m = a.length();
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
//    for(int i = 0; i < T_MAX_LENGTH;++i)
    while(true)
    {
//        sleep_ms(100);
        p->produceData(createAlphanumeric());
    }
//    printf("product end\n");
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
//
//    QString text;
//    for(int i = 0; i < T_MAX_LENGTH;++i)
//    {
//        printf("%c",datas[i]);
//        text.append(datas[i]);
//    }
//    printf("custom end\n");
//    qDebug()<<"==========";
//    qDebug()<<text;
//    qDebug()<<"==========";
//    pthread_exit(0);
}

void doTest()
{

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
//    for(int i = 0; i <T_MAX_LENGTH;++i)
//    {
//        printf("%c",createAlphanumeric());
//    }
//    printf("\n");
//    return 0;
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//    printf("LINE=%d\n",__LINE__);
    srand((unsigned)time(NULL));
    p = new TQueue<char>(12);

    pthread_t product_thread;
//    pthread_t product_thread2;
    pthread_t custom_thread;
//    pthread_create(&product_thread2, NULL, (void *(*)(void*))product,NULL);
    pthread_create(&product_thread, NULL, (void *(*)(void*))product,NULL);
    pthread_create(&custom_thread, NULL, (void *(*)(void*))custom,NULL);
//    pthread_join(custom_thread,NULL);
//    delete p;
//    p = NULL;
//    return a.exec();
    //    while(true)
    //    {
    //        sleep_ms(10);
    //    }
    sleep_ms(10000);
    pthread_cancel(custom_thread);
    pthread_cancel(product_thread);
//    pthread_join(custom_thread, NULL);
//    pthread_join(product_thread, NULL);

//    pthread_mutex_destroy(&(p->cond_mutex));
//    pthread_cond_destroy(&(p->cond1));
//    pthread_cond_destroy(&(p->cond2));

//    pthread_exit(NULL);
    printf("TTTTTV:count=%d\n",g_finished_count);
    qDebug()<<"g_finished_count "<<g_finished_count;
    return 0;
}

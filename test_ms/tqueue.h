#ifndef TQUEUE_H
#define TQUEUE_H

#include <stdint.h>
#include <mutex>
#include <stdio.h>
#include <iostream>
#include <QDebug>

const int T_MAX_QUEUE_LENGTH = 12;

template <typename T>
class TQueue
{
public:
    TQueue(size_t maxsize = 12);
    ~TQueue();
    void produceData(const T& data);
    T consumeData();

private:
    T* m_datas;
    size_t m_maxsize;
    size_t read_position;
    size_t write_position;
    pthread_mutex_t cond_mutex;
    pthread_mutex_t produce_mutex;
    pthread_mutex_t consume_mutex;
    pthread_cond_t  cond1;
    pthread_cond_t  cond2;
};

template<typename T>
TQueue<T>::TQueue(size_t maxsize)
    :m_datas(NULL)
    ,m_maxsize(maxsize)
    ,read_position(0)
    ,write_position(0)

{
    pthread_mutex_init( &(this->cond_mutex), NULL );
    pthread_cond_init( &(this->cond1), NULL );
    pthread_cond_init( &(this->cond2), NULL );
    if(m_maxsize <= 0)
    {
        m_maxsize = 12;
    }
    m_datas = new T[m_maxsize];
//    assert(array != NULL);
}

template<typename T>
TQueue<T>::~TQueue()
{
  if(m_datas)
  {
      delete []m_datas;
  }
}

template<class T>
void TQueue<T>::produceData(const T &data)
{
    printf("LINE=%d\n",__LINE__);
    pthread_mutex_lock(&produce_mutex);
    qDebug()<<QString("QLINE=%1").arg(__LINE__);
    while (((write_position + 1) % m_maxsize) == read_position)
    {
        {
            pthread_mutex_lock(&cond_mutex);
            printf("LINE=%d\n",__LINE__);
            qDebug()<<QString("QLINE=%1").arg(__LINE__);
            pthread_cond_wait(&cond1,&cond_mutex); // 生产者等待"产品库缓冲区不为满"这一条件发生.
            qDebug()<<QString("QLINE=%1").arg(__LINE__);
            pthread_mutex_unlock(&cond_mutex); // 解锁
        }
    }
    m_datas[write_position] = data;
    write_position++;
    write_position = write_position % m_maxsize;
    pthread_cond_signal(&cond2);
    qDebug()<<QString("QLINE=%1").arg(__LINE__);
    printf("LINE=%d\n",__LINE__);
    pthread_mutex_unlock(&produce_mutex);
}

template<class T>
T TQueue<T>::consumeData()
{
    printf("LINE=%d\n",__LINE__);
    pthread_mutex_lock(&consume_mutex);
    qDebug()<<QString("QLINE=%1").arg(__LINE__);
    while (this->write_position == this->read_position)
    {
        pthread_mutex_lock(&cond_mutex);
        printf("LINE=%d\n",__LINE__);
        qDebug()<<QString("QLINE=%1").arg(__LINE__);
        pthread_cond_wait(&cond2,&cond_mutex); // 生产者等待"产品库缓冲区不为满"这一条件发生.
        qDebug()<<QString("QLINE=%1").arg(__LINE__);
        pthread_mutex_unlock(&cond_mutex); // 解锁
    }
    T data = m_datas[read_position]; // 读取某一产品
    read_position++; // 读取位置后移
    read_position = read_position % m_maxsize;
    pthread_cond_signal(&cond1);
    printf("LINE=%d\n",__LINE__);
    qDebug()<<QString("QLINE=%1").arg(__LINE__);
    pthread_mutex_unlock(&consume_mutex);

    return data;
}

#endif // TQUEUE_H

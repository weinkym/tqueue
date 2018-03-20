#ifndef TQUEUE_H
#define TQUEUE_H

#include <stdint.h>
#include <mutex>
#include <stdio.h>
#include <iostream>

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
    size_t m_read_position;
    size_t m_write_position;
    pthread_mutex_t m_cond_mutex;
    pthread_cond_t  m_produce_cond;
    pthread_cond_t  m_consume_cond;
};

template<typename T>
TQueue<T>::TQueue(size_t maxsize)
    :m_datas(NULL)
    ,m_maxsize(maxsize)
    ,m_read_position(0)
    ,m_write_position(0)

{
    pthread_mutex_init( &(this->m_cond_mutex), NULL );
    pthread_cond_init( &(this->m_produce_cond), NULL );
    pthread_cond_init( &(this->m_consume_cond), NULL );
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
    pthread_mutex_lock(&m_cond_mutex);
    while (((m_write_position + 1) % m_maxsize) == m_read_position)
    {
        //缓冲区满，等待消费
        pthread_cond_wait(&m_produce_cond,&m_cond_mutex);
    }
    m_datas[m_write_position] = data;
    m_write_position++;
    m_write_position = m_write_position % m_maxsize;
    pthread_cond_signal(&m_consume_cond);
    pthread_mutex_unlock(&m_cond_mutex); // 解锁
}

template<class T>
T TQueue<T>::consumeData()
{
    pthread_mutex_lock(&m_cond_mutex);
    while (this->m_write_position == this->m_read_position)
    {
        //缓冲区为空，等待生产
        pthread_cond_wait(&m_consume_cond,&m_cond_mutex);
    }
    T data = m_datas[m_read_position]; // 读取一个数据
    m_read_position++; // 读取位置后移
    m_read_position = m_read_position % m_maxsize;
    pthread_cond_signal(&m_produce_cond);
    pthread_mutex_unlock(&m_cond_mutex); // 解锁
    return data;
}

#endif // TQUEUE_H

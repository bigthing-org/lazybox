#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QtCore/QMutex>
#include <QtCore/QRunnable>
#include <QtCore/QThread>
#include <QtCore/QThreadPool.h>
#include <QtCore/QVector>

// 该文件实现调度器
template<typename LambdaF>
class QTask : public QRunnable
{
public:
    QTask(LambdaF f) : QRunnable(), F(f) { }
    virtual ~QTask() {}

    void run() override
    {
        F();
    }
    LambdaF& F;
};

class QTaskPool
{
private:
    QThreadPool* pool;
    QMutex mutex;

private:
    QTaskPool()
    {
        pool = QThreadPool::globalInstance();
        pool->setMaxThreadCount(4); // 最大4线程，超时30s
    }

    QTaskPool(const QTaskPool&);
    QTaskPool& operator=(const QTaskPool&);

public:
    QTaskPool getInstance()
    {
        static QTaskPool instance;
        return instance;
    }

    template<typename LambdaF>
    void addTask(const QTask<LambdaF>& task)
    {
        mutex.lock();
        pool->start(task);
        mutex.unlock();
    }

    void clearTask()
    {
        mutex.lock();
        pool->clear();
        mutex.unlock();
    }

    void waitForDone()
    {
        pool->waitForDone();
    }
};

#endif // SCHEDULER_H

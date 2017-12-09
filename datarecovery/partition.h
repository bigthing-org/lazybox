#ifndef PARTITION_H
#define PARTITION_H

// 定义抽象接口

class QRecordFile
{// 跨分区类型的通用文件信息
public:
    virtual bool IsDir() { return false; }
    virtual bool IsFile() { return false; }
    virtual unsigned long long GetFileSize() { return 0; }
    virtual bool GetType() { return 0; }
};

class QPartition
{
public:
    QPartition() { type = 0; }

    // 解析分区，检测是否有误
    virtual bool IsValid() = 0;

    // 获取根目录文件
    virtual QVector<QRecordFile*> GetRootFiles() { return QVector<QRecordFile*>(); }

    // 获取当前目录下的文件和目录
    virtual QVector<QRecordFile*> ListFiles(const QRecordFile* filerecord) { return QVector<QRecordFile*>(); }

    // 根据路径定位文件
    virtual QRecordFile* GetFileForPath(const QString& path) { return false; }

    //拷贝文件
    virtual bool CopyFile(const QString& dstpath, const QString& srcpath, QFile& f) { return false; }

    //拷贝文件夹及所有子文件
    virtual bool CopyDir(const QString& dstpath, const QString& srcpath, QFile& f) { return false; }

    //拷贝文件
    virtual bool CopyFile(const QString& dstpath, const QRecordFile* filerecord, QFile& f) { return false; }

    //拷贝文件夹及所有子文件
    virtual bool CopyDir(const QString& dstpath, const QRecordFile* filerecord, QFile& f) { return false; }


    virtual ~QPartition() {}

public:
    unsigned char type;         // 磁盘类型
    unsigned long long begin;   // 起始偏移
    unsigned long long size;    // 分区大小
};

#endif // PARTITION_H

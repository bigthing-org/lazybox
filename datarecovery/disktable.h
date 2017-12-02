#ifndef DISKTABLE_H
#define DISKTABLE_H


// 分区表格式 GPT / MBR / 动态磁盘
class QPartition
{
public:
    QPartition()
    {

    }
};

class QDisk
{
public:
    QDisk() {}

    QDisk(const QFile& f)
    {

    }

    bool IsValid()
    {
        return true;
    }

private:
    QVector<QPartition> partitions;
};

class QDiskUtils
{
public:
    static QVector<QDisk> GetPhysicalDrives()
    {
        QVector<QDisk> disks;
        QFile tdisk;
        QString t;
        tdisk.setFileName("\\\\.\\PhysicalDrive0"); //  0~16
        if (tdisk.open(QFile::ReadOnly))
        {
            // Windows Drive
            tdisk.close();
            for (int i = 0; i < 64; i++)
            {
                tdisk.setFileName(t.sprintf("\\\\.\\PhysicalDrive%u", i));
                if (tdisk.open(QFile::ReadOnly))
                {
                    QDisk disk(tdisk);
                    if (disk.IsValid())
                    {
                        disks.push_back(disk);
                    }
                    tdisk.close();
                }
            }
        }
        tdisk.setFileName("/dev/sda"); // SCSI/SATA     a-z
        if (tdisk.open(QFile::ReadOnly))
        {
            // Linux Drive
            tdisk.close();
            for (int i = 'a'; i < 'z'; i++)
            {
                tdisk.setFileName(t.sprintf("/dev/sd%c", i));
                if (tdisk.open(QFile::ReadOnly))
                {
                    QDisk disk(tdisk);
                    if (disk.IsValid())
                    {
                        disks.push_back(disk);
                    }
                    tdisk.close();
                }
            }
        }
        tdisk.setFileName("/dev/hda"); // IDE           a-z
        if (tdisk.open(QFile::ReadOnly))
        {
            // Linux Drive
            for (int i = 'a'; i < 'z'; i++)
            {
                tdisk.setFileName(t.sprintf("/dev/hd%c", i));
                if (tdisk.open(QFile::ReadOnly))
                {
                    QDisk disk(tdisk);
                    if (disk.IsValid())
                    {
                        disks.push_back(disk);
                    }
                    tdisk.close();
                }
            }
        }
        tdisk.setFileName("/dev/disk0"); // 0-20
        if (tdisk.open(QFile::ReadOnly))
        {
            // Mac Drive
            tdisk.close();
            for (int i = 0; i < 64; i++)
            {
                tdisk.setFileName(t.sprintf("/dev/disk%d", i));
                if (tdisk.open(QFile::ReadOnly))
                {
                    QDisk disk(tdisk);
                    if (disk.IsValid())
                    {
                        disks.push_back(disk);
                    }
                    tdisk.close();
                }
            }
        }
        tdisk.setFileName("/dev/rdisk0"); // 0-20
        if (tdisk.open(QFile::ReadOnly))
        {
            // Mac Drive
            tdisk.close();
            for (int i = 0; i < 64; i++)
            {
                tdisk.setFileName(t.sprintf("/dev/rdisk%d", i));
                if (tdisk.open(QFile::ReadOnly))
                {
                    QDisk disk(tdisk);
                    if (disk.IsValid())
                    {
                        disks.push_back(disk);
                    }
                    tdisk.close();
                }
            }
        }
        // todo Other File System
        return disks;
    }
};

#endif // DISKTABLE_H

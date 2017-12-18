#include "partition.h"
#include "fatx.hpp"
#include "ntfs.hpp"

QMutex QDiskUtils::lock;
QHash<QString, QPartition*> QDiskUtils::g_part;


QDisk::QDisk()
{
    this->valid = false;
}

QDisk::QDisk(QFile& f)
{
    // 分区文件f可以是\\\\.\\PhysicalDrive这种本地硬盘形式，未来也可以支持ISO、GHO等类型的文件
    // 我通常在小容量U盘上进行分区，然后用Winhex将整个U盘扇区写入文件，然后解析文件即可，避免调试UAC权限的程序
    this->type = 0;
    this->valid = false;
    this->symbol = f.fileName();
    f.seek(0);
    memset(&this->mbr, 0, sizeof(this->mbr));
    f.read((char*)&this->mbr, sizeof(this->mbr));
    for (int i = 0; i < 4; i++)
    {
        this->commonHandler(f, this->mbr.entry[i]);
    }
    this->valid = true;
    if (this->type == 0)
    {
        this->type = DISK_TYPE_BASIC;
    }
    this->UpdateSize(f);
}

void QDisk::UpdateSize(QFile& f)
{
    // 由于对于扇区文件，f.size()不能获取大小，因此二分法确定磁盘大小
    this->size = f.size();

    if (this->size <= 0)
    {
        char t;
        unsigned long long begin = 0;
        unsigned long long end = (unsigned long long)(-1);
        while (begin < end - 1)
        {
            unsigned long long mid = (begin + end) / 2;
            f.seek(mid);
            int readnum = f.read(&t, 1);
            if (readnum < 0)
            {
                end = mid;
            }
            else
            {
                begin = mid;
            }
        }
        this->size = end;
    }
}

void QDisk::commonHandler(QFile& f, const PartitionTableEntry& entry, unsigned int basesec)
{
    // 根据分区类型确定处理分区
    int disktype = entry.typeindicator;
    switch(disktype)
    {
    case EMPTY:
        // Empty  空分区，标志分区结束
        break;
    case EXTENDED:
        // 扩展分区：MBR只能存放4个分区，扩展分区一般用于容纳超过4个分区的情况，可以多层嵌套
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;
            QCommonPartition* part = new QCommonPartition(f, disktype, begin, size);
            QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
            this->partitions.push_back(out);
            handleExtended(f, entry.sectorprecede, basesec);
        }
        break;
    case NTFS_HPFS:
        // Ntfs
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;
            NTFS::QNtfsPartition* part = new NTFS::QNtfsPartition(f, begin, size);
            if (part->IsValid())
            {
                QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
                this->partitions.push_back(out);
            }
        }
        break;
    case PRI_FAT32_INT13:
        // Fat32
        break;
    case EXT_FAT32_INT13:
        // Fat32
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;
            FAT::QFatPartition* part = new FAT::QFatPartition(f, begin, size);
            if (part->IsValid())
            {
                QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
                this->partitions.push_back(out);
            }
        }
        break;
    case WIN95_EXT_PARTITION:
        // 扩展分区，类似于EXTENDED
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;
            QCommonPartition* part = new QCommonPartition(f, disktype, begin, size);
            QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
            this->partitions.push_back(out);
            handleExtended(f, entry.sectorprecede, basesec);
        }
        break;
    case NTFS_HPFS_HIDDEN:
        // Ntfs
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;
            NTFS::QNtfsPartition* part = new NTFS::QNtfsPartition(f, begin, size);
            if (part->IsValid())
            {
                QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
                this->partitions.push_back(out);
            }
        }
        break;
    case OSR2_FAT32:
        // Fat32
        break;
    case OSR2_FAT32_LBA:
        // Fat32
        break;
    case WIN_RAID:
        this->type = DISK_TYPE_DYN;
        // Windows RAID dynamic drive
        // disktype需要重新推测
        break;
    case LINUX_NATIVE_2:
        // Linux  FS code for ext2, reiserfs and xfs
        // ext2
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;
            QCommonPartition* part = new QCommonPartition(f, disktype, begin, size);
            QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
            this->partitions.push_back(out);
        }
        break;
    case LINUX_EXTENDED:
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;

            QCommonPartition* part = new QCommonPartition(f, disktype, begin, size);
            QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
            this->partitions.push_back(out);
        }
        break;
    case LEGACY_MBR_EFI_HEADER:
        // GPT
        {
            unsigned long long begin = basesec + entry.sectorprecede;
            begin *= DEFAULT_SECTOR_SIZE;
            unsigned long long size = entry.sectornum;
            size *= DEFAULT_SECTOR_SIZE;

            QCommonPartition* part = new QCommonPartition(f, disktype, begin, size);
            QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
            this->partitions.push_back(out);

            this->type = DISK_TYPE_GPT;
            handleLegacyMbrEfi(f, entry.sectorprecede);
        }
        break;
    default:
        // Not handled
        break;
    }
}

void QDisk::handleExtended(QFile& f, unsigned int sectorprecede, unsigned int basesec)
{
    // 解析扩展分区
    struct MasterBootRecord extended;
    qint64 offset = sectorprecede + basesec;
    offset *= DEFAULT_SECTOR_SIZE; // 必须分开写，合写存在64位运算存在问题
    f.seek(offset);
    memset(&extended, 0, sizeof(extended));
    f.read((char*)&extended, sizeof(extended));
    for (int i = 0; i < 4; i++)
    {
        this->commonHandler(f, extended.entry[i], sectorprecede + basesec);
    }
}

void QDisk::handleLegacyMbrEfi(QFile& f, unsigned int sectorprecede)
{
    // 解析GPT分区格式
    struct EfiPartitionHeader header;
    qint64 offset = sectorprecede;
    offset *= DEFAULT_SECTOR_SIZE;
    memset(&header, 0, sizeof(header));
    f.seek(offset);
    f.read((char*)&header, sizeof(header));
    offset += DEFAULT_GPT_HEADER_SIZE;
    for (unsigned int i = 0; i < header.numpartitions; i++)
    {
        f.seek(offset);
        struct EfiPartitionEntry partition;
        memset(&partition, 0, sizeof(partition));
        f.read((char*)&partition, sizeof(partition));
        if (partition.u.type == 0)
        {
            break;
        }

        unsigned long long begin = partition.firstlba * DEFAULT_SECTOR_SIZE;
        unsigned long long size = partition.lastlba - partition.firstlba;
        size *= DEFAULT_SECTOR_SIZE;
        char buf[SIGSIZE];
        memset(buf, 0, SIGSIZE);
        f.seek(begin);
        f.read(buf, SIGSIZE);
        if (((NTFS::ntfs_boot_sector*)buf)->system_id == NTFS::NTFS_Magic)
        {
            NTFS::QNtfsPartition* part = new NTFS::QNtfsPartition(f, begin, size);
            if (part->IsValid())
            {
                QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
                this->partitions.push_back(out);
            }
        }
        else if (((FAT::fat_boot_sector*)buf)->n.system_id  == FAT::FAT_Magic)
        {
            FAT::QFatPartition* part = new FAT::QFatPartition(f, begin, size);
            if (part->IsValid())
            {
                QPartition* out = QDiskUtils::AddPartition(static_cast<QPartition*>(part));
                this->partitions.push_back(out);
            }
        }


        offset += header.partitionsize;
    }
}

bool QDisk::IsValid()
{
    return valid;
}


const char* QDiskUtils::GetTypeStr(int type)
{
    switch(type)
    {
        case PARTITION_TYPE_NTFS:
            return "NTFS";
        case PARTITION_TYPE_FAT32:
            return "FAT32";
        case PARTITION_TYPE_EXT:
            return "EXT2";
        case PARTITION_TYPE_HFS:
            return "HFS";
        case DISK_TYPE_BASIC:
            return "MBR";
        case DISK_TYPE_GPT:
            return "GPT";
        case DISK_TYPE_DYN:
            return "RAID";
        case EXTENDED:
        case WIN95_EXT_PARTITION:
            return "EXTEND";
        case LINUX_NATIVE_2:
            return "EXT2";
    }
    return "???";
}

QPartition* QDiskUtils::AddPartition(QPartition* part)
{
    // 分区对象交给QDiskUtils维护
    QMutexLocker locker(&lock);
    if (part != 0)
    {
        if (g_part.find(part->uniqueid) == g_part.end())
        {
            g_part[part->uniqueid] = part;
            return part;
        }
        else
        {
            return g_part[part->uniqueid];
        }
    }
}

QVector<QPartition*> QDiskUtils::GetPartitions()
{
    QMutexLocker locker(&lock);
    QVector<QPartition*> output;
    QHash<QString, QPartition*>::iterator iter;
    for (iter = g_part.begin(); iter != g_part.end(); ++iter)
    {
        if (iter.value() != 0)
        {
            output.push_back(iter.value());
        }
    }
}

QPartition* QDiskUtils::GetPartition(QString id)
{
    QMutexLocker locker(&lock);
    if (g_part.find(id) != g_part.end())
    {
        return g_part[id];
    }
    return 0;
}

void QDiskUtils::Free()
{
    QMutexLocker locker(&lock);
    QHash<QString, QPartition*>::iterator iter;
    for (iter = g_part.begin(); iter != g_part.end(); ++iter)
    {
        if (iter.value() != 0)
        {
            delete iter.value();
        }
    }
}

void QDiskUtils::GetPhysicalDrives(QVector<QDisk>& disks, QVector<QString> addition)
{
    QFile tdisk;
    QString t;

    for (QString& s : addition)
    {
        tdisk.setFileName(s);
        if(tdisk.open(QFile::ReadOnly))
        {
            QDisk disk(tdisk);
            disks.push_back(disk);
            tdisk.close();
        }
    }
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
                disks.push_back(disk);
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
                disks.push_back(disk);
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
                disks.push_back(disk);
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
                disks.push_back(disk);
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
                disks.push_back(disk);
                tdisk.close();
            }
        }
    }
    // todo Other File System
}

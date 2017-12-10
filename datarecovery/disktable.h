#ifndef DISKTABLE_H
#define DISKTABLE_H

#define SIGSIZE 0x1000 // 预留0x1000字节判断磁盘类型

#include "ntfs.h"
#include "fat32.h"
#include "partition_common.h"

// 分区表格式 GPT / MBR / 动态磁盘

#pragma pack(push)
#pragma pack(1)
struct PartitionTableEntry
{
    unsigned char bootindicator;    // 活动分区0x80
    unsigned char starthead;        // 开始磁头
    unsigned short startsector:6;    // 起始扇区
    unsigned short startcylinder:10;// 起始柱面
    unsigned char typeindicator;    // 类型描述
    unsigned char endhead;          // 结束磁头
    unsigned short endsector:6;      // 结束扇区
    unsigned short endcylinder:10;  // 结束柱面
    unsigned int sectorprecede;     // 之前的扇区数
    unsigned int sectornum;         // 总扇区数
};
#pragma pack(pop)

struct MasterBootRecord
{
    unsigned char loader_code[0x1B8];   // 加载器代码
    unsigned char disksig[4];           // 58 3F 98 EC
    unsigned char nop[2];
    PartitionTableEntry entry[4];       // 分区表
    unsigned char endsig[2];            // 结束符 55 AA
};

struct EfiPartitionHeader
{
    unsigned char signature[8];         // 签名 EFI PART
    unsigned int revision;              // 版本号
    unsigned int headersize;            // GPT头字节总数
    unsigned int headercrc32;           // GPT头CRC校验和
    unsigned int reserved;              // 保留
    unsigned long long currentlba;      // GPT头所在扇区号
    unsigned long long backuplba;       // GPT头备份所在扇区号
    unsigned long long firstusablelba;  // GPT分区区域起始扇区号
    unsigned long long lastusablelba;   // GPT分区区域结束扇区号
    unsigned char diskguid[16];         // 磁盘GUID
    unsigned long long partitionlba;    // GPT分区表起始扇区号
    unsigned int numpartitions;         // 分区表项数
    unsigned int partitionsize;         // 每个分区表项的字节数
    unsigned int partitioncrc32;        // 分区表CRC校验和
    unsigned char empty[420];           // 保留
};

struct EfiPartitionEntry
{
    union
    {
        unsigned int type;                  // 分区类型
        unsigned char typeguid[16];         // 分区类型GUID
    } u;
    unsigned char paritionguid[16];         // 分区GUID
    unsigned long long firstlba;            // 分区起始地址
    unsigned long long lastlba;             // 分区结束地址
    unsigned long long prop;                // 分区属性
    wchar_t name[36];                       // 分区名
};

class QDisk
{
protected:
    struct MasterBootRecord mbr; // Main Boot Record
    QVector<QPartition> partitions;
    QString symbol;
    bool valid;

    enum
    {
        DEFAULT_SECTOR_SIZE = 0x200,
        DEFAULT_GPT_HEADER_SIZE = 0x200,
    };

public:
    QDisk() { this->valid = false; }

    QDisk(QFile& f)
    {
        this->valid = false;
        this->symbol = f.fileName();
        f.seek(0);
        memset(&this->mbr, 0, sizeof(this->mbr));
        f.read((char*)&this->mbr, sizeof(this->mbr));
        for (int i = 0; i < 4; i++)
        {
            int disktype = this->mbr.entry[i].typeindicator;
            switch(disktype)
            {
            case EMPTY:
                // Empty
                break;
            case EXTENDED:
                // 扩展分区
                break;
            case NTFS_HPFS:
                // Ntfs
                break;
            case PRI_FAT32_INT13:
                // Fat32
                break;
            case EXT_FAT32_INT13:
                // Fat32
                break;
            case NTFS_HPFS_HIDDEN:
                // Ntfs
                break;
            case OSR2_FAT32:
                // Fat32
                break;
            case OSR2_FAT32_LBA:
                // Fat32
                break;
            case WIN_RAID:
                // Windows RAID dynamic drive
                // disktype需要重新推测
                break;
            case LINUX_NATIVE_2:
                // Linux  FS code for ext2, reiserfs and xfs
                // ext2
                break;
            case LINUX_EXTENDED:

                break;
            case LEGACY_MBR_EFI_HEADER:
                // GPT
                handleLegacyMbrEfi(f, this->mbr.entry[i].sectorprecede);
                break;
            default:
                // Not handled
                break;
            }
        }
        this->valid = true;
    }

    virtual void handleLegacyMbrEfi(QFile& f, unsigned int sectorprecede)
    {
        struct EfiPartitionHeader header;
        qint64 offset = sectorprecede * DEFAULT_SECTOR_SIZE;
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
            unsigned long long size = (partition.lastlba - partition.firstlba) * DEFAULT_SECTOR_SIZE;
            char buf[SIGSIZE];
            memset(buf, 0, SIGSIZE);
            f.seek(begin);
            f.read(buf, SIGSIZE);
            if (((NTFS::ntfs_boot_sector*)buf)->system_id == NTFS::NTFS_Magic)
            {
                NTFS::QNtfsPartition partition(f, begin, size);
                if (partition.IsValid())
                {
                    this->partitions.push_back(partition);
                }
            }
            else if (((FAT::fat_boot_sector*)buf)->n.system_id  == FAT::FAT_Magic)
            {
                FAT::QFatPartition partition(f, begin, size);
                if (partition.IsValid())
                {
                    this->partitions.push_back(partition);
                }
            }


            offset += header.partitionsize;
        }
    }

    virtual bool IsValid()
    {
        return valid;
    }
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

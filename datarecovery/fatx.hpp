#ifndef FAT32_H
#define FAT32_H

#include "partition.h"

namespace FAT
{
    enum
    {
        PRSIZE = 0x800,
        HEADER_OFF = 0,
        BLOCK_SIZE = 0x100000,
        FAT_Magic = 0x544146,
        DELETED_FLAG = 0xe5,
        FAT_ROOT_INDX = 0,
    };

    enum FILE_ATTRIBUTE
    {
        ATTR_RO       = 1 ,
        ATTR_HIDDEN   = 2 ,
        ATTR_SYS      = 4 ,
        ATTR_VOLUME   = 8 ,
        ATTR_DIR      = 16,
        ATTR_ARCH     = 32,
        ATTR_NONE     = 0 ,
        ATTR_UNUSED   = (ATTR_VOLUME | ATTR_ARCH | ATTR_SYS | ATTR_HIDDEN),
        ATTR_EXT      = (ATTR_RO | ATTR_HIDDEN | ATTR_SYS | ATTR_VOLUME),
        ATTR_EXT_MASK = (ATTR_RO | ATTR_HIDDEN | ATTR_SYS | ATTR_VOLUME | ATTR_DIR | ATTR_ARCH),
    };

#pragma pack(push)
#pragma pack(1)

    struct fat_boot_sector
    {
        uint8_t     ignored[3];	/* 0x00 Boot strap short or near jump */
        int8_t      system_id[8];	/* 0x03 Name - can be used to special case
                       partition manager volumes */
        uint16_t	sector_size;	/* 0x0B bytes per logical sector */
        uint8_t     sectors_per_cluster;	/* 0x0D sectors/cluster */
        uint16_t	reserved;	/* 0x0E reserved sectors */
        uint8_t     fats;		/* 0x10 number of FATs */
        uint16_t    dir_entries;	/* 0x11 root directory entries */
        uint16_t	sectors;	/* 0x13 number of sectors */
        uint8_t     media;		/* 0x15 media code (unused) */
        uint16_t	fat_length;	/* 0x16 sectors/FAT */
        uint16_t	secs_track;	/* 0x18 sectors per track */
        uint16_t	heads;		/* 0x1A number of heads */
        uint32_t	hidden;		/* 0x1C hidden sectors (unused) */
        uint32_t	total_sect;	/* 0x20 number of sectors (if sectors == 0) */

        /* The following fields are only used by FAT32 */
        uint32_t	fat32_length;	/* 0x24=36 sectors/FAT */
        uint16_t	flags;		/* 0x28 bit 8: fat mirroring, low 4: active fat */
        uint16_t    version;	/* 0x2A major, minor filesystem version */
        uint32_t	root_cluster;	/* 0x2C first cluster in root directory */
        uint16_t	info_sector;	/* 0x30 filesystem info sector */
        uint16_t	backup_boot;	/* 0x32 backup boot sector */
        uint8_t     BPB_Reserved[12];	/* 0x34 Unused */
        uint8_t     BS_DrvNum;		/* 0x40 */
        uint8_t     BS_Reserved1;		/* 0x41 */
        uint8_t     BS_BootSig;		/* 0x42 */
        uint8_t     BS_VolID[4];		/* 0x43 */
        uint8_t     BS_VolLab[11];		/* 0x47 */
        union
        {
            uint32_t    system_id : 24;     // 'FAT'
            uint8_t     BS_FilSysType[8];	/* 0x52=82*/
        } n;
        /* */
        uint8_t     nothing[420];	/* 0x5A */
        uint16_t	marker;
    };

    struct fat_fsinfo
    {
      uint32_t  leadsig;		/* 0x41615252 */
      uint8_t   reserved1[480];
      uint32_t  strucsig;		/* 0x61417272 */
      uint32_t  freecnt;     	/* free clusters 0xfffffffff if unknown */
      uint32_t  nextfree;		/* next free cluster */
      uint8_t   reserved3[12];
      uint32_t  magic3;		/* 0xAA550000 */
    };

    union msdos_dir
    {
        struct
        { // Short Entry
            int8_t      name[8];		/* 00 name and extension */
            int8_t      ext[3];
            uint8_t     attr;			/* 0B attribute bits */
            uint8_t     lcase;		/* 0C Case for base and extension */
            uint8_t	    ctime_ms;	/* 0D Creation time, milliseconds */
            uint16_t	ctime;		/* 0E Creation time */
            uint16_t	cdate;		/* 10 Creation date */
            uint16_t	adate;		/* 12 Last access date */
            uint16_t    starthi;	/* 14 High 16 bits of cluster in FAT32 */
            uint16_t	time;           /* 16 time, date and first cluster */
            uint16_t    date;		/* 18 */
            uint16_t    start;		/* 1A */
            uint32_t	size;		/* 1C file size (in bytes) */
        } msdos_dir_entry;

        /* Up to 13 characters of the name */
        struct
        { // Long Entry
            union
            {
                uint8_t    char0;			/* 00 sequence number for slot */
                struct
                {
                    uint8_t lfn_recseqnum : 6; // long entry num
                    uint8_t last_lfn_record : 1;
                    uint8_t lfn_erase : 1;
                } seqnum;
            } lfn;
            uint8_t    name0_4[10];		/* 01 first 5 characters in name */
            uint8_t    attr;		/* 0B attribute byte */
            uint8_t    reserved;		/* 0C always 0 */
            uint8_t    alias_checksum;	/* 0D checksum for 8.3 alias */
            uint8_t    name5_10[12];	/* 0E 6 more characters in name */
            uint16_t   start;		/* 1A starting cluster number, 0 in long slots */
            uint8_t    name11_12[4];	/* 1C last 2 characters in name */
        } msdos_dir_slot;
    };

#pragma pack(pop)


    unsigned int fat_sector_size(const struct fat_boot_sector *fat_header)
    {
        return fat_header->sector_size;
    }

    unsigned int get_dir_entries(const struct fat_boot_sector *fat_header)
    {
        return fat_header->dir_entries;
    }

    unsigned int fat_sectors(const struct fat_boot_sector *fat_header)
    {
        return fat_header->sectors;
    }

    unsigned int fat_get_cluster_from_entry(const msdos_dir *entry)
    {
        return (entry->msdos_dir_entry.starthi << 16) | entry->msdos_dir_entry.start;
    }

    int is_fat_directory(const unsigned char *buffer)
    {
      return(buffer[0]=='.' &&
          memcmp(buffer,         ".          ", 8+3)==0 &&
          memcmp(&buffer[0x20], "..         ", 8+3)==0 &&
          buffer[0xB]!=ATTR_EXT && (buffer[0xB]&ATTR_DIR)!=0 &&
          buffer[1*0x20+0xB]!=ATTR_EXT && (buffer[1*0x20+0xB]&ATTR_DIR)!=0);
    }


    class QFatFileRecord : public QRecordFile
    {
    public:
        unsigned long long fatbase;     // 文件所属MFT相对于Disk偏移，用于扩展功能
        unsigned long long creationtime;// 文件创建时间
        unsigned long long modifiedtime;// 文件最后修改时间
        unsigned long long accesstime;  // 文件最后访问时间
        unsigned long long dataallocsize;// 文件分配大小(按簇存储)
        unsigned long long datasize;    // 文件实际大小
        unsigned long long databegin;   // 文件数据偏移
        unsigned long flags;            // 文件属性
        unsigned long fatindx;          // 本文件FAT号
        unsigned long fatherfatindx;    // 父目录FAT号
        char16_t filename[260];         // 文件名

    public:
        virtual bool IsDir() { return this->flags & ATTR_DIR; }
        virtual bool IsFile() { return this->flags & ATTR_ARCH; } // ??
        virtual unsigned long long GetFileSize() { return this->datasize; }
        virtual int GetType() { return PARTITION_TYPE_FAT32; }
        virtual QString GetName() { return QString::fromUtf16(this->filename); }
    };

    class QFatPartition : public QPartition
    {
    public:
        QFatPartition(QFile& f, unsigned long long begin, unsigned long long size)
        {
            this->begin = begin;
            this->size = size;
            this->type = 0; // 设置为无效
            this->readHeader(f);
            this->type = PARTITION_TYPE_FAT32; // 设置为有效
            this->Analysis(f);
        }

        bool Analysis(QFile& f)
        {
            this->readFAT(f);
            return true;
        }

        // 解析分区，检测是否有误
        virtual bool IsValid()
        {
            return this->errtype == 0;
        }

        // 获取根目录文件
        virtual QVector<QRecordFile*> GetRootFiles()
        {
            QVector<QRecordFile*> result;
            for (QFatFileRecord& ff : filesystem[FAT_ROOT_INDX])
            {
                result.push_back(static_cast<QRecordFile*>(&ff));
            }
            return result;
        }

        // 获取当前目录下的文件和目录
        virtual QVector<QRecordFile*> ListFiles(const QRecordFile* filerecord)
        {
            QVector<QRecordFile*> result;
            if (const_cast<QRecordFile*>(filerecord)->GetType() != PARTITION_TYPE_FAT32)
            {
                return result;
            }
            unsigned long fatinx = ((QFatFileRecord*)filerecord)->fatindx;
            for (QFatFileRecord& ff : filesystem[fatinx])
            {
                result.push_back(static_cast<QRecordFile*>(&ff));
            }
            return result;
        }

        virtual QVector<QRecordFile*> ListFiles(const QString& path)
        {
            if (path == "/")
            {
                return GetRootFiles();
            }
            QRecordFile* f = this->GetFileForPath(path);
            if (f == 0)
            {
                return QVector<QRecordFile*>();
            }
            return this->ListFiles(f);
        }

        // 根据路径定位文件
        virtual QRecordFile* GetFileForPath(const QString& path)
        {
            // 路径形式:  /a/b/c/d.txt
            int current_fat = FAT_ROOT_INDX;
            QVector<QRecordFile*> result;
            QStringList path_vec;
            for (const QString& p : path.split("/"))
            {
                if (p.length() != 0)
                {
                    path_vec.push_back(p);
                }
            }
            QFatFileRecord* target = 0;
            for (const QString& p : path_vec)
            {
                bool find = false;
                for (QFatFileRecord& file : filesystem[current_fat])
                {
                    if (file.GetName() == p)
                    {
                        current_fat = file.fatindx;
                        target = &file;
                        find = true;
                    }
                }
                if (!find)
                {
                    break;
                }
            }
            return target;
        }

        //拷贝文件
        virtual bool CopyFile(const QString& dstpath, const QString& srcpath, QFile& f) { return false; }

        //拷贝文件夹及所有子文件
        virtual bool CopyDir(const QString& dstpath, const QString& srcpath, QFile& f) { return false; }

        //拷贝文件
        virtual bool CopyFile(const QString& dstpath, const QRecordFile* filerecord, QFile& f) { return false; }

        //拷贝文件夹及所有子文件
        virtual bool CopyDir(const QString& dstpath, const QRecordFile* filerecord, QFile& f) { return false; }


        virtual ~QFatPartition() {}

    private:
        virtual void readHeader(QFile& f)
        {
            f.seek(this->begin + HEADER_OFF);
            memset(&this->fat_header, 0, sizeof(this->fat_header));
            f.read((char*)&this->fat_header, sizeof(this->fat_header));
            this->sectors_per_cluster = this->fat_header.sectors_per_cluster;
            this->sector_size = this->fat_header.sector_size;
            uint8_t* asm_ = this->fat_header.ignored;
            this->errtype = 0;
            if (!(this->fat_header.marker == 0xAA55 && (asm_[0] == 0xeb || asm_[0] == 0xe9) &&
                    (this->fat_header.fats == 1 || this->fat_header.fats == 2)))
            { // Not FAT
                this->errtype |= 1;
            }

            if (!((asm_[0] == 0xeb && asm_[2] == 0x90) || asm_[0] == 0xe9))
            {
                this->errtype |= 2;
            }
            switch (this->fat_header.sectors_per_cluster)
            {
              case 1: case 2: case 4: case 8: case 16: case 32: case 64: case 128:
                break;
              default:
                this->errtype |= 4;
            }
            if (!(this->fat_header.fats == 1 || this->fat_header.fats == 2))
            {// Bad number
                this->errtype |= 8;
            }
            if (this->fat_header.media != 0xf0 && this->fat_header.media < 0xf8)
            {/* Legal values are 0xF0, 0xF8-0xFF */
                this->errtype |= 16;
            }

            uint64_t start_fat1;
            uint64_t start_fat2;
            uint64_t start_data;
            uint64_t part_size;
            uint64_t start_rootdir;
            unsigned long int no_of_cluster;
            unsigned long int fat_length;
            unsigned long int fat_length_calc;

            fat_length = this->fat_header.fat_length > 0 ? this->fat_header.fat_length : this->fat_header.fat32_length;
            unsigned long long t1 = this->fat_header.reserved + fat_length * this->fat_header.fats;
            t1 += ((unsigned long long)(this->fat_header.root_cluster - 2)) * this->sectors_per_cluster;
            t1 *= sector_size;
            this->direntryoff = this->begin +  t1;

            part_size = fat_sectors(&this->fat_header) > 0 ? fat_sectors(&this->fat_header) : this->fat_header.total_sect;
            start_fat1 = this->fat_header.reserved;
            start_fat2 = start_fat1 + (this->fat_header.fats > 1 ? fat_length : 0);
            start_data = start_fat1 + this->fat_header.fats * fat_length + (get_dir_entries(&this->fat_header) * 32 +
                            fat_sector_size(&this->fat_header) - 1) / fat_sector_size(&this->fat_header);
            no_of_cluster = (part_size - start_data) / this->fat_header.sectors_per_cluster;
            // end_data = start_data + no_of_cluster * this->fat_header.sectors_per_cluster - 1;

            if (no_of_cluster < 4085)
            {
                if ((get_dir_entries(&this->fat_header) == 0 || (get_dir_entries(&this->fat_header) % 16) != 0))
                {
                    this->errtype |= 0x20;
                }
                if (this->fat_header.fat_length > 256 || this->fat_header.fat_length == 0)
                {
                    this->errtype |= 0x40;
                }
                start_rootdir = start_fat2 + fat_length;
                fat_length_calc = ((no_of_cluster + 2 + fat_sector_size(&this->fat_header) * 2 / 3 - 1) * 3 / 2 / fat_sector_size(&this->fat_header));
            }
            else if (no_of_cluster < 65525)
            {
                if(this->fat_header.fat_length == 0)
                {
                    this->errtype |= 0x80;
                }
                if (get_dir_entries(&this->fat_header) == 0 || (get_dir_entries(&this->fat_header) % 16) != 0)
                {
                    this->errtype |= 0x100;
                }
                start_rootdir = start_fat2 + fat_length;
                fat_length_calc = ((no_of_cluster + 2 + fat_sector_size(&this->fat_header) / 2 - 1) * 2 / fat_sector_size(&this->fat_header));
            }
            else
            {
                if (fat_sectors(&this->fat_header) != 0)
                {
                    this->errtype |= 0x200;
                }
                if (get_dir_entries(&this->fat_header) != 0)
                {
                    this->errtype |= 0x400;
                }
                if(this->fat_header.root_cluster < 2 || this->fat_header.root_cluster >= 2 + no_of_cluster)
                {
                    this->errtype |= 0x800;
                }

                start_rootdir = start_data + (uint64_t)(this->fat_header.root_cluster - 2) * this->fat_header.sectors_per_cluster;
                fat_length_calc = ((no_of_cluster + 2 + fat_sector_size(&this->fat_header) / 4 - 1) * 4 / fat_sector_size(&this->fat_header));
            }

            if (fat_length < fat_length_calc)
            {
                this->errtype |= 0x1000;
            }

            QByteArray headdata = QByteArray::fromRawData((char*)&this->fat_header, sizeof(this->fat_header));
            this->uniqueid = QCryptographicHash::hash(headdata, QCryptographicHash::Md5).toHex();
        }

        virtual unsigned long readFATFile(QFile& f, unsigned char* buf, unsigned long len, unsigned long findx)
        {// 读取到结束则返回0
            QFatFileRecord file_record;
            unsigned char strbuf[600];
            msdos_dir* slot = (msdos_dir*)buf;
            memset(strbuf, 0, 600);
            memset(&file_record, 0, sizeof(file_record));
            file_record.fatherfatindx = findx;
            file_record.fatbase = f.pos();
            int fatsize = 0;
            uint8_t longentrynum = 0;
            msdos_dir* shortentry = 0;
            if ((slot->msdos_dir_entry.attr & ATTR_EXT_MASK) == ATTR_EXT)
            { // Long FAT Entry
                longentrynum = slot->msdos_dir_slot.lfn.seqnum.lfn_recseqnum;
                if (slot->msdos_dir_slot.lfn.char0 == DELETED_FLAG)
                { // Search deleted
                    longentrynum = 0;
                    uint8_t alias_checksum = slot->msdos_dir_slot.alias_checksum;
                    for (int i = 0; i < 63; i++)
                    {
                        if (slot[i].msdos_dir_slot.alias_checksum == alias_checksum)
                        {
                            longentrynum++;
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                // 拼接长文件名
                int pos = 0;
                for (int i = longentrynum - 1; i >= 0 && pos < 600 - 26; i--) // 防止溢出
                {
                    memcpy(strbuf + pos, slot[i].msdos_dir_slot.name0_4, 10);
                    pos += 10;
                    memcpy(strbuf + pos, slot[i].msdos_dir_slot.name5_10, 12);
                    pos += 12;
                    memcpy(strbuf + pos, slot[i].msdos_dir_slot.name11_12, 4);
                    pos += 4;
                }
                memcpy(file_record.filename, strbuf, sizeof(file_record.filename) - 2);
                shortentry = &slot[longentrynum];
                if (shortentry->msdos_dir_slot.attr == ATTR_EXT)
                {
                    // Error here
                }
            }
            else if (slot->msdos_dir_entry.attr != 0)
            { // Short FAT Entry
                shortentry = slot;
            }
            else
            {
                return 0;
            }
            file_record.flags = shortentry->msdos_dir_entry.attr;
            file_record.accesstime = shortentry->msdos_dir_entry.adate; // 访问（日期）dos时间
            file_record.creationtime = shortentry->msdos_dir_entry.cdate;// 创建（日期）dos时间
            file_record.modifiedtime = shortentry->msdos_dir_entry.date;
            file_record.fatindx = shortentry->msdos_dir_entry.start | (shortentry->msdos_dir_entry.starthi << 16);
            file_record.databegin = file_record.fatindx - this->fat_header.fats;
            file_record.databegin *= this->sectors_per_cluster * this->sector_size;
            file_record.databegin += this->direntryoff;
            file_record.datasize = shortentry->msdos_dir_entry.size;

            if ((file_record.flags & ATTR_DIR) != 0)
            {
                if ((file_record.filename[0] == '.' && file_record.filename[2] == '\0') ||
                        (file_record.filename[0] == '.' && file_record.filename[0] == '.'  && file_record.filename[4] == '\0'))
                {
                    // skip directory '.'  '..'
                }
                else
                {
                    this->filesystem[findx].push_back(file_record); // 以簇号作为文件树生成索引

                    // Tranvese sub folder
                    unsigned char buf[BLOCK_SIZE + PRSIZE]; // 保护措施，防止FAT属性被1M大小分割
                    memset(buf, 0, sizeof(buf));
                    f.seek(file_record.databegin);
                    f.read((char*)buf, BLOCK_SIZE + PRSIZE);
                    unsigned long offset_l = 0;
                    while (offset_l < BLOCK_SIZE)
                    {
                        unsigned long readsize = readFATFile(f , buf + offset_l, BLOCK_SIZE - offset_l, file_record.fatindx);
                        offset_l += readsize;
                        if (readsize == 0) // 读取到结束
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                this->filesystem[findx].push_back(file_record);
            }

            fatsize = sizeof(msdos_dir) * (longentrynum + 1); // N * Long Entry + Short Entry
            return fatsize;
        }

        virtual void AddSubDirs(QQueue<unsigned long long>& dirobj, unsigned long long base)
        {
            const int CLUSTER = 4096; // 按簇读取
            unsigned long indx = (unsigned long)((base - this->direntryoff) / sizeof(msdos_dir));
            
        }

        virtual void readFAT(QFile& f)
        {
            QQueue<unsigned long long> dirobj;
            dirobj.enqueue(this->direntryoff);
            while (dirobj.size() > 0)
            {
                unsigned long long base = dirobj.dequeue();
                AddSubDirs(dirobj, base);
            }

            /*
            while (!rootend)
            {//............todo 改进new

                unsigned char *buf = new unsigned char[BLOCK_SIZE + PRSIZE]; // 保护措施，防止FAT属性被1M大小分割
                memset(buf, 0, sizeof(BLOCK_SIZE + PRSIZE));
                f.seek(offset);
                f.read((char*)buf, BLOCK_SIZE + PRSIZE);
                unsigned long offset_l = 0;
                while (offset_l < BLOCK_SIZE)
                {
                    unsigned long readsize = readFATFile(f , buf + offset_l, BLOCK_SIZE - offset_l, FAT_ROOT_INDX);
                    offset_l += readsize;
                    if (readsize == 0) // 读取到结束
                    {
                        break;
                    }
                }

                offset += BLOCK_SIZE; // 如果目录较大则一部分文件会重复

            }
            */
        }

    private:

        unsigned long long GetNewIndx()
        {
            static unsigned long long gindx = FAT_ROOT_INDX + 1;
            return gindx++;
        }

        fat_boot_sector fat_header;
        int errtype; // 如果分区存在错误则记录
        int sectors_per_cluster;
        int sector_size;
        unsigned long long direntryoff;
        unsigned long long direntrysize;
        QHash<unsigned long, QVector<QFatFileRecord>> filesystem;
    };
};
#endif // FAT32_H

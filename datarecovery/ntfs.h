#ifndef NTFS_H
#define NTFS_H

#pragma pack(push)
#pragma pack(1)

#include <stdint.h>
#include "partition.h"

namespace NTFS
{

    enum
    {
        HEADER_OFF = 0,
        MFT_SIZE = 0x400,
        NTFS_Magic = 0x5346544E,
        FILE_Magic = 0x454C4946,
        BLOCK_SIZE = 0x100000,
        MFT_ROOT_INDX = 5, // 根目录MFT索引为5
    };

    enum FILE_ATTRIBUTE
    {
        FILE_ATTRIBUTE_READONLY =           0x1,
        FILE_ATTRIBUTE_HIDDEN =             0x2,
        FILE_ATTRIBUTE_SYSTEM =             0x4,
        FILE_ATTRIBUTE_NONE =               0x8,
        FILE_ATTRIBUTE_NONE1 =              0x10,
        FILE_ATTRIBUTE_ARCHIVE =            0x20,
        FILE_ATTRIBUTE_DEVICE =             0x40,
        FILE_ATTRIBUTE_NORMAL =             0x80,
        FILE_ATTRIBUTE_TEMPORARY =          0x100,
        FILE_ATTRIBUTE_SPARSE_FILE =        0x200,
        FILE_ATTRIBUTE_REPARSE_POINT =      0x400,
        FILE_ATTRIBUTE_COMPRESSED =         0x800,
        FILE_ATTRIBUTE_OFFLINE =            0x1000,
        FILE_ATTRIBUTE_NOT_CONTENT_INDEXED =0x2000,
        FILE_ATTRIBUTE_ENCRYPTED =          0x4000,
        FILE_ATTRIBUTE_NONE2 =              0x8000,
        FILE_ATTRIBUTE_NONE3 =              0x10000,
        FILE_ATTRIBUTE_NONE4 =              0x20000,
        FILE_ATTRIBUTE_NONE5 =              0x40000,
        FILE_ATTRIBUTE_NONE6 =              0x80000,
        FILE_ATTRIBUTE_NONE7 =              0x100000,
        FILE_ATTRIBUTE_NONE8 =              0x200000,
        FILE_ATTRIBUTE_NONE9 =              0x400000,
        FILE_ATTRIBUTE_NONE10 =             0x800000,
        FILE_ATTRIBUTE_NONE11 =             0x1000000,
        FILE_ATTRIBUTE_NONE12 =             0x2000000,
        FILE_ATTRIBUTE_NONE13 =             0x4000000,
        FILE_ATTRIBUTE_NONE14 =             0x8000000,
        FILE_ATTRIBUTE_DIRECTORY =          0x10000000,
        FILE_ATTRIBUTE_INDEXVIEW =          0x20000000,
    };

    enum NTFS_NAMESPACE
    {
        NAMESPACE_POSIX     = 0,
        NAMESPACE_WIN32     = 1,
        NAMESPACE_DOS       = 2,
        NAMESPACE_WIN32DOS  = 3
    };

    struct ntfs_boot_sector
    {
        uint8_t     ignored[3];	/* 0x00 Boot strap short or near jump */
        uint32_t    system_id;	/* 0x03 Name : NTFS */
        uint8_t     padding[4];
        uint16_t    sector_size;	/* 0x0B bytes per logical sector */
        uint8_t     sectors_per_cluster;	/* 0x0D sectors/cluster */
        uint16_t    reserved;	/* 0x0E reserved sectors = 0 */
        uint8_t     fats;		/* 0x10 number of FATs = 0 */
        uint8_t     dir_entries[2];	/* 0x11 root directory entries = 0 */
        uint8_t     sectors[2];	/* 0x13 number of sectors = 0 */
        uint8_t     media;		/* 0x15 media code (unused) */
        uint16_t    fat_length;	/* 0x16 sectors/FAT = 0 */
        uint16_t    secs_track;	/* 0x18 sectors per track */
        uint16_t	heads;		/* 0x1A number of heads */
        uint32_t	hidden;		/* 0x1C hidden sectors (unused) */
        uint32_t	total_sect;	/* 0x20 number of sectors = 0 */
        uint8_t     physical_drive;	/* 0x24 physical drive number  */
        uint8_t     unused;		/* 0x25 */
        uint16_t	reserved2;	/* 0x26 usually 0x80 */
        uint64_t	sectors_nbr;	/* 0x28 total sectors nbr */
        uint64_t	mft_lcn;	/* 0x30 Cluster location of mft data.*/
        uint64_t	mftmirr_lcn;	/* 0x38 Cluster location of copy of mft.*/
        int8_t      clusters_per_mft_record;		/* 0x40 */
        uint8_t  	reserved0[3];               	/* zero */
        int8_t      clusters_per_index_record;	/* 0x44 clusters per index block */
        uint8_t  	reserved1[3];               	/* zero */
        uint64_t 	volume_serial_number;       	/* 0x48 Irrelevant (serial number). */
        uint32_t 	checksum;                   	/* 0x50 Boot sector checksum. */
        uint8_t  	bootstrap[426];             	/* 0x54 Irrelevant (boot up code). */
        uint16_t	marker;				/* 0x1FE */
    };

    struct ntfs_mft_record
    {
      uint32_t	magic;		/* FILE */
      uint16_t	usa_ofs;
      uint16_t	usa_count;
      uint64_t	lsn;
      uint16_t	sequence_number;
      uint16_t	link_count;
      uint16_t	attrs_offset;	/* Must be aligned to 8-byte boundary */
      uint16_t	flags;
      uint32_t	bytes_in_use;	/* Must be aligned to 8-byte boundary */
      uint32_t	bytes_allocated;
      uint64_t	base_mft_record;
      uint16_t	next_attr_instance;
      uint16_t	reserved;		/* NTFS 3.1+ */
      uint32_t	mft_record_number;	/* NTFS 3.1+ */
      uint8_t   padding1[8];
      uint8_t   padding[0x3c8];
    };

    struct ntfs_attribheader
    {
      uint32_t type;		/* Attribute Type (e.g. 0x10, 0x60) */
      uint32_t cbAttribute;         /* Length (including this header) */
      uint8_t  bNonResident;        /* Non-resident flag */
      uint8_t  cName;               /* Name length */
      uint16_t offName;             /* Offset to the Attribute */
      uint16_t flags;               /* Flags */
      uint16_t idAttribute;         /* Attribute Id (a) */
    };

    struct ntfs_attribresident
    {
      ntfs_attribheader header;
      uint32_t cbAttribData;    	/* Length of the Attribute */
      uint16_t offAttribData;   	/* Offset to the Attribute */
      uint8_t  bIndexed;            /* Indexed flag */
      uint8_t  padding;             /* 0x00 Padding */
    };

    struct ntfs_attribnonresident
    {
      ntfs_attribheader header;
      uint64_t startVCN;            /* Starting VCN */
      uint64_t lastVCN;             /* Last VCN */
      uint16_t offDataRuns;         /* Offset to the Data Runs */
      uint16_t compUnitSize;    	/* Compression Unit Size (b) */
      uint32_t padding;             /* Padding */
      uint64_t cbAllocated;         /* Allocated size of the attribute (c) */
      uint64_t cbAttribData;    	/* Real size of the attribute */
      uint64_t cbInitData;          /* Initialized data size of the stream (d) */
    };

    struct TD_INDEX_HEADER
    {
        uint32_t entries_offset;/* Byte offset from the INDEX_HEADER to first
                                       INDEX_ENTRY, aligned to 8-byte boundary.  */
        uint32_t index_length;  /* Data size in byte of the INDEX_ENTRY's,
                                       including the INDEX_HEADER, aligned to 8. */
        uint32_t allocated_size;/* Allocated byte size of this index (block),
                                       multiple of 8 bytes. See more below.      */
            /*
               For the index root attribute, the above two numbers are always
               equal, as the attribute is resident and it is resized as needed.

               For the index allocation attribute, the attribute is not resident
               and the allocated_size is equal to the index_block_size specified
               by the corresponding INDEX_ROOT attribute minus the INDEX_BLOCK
               size not counting the INDEX_HEADER part (i.e. minus -24).
             */
        uint8_t ih_flags;    	/* Bit field of INDEX_HEADER_FLAGS.  */
        uint8_t reserved[3];    /* Reserved/align to 8-byte boundary.*/
    };

    struct TD_FILE_NAME_ATTR
    {
        uint64_t mft_father_record_number : 48;	/* Father MFT number */
        uint64_t sequence_number : 16;
        int64_t creation_time;		/* Time file was created. */
        int64_t last_data_change_time;	/* Time the data attribute was last
                           modified. */
        int64_t last_mft_change_time;	/* Time this mft record was last
                           modified. */
        int64_t last_access_time;		/* Last time this mft record was
                           accessed. */
        int64_t allocated_size;		/* Byte size of on-disk allocated space
                           for the data attribute.  So for
                           normal $DATA, this is the
                           allocated_size from the unnamed
                           $DATA attribute and for compressed
                           and/or sparse $DATA, this is the
                           compressed_size from the unnamed
                           $DATA attribute.  NOTE: This is a
                           multiple of the cluster size. */
        int64_t data_size;			/* Byte size of actual data in data
                           attribute. */
        uint32_t file_attributes;	/* Flags describing the file. */
        union
        {
            struct
            {
                uint16_t packed_ea_size;	/* Size of the buffer needed to
                               pack the extended attributes
                               (EAs), if such are present.*/
                uint16_t reserved;		/* Reserved for alignment. */
            };
            uint32_t reparse_point_tag;		/* Type of reparse point,
                               present only in reparse
                               points and only if there are
                               no EAs. */
        };
        uint8_t file_name_length;			/* Length of file name in
                               (Unicode) characters. */
        uint8_t file_name_type;	/* Namespace of the file name.*/ // NTFS_NAMESPACE
        char *file_name[0];			/* File name in Unicode. */
    };

    struct TD_INDEX_ROOT
    {
        uint32_t type;			/* Type of the indexed attribute. Is
                           $FILE_NAME for directories, zero
                           for view indexes. No other values
                           allowed. */
        uint32_t collation_rule;	/* Collation rule used to sort the
                           index entries. If type is $FILE_NAME,
                           this must be COLLATION_FILE_NAME. */
        uint32_t index_block_size;	/* Size of index block in bytes (in
                           the index allocation attribute). */
        int8_t clusters_per_index_block;/* Size of index block in clusters (in
                           the index allocation attribute), when
                           an index block is >= than a cluster,
                           otherwise sectors per index block. */
        uint8_t reserved[3];		/* Reserved/align to 8-byte boundary. */
        TD_INDEX_HEADER index;		/* Index header describing the
                           following index entries. */
    };


    #pragma pack(pop)



    class QNtfsFileRecord : public QRecordFile
    {
    public:
        // 描述一般文件，包括NTFS元文件及普通文件，考虑到内存占用，采用简单类型存储
        unsigned long long mftbase;     // 文件所属MFT相对于Disk偏移，用于扩展功能
        unsigned long long creationtime;// 文件创建时间
        unsigned long long modifiedtime;// 文件最后修改时间
        unsigned long long accesstime;  // 文件最后访问时间
        unsigned long long dataallocsize;// 文件分配大小(按簇存储)
        unsigned long long datasize;    // 文件实际大小
        unsigned long long databegin;   // nonresident = 0 常驻属性，文件内容相对于Disk偏移
                                        // nonresident = 1 非常驻属性，文件run-list相对于Disk偏移
        unsigned long flags;            // 文件属性
        unsigned long mftindx;          // 本文件MFT号
        unsigned long fathermftindx;    // 父目录MFT号
        unsigned long nonresident;      // 是否为常驻
        unsigned long runlistsize;      // nonresident = 1 非常驻属性，run-list长度
        char16_t filename[260];         // 文件名

    public:
        virtual bool IsDir() { return flags & FILE_ATTRIBUTE_DIRECTORY; }
        virtual bool IsFile() { return flags & FILE_ATTRIBUTE_ARCHIVE; } // ??
        virtual unsigned long long GetFileSize() { return datasize; }
        virtual int GetType() { return PARTITION_TYPE_NTFS; }
    };

    class QNtfsPartition : public QPartition
    {
    public:
        QNtfsPartition(QFile& f, unsigned long long begin, unsigned long long size)
        {
            this->begin = begin;
            this->size = size;
            this->type = 0; // 设置为无效
            this->readHeader(f);
            this->mftoff = begin + this->ntfs_header.mft_lcn * this->sectors_per_cluster * this->sector_size;
            this->readMFT(f);

            this->type = PARTITION_TYPE_NTFS; // 设置为有效
        };

        virtual ~QNtfsPartition() {}

    private:
        virtual QVector<unsigned long long> readFileData(QFile& file, QNtfsFileRecord& filerecord)
        {
            // 获取文件存储区块
            QVector<unsigned long long> data;
            if (filerecord.nonresident == 0)
            {
                // 常驻属性
                data.push_back(filerecord.databegin);
                data.push_back(filerecord.datasize);
            }
            else
            {
                // 非常驻属性
                unsigned char buf[MFT_SIZE] = { 0 };
                file.seek(filerecord.databegin);
                file.read((char*)buf, MFT_SIZE);
                unsigned long wl = filerecord.runlistsize;
                if (wl > MFT_SIZE)
                {
                    wl = MFT_SIZE;
                }

                long long baselcn = 0;
                for (unsigned long indx = 0; indx < wl; indx++)
                {
                    uint8_t sl = buf[indx] & 0xf; // size len
                    uint8_t ol = (buf[indx] >> 4) & 0xf; // offset len
                    uint8_t sl_ = sl, ol_ = ol;
                    if (sl > 8 || ol > 8 || sl == 0 || ol ==0)
                    {// 数据有误
                        break;
                    }
                    long long lcnsize = 0, curlcn = 0;
                    for (lcnsize = (uint8_t)buf[indx + sl--]; sl > 0; sl--)
                    {
                        lcnsize = (lcnsize << 8) + (uint8_t)buf[indx + sl];
                    }
                    indx += sl_;
                    for (curlcn = (int8_t)buf[indx + ol--]; ol > 0; ol--) // 负偏移
                    {
                        curlcn = (curlcn << 8) + (uint8_t)buf[indx + ol];
                    }
                    baselcn += curlcn; // 每个runlist记录偏移地址都是相对于上一次
                    data.push_back(baselcn * this->sectors_per_cluster * this->sector_size);
                    data.push_back(lcnsize * this->sectors_per_cluster * this->sector_size);
                    indx += ol_;
                }
            }
            return data;
        }

        virtual void readMFT(QFile& f)
        {
            // 读取$MFT，从run-list获取整个分区所有文件(夹)的MFT元数据
            ntfs_mft_record mftrecord;
            QNtfsFileRecord $mft; // $MFT元文件
            memset(&mftrecord, 0, sizeof(mftrecord));
            f.seek(this->mftoff);
            f.read((char*)&mftrecord, sizeof(mftrecord));
            if (this->readMFTFile(mftrecord, $mft))
            {
                // 解析$MFT的runlist，该数据记录所有分区文件MFT位置
                QVector<unsigned long long> data = this->readFileData(f, $mft);
                for (int i = 0; i < data.length() / 2; i++)
                {
                    // 0:offset 1:size
                    unsigned long long runlistoff = data[i * 2];
                    unsigned long long runlistsize = data[i * 2 + 1];
                    for (unsigned long j = 0; j < (runlistsize + BLOCK_SIZE - 1) / BLOCK_SIZE; j++)
                    {
                        ntfs_mft_record mft_record_[BLOCK_SIZE / MFT_SIZE];
                        memset(&mft_record_, 0, sizeof(mft_record_));
                        f.seek(runlistoff + BLOCK_SIZE * j);
                        f.read((char*)&mft_record_, sizeof(mft_record_));
                        for (int k = 0; k < BLOCK_SIZE / MFT_SIZE; k++)
                        {
                            QNtfsFileRecord file;
                            file.mftbase = runlistoff + BLOCK_SIZE * j + MFT_SIZE * k;
                            if (this->readMFTFile(mft_record_[k], file))
                            {
                                filesystem[file.fathermftindx].push_back(file); // 插入记录
                            }
                        }
                    }
                }
            }
        }

        virtual bool readMFTFile(const ntfs_mft_record& mftrecord, QNtfsFileRecord& filerecord)
        {
            if (mftrecord.magic != FILE_Magic)
            {
                this->errtype = 100; // begins at 100
                return false;
            }

            filerecord.mftindx = mftrecord.mft_record_number; // 本文件MFT号
            filerecord.runlistsize = 0;
            uint16_t attriboff = mftrecord.attrs_offset;
            while (attriboff < MFT_SIZE)
            {
                ntfs_attribheader* header = (ntfs_attribheader*)((char*)&mftrecord + attriboff);
                if (header->type == 0xffffffff || (header->type & 0xF0) == 0)
                    break; // 遇到结尾
                if (header->type == 0x30)
                {
                    // 文件名属性，必常驻
                    if (header->bNonResident != 0)
                    {
                        this->errtype = 101;
                        break; // Attribute Error
                    }
                    ntfs_attribresident* header_ = (ntfs_attribresident*)header;
                    TD_FILE_NAME_ATTR* nameattr = (TD_FILE_NAME_ATTR*)((char*)header_ + header_->offAttribData);
                    unsigned long namelen = nameattr->file_name_length * 2;
                    if (namelen > sizeof(filerecord.filename)) // 超过则截取
                    {
                        namelen = sizeof(filerecord.filename) - 2;
                    }
                    filerecord.creationtime = nameattr->creation_time;
                    filerecord.modifiedtime = nameattr->last_data_change_time;
                    filerecord.accesstime = nameattr->last_access_time;
                    filerecord.flags = nameattr->file_attributes;
                    memcpy(filerecord.filename, (wchar_t*)(nameattr + 1), namelen); // 这里用memcpy防止原始数据未以\0结尾
                    filerecord.filename[nameattr->file_name_length] = 0;
                    filerecord.fathermftindx = (unsigned long)nameattr->mft_father_record_number;
                }
                else if (header->type == 0x80)
                {
                    filerecord.nonresident = header->bNonResident;
                    if (header->bNonResident == 0)
                    {
                        // 文件数据属性，常驻情况只存在于小文件
                        ntfs_attribresident* header_ = (ntfs_attribresident*)header;
                        filerecord.databegin = filerecord.mftbase + attriboff + header_->offAttribData; // 此偏移开始为文件内容
                        filerecord.datasize = header_->cbAttribData;
                        filerecord.dataallocsize = header_->cbAttribData;
                    }
                    else
                    {
                        ntfs_attribnonresident* header_ = (ntfs_attribnonresident*)header;
                        filerecord.databegin = filerecord.mftbase + attriboff + header_->offDataRuns;// 此偏移开始为run-list内容
                        filerecord.datasize = header_->cbAttribData;
                        filerecord.dataallocsize = header_->cbAllocated;
                        filerecord.runlistsize = header->cbAttribute - sizeof(ntfs_attribnonresident);
                    }
                }
                attriboff += header->cbAttribute;
            }
            return true;
        }

        virtual void readHeader(QFile& f)
        {
            f.seek(this->begin + HEADER_OFF);
            memset(&this->ntfs_header, 0, sizeof(this->ntfs_header));
            f.read((char*)&this->ntfs_header, sizeof(this->ntfs_header));
            this->sectors_per_cluster = this->ntfs_header.sectors_per_cluster;
            this->sector_size = this->ntfs_header.sector_size;
            this->errtype = 0;
            if (this->ntfs_header.marker != 0xAA55)
            {
                this->errtype = 1;
                return;
            }
            if (this->ntfs_header.reserved > 0)
            {
                this->errtype = 2;
                return;
            }
            if (this->ntfs_header.fats > 0)
            {
                this->errtype = 3;
                return;
            }
            if (this->ntfs_header.dir_entries[0] != 0 || this->ntfs_header.dir_entries[1] != 0 )
            {
                this->errtype = 4;
                return;
            }
            if (this->ntfs_header.sectors[0]!=0 || this->ntfs_header.sectors[1]!=0)
            {
                this->errtype = 5;
                return;
            }
            if (this->ntfs_header.fat_length !=0)
            {
                this->errtype = 6;
                return;
            }
            if( this->ntfs_header.total_sect != 0)
            {
                this->errtype = 7;
                return;
            }
            if (this->ntfs_header.sectors_nbr == 0)
            {
                this->errtype = 8;
                return;
            }
            switch (this->ntfs_header.sectors_per_cluster)
            {
              case 1: case 2: case 4: case 8: case 16: case 32: case 64: case 128:
                break;
              default:
                this->errtype = 9;
                return;
            }
            // 生成Unique Id
            QByteArray headdata = QByteArray::fromRawData((char*)&this->ntfs_header, sizeof(this->ntfs_header));
            this->uniqueid = QCryptographicHash::hash(headdata, QCryptographicHash::Md5).toHex();
        }

    private:
        QString uniqueid; // 唯一标识该分区的字符串，用于数据库查询
        ntfs_boot_sector ntfs_header;
        int errtype; // 如果分区存在错误则记录
        unsigned long long mftoff;
        int sectors_per_cluster;
        int sector_size;
        QHash<unsigned long, QVector<QNtfsFileRecord>> filesystem;// 构造文件系统树，如果内存占用过大考虑存储到数据库

    public:
        virtual bool IsValid()
        {
            return this->errtype == 0;
        }

        // 获取根目录文件
        virtual QVector<QRecordFile*> GetRootFiles()
        {
            QVector<QRecordFile*> result;
            for (QNtfsFileRecord& ff : filesystem[MFT_ROOT_INDX])
            {
                result.push_back(static_cast<QRecordFile*>(&ff));
            }
            return result;
        }

        // 获取当前目录下的文件和目录
        virtual QVector<QRecordFile*> ListFiles(const QRecordFile* filerecord)
        {
            QVector<QRecordFile*> result;
            if (const_cast<QRecordFile*>(filerecord)->GetType() != PARTITION_TYPE_NTFS)
            {
                return result;
            }
            unsigned long mftindx = ((QNtfsFileRecord*)filerecord)->mftindx;
            for (QNtfsFileRecord& ff : filesystem[mftindx])
            {
                result.push_back(static_cast<QRecordFile*>(&ff));
            }
            return result;
        }

        // 根据路径定位文件
        virtual QRecordFile* GetFileForPath(const QString& path)
        {
            // 路径形式:  /a/b/c/d.txt
            QStringList seg = path.split('/');
            // todo
            return 0;
        }

        //拷贝文件
        virtual bool CopyFile(const QString& dstpath, const QString& srcpath, QFile& f)
        {
            // todo
            return false;
        }

        //拷贝文件夹及所有子文件
        virtual bool CopyDir(const QString& dstpath, const QString& srcpath, QFile& f)
        {
            // todo
            return false;
        }

        //拷贝文件
        virtual bool CopyFile(const QString& dstpath, const QRecordFile* filerecord, QFile& f)
        {
            // todo
            return false;
        }

        //拷贝文件夹及所有子文件
        virtual bool CopyDir(const QString& dstpath, const QRecordFile* filerecord, QFile& f)
        {
            // todo
            return false;
        }
    };
};

#endif // NTFS_H

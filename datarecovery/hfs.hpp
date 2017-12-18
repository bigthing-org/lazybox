#ifndef HFS_H
#define HFS_H

#define HFS_SUPERBLOCK_SIZE 512
#define HFS_SUPER_MAGIC           0x4244 /* "BD": HFS MDB (super block) */

struct hfs_extent
{
  uint16_t block;
  uint16_t count;
};
typedef struct hfs_extent hfs_extent_rec[3];

typedef struct hfs_mdb hfs_mdb_t;
struct hfs_mdb
{
  uint16_t drSigWord;                  /* 0x00 Signature word indicating fs type */
  uint32_t drCrDate;                   /* 0x02 fs creation date/time */
  uint32_t drLsMod;                    /* 0x06 fs modification date/time */
  uint16_t drAtrb;                     /* 0x0A fs attributes */
  uint16_t drNmFls;                    /* 0x0C number of files in root directory */
  uint16_t drVBMSt;                    /* 0x0E location (in 512-byte blocks)
                      of the volume bitmap */
  uint16_t drAllocPtr;                 /* 0x10 location (in allocation blocks)
                      to begin next allocation search */
  uint16_t drNmAlBlks;                 /* 0x12 number of allocation blocks */
  uint32_t drAlBlkSiz;                 /* 0x14 bytes in an allocation block */
  uint32_t drClpSiz;                   /* 0x18 clumpsize, the number of bytes to
                      allocate when extending a file */
  uint16_t drAlBlSt;                   /* 0x1C location (in 512-byte blocks)
                      of the first allocation block */
  uint32_t drNxtCNID;                  /* 0x1E CNID to assign to the next
                      file or directory created */
  uint16_t drFreeBks;                  /* 0x22 number of free allocation blocks */
  uint8_t  drVN[28];                   /* 0x24 the volume label */
  uint32_t drVolBkUp;                  /* 0x40 fs backup date/time */
  uint16_t drVSeqNum;                  /* 0x44 backup sequence number */
  uint32_t drWrCnt;                    /* 0x46 fs write count */
  uint32_t drXTClpSiz;                 /* 0x4a clumpsize for the extents B-tree */
  uint32_t drCTClpSiz;                 /* 0x4e clumpsize for the catalog B-tree */
  uint16_t drNmRtDirs;                 /* 0x52 number of directories in
                      the root directory */
  uint32_t drFilCnt;                   /* 0x54 number of files in the fs */
  uint32_t drDirCnt;                   /* 0x58 number of directories in the fs */
  uint8_t  drFndrInfo[32];             /* 0x5c data used by the Finder */
  uint16_t drEmbedSigWord;             /* 0x7c embedded volume signature */
  uint32_t drEmbedExtent;              /* 0x7e starting block number (xdrStABN)
                      and number of allocation blocks
                      (xdrNumABlks) occupied by embedded
                      volume */
  uint32_t drXTFlSize;                 /* 0x82 bytes in the extents B-tree */
  hfs_extent_rec drXTExtRec;           /* 0x86 extents B-tree's first 3 extents */
  uint32_t drCTFlSize;                 /* 0x92 bytes in the catalog B-tree */
  hfs_extent_rec drCTExtRec;           /* 0x96 catalog B-tree's first 3 extents */
};

#endif // HFS_H

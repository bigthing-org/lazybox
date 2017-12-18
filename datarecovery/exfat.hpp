
#ifndef EXFAT_H
#define EXFAT_H

#define EXFAT_BS_SIZE	512

struct exfat_super_block {
    unsigned char   jmp_boot[3];            /* boot strap short or near jump */
    unsigned char   oem_id[8];              /* oem-id */
    unsigned char   unused0;                /* 0x00... */
    uint32_t  	unused1[13];
    uint64_t  	start_sector;           /* 0x40 start sector of partition */
    uint64_t  	nr_sectors;             /* number of sectors of partition */
    uint32_t  	fat_blocknr;            /* 0x50 start blocknr of FAT */
    uint32_t  	fat_block_counts;       /* number of FAT blocks */
    uint32_t  	clus_blocknr;           /* start blocknr of cluster */
    uint32_t  	total_clusters;         /* number of total clusters */
    uint32_t  	rootdir_clusnr;         /* 0x60 start clusnr of rootdir */
    uint32_t  	serial_number;          /* volume serial number */
    unsigned char   xxxx01;                 /* ??? (0x00 or any value (?)) */
    unsigned char   xxxx02;                 /* ??? (0x01 or 0x00 (?)) */
    uint16_t  	state;                  /* state of this volume */
    unsigned char   blocksize_bits;         /* bits of block size */
    unsigned char   block_per_clus_bits;    /* bits of blocks per cluster */
    unsigned char   number_of_fats;
    unsigned char   drive_select;           /* Used by INT 13 */
    unsigned char   allocated_percent;      /* 0x70 percentage of allocated space (?) */
    unsigned char   xxxx05[397];            /* ??? (0x00...) */
    uint16_t  	signature;              /* 0xaa55 */
} __attribute__ ((gcc_struct, __packed__));

struct exfat_file_entry
{
  uint8_t  type;
  uint8_t  sec_count;
  uint16_t checksum;
  uint16_t attr;
  uint16_t reserved1;
  uint16_t ctime;
  uint16_t cdate;
  uint16_t mtime;
  uint16_t mdate;
  uint16_t atime;
  uint16_t adate;
  uint8_t  cms;
  uint8_t  mms;
  uint8_t  ctz;
  uint8_t  mtz;
  uint8_t  reserved2[7];
} __attribute__ ((gcc_struct, __packed__));

struct exfat_stream_ext_entry
{
  uint8_t  type;
  uint8_t  sec_flags;
  uint8_t  reserved1;
  uint8_t  name_length;
  uint16_t name_hash;
  uint16_t reserved2;
  uint64_t valid_data_length;
  uint32_t reserved3;
  uint32_t first_cluster;
  uint64_t data_length;
} __attribute__ ((gcc_struct, __packed__));

struct exfat_alloc_bitmap_entry
{
  uint8_t  type;
  uint8_t  bitmap_flags;
  uint8_t  reserved[18];
  uint32_t first_cluster;
  uint64_t data_length;
} __attribute__ ((gcc_struct, __packed__));

#endif // EXFAT_H

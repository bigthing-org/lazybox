#ifndef FAT32_H
#define FAT32_H

#pragma pack(push)
#pragma pack(1)


struct fat_boot_sector {
    uint8_t	ignored[3];	/* 0x00 Boot strap short or near jump */
    int8_t	system_id[8];	/* 0x03 Name - can be used to special case
                   partition manager volumes */
    uint8_t	sector_size[2];	/* 0x0B bytes per logical sector */
    uint8_t	sectors_per_cluster;	/* 0x0D sectors/cluster */
    uint16_t	reserved;	/* 0x0E reserved sectors */
    uint8_t	fats;		/* 0x10 number of FATs */
    uint8_t	dir_entries[2];	/* 0x11 root directory entries */
    uint8_t	sectors[2];	/* 0x13 number of sectors */
    uint8_t	media;		/* 0x15 media code (unused) */
    uint16_t	fat_length;	/* 0x16 sectors/FAT */
    uint16_t	secs_track;	/* 0x18 sectors per track */
    uint16_t	heads;		/* 0x1A number of heads */
    uint32_t	hidden;		/* 0x1C hidden sectors (unused) */
    uint32_t	total_sect;	/* 0x20 number of sectors (if sectors == 0) */

    /* The following fields are only used by FAT32 */
    uint32_t	fat32_length;	/* 0x24=36 sectors/FAT */
    uint16_t	flags;		/* 0x28 bit 8: fat mirroring, low 4: active fat */
    uint8_t	version[2];	/* 0x2A major, minor filesystem version */
    uint32_t	root_cluster;	/* 0x2C first cluster in root directory */
    uint16_t	info_sector;	/* 0x30 filesystem info sector */
    uint16_t	backup_boot;	/* 0x32 backup boot sector */
    uint8_t	BPB_Reserved[12];	/* 0x34 Unused */
    uint8_t	BS_DrvNum;		/* 0x40 */
    uint8_t	BS_Reserved1;		/* 0x41 */
    uint8_t	BS_BootSig;		/* 0x42 */
    uint8_t	BS_VolID[4];		/* 0x43 */
    uint8_t	BS_VolLab[11];		/* 0x47 */
    uint8_t	BS_FilSysType[8];	/* 0x52=82*/

    /* */
    uint8_t	nothing[420];	/* 0x5A */
    uint16_t	marker;
} __attribute__ ((gcc_struct, __packed__));

struct fat_fsinfo {
  uint32_t leadsig;		/* 0x41615252 */
  uint8_t reserved1[480];
  uint32_t strucsig;		/* 0x61417272 */
  uint32_t freecnt;     	/* free clusters 0xfffffffff if unknown */
  uint32_t nextfree;		/* next free cluster */
  uint8_t reserved3[12];
  uint32_t magic3;		/* 0xAA550000 */
} __attribute__ ((gcc_struct, __packed__));

struct msdos_dir_entry {
    int8_t	name[8];		/* 00 name and extension */
    int8_t  ext[3];
    uint8_t	attr;			/* 0B attribute bits */
    uint8_t    lcase;		/* 0C Case for base and extension */
    uint8_t	        ctime_ms;	/* 0D Creation time, milliseconds */
    uint16_t	ctime;		/* 0E Creation time */
    uint16_t	cdate;		/* 10 Creation date */
    uint16_t	adate;		/* 12 Last access date */
    uint16_t        starthi;	/* 14 High 16 bits of cluster in FAT32 */
    uint16_t	time;           /* 16 time, date and first cluster */
        uint16_t        date;		/* 18 */
        uint16_t        start;		/* 1A */
    uint32_t	size;		/* 1C file size (in bytes) */
} __attribute__ ((gcc_struct, __packed__));

/* Up to 13 characters of the name */
struct msdos_dir_slot {
    uint8_t    id;			/* 00 sequence number for slot */
    uint8_t    name0_4[10];		/* 01 first 5 characters in name */
    uint8_t    attr;		/* 0B attribute byte */
    uint8_t    reserved;		/* 0C always 0 */
    uint8_t    alias_checksum;	/* 0D checksum for 8.3 alias */
    uint8_t    name5_10[12];	/* 0E 6 more characters in name */
    uint16_t   sta

#pragma pack(pop)

#endif // FAT32_H

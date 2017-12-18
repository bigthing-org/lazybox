#ifndef EXT2_H
#define EXT2_H

#include "partition.h"

namespace EXT
{

    #define EXT2_NAME_LEN 255

    #define EXT2_SUPERBLOCK_SIZE 1024

    #define EXT2_SB(sb)     (sb)

    /*
     * The second extended file system magic number
     */
    #define EXT2_SUPER_MAGIC	0xEF53
    #define EXT2_MIN_BLOCK_SIZE		1024
    #define	EXT2_MAX_BLOCK_SIZE		4096
    #define EXT2_MIN_BLOCK_LOG_SIZE		  10

    #define EXT2_MIN_BLOCK (EXT2_MIN_BLOCK_SIZE/DEFAULT_SECTOR_SIZE)

    /*
     * File system states
     */
    #define	EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
    #define	EXT2_ERROR_FS			0x0002	/* Errors detected */

    /*
     * Behaviour when detecting errors
     */
    #define EXT2_ERRORS_CONTINUE		1	/* Continue execution */
    #define EXT2_ERRORS_RO			2	/* Remount fs read-only */
    #define EXT2_ERRORS_PANIC		3	/* Panic */
    #define EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

    /*
     * Feature set definitions
     */
    #define EXT2_HAS_COMPAT_FEATURE(sb,mask)                        \
            ( le32(EXT2_SB(sb)->s_feature_compat) & (mask) )
    #define EXT2_HAS_RO_COMPAT_FEATURE(sb,mask)                     \
            ( le32(EXT2_SB(sb)->s_feature_ro_compat) & (mask) )
    #define EXT2_HAS_INCOMPAT_FEATURE(sb,mask)                      \
            ( le32(EXT2_SB(sb)->s_feature_incompat) & (mask) )

    #define EXT2_FEATURE_COMPAT_DIR_PREALLOC        0x0001
    #define EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
    #define EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
    #define EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
    #define EXT2_FEATURE_COMPAT_RESIZE_INO		0x0010
    #define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020
    #define EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
    #define EXT2_FEATURE_COMPAT_ANY			0xffffffff


    #define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER     0x0001
    #define EXT2_FEATURE_RO_COMPAT_LARGE_FILE       0x0002
    //#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR        0x0004
    #define EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
    #define EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
    #define EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
    #define EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
    #define EXT2_FEATURE_RO_COMPAT_ANY		0xffffffff

    #define EXT2_FEATURE_INCOMPAT_COMPRESSION       0x0001
    #define EXT2_FEATURE_INCOMPAT_FILETYPE          0x0002
    #define EXT3_FEATURE_INCOMPAT_RECOVER		0x0004
    #define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008
    #define EXT2_FEATURE_INCOMPAT_META_BG		0x0010
    #define EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
    #define EXT4_FEATURE_INCOMPAT_64BIT		0x0080
    #define EXT4_FEATURE_INCOMPAT_MMP		0x0100
    #define EXT2_FEATURE_INCOMPAT_ANY		0xffffffff

    #define	EXT2_ERRORS_CONTINUE	1
    #define	EXT2_ERRORS_RO		2
    #define	EXT2_ERRORS_PANIC	3
    #define	EXT2_ERRORS_DEFAULT	1

    /* inode: i_mode */
    #define	EXT2_S_IFMT	0xF000		/* format mask  */
    #define	EXT2_S_IFSOCK	0xC000	/* socket */
    #define	EXT2_S_IFLNK	0xA000	/* symbolic link */
    #define	EXT2_S_IFREG	0x8000	/* regular file */
    #define	EXT2_S_IFBLK	0x6000	/* block device */
    #define	EXT2_S_IFDIR	0x4000	/* directory */
    #define	EXT2_S_IFCHR	0x2000	/* character device */
    #define	EXT2_S_IFIFO	0x1000	/* fifo */

    #define	EXT2_S_ISUID	0x0800	/* SUID */
    #define	EXT2_S_ISGID	0x0400	/* SGID */
    #define	EXT2_S_ISVTX	0x0200	/* sticky bit */
    #define	EXT2_S_IRWXU	0x01C0	/* user access rights mask */
    #define	EXT2_S_IRUSR	0x0100	/* read */
    #define	EXT2_S_IWUSR	0x0080	/* write */
    #define	EXT2_S_IXUSR	0x0040	/* execute */
    #define	EXT2_S_IRWXG	0x0038	/* group access rights mask */
    #define	EXT2_S_IRGRP	0x0020	/* read */
    #define	EXT2_S_IWGRP	0x0010	/* write */
    #define	EXT2_S_IXGRP	0x0008	/* execute */
    #define	EXT2_S_IRWXO	0x0007	/* others access rights mask */
    #define	EXT2_S_IROTH	0x0004	/* read */
    #define	EXT2_S_IWOTH	0x0002	/* write */
    #define	EXT2_S_IXOTH	0x0001	/* execute */

    #define EXT2_INUM_ROOT	2


#pragma pack(push)
#pragma pack(1)
    /*
     * Structure of the super block
     */
    struct ext2_super_block
    {
        uint8_t     padding[0x400];
        uint32_t	s_inodes_count;		/* Inodes count */
        uint32_t	s_blocks_count;		/* Blocks count */
        uint32_t	s_r_blocks_count;	/* Reserved blocks count */
        uint32_t	s_free_blocks_count;	/* Free blocks count */
        uint32_t	s_free_inodes_count;	/* Free inodes count */
        uint32_t	s_first_data_block;	/* First Data Block */
        uint32_t	s_log_block_size;	/* Block size */
        int32_t     s_log_frag_size;	/* Fragment size */
        uint32_t	s_blocks_per_group;	/* # Blocks per group */
        uint32_t	s_frags_per_group;	/* # Fragments per group */
        uint32_t	s_inodes_per_group;	/* # Inodes per group */
        uint32_t	s_mtime;		/* Mount time */
        uint32_t	s_wtime;		/* Write time */
        uint16_t	s_mnt_count;		/* Mount count */
        int16_t     s_max_mnt_count;	/* Maximal mount count */
        uint16_t	s_magic;		/* Magic signature */
        uint16_t	s_state;		/* File system state */
        uint16_t	s_errors;		/* Behaviour when detecting errors */
        uint16_t	s_minor_rev_level; 	/* minor revision level */
        uint32_t	s_lastcheck;		/* time of last check */
        uint32_t	s_checkinterval;	/* max. time between checks */
        uint32_t	s_creator_os;		/* OS */
        uint32_t	s_rev_level;		/* Revision level */
        uint16_t	s_def_resuid;		/* Default uid for reserved blocks */
        uint16_t	s_def_resgid;		/* Default gid for reserved blocks */
        /*
         * These fields are for EXT2_DYNAMIC_REV superblocks only.
         *
         * Note: the difference between the compatible feature set and
         * the incompatible feature set is that if there is a bit set
         * in the incompatible feature set that the kernel doesn't
         * know about, it should refuse to mount the filesystem.
         *
         * e2fsck's requirements are more strict; if it doesn't know
         * about a feature in either the compatible or incompatible
         * feature set, it must abort and not try to meddle with
         * things it doesn't understand...
         */
        uint32_t	s_first_ino; 		/* First non-reserved inode */
        uint16_t   s_inode_size; 		/* size of inode structure */
        uint16_t	s_block_group_nr; 	/* block group # of this superblock */
        uint32_t	s_feature_compat; 	/* compatible feature set */
        uint32_t	s_feature_incompat; 	/* incompatible feature set */
        uint32_t	s_feature_ro_compat; 	/* readonly-compatible feature set */
        uint8_t	s_uuid[16];		/* 128-bit uuid for volume */
        char	s_volume_name[16]; 	/* volume name */
        char	s_last_mounted[64]; 	/* directory where last mounted */
        uint32_t	s_algorithm_usage_bitmap; /* For compression */
        /*
         * Performance hints.  Directory preallocation should only
         * happen if the EXT2_COMPAT_PREALLOC flag is on.
         */
        uint8_t	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
        uint8_t	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
        uint16_t	s_reserved_gdt_blocks;	/* Per group table for online growth */
        /*
         * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
         */
        uint8_t		s_journal_uuid[16];	/* uuid of journal superblock */
        uint32_t	s_journal_inum;		/* inode number of journal file */
        uint32_t	s_journal_dev;		/* device number of journal file */
        uint32_t	s_last_orphan;		/* start of list of inodes to delete */
        uint32_t	s_hash_seed[4];		/* HTREE hash seed */
        uint8_t		s_def_hash_version;	/* Default hash version to use */
        uint8_t		s_jnl_backup_type; 	/* Default type of journal backup */
        uint16_t	s_desc_size;		/* Group desc. size: INCOMPAT_64BIT */
        uint32_t	s_default_mount_opts;
        uint32_t	s_first_meta_bg;	/* First metablock group */
        uint32_t	s_mkfs_time;		/* When the filesystem was created */
        uint32_t	s_jnl_blocks[17]; 	/* Backup of the journal inode */
        uint32_t	s_blocks_count_hi;	/* Blocks count high 32bits */
        uint32_t	s_r_blocks_count_hi;	/* Reserved blocks count high 32 bits*/
        uint32_t	s_free_blocks_hi; 	/* Free blocks count */
        uint16_t	s_min_extra_isize;	/* All inodes have at least # bytes */
        uint16_t	s_want_extra_isize; 	/* New inodes should reserve # bytes */
        uint32_t	s_flags;		/* Miscellaneous flags */
        uint16_t   	s_raid_stride;		/* RAID stride */
        uint16_t   	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
        uint64_t   	s_mmp_block;            /* Block for multi-mount protection */
        uint32_t   	s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
        uint8_t		s_log_groups_per_flex;	/* FLEX_BG group size */
        uint8_t    	s_reserved_char_pad;
        uint16_t	s_reserved_pad;		/* Padding to next 32bits */
        uint64_t	s_kbytes_written;	/* nr of lifetime kilobytes written */
        uint32_t	s_snapshot_inum;	/* Inode number of active snapshot */
        uint32_t	s_snapshot_id;		/* sequential ID of active snapshot */
        uint64_t	s_snapshot_r_blocks_count; /* reserved blocks for active
                              snapshot's future use */
        uint32_t	s_snapshot_list;	/* inode number of the head of the on-disk snapshot list */
        uint32_t	s_error_count;		/* number of fs errors */
        uint32_t	s_first_error_time;	/* first time an error happened */
        uint32_t	s_first_error_ino;	/* inode involved in first error */
        uint64_t	s_first_error_block;	/* block involved of first error */
        uint8_t		s_first_error_func[32];	/* function where the error happened */
        uint32_t	s_first_error_line;	/* line number where error happened */
        uint32_t	s_last_error_time;	/* most recent time of an error */
        uint32_t	s_last_error_ino;	/* inode involved in last error */
        uint32_t	s_last_error_line;	/* line number where error happened */
        uint64_t	s_last_error_block;	/* block involved of last error */
        uint8_t		s_last_error_func[32];	/* function where the error happened */
        uint8_t		s_mount_opts[64];
        uint32_t	s_usr_quota_inum;	/* inode number of user quota file */
        uint32_t	s_grp_quota_inum;	/* inode number of group quota file */
        uint32_t	s_overhead_blocks;	/* overhead blocks/clusters in fs */
        uint32_t   	s_reserved[108];        /* Padding to the end of the block */
        uint32_t	s_checksum;		/* crc32c(superblock) */
    };

    struct ext2_group_desc
    {
        uint32_t bg_block_bitmap;	/* Id of the first block of the "block bitmap" */
        uint32_t bg_inode_bitmap;	/* Id of the first block of the "inode bitmap" */
        uint32_t bg_inode_table;	/* Id of the first block of the "inode table" */
        uint16_t bg_free_blocks_count;	/* Total number of free blocks */
        uint16_t bg_free_inodes_count;	/* Total number of free inodes */
        uint16_t bg_used_dirs_count;	/* Number of inodes allocated to directories */
        uint16_t bg_pad;		/* Padding the structure on a 32bit boundary */
        uint32_t bg_reserved[3];	/* Future implementation */
    };

    struct ext2_inode
    {
        uint16_t i_mode;		/* File type + access rights */
        uint16_t i_uid;
        uint32_t i_size;
        uint32_t i_atime;
        uint32_t i_ctime;
        uint32_t i_mtime;
        uint32_t i_dtime;
        uint16_t i_gid;
        uint16_t i_links_count;
        uint32_t i_blocks;		/* 512 bytes blocks ! */
        uint32_t i_flags;
        uint32_t i_osd1;

        /*
         * [0] -> [11] : block number (32 bits per block)
         * [12]        : indirect block number
         * [13]        : bi-indirect block number
         * [14]        : tri-indirect block number
         */
        uint32_t i_block[15];

        uint32_t i_generation;
        uint32_t i_file_acl;
        uint32_t i_dir_acl;
        uint32_t i_faddr;
        uint8_t i_osd2[12];
    } ;

    struct ext2_directory_entry
    {
        uint32_t inode;		/* inode number or 0 (unused) */
        uint16_t rec_len;		/* offset to the next dir. entry */
        uint8_t name_len;		/* name length */
        uint8_t file_type;
        char name[EXT2_NAME_LEN];
    } ;

#pragma pack(pop)


    class QExtxTPartition : public QPartition
    {
        /*
         *  d 块大小   超级块扇区数    组描述符扇区数    块位图块扇区数     节点位图块
            0 1024    2,2           4,2             6,2             8,2
            1 2048    2,2           4,4             8,4             12,4
            2 4096    2,2           8,8             16,8            24,8
            3 8192    2,2           16,16           32,16           48,16
            4 16384   2,2           32,32           64,32           96,32
            5 32768   2,2           64,64           128,64          192,64
            6 65536   2,2           128,128         256,128         384,128
         */
        struct
        {
            uint16_t superoff;
            uint16_t supersize;
            uint16_t groupoff;
            uint16_t groupsize;
            uint16_t blockoff;
            uint16_t blocksize;
            uint16_t nodeoff;
            uint16_t nodesize;
        } _offs[] =
        {
            { 2,2, 4,2    , 6,2    , 8,2     },
            { 2,2, 4,4    , 8,4    , 12,4    },
            { 2,2, 8,8    , 16,8   , 24,8    },
            { 2,2, 16,16  , 32,16  , 48,16   },
            { 2,2, 32,32  , 64,32  , 96,32   },
            { 2,2, 64,64  , 128,64 , 192,64  },
            { 2,2, 128,128, 256,128, 384,128 },
        };
    };
};


#endif // EXT2_H

#ifndef PARTITION_COMMON_H
#define PARTITION_COMMON_H

enum DiskTypeDescrib
{
    EMPTY,            FAT_12,           XENIX_ROOT,           XENIX_USR,
    FAT_16_INF32MB,   EXTENDED,         FAT_16,               NTFS_HPFS,
    AIX,              AIX_BOOT,         OS2_BOOT_MGR,         PRI_FAT32_INT13,
    EXT_FAT32_INT13,  SILICON_SAFE,     EXT_FAT16_INT13,      WIN95_EXT_PARTITION,
    OPUS,             FAT_12_HIDDEN,    COMPAQ_DIAG,          UNKNOWN_0,
    FAT_16_HIDDEN_INF32MB,UNKNOWN_1,    FAT_16_HIDDEN,        NTFS_HPFS_HIDDEN,
    AST_SMARTSLEEP_PARTITION,UNKNOWN_2, UNKNOWN_3,            OSR2_FAT32,
    OSR2_FAT32_LBA,   UNKNOWN_4,        HIDDEN_FAT16_LBA,     UNKNOWN_5,
    UNKNOWN_6,        UNKNOWN_7,        UNKNOWN_8,            UNKNOWN_9,
    NEC_DOS,          UNKNOWN_10,       UNKNOWN_11,           PQSERVICE_ROUTERBOOT,
    UNKNOWN_12,       UNKNOWN_13,       ATHEOS_FILE_SYSTEM,   UNKNOWN_14,
    UNKNOWN_15,       UNKNOWN_16,       UNKNOWN_17,           UNKNOWN_18,
    UNKNOWN_19,       UNKNOWN_20,       NOS,                  UNKNOWN_21,
    UNKNOWN_22,       JFS_ON_OS2_OR_ECS,UNKNOWN_23,           UNKNOWN_24,
    THEOS_2GB,        PLAN_9_THEOS_SPANNED,THEOS_4GB,         THEOS_EXTENDED,
    PARTITIONMAGIC_RECOVERY,HIDDEN_NETWARE,UNKNOWN_25,        UNKNOWN_26,
    VENIX,            LINUX_PPC_PREP,   WIN_RAID,             LINUX_NATIVE,
    GOBACK,           BOOT_US_EUMEL_ELAN,EUMEL_ELAN_1,        EUMEL_ELAN_2,
    EUMEL_ELAN_3,     UNKNOWN_25_,       UNKNOWN_26_,           UNKNOWN_27_,
    OBERON,           QNX4_X,           QNX4_X_2ND_PART,      QNX4_X_3RD_PART_OBERON,
    ONTRACK_LYNX_OBERON,ONTRACK_NOVELL, CP_M_MICROPORT_SYSV_AT,DISK_MANAGER_AUX3,
    DISK_MANAGER_DDO, EZ_DRIVE,         GOLDEN_BOW_EZ_BIOS,   DRIVEPRO_VNDI,
    UNKNOWN_28,       UNKNOWN_29,       UNKNOWN_30,           UNKNOWN_31,
    PRIAM_EDISK,      UNKNOWN_32,       UNKNOWN_33,           UNKNOWN_34,
    UNKNOWN_35,       SPEEDSTOR,        UNKNOWN_36,           GNU_HURD,
    NOVEL1,           NETWARE_386,      NETWARE_SMS_PARTITION,NOVELL_1,
    NOVELL_2,         NETWARE_NSS,      UNKNOWN_37,           UNKNOWN_38,
    UNKNOWN_39,       UNKNOWN_40,       UNKNOWN_41,           UNKNOWN_42,
    DISKSECURE_MULTI_BOOT,UNKNOWN_43,   V7_X86,               UNKNOWN_44,
    UNKNOWN_45,       PC_IX,            UNKNOWN_46,           M2FS_M2CS_VNDI,
    XOSL_FS,          UNKNOWN_47,       UNKNOWN_48,           UNKNOWN_49,
    UNKNOWN_50,       UNKNOWN_51,       UNKNOWN_52,           UNKNOWN_53,
    MINUX_OLD,        MINUX_LINUX,      LINUX_SWAP_2,         LINUX_NATIVE_2,
    OS2_HIDDEN_HIBERNATION,       LINUX_EXTENDED,       OLD_LINUX_RAID_FAT16,         NTFS_VOLUME_SET,
    LINUX_PLAINTEXT_TABLE,        UNKNOWN_54,           LINUX_KERNEL_AIR_BOOT,        FAULT_TOLERANT_FAT32,
    FAULT_TOLERANT_FAT32_INT13H,  FREE_FDISK_FAT12,     LINUX_LOGICAL_VOLUME_MANAGER, UNKNOWN_55,
    FREE_FDISK_PRIMARY_FAT16,     FREE_FDISK_EXTENDED,  FREE_FDISK_LARGE_FAT16,       AMOEBA,
    AMOEBA_BBT,                   MIT_EXOPC,            CHRP_ISO_9660,                FREE_FDISK_FAT32,
    FREE_FDISK_FAT32_LBA,         DCE376,               FREE_FDISK_FAT16_LBA,         FREE_FDISK_EXTENDED_LBA,
    UNKNOWN_56,       UNKNOWN_57,       FORTHOS,              BSD_OS,
    LAPTOP_HIBERNATION,LAPTOP_HIBERNATION_HP,UNKNOWN_58,      HP_EXPANSION_SPEEDSTOR_1,
    HP_EXPANSION_SPEEDSTOR_2,BSD_386,   OPENBSD_SPEEDSTOR,    NEXTSTEP,
    MAC_OS_X,         NETBSD,           OLIVETTI,             MAC_OS_X_BOOT_GO,
    UNKNOWN_58_,       RISC_OS_ADFS,     SHAGOS,               SHAGOS_SWAP_MACOS_X_HFS,
    BOOTSTAR_DUMMY,   HP_EXPANSION_QNX, QNX_POWER_SAFE,       HP_EXPANSION_QNX_2,
    HP_EXPANSION_SPEEDSTOR_3,UNKNOWN_59,HP_EXPANSION_FAT16,   BSDI_FS,
    BSDI_SWAP,        UNKNOWN_60,       UNKNOWN_61,           BOOT_WIZARD_HIDDEN,
    ACRONIS_BACKUP,   BONNYDOS_286,     SOLARIS_8_BOOT,       NEW_SOLARIS,
    CTOS_REAL_32_DR_DOS,DRDOS_SECURED,  UNKNOWN_62,           HIDDEN_LINUX_SWAP,
    DRDOS_SECURED_FAT16,DRDOS_SECURED_EXTENDED,DRDOS_SECURED_FAT16_STRIPE,SYRINX,
    DR_DOS_8_1,       DR_DOS_8_2,       DR_DOS_8_3,           DR_DOS_7_SECURED_FAT32_CHS,
    DR_DOS_7_SECURED_FAT32_LBA,CTOS_MEMDUMP,DR_DOS_7_FAT16X,  DR_DOS_7_SECURED_EXT_DOS,
    REAL_32_SECURE,   OLD_MULTIUSER_FAT12,UNKNOWN_63,         UNKNOWN_64,
    OLD_MULTIUSER_FAT16,OLD_MULTIUSER_EXTENDED,OLD_MULTIUSER_FAT16_2,UNKNOWN_65,
    CP_M_86,          UNKNOWN_66,       NON_FS_DATA_POWERCOPY_BACKUP,CP_M,
    UNKNOWN_66_,       HIDDEN_CTOS_MEMDUMP,DELL_POWEREDGE_UTIL,DG_UX_DISK_MANAGER_BOOTIT,
    UNKNOWN_67,       ACCESS_DOS,       UNKNOWN_68,           DOS_R_O,
    SPEEDSTOR_FAT16_EXTENDED,UNKNOWN_69,STORAGE_DIMENSIONS_SPEEDSTOR,UNKNOWN_70,
    LUKS,             UNKNOWN_71,       RUFUS_EXTRA_FREEDESKTOP,BEOS_BFS,
    SKYOS_SKYFS,      UNKNOWN_72,       LEGACY_MBR_EFI_HEADER,EFI_FS,
    LINUX_PA_RISC_BOOT,STORAGE_DIMENSIONS_SPEEDSTOR_2,DOS_SECONDARY,UNKNOWN_73,
    SPEEDSTOR_LARGE_PROLOGUE,PROLOGUE_MULTI_VOLUME,STORAGE_DIMENSIONS_SPEEDSTOR_3,DDRDRIVE_SOLID_STATE_FS,
    UNKNOWN_74,       PCACHE,           BOCHS,                VMWARE_FILE_SYSTEM,
    VMWARE_SWAP,      LINUX_RAID,       SPEEDSTOR_LANSTEP_LINUX,BBT,

    EFI_UNUSED              = 0x00000000,   EFI_MBR                 = 0x024DEE41,
    EFI_SYSTEM              = 0xC12A7328,   EFI_BIOS_BOOT           = 0x21686148,
    EFI_IFFS                = 0xD3BFE2DE,   EFI_SONY_BOOT           = 0xF4019732,
    EFI_LENOVO_BOOT         = 0xBFBFAFE7,   EFI_MSR                 = 0xE3C9E316,
    EFI_BASIC_DATA          = 0xEBD0A0A2,   EFI_LDM_META            = 0x5808C8AA,
    EFI_LDM                 = 0xAF9B60A0,   EFI_RECOVERY            = 0xDE94BBA4,
    EFI_GPFS                = 0x37AFFC90,   EFI_STORAGE_SPACES      = 0xE75CAF8F,
    EFI_HPUX_DATA           = 0x75894C1E,   EFI_HPUX_SERVICE        = 0xE2A1E728,
    EFI_LINUX_DAYA          = 0x0FC63DAF,   EFI_LINUX_RAID          = 0xA19D880F,
    EFI_LINUX_ROOT32        = 0x44479540,   EFI_LINUX_ROOT64        = 0x4F68BCE3,
    EFI_LINUX_ROOT_ARM32    = 0x69DAD710,   EFI_LINUX_ROOT_ARM64    = 0xB921B045,
    EFI_LINUX_SWAP          = 0x0657FD6D,   EFI_LINUX_LVM           = 0xE6D6D379,
    EFI_LINUX_HOME          = 0x933AC7E1,   EFI_LINUX_SRV           = 0x3B8F8425,
    EFI_LINUX_DM_CRYPT      = 0x7FFEC5C9,   EFI_LINUX_LUKS          = 0xCA7D7CCB,
    EFI_LINUX_RESERVED      = 0x8DA63339,   EFI_FREEBSD_BOOT        = 0x83BD6B9D,
    EFI_FREEBSD_DATA        = 0x516E7CB4,   EFI_FREEBSD_SWAP        = 0x516E7CB5,
    EFI_FREEBSD_UFS         = 0x516E7CB6,   EFI_FREEBSD_VINUM       = 0x516E7CB8,
    EFI_FREEBSD_ZFS         = 0x516E7CBA,   EFI_OSX_HFS             = 0x48465300,
    EFI_OSX_UFS             = 0x55465300,   EFI_OSX_ZFS             = 0x6A898CC3,
    EFI_OSX_RAID            = 0x52414944,   EFI_OSX_RAID_OFFLINE    = 0x52414944,
    EFI_OSX_RECOVERY        = 0x426F6F74,   EFI_OSX_LABEL           = 0x4C616265,
    EFI_OSX_TV_RECOVERY     = 0x5265636F,   EFI_OSX_CORE_STORAGE    = 0x53746F72,
    EFI_SOLARIS_BOOT        = 0x6A82CB45,   EFI_SOLARIS_ROOT        = 0x6A85CF4D,
    EFI_SOLARIS_SWAP        = 0x6A87C46F,   EFI_SOLARIS_BACKUP      = 0x6A8B642B,
    EFI_SOLARIS_USR         = 0x6A898CC3,   EFI_SOLARIS_VAR         = 0x6A8EF2E9,
    EFI_SOLARIS_HOME        = 0x6A90BA39,   EFI_SOLARIS_ALTERNATE   = 0x6A9283A5,
    EFI_SOLARIS_RESERVED1   = 0x6A945A3B,   EFI_SOLARIS_RESERVED2   = 0x6A9630D1,
    EFI_SOLARIS_RESERVED3   = 0x6A980767,   EFI_SOLARIS_RESERVED4   = 0x6A96237F,
    EFI_SOLARIS_RESERVED5   = 0x6A8D2AC7,   EFI_NETBSD_SWAP         = 0x49F48D32,
    EFI_NETBSD_FFS          = 0x49F48D5A,   EFI_NETBSD_LFS          = 0x49F48D82,
    EFI_NETBSD_RAID         = 0x49F48DAA,   EFI_NETBSD_CONCAT       = 0x2DB519C4,
    EFI_NETBSD_ENCRYPT      = 0x2DB519EC,   EFI_CHROMEOS_KERNEL     = 0xFE3A2A5D,
    EFI_CHROMEOS_ROOTFS     = 0x3CB8E202,   EFI_CHROMEOS_FUTURE     = 0x2E0A753D,
    EFI_HAIKU               = 0x42465331,   EFI_MIDNIGHTBSD_BOOT    = 0x85D5E45E,
    EFI_MIDNIGHTBSD_DATA    = 0x85D5E45A,   EFI_MIDNIGHTBSD_SWAP    = 0x85D5E45B,
    EFI_MIDNIGHTBSD_UFS     = 0x0394EF8B,   EFI_MIDNIGHTBSD_VINUM   = 0x85D5E45C,
    EFI_MIDNIGHTBSD_ZFS     = 0x85D5E45D,   EFI_CEPH_JOURNAL        = 0x45B0969E,
    EFI_CEPH_ENCRYPT        = 0x45B0969E,   EFI_CEPH_OSD            = 0x4FBD7E29,
    EFI_CEPH_ENCRYPT_OSD    = 0x4FBD7E29,   EFI_CEPH_CREATE         = 0x89C57F98,
    EFI_CEPH_ENCRYPT_CREATE = 0x89C57F98,   EFI_OPENBSD             = 0x824CC7A0,
    EFI_QNX                 = 0xCEF5A9AD,   EFI_PLAN9               = 0xC91818F9,
    EFI_VMWARE_VMKCORE      = 0x9D275380,   EFI_VMWARE_VMFS         = 0xAA31E02A,
    EFI_VMWARE_RESERVED     = 0x9198EFFC,
};

#endif // PARTITION_COMMON_H
#include <string.h>
#include <stdio.h>

#include "pbs.h"
#include "fatSupport.h"
#include <crtdbg.h>
/************************************************************************/
/* Print boot sector command
/* will be used to print the information about the current file system
/*
/*
/* Written by: Daniel Houghton
/************************************************************************/

extern FATAttributes BOOT_SECTOR_ATTRIBUTES;
extern int BYTES_PER_SECTOR;

void readBootSector(void)
{
	unsigned char* bootSector = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
	int bytesRead;

	bytesRead = read_sector(0, bootSector);

	BOOT_SECTOR_ATTRIBUTES.BytesPerSector = bootSector[12] << 8 | bootSector[11];
	BOOT_SECTOR_ATTRIBUTES.SectorsPerCluster = bootSector[14] << 4 | bootSector [13];
	BOOT_SECTOR_ATTRIBUTES.NumberOfReservedSectors = bootSector[15] << 8 | bootSector[14];
	BOOT_SECTOR_ATTRIBUTES.NumberOfFAT = bootSector[17] << 4 | bootSector[16];
	BOOT_SECTOR_ATTRIBUTES.NumberOfRootEntries = bootSector[18] << 8 | bootSector[17];
	BOOT_SECTOR_ATTRIBUTES.TotalSectorCount = bootSector[20] << 8 | bootSector[19];
	BOOT_SECTOR_ATTRIBUTES.SectorsPerFat = bootSector[23] << 8 | bootSector[22];
	BOOT_SECTOR_ATTRIBUTES.SectorsPerTrack = bootSector[25] << 8 | bootSector[24];
	BOOT_SECTOR_ATTRIBUTES.NumberOfHeads = bootSector[27] << 8 | bootSector[26];
	BOOT_SECTOR_ATTRIBUTES.BootSignature = bootSector[39] << 4 | bootSector[38];
	BOOT_SECTOR_ATTRIBUTES.VolumeID = bootSector[40] << 16 | bootSector[39];

	memcpy(BOOT_SECTOR_ATTRIBUTES.volumeLabel, bootSector+43, 11);
	BOOT_SECTOR_ATTRIBUTES.volumeLabel[12] = (const char)"\0";
	memcpy(BOOT_SECTOR_ATTRIBUTES.FileSystemType, bootSector+54, 8);
}

void printBootSector(void)
{
	printf("%s%d\n", "Bytes per sector: ", BOOT_SECTOR_ATTRIBUTES.BytesPerSector);
	printf("%s%d\n", "Sectors per Cluster: ", BOOT_SECTOR_ATTRIBUTES.SectorsPerCluster);
	printf("%s%d\n", "Number of reserved Sectors: ", BOOT_SECTOR_ATTRIBUTES.NumberOfReservedSectors);
	printf("%s%d\n", "Number of FATs: ", BOOT_SECTOR_ATTRIBUTES.NumberOfFAT);
	printf("%s%d\n", "Number of root entries: ", BOOT_SECTOR_ATTRIBUTES.NumberOfRootEntries);
	printf("%s%d\n", "Total sector counts: ", BOOT_SECTOR_ATTRIBUTES.TotalSectorCount);
	printf("%s%d\n", "Sectors per FAT: ", BOOT_SECTOR_ATTRIBUTES.SectorsPerFat);
	printf("%s%d\n", "Sectors per track: ", BOOT_SECTOR_ATTRIBUTES.SectorsPerTrack);
	printf("%s%d\n", "Number of heads: ", BOOT_SECTOR_ATTRIBUTES.NumberOfHeads);
	printf("%s0x%x\n", "Boot signature: ", BOOT_SECTOR_ATTRIBUTES.BootSignature);
	printf("%s0x%x\n", "VolumeID: ", BOOT_SECTOR_ATTRIBUTES.VolumeID);
	printf("%s%s\n",  "Volume Label :", BOOT_SECTOR_ATTRIBUTES.volumeLabel);
	printf("%s%s\n", "File system type: ", BOOT_SECTOR_ATTRIBUTES.FileSystemType);
}
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

	memcpy(BOOT_SECTOR_ATTRIBUTES.BytesPerSector, bootSector+11, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.SectorsPerCluster, bootSector+13, 1);
	memcpy(BOOT_SECTOR_ATTRIBUTES.NumberOfReservedSectors, bootSector+14, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.NumberOfFAT, bootSector+16, 1);
	memcpy(BOOT_SECTOR_ATTRIBUTES.NumberOfRootEntries, bootSector+17, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.TotalSectorCount, bootSector+19, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.SectorsPerFat, bootSector+22, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.SectorsPerTrack, bootSector+24, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.NumberOfHeads, bootSector+26, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES.BootSignature, bootSector+38, 1);
	memcpy(BOOT_SECTOR_ATTRIBUTES.VolumeID, bootSector+39, 4);
	memcpy(BOOT_SECTOR_ATTRIBUTES.volumeLabel, bootSector+43, 11);
	memcpy(BOOT_SECTOR_ATTRIBUTES.FileSystemType, bootSector+54, 8);
}

void printBootSector(void)
{
	printf("%s%d\n", "Bytes per sector: ", (int)BOOT_SECTOR_ATTRIBUTES.BytesPerSector);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.SectorsPerCluster);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.NumberOfReservedSectors);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.NumberOfFAT);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.NumberOfRootEntries);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.TotalSectorCount);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.SectorsPerFat);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.SectorsPerTrack);
	printf("%d\n", BOOT_SECTOR_ATTRIBUTES.NumberOfHeads);
	printf("0x%x\n", BOOT_SECTOR_ATTRIBUTES.BootSignature);
	printf("0x%x\n", BOOT_SECTOR_ATTRIBUTES.VolumeID);
	printf("%s\n", BOOT_SECTOR_ATTRIBUTES.volumeLabel);
	printf("%s\n", BOOT_SECTOR_ATTRIBUTES.FileSystemType);
}
#include "pbs.h"
#include "fatSupport.h"
#include <cstring>
#include <stdio.h>
/************************************************************************/
/* Print boot sector command
/* will be used to print the information about the current file system
/*
/*
/* Written by: Daniel Houghton
/************************************************************************/

extern FATAttributes* BOOT_SECTOR_ATTRIBUTES;

void readBootSector(void)
{
	unsigned char* bootSector;
	int bytesRead;

	bytesRead = read_sector(0, bootSector);

	memcpy(BOOT_SECTOR_ATTRIBUTES->BytesPerSector, bootSector+11, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->SectorsPerCluster, bootSector+13, 1);
	memcpy(BOOT_SECTOR_ATTRIBUTES->NumberOfReservedSectors, bootSector+14, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->NumberOfFAT, bootSector+16, 1);
	memcpy(BOOT_SECTOR_ATTRIBUTES->NumberOfRootEntries, bootSector+17, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->TotalSectorCount, bootSector+19, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->SectorsPerFat, bootSector+22, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->SectorsPerTrack, bootSector+24, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->NumberOfHeads, bootSector+26, 2);
	memcpy(BOOT_SECTOR_ATTRIBUTES->BootSignature, bootSector+38, 1);
	memcpy(BOOT_SECTOR_ATTRIBUTES->VolumeID, bootSector+39, 4);
	memcpy(BOOT_SECTOR_ATTRIBUTES->volumeLabel, bootSector+43, 11);
	memcpy(BOOT_SECTOR_ATTRIBUTES->FileSystemType, bootSector+54, 8);
}

void printBootSector(void)
{
	printf("%d", BOOT_SECTOR_ATTRIBUTES->BytesPerSector);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->SectorsPerCluster);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->NumberOfReservedSectors);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->NumberOfFAT);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->NumberOfRootEntries);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->TotalSectorCount);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->SectorsPerFat);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->SectorsPerTrack);
	printf("%d", BOOT_SECTOR_ATTRIBUTES->NumberOfHeads);
	printf("0x%x", BOOT_SECTOR_ATTRIBUTES->BootSignature);
	printf("0x%x", BOOT_SECTOR_ATTRIBUTES->VolumeID);
	printf("%s", BOOT_SECTOR_ATTRIBUTES->volumeLabel);
	printf("%s", BOOT_SECTOR_ATTRIBUTES->FileSystemType);
}
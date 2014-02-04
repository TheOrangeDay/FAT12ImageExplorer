#ifndef PBS_H
#define PBS_H

FATAttributes BOOT_SECTOR_ATTRIBUTES;

typedef struct 
{
	int BytesPerSector;
	int SectorsPerCluster;
	int NumberOfFAT;
	int NumberOfReservedSectors;
	int NumberOfRootEntries;
	int TotalSectorCount;
	int SectorsPerFat;
	int SectorsPerTrack;
	int NumberOfHeads;
	int BootSignature;
	int VolumeID;
	char volumeLabel[12];
	char FileSystemType[9];
} FATAttributes;

void readBootSector(void);
void printBootSector(void);

#endif
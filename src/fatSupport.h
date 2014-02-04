#ifndef FAT_H
#define FAT_H

FILE* FILE_SYSTEM_ID;

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

FATAttributes BOOT_SECTOR_ATTRIBUTES;

int read_sector(int sector_number, char* buffer);
int write_sector(int sector_number, char* buffer);

unsigned int get_fat_entry(int fat_entry_number, char* fat);
void set_fat_entry(int fat_entry_number, int value, char* fat);

void readFAT12Table(int table, char* fat);

#endif
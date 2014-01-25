#ifndef PBS_H
#define PBS_H

typedef struct 
{
	unsigned char BytesPerSector[2];
	unsigned char SectorsPerCluster[1];
	unsigned char NumberOfFAT[1];
	unsigned char NumberOfReservedSectors[2];
	unsigned char NumberOfRootEntries[2];
	unsigned char TotalSectorCount[2];
	unsigned char SectorsPerFat[2];
	unsigned char SectorsPerTrack[2];
	unsigned char NumberOfHeads[2];
	unsigned char BootSignature[1];
	unsigned char VolumeID[4];
	unsigned char volumeLabel[11];
	unsigned char FileSystemType[8];
} FATAttributes;

void readBootSector(void);
void printBootSector(void);

#endif
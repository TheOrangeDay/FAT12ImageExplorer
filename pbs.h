#ifndef PBS_H
#define PBS_H

struct FATAttributes 
{
	unsigned short BytesPerSector;
	unsigned char SectorsPerCluster;
	unsigned char NumberOfFAT;
	unsigned char NumberOfReservedSectors;
	unsigned char NumberOfRootEntries;
	unsigned short TotalSectorCount;
	unsigned char SectorsPerFat;
	unsigned char SectorsPerTrack;
	unsigned char NumberOfHeads;
	unsigned char BootSignature;
	unsigned short VolumeID;
	char volumeLabel[32];
	char FileSystemType;
};

#endif
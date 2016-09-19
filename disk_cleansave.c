//disk_cleansave
//
//by LuigiBlood

#include <stdio.h>
#include <stdlib.h>

#define expectedFileSize (offsetRAMarea[6] + systemAreaSize)
#define getRamAreaOffset(type) (offsetRAMarea[type] + systemAreaSize)

const unsigned int offsetRAMarea[7] = {0x18CF180, 0x2156880, 0x2928040, 0x30438C0, 0x36A9200, 0x3B9ED00, 0x3D78F40};
const unsigned int systemAreaSize = 0x738C0;
const unsigned int systemAreaSizeD64 = 0x200;

int main(int argc, char* argv[])
{
	FILE* in;
	unsigned char SystemData[0xE8];
	unsigned char diskType = 0;

	printf("disk_cleansave by LuigiBlood\n-----\n");

	if(argc != 2)
	{
		printf("Usage: disk_cleansave <disk dump file>\n");
	}
	else
	{
		in = fopen(argv[1], "rb+");
		if (in != NULL)
		{
			//File loaded
			fseek(in, 0, SEEK_END);
			long fileSize = ftell(in);

			if (fileSize != expectedFileSize)
			{
				printf("Wrong file size (expected 0x%X), either the file is not a disk dump, or the dump format is not supported\n", expectedFileSize);
				return 1;
			}

			fseek(in, 0, SEEK_SET);
			fread(SystemData, 0xE8, 1, in);

			diskType = SystemData[5] & 0xF;
			printf("Disk Type = %d\n", diskType);

			if (diskType >= 6)
			{
				printf("No RAM area (Type 6 or unknown)\n");
				return 1;
			}

			fseek(in, getRamAreaOffset(diskType), SEEK_SET);
			printf("RAM Area Offset = 0x%X\n", getRamAreaOffset(diskType));

			//Not very performant but does the job as I want it to be
			for (int i = getRamAreaOffset(diskType); i < expectedFileSize; i++)
				fputc(0, in);

			printf("Save Data Erased\n");

			fclose(in);
		}
		else
		{
			printf("File loading error\n");
			return 1;
		}
	}

	return 0;
}
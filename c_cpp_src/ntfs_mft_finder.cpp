
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 512

long unsigned int get_disk_size(FILE *fp) 
{
	long unsigned int currentPos = ftell(fp);
	long unsigned int size;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, currentPos, SEEK_SET);

	return size;
}

int main(int argc, char**argv)
{
	int n;
	unsigned char buffer[BUF_SIZE];
	long unsigned int size;
	const unsigned char mft_pattern[] = {0x46, 0x49, 0x4c, 0x45, 0x30}; 
	const int pattern_size = (int) sizeof(mft_pattern);

	FILE* fp = fopen(argv[1], "rb");
	printf("Opening File: %s\n", (fp == NULL)? "File not Found... ABORT!" : "Ok!");
	if (fp == NULL)
		return 1;

	size = get_disk_size(fp);
	printf("Count of Sectors: %lu \n", size / 512);

	for (int i = 0; i < size / 512; i++) 
	{
		if ( i % 10000 == 0) {
			printf("Sector: %d \r", i);
		}
		fread(buffer, 1, BUF_SIZE, fp);
		if ( strncmp((char *) buffer, (char *)mft_pattern, pattern_size) == 0 ) {
			printf("Found something in sector: %d \n", i);
			return 0;
		}
	}

	printf("Nothing found :( \n");
	return 0;
}

/****************************************************************/
/****************************************************************/
/** Description: This program creat formated floppy disk ********/
/*************** image with FAT12 file system  ******************/
/** Name: Eli Kessem ********************************************/
/** ID: 066127549 ***********************************************/
/****************************************************************/
/****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "fat12.h"


int fid; /* global variable set by the open() function */
fat_dirent_t buff[];
int fd_read(int sector_number, char *buffer);
int fd_write(int sector_number, char *buffer);
int GetFatEntry(int FATindex);



int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		printf("Usage: %s <floppy_image>\n", argv[0]);
		exit(1);
	}

	if ( (fid = open (argv[1], O_RDWR|O_CREAT, 0644))  < 0 )
	{
		perror("Error: ");
		return -1;
	}	
	according
	// Step 1. Create floppy.img with the school solution. Read the values 
	// from the boot sector of the floppy.img and initialize boot sector
	// with those values. If you read bootsector of the floppy.img correctly,
	// the values will be:
	boot_record_t boot;

	boot.sector_size = 512;
	boot.sectors_per_cluster = 1;
	boot.reserved_sector_count = 1;
	boot.number_of_fats = 2;
	boot.number_of_dirents = 224;
	boot.sector_count = 2880;
	boot.media_type = 0xf0;
	boot.fat_size_sectors = 9;
	boot.sectors_per_track = 18;
	boot.nheads = 2;
	boot.sectors_hidden = 0;
	boot.sector_count_large = 0;
	
	 boot_record_t boot;
     if (read(fid, &boot, sizeof (boot)) > 0){
	 	 printf("sector_size: %d\n", boot.sector_size);
	 	 printf("sectors_per_cluster: %d\n", boot.sectors_per_cluster);
	 	 printf("reserved_sector_count: %d\n", boot.reserved_sector_count);
	 	 printf("number_of_fats: %d\n", boot.number_of_fats);
	 	 printf("number_of_dirents: %d\n", boot.number_of_dirents);
         printf("sector_count: %d\n", boot.sector_count);
	 }
	
	// Step 2. Set FAT1/FAT2 table entires to 0x0000 (unused cluster)
        // according to the fat12.pdf. 
	char buff[512];
	char bdirentries[512] = {0};
	bdirentries[0] = 0xe5;
	buff[0] = 0xFF0;
	buff[1] = 0xFF0;
	int i;
	fd_write(0, boot);
	for(i = 2; i < 512; i++){
		buff[i] = 0x0000;
	}
	for(i =1; i < 19; i++){
		fd_write(i, &buff);
	}

	for(i = 19; i < 33; i++){
		fd_write(i, &bdirentries);
	}
	buff[0] = 0x0000;
	buff[1] = 0x0000;
	for(i = 33; i < 2088; i++){
		fd_write(i, &buff);
	}
	
	
	// Step 3. Set direntries as free (0xe5) according to the fat12.pdf.
        // While zeroing dentries will also work, we prefer a solution
        // that mark them free. In that case it will be possible to perform
        // unformat operation. However, school solution simply zeros dentries.

	// Step 4. Handle data block (e.g you can zero them or just leave 
	// untouched. What are the pros/cons?)
	
	// For steps 2-3, you can also read the sectors from the image that was 
	// generated by the school solution if not sure what should be the values.
	
	
	close(fid);
	return 0;
}

int GetFatEntry(int FATindex){
	unsigned int FATEntryCode;  // The return value 
	int FatOffset = ((FATindex * 3) / 2);
	if(FATindex % 2 == 1){  // If the index is odd 
		FATEntryCode = _WORD(&FAT[FatOffset]);
		FATEntryCode >>= 4;  // Extract the high-order 12 bits 
	}
	else{  // If the index is even 
		FATEntryCode = _WORD(&FAT[FatOffset]);
		FATEntryCode &= 0x0fff;  // Extract the low-order 12 bits 
	}
	return FATEntryCode;
} 

int fd_read(int sector_number, char *buffer){
	int dest, len;
	dest = lseek(fid, sector_number * DEFAULT_SECTOR_SIZE, SEEK_SET); 
	if(dest != sector_number * bps){
		printf("\n***\nERROR: in fd_read\n***\n");
	}
	len = read(fid, buffer, bps);
	if(len != bps){
		printf("\n***\nERROR: in fd_read\n***\n");
	}
	return len;
}


int fd_write(int sector_number, char *buffer){
	int dest, len;
	dest = lseek(fid, sector_number * DEFAULT_SECTOR_SIZE, SEEK_SET); 
	if(dest != sector_number * bps){
		printf("\n***\nERROR: in fd_write\n***\n");
	}
	len = write(fid, buffer, bps);
	if(len != bps){
		printf("\n***\nERROR: in fd_write\n***\n");
	}
	return len;
}


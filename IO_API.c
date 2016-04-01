#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Include/FS_API.h"

#define SIZE_OF_BLOCK 1024


// Assuming that a block of data needs to be copied from block
int writeFile(int des, char *file_name, void *block ) {
	

	FS_info_t *info = getFSInfo(des);

	// printf("INFO %d %d %s\n", info->num_IB, info->num_DB, info->general_info.name);


	// Get bitmaps
	bool *inodeBM = getInodeBMByDes(des);
	bool *dataBM = getDataBMByDes(des);


	int i, num_inodes = info->num_IB *18;

	for( i=0; i<num_inodes; i++ ) {
		if(inodeBM[i]==false)break;
	}
	int inode_index = i;
	printf("IB::%d\n", info->num_IB);
	for( i=0; i<info->num_DB; i++ ) {
		if(dataBM[i]==false)break;
	}

	int data_index = i;
	printf("DATA_index: %d\n",data_index );
	printf("I:%d  D:%d\n",inode_index, data_index);

	FILE *f = fdopen(des, "r+b");
	
	// Update Bitmaps
	bool a = true;
	fseek(f, (1*SIZE_OF_BLOCK)+inode_index, SEEK_SET);
	fwrite(&a,sizeof(bool), 1, f);
	fseek(f, (2*SIZE_OF_BLOCK)+(data_index), SEEK_SET);
	fwrite(&a,sizeof(bool), 1, f);

	// Update inode
	inode *node = malloc( sizeof(inode));
	strcpy(node->file_name, file_name);
	node->size = CURRENT_SIZE;
	node->data_block_no = data_index;

	fseek(f, ((3+(inode_index/18))*SIZE_OF_BLOCK)+((inode_index%18)*sizeof(inode)), SEEK_SET);
	printf("WRITING TO: %ld\n",ftell(f) );
	fwrite( node, sizeof(inode), 1, f);

	fseek(f, 0, SEEK_SET);

	// Write data block
	writeBlock(des, data_index, block);
}


// Reads and returns a block associated with a "file_name" if it exists and returns -1 if no such
// file exists
int readFile(int des, char *file_name, void *block) {
		
	int i;
	bool *inodes = getInodeBMByDes(des);
	inode *node ;
	int ar[1020];

	for( i=0; i<1020; i++ ) {
		if( inodes[i]==1) {
			node = getInode(des, i);

			if(strcmp(file_name, node->file_name)==0) {
				CURRENT_SIZE = node->size;
				return readBlock(des,node->data_block_no, block);

				// void *temp = malloc(sizeof(SIZE_OF_BLOCK));
				// FILE *f = fdopen(des, "r+b");
				// fseek(f, node->data_block_no*SIZE_OF_BLOCK, SEEK_SET);
				// printf("BLOCK NO::%d Add:%ld\n",node->data_block_no, ftell(f) );
				// fread(temp, SIZE_OF_BLOCK, 1, f);
				// block = temp;

				// return SIZE_OF_BLOCK;
			}
		}
	}
	return -1;
}

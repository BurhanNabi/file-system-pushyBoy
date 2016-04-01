#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include "Include/general.h"
// #include "Include/IO_API.h"

#define SIZE_OF_BLOCK 1024
#define BLOCK_SIZE 1024

int CURRENT_SIZE = -1;




void reset_current_size() {
	CURRENT_SIZE = -1;
}

typedef struct file_system_t
{
	char name[40];
	int size;
	int id;
}file_system_t;


typedef struct FS_info_t
{
	// Has name,size,id
	file_system_t general_info;

	// Number of inodeblocks and datablocks
	int num_IB, num_DB;

	// Pointers to starting addresses
	void *I_start;
	void *D_start;

	// Pointer to Bitmaps
	void *BMI;
	void *BMD;
}FS_info_t;

typedef struct inode
{
	char file_name[40];
	int data_block_no;
	int size;
}inode;


inode* getInode(int des, int inode_num);
int writeBlock(int des, int block_number, void *block );
int createSFS(char* filename, int nbytes);
int generateID();
int initFS(file_system_t *FS);


// Allocates memory to the file_system.
// If fails,returns -1 else returns the unique ID 
// of the newly created file_system
int createSFS(char* filename, int nbytes) {
	
	file_system_t *new;
	new = malloc(sizeof(file_system_t));
	if(new == NULL) {
		return -1;
	}
	

	strcpy(new->name,filename);
	new->size = nbytes;
	new->id = generateID();

	initFS(new);

	return new->id;
}

// Generates a random ID for each file_system_t
int generateID() {
	srand(time(NULL));
	return rand();
}


// Returns exit status of function
int initFS(file_system_t *FS) {

	if(FS==NULL)return -1;
	int totalSize = FS->size;

	// Check for size
	if(totalSize < 12000)return -1;


	// Find number of blocks of each type
	int numBlocks = totalSize/SIZE_OF_BLOCK;
	int num_Inodes = (numBlocks * 1)/10;
	int num_Data = numBlocks;
	// 10%Inodes, 1 block Super, BMI, BMD each
	// Rest Data

	// printf("Total BLOCKS:%d\n",numBlocks );


	// Object to be written to first block of memory in the FS
	FS_info_t *info = malloc( sizeof(FS_info_t) );
	info->num_IB = num_Inodes;
	if(num_Inodes == 0)
		info->num_IB = 1;

	// Number of data blocks is the number of total blocks
	info->num_DB = num_Data;


	printf("NUM BLOCKS INODES::%d\n", num_Inodes);

	// Size name and id of a file system
	info->general_info.size = FS->size;	
	strcpy(info->general_info.name, FS->name);
	info->general_info.id = FS->id;	

	// printf("NUM IBLOCKS:%d\n",num_Inodes );

	// Create BM . Ready for persistent storage
	int i,num_inodes = info->num_IB * (SIZE_OF_BLOCK/sizeof(inode));
	// Num elem in array for inode BM is numofblocks allocated for
	// id nodes multiplied by Blcoksize /inodesize
	// 18 inodes per block
	bool *bmi  = malloc( 1020 * sizeof(bool));
	// bmi[0] = true;
	// bmi[101] = true;
	// Number of elements in the array for a data block is the number of
	// blocks allocated
	bool *bmd  = malloc(1020 * sizeof(bool));

	// Set initial few blocks to true
	for( i=0; i<= 2 + num_Inodes ; i++ ) {
		bmd[i] = true;
	}


	inode *ptr = malloc( 18 * sizeof(inode));
	// strcpy(ptr[0].file_name,"Burhan.secrets");
	// ptr[0].size = 0;
	// printf("NUM INODES:%d\n",num_inodes );

	// First write
	char name[100];
	sprintf(name, "./File Systems/%d", FS->id);

	FILE *f = fopen(name, "wb+");
	fwrite(info,sizeof(FS_info_t), 1, f);
	fseek(f,nextBlockStart(ftell(f)),SEEK_CUR);
	fwrite(bmi, 1, 1020, f);
	fseek(f,nextBlockStart(ftell(f)),SEEK_CUR);
	// printf("FTELL:%ld\n",ftell(f) );
	fwrite(bmd, 1, num_Data, f);
	fseek(f,nextBlockStart(ftell(f)),SEEK_CUR);
	while( num_Inodes -- ) { 
		fwrite(ptr, sizeof(inode), 18, f);
		fseek(f,nextBlockStart(ftell(f)),SEEK_CUR);
	}
	fseek(f, FS->size, SEEK_SET);
	printf("LAST IND:%ld\n", ftell(f));
	fwrite("\n", sizeof("\n"), 1, f);
	fclose(f);

}


int nextBlockStart( int n) {
	return (SIZE_OF_BLOCK - (n%SIZE_OF_BLOCK));
}


// Returns a pointer to array of BM if file system exists 
// Else returns NULL
bool* getInodeBMById(long id) {
	printf("HERE1\n");
	int des = readFS(id);
	printf("DES:%d\n",des );
	if(des==-1)return NULL;
	bool *ar = malloc( 1020 * sizeof(bool) );
	FILE *f = fdopen(des, "rb+");
	fseek(f, SIZE_OF_BLOCK, SEEK_SET);
	fread(ar, 1, 1020, f);
	fclose(f);
	return ar;
}


// Returns a pointer to array of BM if file system exists 
// Else returns NULL
bool* getDataBMById(long id) {
	int des = readFS(id);
	if(des==-1)return NULL;
	bool *ar = malloc( 1020 * 1);
	FILE *f = fdopen(des, "rb+");
	fseek(f, 2*SIZE_OF_BLOCK, SEEK_SET);
	fread(ar, 1, 1020, f);
	// fclose(f);
	return ar;
}

bool* getInodeBMByDes(int des) {
	if(des==-1)return NULL;
	bool *ar = malloc( 1020 * 1);
	FILE *f = fdopen(des, "rb+");
	fseek(f, SIZE_OF_BLOCK, SEEK_SET);
	fread(ar, 1, 1020, f);
	// fclose(f);
	return ar;
}


// Returns a pointer to array of BM if file system exists 
// Else returns NULL
bool* getDataBMByDes(int des) {
	if(des==-1)return NULL;
	bool *ar = malloc( 1020 * 1);
	FILE *f = fdopen(des, "rb+");
	fseek(f, 2*SIZE_OF_BLOCK, SEEK_SET);
	fread(ar, 1, 1020, f);
	// fclose(f);
	return ar;
}


// Prints a list of all the files saved in the file system
int* getFiles( int id) {
	int des = readFS(id), i;

	bool *inodes = getInodeBMByDes(des);
	inode *node ;

	for( i=0; i<1020; i++ ) {
		if( inodes[i]==1) {
			node = getInode(des, i);
			printf("NAME: %s SIZE:%d\n",node->file_name, node->size );
		}
	}
}


// Returns file descriptor of FS with passed id
int readFS(int id) {
	char name[50];
	sprintf(name, "./File Systems/%d", id);
	FILE *f = fopen(name, "r+");	
	if(f==NULL) {
		printf("NO such FS exists\n");
	}else {
		int fno  = fileno(f);
		return fno;
	}
}


FS_info_t* getFSInfo(int des) {
	if(des<0)return NULL;
	FILE *f = fdopen(des, "r+");
	FS_info_t *temp = malloc(sizeof(FS_info_t));
	fread(temp, sizeof(FS_info_t), 1, f);
	// fclose(f);
	return temp;
}


// Read a block #block_number from FS into a buffer passed as a pointer
int readBlock( int des, int block_number, void *buffer) {
	FILE *f = fdopen(des, "r+");
	fseek(f, block_number * SIZE_OF_BLOCK , SEEK_SET);
	printf("READING BLOCK FROM::%ld\n", ftell(f));

	int read = fread(buffer, CURRENT_SIZE, 1, f);
	reset_current_size();

	return read;
}


int writeBlock(int des, int block_number, void *block ) {

	FILE *f = fdopen(des,"r+b");
	// block_number++;

	// Write new block
	fseek(f, (block_number*SIZE_OF_BLOCK), SEEK_SET);
	printf("WRITING NEW BLOCK TO:%ld\n", ftell(f));

	fwrite(block , CURRENT_SIZE, 1, f);

	fseek(f, 0, SEEK_END);
	fwrite("\0", sizeof("\0"),1,f);

	printf("FINAL FTELL:%ld\n",ftell(f) );

}



inode* getInode(int des, int inode_num) {
	int block_num = inode_num/18;
	int inode_index = inode_num%18;
	inode *ar = malloc(sizeof(inode));
	FILE *f = fdopen(des, "r+b");
	fseek(f, (3+block_num)*SIZE_OF_BLOCK + (inode_index%18)*sizeof(inode), SEEK_SET);
	fread(ar, sizeof(inode), 1, f);
	return ar;
}
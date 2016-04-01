#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE_OF_BLOCK 1024

extern int CURRENT_SIZE;

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


void reset_current_size();

int createSFS(char* filename, int nbytes);
file_system_t* getFileSystemByID(int id);
int initFS(file_system_t *FS);
int nextBlockStart( int n);
bool* getInodeBMByDes(int des);
bool* getDataBMByDes(int des);

bool* getInodeBMById(long id);
bool* getDataBMById(long id);

int readFS(int id);
FS_info_t* getFSInfo(int des);
int writeBlock(int des, int block_number, void *block );

int readBlock( int des, int block_number, void *buffer);
inode* getInodeBlock(int des, int index);

inode* getInode(int des, int inode_num);

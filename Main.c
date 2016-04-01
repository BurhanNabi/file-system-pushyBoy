#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Include/FS_API.h"
#include "Include/IO_API.h"


void main_menu();
void getInputId();
void getChoice();

int choice, id;




int main( int argc, char const *argv[] ) {

/*	int i, id = createSFS("FATTY SPINDS", 20000);
	int des = readFS(id);

	// WORKS :: FS_info, BMs , getInode, overwritin inode,BMs, files, read

	char data[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	char *pointer = data;
	char arg[] = "answer";

	// Write Block in a file
	CURRENT_SIZE = strlen(data);
	writeFile(des, arg, pointer);
	reset_current_size();

	bool *ar = getDataBMByDes(des);
	for( i=0; i<100; i++ ) {
		printf("%d ",ar[i] );
	}
	printf("\n***********************************\n");

	ar = getInodeBMByDes(des);
	for( i=0; i<100; i++ ) {
		printf("%d ",ar[i] );
	}	
	printf("\n***********************************\n");

	// getFiles(id);

	void *block = malloc(1024);
	readFile(des, "answer", block);
	reset_current_size();

	printf("%p\n",&block );
*/



	/*
	TODO
	1. Menu
	2. Return values display
	*/

	main_menu();
	return 0;
}


void main_menu() {
	printf("Press:\n");
	printf("\t1 for new File System\n");
	printf("\t2 for creating a new file\n");
	printf("\t3 for reading an existing file\n");
	printf("\t4 to print an inode BM\n");
	printf("\t5 to print a data BM\n");
	printf("\t6 to print all the existing files in a FS\n");
	printf("\t7 for reading an existing file systems info\n");
	printf(">> ");
	getChoice();
}


void getChoice() {


	char name[40];
	int size;


	scanf( "%d", &choice );
	if(choice==1) {

		printf("Enter name of FS:");
		scanf( "%s", name );
		printf("ENTER size:");
		scanf( "%d", &size );

		int id = createSFS(name, size);
		printf("ID is::%d\n", id);
		printf("SUCCESSFUL\n");

		// getchar();
		main_menu();

	}else if(choice>1 && choice<8) {
		getInputId();
	}else {
		printf("WRONG CHOICE\n");
		main_menu();
	}
}

void getInputId() {

	printf("ENTER ID OF the file system\n");
	printf(">> ");
	scanf( "%d", &id );

	char name[50];
	char data[1024];
	bool *ar;
	int i;
	sprintf(name, "File Systems/%d",id);

	FILE *f = fopen(name, "r+");
	if(f == NULL) {
		printf("INCORRECT ID\n");
	}else {
				
		// Handle request
		fclose(f);
		
		int des = readFS(id);

		switch(choice) {
			case 2:
				;
				// char name[30];
				printf("Enter name of file: ");
				scanf("%s", name);
				getchar();
				printf("Enter data::\n");
				fgets( data, 1024, stdin);
				CURRENT_SIZE = strlen(data);
				writeFile(des, name, data);
				reset_current_size(CURRENT_SIZE);
				main_menu();
				break;

			case 3:
				// Read file
				printf("Enter name of file: ");
				scanf("%s", name);
				char *buffer = malloc(sizeof(1024));
				readFile(des, name, buffer);
				char *ptr = (char*)buffer;
				fputs(buffer, stdout);
				reset_current_size();
				main_menu();


				break;
			case 4:
				;
				// Print Inode BM
				ar = getInodeBMByDes(des);
				i;
				for( i=0; i<1000; i++ ) {
					printf("%d ",ar[i]);
				}
				main_menu();
				break;
			case 5:
				;
				// Print data BM
				ar = getDataBMByDes(des);
				i;
				for( i=0; i<1000; i++ ) {
					printf("%d ",ar[i]);
				}
				main_menu();
				break;
				main_menu();
				break;
			case 6:
				// Print list of files
				;
				getFiles(id);
				main_menu();
				break;
			case 7:
				// Read info of an existing FS
				;
				FS_info_t *fs = getFSInfo( readFS(id) );
				printf("Name: %s\n", fs->general_info.name);
				printf("Size: %d\n", fs->general_info.size);
				// getchar();
				main_menu();
				break;
		}



	}

}
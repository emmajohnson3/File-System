#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;
const int INODE_SIZE = 32;
const int FILE_SIZE = 2000000;//2mb
const int INODE_OFFSET = 4096; //num of blocks for inodes

/***********disk instructions ******************************/

void readBlock(FILE* disk, int blockNum, char* buffer){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void writeBlock(FILE* disk, int blockNum, char* data){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, strlen(data), 1, disk); 
}

void CreateDisk(FILE* disk){
    //make it 2 mb
    fseek(disk, FILE_SIZE -1 , SEEK_SET);
    fputc('\0', disk);

    //init superblock
    char* super = malloc(512);
    super[0] = 66;
    super[1] = 2; //super + vector (num blocks)
    super[3] = 0; //(num inodes)
    writeBlock(disk, 0, super);
    free(super);
  
    //set free block vector
    char* block = malloc(512); //this needs to be in signed char
    for(int i = 0; i < 512; i++){
        super[i] = 0;
    }
    for(int i = 0; i < 9; i++){
        super[i] = 1;
    }
    writeBlock(disk, 1, block);
    free(block);
}

void DeleteDisk(){
    remove("vdisk");
}

/*********** FS instructions ******************************/

int getNumBlocks(FILE* disk) {//gets num of blocks being used
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 0, buffer);
    int a = buffer[1];
    free(buffer);
    return a;
}

char* createInode(FILE* disk, char* data) {
    char* inode = malloc(32);
    inode[0] = strlen(data);//file size
    inode[1] = 0;

    //find what blocks are free
    char* blocks = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 1, blocks);
    for(int i = 0; i < 11; i++){
       printf("%d\n", blocks[i]);     
    }

    return inode;
}

//returns the int indentifier of the inode
int createFile(FILE* disk, char* data) {
    //allocate inode    
    char* inode = createInode(disk,data);
    //find where to put inode
    //writeBlock(disk, 2, inode); 
    //write the data to blocks specified by inode
    free(inode);
}


int createDirectory(FILE* disk, char* data) {
    //allocate inode    
    char* inode = createInode(disk,data);
    inode[1] = 1;//means its a directory
    //find where to put inode
    
    //write the data to blocks specified by inode
    free(inode);
}

void writeToFile(FILE* disk, char* data) {
    char* inodeBuffer = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 2, inodeBuffer);
    int fileBlockNumber = inodeBuffer[10];
    writeBlock(disk, fileBlockNumber, data)     ;

    free(inodeBuffer);
}

void readFile(FILE* disk, char* buffer) {
    char* inodeBuffer = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 2, inodeBuffer);
    int fileBlockNumber = inodeBuffer[10];
    readBlock(disk, fileBlockNumber, buffer);

    free(inodeBuffer);
}

int main(int argc, char* argv[]) {
FILE* disk = fopen("vdisk", "w+b");
    //DeleteDisk();
    CreateDisk(disk);

    createFile(disk, "Hello World! 5"); 
    writeBlock(disk, 4, "Hello World!!");  
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    //readFile(disk, buffer);
    //printf("%s\n", buffer);
    free(buffer);

    fclose(disk);
    return 0;
}
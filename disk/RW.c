#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    fwrite(data, BLOCK_SIZE, 1, disk); 
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
    char* block = malloc(512);
    for(int i = 0; i < 512; i++){
        super[i] = 0;
    }
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

char* createInode() {
    char* inode = malloc(32);
    for(int i = 0; i < 512; i++){
        inode[i] = 0;
    }
    inode[0] = 9;//file size
    inode[10] = 3; //breaks if changed
    return inode;
}

void createFile(FILE* disk) {
    char* inode = createInode();
    writeBlock(disk, 2, inode); 
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

    createFile(disk);    
    writeToFile(disk, "Hello World! 4");
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    readFile(disk, buffer);
    printf("%s\n", buffer);
    free(buffer);

    fclose(disk);
    return 0;
}
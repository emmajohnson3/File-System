#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;
const int INODE_SIZE = 32;
const int FILE_SIZE = 2000000;
const int OFFSET = 4096;

/***********disk instructions ******************************/

void readBlock(FILE* disk, int blockNum, char* buffer){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void writeBlock(FILE* disk, int blockNum, char* data){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, BLOCK_SIZE, 1, disk); 
}

void CreateDisk(){
    remove("vdisk");
    //make it 2 mb
    FILE *fp = fopen("vdisk", "w+b");
    fseek(fp, FILE_SIZE -1 , SEEK_SET);
    fputc('\0', fp);

    //init superblock
    char* super = malloc(512);
    super[0] = 66;
    super[1] = 2; //super + vector (num blocks)
    super[1] = 0; //(num inodes)
    writeBlock(fp, 2, super);
    free(super);

    //init inode map
    fclose(fp);
}

/*********** FS instructions ******************************/

char* createEmptyInode() {
    char* inode = malloc(32);
    inode[10] = 3;
    return inode;
}

void createFile(FILE* disk) {
    char* inode = createEmptyInode();
    // Add more things to inode?
    writeBlock(disk, 2, inode);
    
    free(inode);
}

void writeToFile(FILE* disk, char* data) {
    char* inodeBuffer = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 2, inodeBuffer);
    int fileBlockNumber = inodeBuffer[10];
    writeBlock(disk, fileBlockNumber, data);

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
    createFile(disk);    
    writeToFile(disk, "Hello World! 2");
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    readFile(disk, buffer);
    printf("%s\n", buffer);

    free(buffer);
    fclose(disk);
    return 0;
}
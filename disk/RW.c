#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;
const int INODE_SIZE = 32;
const int FILE_SIZE = 2000000;
const int OFFSET = 4096;

void readBlock(FILE* disk, int blockNum, char* buffer){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void writeBlock(FILE* disk, int blockNum, char* data){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, BLOCK_SIZE, 1, disk); // Note: this will overwrite existing data in the block
}

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


void CreateDisk(){
    remove("vdisk");
    //make it 2 mb
    FILE *fp = fopen("vdisk", "w");
    fseek(fp, FILE_SIZE -1 , SEEK_SET);
    fputc('\0', fp);
    //int superblock

    //int freeblock
    fclose(fp);
}

int main(int argc, char* argv[]) {
        
    FILE* disk = fopen("vdisk", "w+b");
    createFile(disk);    
    writeToFile(disk, "Hello World!");
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    readFile(disk, buffer);
    printf("%s\n", buffer);

    free(buffer);
    fclose(disk);
    return 0;
}
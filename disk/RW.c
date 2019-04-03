#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;
const int FILE_SIZE = 2000000;
const int OFFSET = 4096;

void readBlock(FILE* disk, int blockNum, char* buffer){
        disk = fopen("vdisk", "w+b");
        fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
        fread(buffer, BLOCK_SIZE, 1, disk);
        fclose(disk);
}

void writeBlock(FILE* disk, int blockNum, char* data){
        disk = fopen("vdisk", "w+b");
        fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
        fwrite(data, BLOCK_SIZE, 1, disk); // will overwrite 
        fclose(disk);  
}

void CreateDisk(){
    remove("vdisk");
    FILE *fp = fopen("vdisk", "w");
    fseek(fp, FILE_SIZE -1 , SEEK_SET);
    fputc('\0', fp);
    fclose(fp);
}

int main(int argc, char* argv[]) {
    FILE* disk = fopen("vdisk", "w+b"); // Notice that we are now reading and writing
    writeBlock(disk, 2, "Hello world!");
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE*2);
    printf("aaaaaa");
    readBlock(disk, 2, buffer);
    //printf("%s", buffer);
    for (int i = 0; i < BLOCK_SIZE*2; i++){
        printf("%2x ", buffer[i]);
    }

    free(buffer);
    fclose(disk);
    return 0;
}
#include "RW.h"

int main(int argc, char* argv[]) {
    char* buffer =  malloc(sizeof(char) * 512 * 2);
    FILE* disk = fopen("vdisk", "w+b");
    CreateDisk(disk);
    
    printf("This fil tests directories\n\n");
    printf("We know the disk is persistant because the old files are still here:\n");
    ReadDirectory(disk, "root");
    printf("\n");

    printf("Creating 'par' directory in root folder:\n");
    createDirectory(disk, "par");
    printf("Creating 'sub' directory in 'par' folder:\n");
    createDirectory(disk, "par/sub");

    printf("We can list the content of these directories:\n");
    ReadDirectory(disk, "root");
    printf("\n");
    ReadDirectory(disk, "par");
    printf("\n");
    ReadDirectory(disk, "par/sub");
    printf("\n");

    free(buffer);
    printf("Finished the second test file\n\n");

    fclose(disk);
    return 0;
}
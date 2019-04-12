#include "RW.h"

int main(int argc, char* argv[]) {
    char* buffer =  malloc(sizeof(char) * 512 * 2);
    FILE* disk = fopen("vdisk", "r+b");
    printf("This fil tests deleting directiories\n\n");
    
    printf("Creating 'qqq' directory in root folder:\n");
    createDirectory(disk, "par");

    printf("file is now in the root directory:\n");
    ReadDirectory(disk, "root");
    printf("\n");
    ReadDirectory(disk, "qqq");
    printf("\n\n");

    printf("Deleting the 'qqq' directory :\n");
    deleteDirectory(disk,"qqq");
    printf("We can no longer acesss the directory :\n");
    ReadDirectory(disk, "root");
    printf("\n");
    ReadDirectory(disk, "qqq");
    printf("\n\n");

    fclose(disk);
    return 0;
}
#include "RW.h"

int main(int argc, char* argv[]) {
    char* buffer =  malloc(sizeof(char) * 512 * 2);
    FILE* disk = fopen("vdisk", "r+b");
    printf("\nThis file tests deleting directiories\n\n");
    
    printf("Creating 'qqq' directory in root folder:\n");
    createDirectory(disk, "qqq");

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

    printf("End of test 3\n");
    fclose(disk);
    return 0;
}
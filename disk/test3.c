#include "RW.h"

int main(int argc, char* argv[]) {
    char* buffer =  malloc(sizeof(char) * 512 * 2);
    FILE* disk = fopen("vdisk", "r+b");
    printf("This fil tests deleting directiories\n\n");
   /* 
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
    printf("\n\n");

    printf("files can also be added to new directories:\n");
    printf("Adding file 'I like dogs' to dir 'par':\n");
    int dogs = createFile(disk, "par","I like dogs");
    printf("Adding file 'I like mice' to dir 'par/sub':\n");
    int mice = createFile(disk, "par","I like mice");

    printf("\nThe files are now in the dirs and can be read:\n");
    ReadDirectory(disk, "par");
    ReadDirectory(disk, "par/sub");
    printf("\n");
    printf("Reading the dog file: %d\n", dogs);
    buffer = readFile(disk,dogs);
    printf("%s\n\n", buffer);

    printf("Reading the mice file: %d\n", mice);
    buffer = readFile(disk,mice);
    printf("%s\n\n", buffer);

    free(buffer);
    printf("Finished the second test file\n\n");
*/
    fclose(disk);
    return 0;
}
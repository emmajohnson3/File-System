#include <stdio.h>
#include <stdlib.h>

void readBlock(FILE* disk, int blockNum, void* buffer);

void writeBlock(FILE* disk, int blockNum, void* data);

void CreateDisk(FILE* disk);

void DeleteDisk();

int getNumBlocks(FILE* disk);

void addEntry(FILE* disk,int dirNum,char* name, int node) ;

void deleteEntry(FILE* disk,int dirNum,char* path);

char** parse(FILE* disk,char* line);

void ReadDirectory(FILE* disk, char* data) ;

void deleteDirectory(FILE* disk, char* data) ;

int createDirectory(FILE* disk, char* data) ;

int createFile(FILE* disk, char* filePath, char* data) ;

char* readFile(FILE* disk, int id);
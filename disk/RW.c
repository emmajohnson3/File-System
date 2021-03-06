#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;
const int INODE_SIZE = 32;
const int FILE_SIZE = 2000000;//2mb
const int INODE_OFFSET = 4096; //num of blocks for inodes
/***********bit stuff from aniliitb10 ******************************/
#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )            
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

/***********disk instructions ******************************/

void readBlock(FILE* disk, int blockNum, void* buffer){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void writeBlock(FILE* disk, int blockNum, void* data){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, BLOCK_SIZE , 1, disk); //strlen(data)
}

void CreateDisk(FILE* disk){
    //make it 2 mb
    fseek(disk, FILE_SIZE -1 , SEEK_SET);
    fputc('\0', disk);

    //init superblock
    short* super = malloc(512);
    super[0] = 666;
    super[1] = 2; //super + vector (num blocks)
    super[3] = 0; //(num inodes)
    writeBlock(disk, 0, super);
    free(super);
  
    //set free block vector
    int* block = malloc(512); //this needs to be in signed char
    for(int i = 0; i < NUM_BLOCKS; i++){
        ClearBit(block,i);
    }
    for(int i = 0; i < 9; i++){  //then next 200 are for i nodes
        SetBit(block,i);
    }
   //set root directory
    char* root = malloc(512);
    root[0] = 0;
    root[1] = 'h';
    root[2] = 'o'; 
    root[3] = 'm'; 

    writeBlock(disk, 2, root);
    writeBlock(disk, 1, block);
    //free(block);
    //free(super);  //causes double free?
    free(root);
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

short* createInode(FILE* disk, char* data) {
    int num = 0;
    int size = strlen(data);//file size
   
    short* inode = malloc(BLOCK_SIZE);
    inode[0] = size;//file size
    inode[1] = 0;

    //calc how many blocks
    num = (size/BLOCK_SIZE)+1; 

    //find what blocks are free
    int* blocks = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 1, blocks);
    int i = 209;

    for(int k = 0; k< num ; k++){
    for(; i < NUM_BLOCKS ; i++){ //find empty
       if(TestBit(blocks,i) == 0){
               //set bit and add to inode block list
               SetBit(blocks,i);
               //printf("Allocating block: %d\n", i);
               inode[2+k] = i;
               break;
       }  
    }//for
    }//for

    if(i == NUM_BLOCKS-1){
            printf("Error: out of content memory");
            exit(1);
    }
    writeBlock(disk, 1, blocks);
    free(blocks);
    return inode;
}
                              //2                 //id
void addEntry(FILE* disk,int dirNum,char* name, int node){
        char* root = malloc(512);
        readBlock(disk, dirNum, root);  
        int num = root[0];
        root[0] = num +1;
        num = ((num+1) * 4);
        root[0+num] = node;
        root[1+num] = name[0];
        root[2+num] = name[1]; 
        root[3+num] = name[2];
        writeBlock(disk, dirNum, root);
        free(root);
}
// deleteEntry( disk,cur, name);
void deleteEntry(FILE* disk,int dirNum,char* path){
        char* dir = malloc(512);
        readBlock(disk, dirNum, dir);  
        int num = dir[0];
        int start =0;

        for(int i = 0; i < num; i++){ 
                        int num = ((i+1)* 4);
                        int node = dir[0+num];
                        if(dir[1+num]==path[0] && dir[2+num] == path[1] && dir[3+num] == path[2]){
                                 //delete num - (num+3)
                                 dir[0] = dir[0] - 1;
                                for(int k = 0 ; k < 1000; k++ ){
                                      dir[0+num] =  dir[0+num+4];
                                       dir[1+num] =  dir[1+num+4];
                                        dir[2+num] =  dir[2+num+4];
                                         dir[3+num] =  dir[3+num+4];

                                }
                                break;
                        }else if(i == num -1){
                                printf("delete entry failed\n");
                                exit(1); 
                        } 
              } //for

        writeBlock(disk, dirNum, dir);
        free(dir);
}

char** parse(FILE* disk,char* line){
    char **tokens = malloc(512 * sizeof(char*));
    char *token;
    char* name = malloc(512 * sizeof(char));
    strcpy (name, line);

    token = strtok(name, "/\n");
    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, "/\n");
    }
     tokens[i] = NULL;

    return(tokens);
}

void ReadDirectory(FILE* disk, char* data) {
       
        char** tokens = parse(disk,data);  
        char* name;
        int len;
        for(len = 0; tokens[len] != NULL;len++){
                name = tokens[len];
        } 

    //find directory
    char* dir = malloc(sizeof(char) * BLOCK_SIZE);
    short* dirInode = malloc(sizeof(char) * BLOCK_SIZE);
    if(strcmp(name, "root") == 0 || strcmp(name, "hom") == 0){//read root
        readBlock(disk, 2, dir);
    }else{
        short cur =  2;
        int time = 0;
        while(time < len){
              char* path = tokens[time];
              readBlock(disk, cur, dir);
              int files = dir[0]; 
              for(int i = 0; i < files; i++){ //find dir in parent
                        int num = ((i+1)* 4);
                        int node = dir[0+num];
                        if(dir[1+num]==path[0] && dir[2+num] == path[1] && dir[3+num] == path[2]){
                                 //get content block from inode
                                 readBlock(disk, node,  dirInode);
                                 cur = dirInode[2];
                                 readBlock(disk, cur, dir);
                                break;
                        }else if(i == files -1){
                                printf("file %s could not be found in path\n", path);
                                exit(1); 
                        } 
              } //for
              time++;
        } //while
    }//else

    //parse dir
    printf("# of files in %s: %d\n",name, dir[0]);
     printf("File list:\n");
    for(int i = 0 ; i <  dir[0]; i++){
             int num = ((i+1)* 4);
             int node = dir[0+num];
             printf("-%c%c%c\n",dir[1+num],dir[2+num],dir[3+num]);
    }

}

//must be empty
void deleteDirectory(FILE* disk, char* data) {
       
        char** tokens = parse(disk,data);  
        char* name;
        int len;
        for(len = 0; tokens[len] != NULL;len++){
                name = tokens[len];
        }

   int parID = 2;
   int id = 2;
   int contentBlock = 0;
    //find directory
    char* dir = malloc(sizeof(char) * BLOCK_SIZE);
    short* dirInode = malloc(sizeof(char) * BLOCK_SIZE);
    
        short cur =  2; // content
        int time = 0;
        while(time < len){
              char* path = tokens[time];
              readBlock(disk, cur, dir);
              int files = dir[0]; 
              for(int i = 0; i < files; i++){ //find dir in parent
                        int num = ((i+1)* 4);
                        if(dir[1+num]==path[0] && dir[2+num] == path[1] && dir[3+num] == path[2]){
                                 //get content block from inode
                                 parID = id;
                                 id = dir[0+num];
                                break;
                        }else if(i == files -1){
                                printf("file %s could not be found in path\n", path);
                                return; 
                        } 
              } //for
              time++;
        } //while
     //get id of files content block for unset   
     readBlock(disk, id, dirInode);
     contentBlock = dirInode[2];

     //delete from parent
     if(parID == 2){
        deleteEntry( disk,2, name);
     }else{
      readBlock(disk, parID, dirInode);
      cur = dirInode[2];
      deleteEntry( disk,cur, name);
     }

    //unset vector bit of inode and content
    int* blocks = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 1, blocks);  
    ClearBit(blocks,contentBlock);
    ClearBit(blocks,id);
    writeBlock(disk, 1, blocks);
  
}

//also returns pointer
//must have name length 3 or less
int createDirectory(FILE* disk, char* data) {
       
        char** tokens = parse(disk,data);  
        char* name;
        int len;
        for(len = 0; tokens[len] != NULL;len++){
                name = tokens[len];
        } 

        if(strlen(name)> 3){
           printf("name too large please keep it to 3 char\n");
           return -1;
        }else if(strlen(name)< 3){
           printf("name should be 3 chars long\n");
           return -1;
        }else if(name == NULL){
           printf("Invalid directory path\n");
           return -1;  
        }
     
    //allocate inode 
    int id = 0;   
    short* inode = createInode(disk,"aaaa");// will allocate 1 block
    inode[1] = 1;// a directory

    //find where to put inode
    int* blocks = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 1, blocks);  

    for(int i = 10; i < 209; i++){ //find empty
       if(TestBit(blocks,i) == 0){
               SetBit(blocks,i);
               id = i;
               break;
       }else if(i == 208){
            printf("Error: out of inode memory");
            exit(1);
       }
    }//for

    //add directory to its parent
    if( len == 1){//put in root
       addEntry( disk,2, tokens[0], id);
    }else{
        short cur =  2;
        int time = 0;
        while(time < len - 1){
              char* path = tokens[time];

              char* dir = malloc(sizeof(char) * BLOCK_SIZE);
              short* dirInode = malloc(sizeof(char) * BLOCK_SIZE);
              readBlock(disk, cur, dir);
              int files = dir[0]; 
              for(int i = 0; i < files; i++){ //find dir in parent
                      int num = ((i+1)* 4);
                        int node = dir[0+num];
                        if(dir[1+num]==path[0] && dir[2+num] == path[1] && dir[3+num] == path[2]){
                                 //get content block from inode
                                 readBlock(disk, node,  dirInode);
                                 cur = dirInode[2];
                                 readBlock(disk, cur, dir);
                                break;
                        }else if(i == files -1){
                                printf("file %s could not be found in path", path);
                                exit(1);
                        } 
              } //for
              time++;
        } //while
        addEntry( disk,cur, name, id);
    }//else

    //write inode to block
    writeBlock(disk, id, inode);
    writeBlock(disk, 1, blocks);

    //write the name to block 
    char* content = malloc(sizeof(char) * BLOCK_SIZE);
    content [0]= 0;//no files
    content [1] = name[0];  
    content [2] = name[1]; 
    content [3] = name[2]; 
    content [4] = 0;  
    writeBlock(disk, inode[2], content);

    free(blocks);
    free(inode);
    return id;
}

//returns the int indentifier of the inode
int createFile(FILE* disk, char* filePath, char* data) {
        char** tokens = parse(disk,filePath);  
        int len;
        for(len = 0; tokens[len] != NULL;len++){} 

    //allocate inode    
    int id = 0;
    short* inode = createInode(disk,data);
    
    //find where to put inode
    int* blocks = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 1, blocks);  

    for(int i = 10; i < 209; i++){ //find empty
       if(TestBit(blocks,i) == 0){
               SetBit(blocks,i);
               id = i;
               break;
       }else if(i == 208){
            printf("Error: out of inode memory");
            exit(1);
       }
    }//for

    char strID[4];
    sprintf(strID, "%d", id);

        //add directory to its  ***********
    if( strcmp(tokens[0], "root") == 0 || strcmp(tokens[0], "hom") == 0){//put in root
       addEntry( disk,2,  strID, id);
    }else{
        short cur =  2;
        int time = 0;
        while(time < len){
              char* path = tokens[time];
              char* dir = malloc(sizeof(char) * BLOCK_SIZE);
              short* dirInode = malloc(sizeof(char) * BLOCK_SIZE);
              readBlock(disk, cur, dir);
              int files = dir[0]; 
              for(int i = 0; i < files; i++){ //find dir in parent
                      int num = ((i+1)* 4);
                        int node = dir[0+num];
                        if(dir[1+num]==path[0] && dir[2+num] == path[1] && dir[3+num] == path[2]){
                                 //get content block from inode
                                 readBlock(disk, node,  dirInode);
                                 cur = dirInode[2];
                                 readBlock(disk, cur, dir);
                                break;
                        }else if(i == files -1){
                                printf("file %s could not be found in path", path);
                                return -1;
                        } 
              } //for
              time++;
        } //while
        addEntry( disk,cur, strID, id);
    }//else             *******************
    
    //write inode to block
    writeBlock(disk, id, inode);
    writeBlock(disk, 1, blocks);

    //write the data to blocks specified by inode
    for(int i =0; inode[2+i] != 0 ; i++ ){
            char part[BLOCK_SIZE];                       //may cause problems
            strncpy(part, data, BLOCK_SIZE);
            //printf("%d: %s\n\n\n",i,part);
            data += BLOCK_SIZE;
            writeBlock(disk, inode[2+i], part);
    }//for
   
    free(blocks);
    free(inode);
     
    return id;
}

//read file given inode #, returns char *
char* readFile(FILE* disk, int id) {
    short* inode = malloc(sizeof(char) * BLOCK_SIZE);
    char* block = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, id, inode);

    if(inode[1]== 1){
         return("this is a directory");
    }

    int size = inode[0]; 
    char* content = malloc(size);
    strcpy(content,"");
    int num = (size/BLOCK_SIZE)+1; 
    for(int i =0; i < num; i++ ){
        readBlock(disk, inode[2+i], block);
        strcat(content,block);
    }//for

    free(inode);
    return content;
}
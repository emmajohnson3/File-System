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
    fwrite(data, strlen(data), 1, disk); 
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

short* createInode(FILE* disk, char* data) {
    short* super = malloc(512);
    int num = 0;
    int size = strlen(data);//file size
    super[0] = size;
    super[1] = 0; //super + vector (num blocks)
    super[10] = 3; //(num inodes)
    writeBlock(disk, 10, super);
    /*char* inode = malloc(BLOCK_SIZE);
    inode[0] = (char) size;//file size
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
               printf("allocating block:%d\n", i);
               SetBit(blocks,i);
               inode[2+k] = i;
               printf("confirm block:%d\n", inode[2+k]);
               break;
       }  
    }//for
    }//for

    if(i == NUM_BLOCKS-1){
            printf("Error: out of content memory");
            exit(1);
    }
    writeBlock(disk, 1, blocks);
    free(blocks);*/
    return super;
}

//returns the int indentifier of the inode
int createFile(FILE* disk, char* data) {
    //allocate inode    
    int id = 0;
    short* inode = createInode(disk,data);
    /*
    //find where to put inode
    int* blocks = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, 1, blocks);

    

    for(int i = 10; i < 209; i++){ //find empty
       if(TestBit(blocks,i) == 0){
               SetBit(blocks,i);
               writeBlock(disk, 1, blocks);
               id = i;
               break;
       }  
    }//for
    
    //write inode to block
    writeBlock(disk, id, inode);
    
    printf("inode is in block: %d", id);
    int* help = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, id, help);
    for(int i =0; i< 10 ; i++ ){
          printf("inode: %d       block: %d\n", inode[2+i],help[2+i] );
    }//for

    //write the data to blocks specified by inode
    
   
    for(int i =0; inode[2+i] != 0 ; i++ ){
            char part[BLOCK_SIZE];
            strncpy(part, data, BLOCK_SIZE);
            //printf("%d: %s\n\n\n",i,part);
            data += BLOCK_SIZE;
            writeBlock(disk, inode[2+i], part);
    }//for
   
    free(blocks);*/
    free(inode);
     
    return id;
}


int createDirectory(FILE* disk, char* data) {
    //allocate inode    
    char* inode = createInode(disk,data);
    inode[1] = 1;//means its a directory
    //find where to put inode
    
    //write the data to blocks specified by inode
    free(inode);
}

//read file given inode #, returns char *
char* readFile(FILE* disk, int id) {
    char* inode = malloc(sizeof(char) * BLOCK_SIZE);
    char* block = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, id, inode);

    int size = inode[0]; 
    char* content = malloc(size);
    
    for(int i =0; inode[2+i] != 0 ; i++ ){
        readBlock(disk, inode[2+i], block);
        //printf("looking at block: %d\n\n",inode[2+i]);
        //printf("%d: %s\n\n",i,block);
        strcat(content,block);
       //printf("coneccted: %s\n\n",content);
    }//for

    free(inode);
    return content;
}

int main(int argc, char* argv[]) {
FILE* disk = fopen("vdisk", "w+b");
    //DeleteDisk();
    CreateDisk(disk);

    int file = createFile(disk, "y2apffIWMWnotk8uP0k2KuR0MkDpswtqMEJkGP4TcA1KgAc7d3AfAB78IaRTtNMtobRYefjXl0XzmKcRnvyU9Y006Z1raS0W0sZj8tgHbK4UXfTSpIpcFW3KGBIhduHicC5Pobfr9scWZeCgleJamIx4upRyl7Dx10pcKyXAe0N7BjZki6Ve1giPhtUwvItZDPUxs8NAbEoxxosl87aXBT8zCjv7xX6SwDb9s6jXI0fhUQ1o1qfSjGftUmi5mQ4CHcFlBTfhNBOOe3PIPKvaJ5Kwex3U5V25vbhB7ayEtJoHqDBtD70GOqpUfCpN8QazApuJd0301D5Rl0B6NXd54YBlDlI4tgwvqx6aosqZ99WjGaTutg5Ew8IpRn9lMTg9B53AeEcDn9mgQzA4R7rcGO8X189OA5BQH1W0ZWOJ0vGcsXcSXm5GLcAh3IG4P5h8WNYOpmcz14Ezmb2Fnf67RpTBYnI0tz1Mh2hOt04TPswPUDHJpv3z2tu47CNm6UJ5HrRG6nUr1GkaFPt02IOpaepwFqJ4bnVZLxT3fzA0oKaXKvCeMv02dpGMVL6bCFi0wtF4Lo9lHC5A9NqcawYC4UK"); 
   // char* buffer = readFile(disk, file);
    //printf("%s\n", buffer);

   //free(buffer);
    printf("done\n");

    fclose(disk);
    return 0;
}
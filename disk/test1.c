#include "RW.h"

int main(int argc, char* argv[]) {
    char* buffer =  malloc(sizeof(char) * 512 * 2);
    FILE* disk = fopen("vdisk", "w+b");
    CreateDisk(disk);

    printf("This file tests reading and writing files\n\n");
    
    printf("Writing large file (multiple blocks) of 'y2apffIWMWnotk8uP0k2KuR0MkDpswtqMEJkGP4TcA.... ect'\n");
    int file = createFile(disk, "root","y2apffIWMWnotk8uP0k2KuR0MkDpswtqMEJkGP4TcA1KgAc7d3AfAB78IaRTtNMtobRYefjXl0XzmKcRnvyU9Y006Z1raS0W0sZj8tgHbK4UXfTSpIpcFW3HelloaaaaaaacC5Pobfr9scWZeCgleJamIx4upRyl7Dx10pcKyXAe0N7BjZki6Ve1giPhtUwvItZDPUxs8NAbEoxxosl87aXBT8zCjv7xX6SwDb9s6jXI0fhUQ1o1qfSjGftUmi5mQ4CHcFlBTfhNBOOe3PIPKvaJ5Kwex3U5V25vbhB7ayEtJoHqDBtD70GOqpUfCpN8QazApuJd0301D5Rl0B6NXd54YBlDlI4tgwvqx6aosqZ99WjGaTutg5Ew8IpRn9lMTg9B53AeEcDn9mgQzA4R7rcGO8X189OA5BQH1W0ZWOJ0vGcsXcSXm5GLcAh3IG4P5h8WNYOpmcz14Ezmb2Fnf67RpTBYnI0tz1Mh2hOt04TPswPUDHJpv3z2tu47CNm6UJ5HrRG6nUr1GkaFPt02IOpaepwFqJ4bnVZLxT3fzA0oKaXKvCeMv02dpGMVL6bCFi0wtF4Lo9lHC5A9NqcawYBooop"); 
    printf("Function returned id of %d\n\n",file);

    printf("Writing small file of 'hello person there!!!!1'\n");
    int file2 = createFile(disk, "root","hello person there!!!!1");
    printf("Function returned id of %d\n\n",file2);

    printf("Reading the first file: %d\n", file);
    buffer = readFile(disk,file);
    printf("%s\n\n", buffer);

    printf("Reading the second file: %d\n", file2);
    buffer = readFile(disk,file2);
    printf("%s\n\n", buffer);
    
    printf("We can see these files listed by printing to root directory\n");
    ReadDirectory(disk, "root");

    free(buffer);
    printf("Finished the first test file\n\n");

    fclose(disk);
    return 0;
}
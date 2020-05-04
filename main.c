#include <stdio.h>
#include <stdlib.h>

struct fat_entry
{
    int busy;
    int next;
};

struct file_allocation_table
{
    int numberOfBlocks;
    struct fat_entry* file_blocks;
};

struct file
{
    int startingBlockInFAT;
    int sizeInBytes;
};

struct sector
{
    int byteCount;
    char bytes[512];
};

struct hdd
{
    int totalSectors;
    int usedSectors;
    int freeSectors;
    struct sector* sectors;
};

struct block
{
    int usedSectors;
    struct sector* sectors; //this is the base address of the first sector
};

struct formatted_hdd
{
    int numberOfBlocks;
    //the base address of our collection of blocks that abstract
    //an existing collection of sectors
    struct block* blocks; 
    struct file* files;
};

//constructor for a block
struct block genBlock(struct sector* baseAddressOfSectorGroupOf8)
{
    struct block temp;
    temp.usedSectors = 0;
    temp.sectors = baseAddressOfSectorGroupOf8;
    return temp;
}

//constructor for a sector
struct sector genSector()
{
    struct sector temp;
    temp.byteCount = 0;
    return temp;
}


//constructor for a hdd (assume bytes is a multiple of 512)
struct hdd genHDD(int bytes)
{
    struct hdd temp;
    temp.totalSectors = bytes / 512;
    temp.freeSectors = temp.totalSectors;
    temp.usedSectors = 0;
    temp.sectors = malloc(temp.totalSectors * sizeof(struct sector));

    //fill the hdd with sectors
    for(int i = 0; i < temp.totalSectors; i++)
    {
        temp.sectors[i] = genSector();
    }
    return temp;
}

struct formatted_hdd formatHDD(struct hdd theHDD)
{
    struct formatted_hdd temp;
    temp.numberOfBlocks = theHDD.totalSectors/8;
    temp.blocks = malloc(temp.numberOfBlocks * sizeof(struct block));
    temp.files = malloc(temp.numberOfBlocks * sizeof(struct file));

    int posOfNextBaseSector = 0;
    for(int i = 0; i < temp.numberOfBlocks; i++)
    {
        temp.blocks[i] = genBlock(&theHDD.sectors[posOfNextBaseSector]);
        posOfNextBaseSector += 8;
    }
    return temp;
}

struct file getFile()
{
    printf("Enter the size of a file: ");
    struct file theFile;
    scanf("%d", &theFile.sizeInBytes);
    theFile.startingBlockInFAT = -1;
    return theFile;
}


int main(int argc, char** argv)
{
    struct hdd theHDD = genHDD(1 * 1024 * 1024 * 1024);
    struct formatted_hdd theVolume = formatHDD(theHDD);
    struct file theFile = getFile();
    printf("the size is %d", theFile.sizeInBytes);
    return 0;
}
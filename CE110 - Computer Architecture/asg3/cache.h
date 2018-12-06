#pragma once

//Includes
#include "macros.h"
#include <stdlib.h>
#include <stdio.h>

/********************************Structs*****************************/
//Struct for Blocks in Cache
typedef struct Block
{
    bool     valid;    //Valid bit
    uint64_t mdata;    //Metadata of block
    uint64_t data;     //Data of block
} Block;

//Struct for Cache
typedef struct Cache
{
    Block *memory;      //Memory of cache
    int   size;         //Total size of cache in KiB
    int   blkSize;      //Size of blocks within cache
    int   blkCnt;       //Number of blocks in cache
    bool  writeT;       //Boolean to hold write through variable
    int   indexSize;    //Bit length of index
    int   tagSize;      //Bit length of tag
} Cache;
/********************************Structs*****************************/

/********************************Functions***************************/
//Cache write function
void CacheWrite(Cache* c, uint64_t metaData, uint64_t data);

//Check cache for entry given metadata
Block* CheckCache(Cache* c, uint64_t metaData);

//Cache read function
Block CacheRead(Cache* c, int blockNum);
/********************************Functions***************************/

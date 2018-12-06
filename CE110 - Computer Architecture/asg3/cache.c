//Includes
#include "macros.h"
#include "cache.h"
#include <stdlib.h>
#include <stdio.h>

/********************************Functions***************************/
/*Helper function to get offset
uint64_t GetOffset()
{

}*/

//Helper function to get index
uint64_t GetIndex(Cache* c, uint64_t memAddress)
{
    int i, mask = 0;
    //Get mask for index
    for(i = 0; i < c->indexSize; i++)
    {
        mask <<= 1;
        mask |= 1;
    }
    return mask & memAddress;
}

//Helper function to get tag
uint64_t GetTag(Cache* c, uint64_t memAddress)
{
    return memAddress >> (c->indexSize);
}

//Cache write function
void CacheWrite(Cache* c, uint64_t memAddress, uint64_t data)
{
    Block b;
    b.mdata = GetTag(c, memAddress);
    b.data  = data;
    b.valid = true;
    c->memory[GetIndex(c, memAddress)] = b;
}

//Check cache for entry given metadata
Block* CheckCache(Cache* c, uint64_t memAddress)
{
    //Get tag and index
    uint64_t tag, index;
    tag   = GetTag(c, memAddress);
    index = GetIndex(c, memAddress);

    //Check cache for entry else return null
    if (c->memory[index].mdata == tag && c->memory[index].valid)
        return &(c->memory[index]);
    else
        return NULL;
}
/********************************Functions***************************/

/*int main(){
    int i;
    for (i = 0; i < 10; i++){
        iCache->memory[i].mdata = i;
        iCache->memory[i].data =  i;
    }
    Block* b = CheckCache(iCache, 5);
    if (b != NULL){
        printf("SUCCESS: %llu %llu\n", b->mdata, b->data);
    }
    if (CheckCache(iCache, 15) == NULL){
        printf("SUCCESS");
    }
    return 1;
}*/

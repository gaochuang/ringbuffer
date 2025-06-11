#ifndef BIP_BUFFER_H_
#define BIP_BUFFER_H_

#include <stdint.h>

typedef struct Bipbuffer
{
    uint32_t partitionASize;       //A区大小
    uint32_t partitionAIndex;     //A区索引

    uint32_t partitionBSize;       //B区大小
    uint32_t partitionBIndex;      //B区索引

    uint32_t reservedSize;        //预留缓冲区大小
    uint32_t reservedSizeIndex;   //预留区索引
    uint32_t buffersize;          //memorySize
    char buffer[1];               //内存基地址, 将缓冲区与结构体本体连续存储，利于缓存局部性，减少内存碎片
}Bipbuffer;

uint32_t bipbuffer_init(Bipbuffer* bip, uint32_t size);

void bipbuffer_destory(Bipbuffer* bipbuffer);

void* bipBuffer_reserve(Bipbuffer* bipbuffer, uint32_t size);

void bipBuffer_commit(Bipbuffer* bipbuffer, uint32_t size);

void* bipBuffer_get(Bipbuffer* bipbuffer, uint32_t* countinousBlockSizePtr);

void CBipBuffer_Decommit(Bipbuffer* bipbuffer, uint32_t size);


#endif

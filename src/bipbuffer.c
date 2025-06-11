#include "bipbuffer.h"
#include <stddef.h>
#include <string.h>


/* 
   |    indexB + bSize       |    indexA - (indexB + Bsize)      |
--indexB*********************|--------------free---------------indexA-----------
*/
#define getPartitionBFreeSpace() (bipbuffer->partitionAIndex - bipbuffer->partitionBIndex - bipbuffer->partitionBSize)

#define getPartitionAFreeSpace() (bipbuffer->buffersize - bipbuffer->partitionAIndex - bipbuffer->partitionASize)


uint32_t bipbuffer_init(Bipbuffer* bipbuffer, uint32_t size)
{
    memset(bipbuffer, 0, sizeof(*bipbuffer));
    bipbuffer->buffersize = size - offsetof(Bipbuffer, buffer);

    memset(bipbuffer->buffer, 0, bipbuffer->buffersize);

    return 0;
}

void bipbuffer_destory(Bipbuffer* bipbuffer)
{
    memset(bipbuffer, 0, sizeof(*bipbuffer));
}

//申请一块内存用来写
void* bipBuffer_reserve(Bipbuffer* bipbuffer, uint32_t size)
{
    uint32_t freeSpace = 0;
   
    //如果B区存在，则需要从B区后面追加内存
    if(bipbuffer->partitionBIndex != 0)
    {
        freeSpace = getPartitionBFreeSpace();
        if(freeSpace == 0 || freeSpace < size)
        {
            return NULL;
        }
        bipbuffer->reservedSizeIndex = bipbuffer->partitionBIndex + bipbuffer->partitionBSize;
        bipbuffer->reservedSize = size;

        return &bipbuffer->buffer[bipbuffer->reservedSizeIndex];
    }else
    {
        //B区不存在, A区空间够
        freeSpace = getPartitionAFreeSpace();
        if(freeSpace >= size)
        {
            if(freeSpace == 0)
            {
                return NULL;
            }

            bipbuffer->reservedSizeIndex = bipbuffer->partitionAIndex + bipbuffer->partitionASize;
            bipbuffer->reservedSize = size;
        }else
        {
            //A区全部读完，全部从起点开始
            //-------------------------indexA-------------------------------
            if(bipbuffer->partitionAIndex > 0 && bipbuffer->partitionASize == 0 && size <= bipbuffer->buffersize)
            {
                bipbuffer->partitionAIndex = 0;
                bipbuffer->reservedSizeIndex = 0;
                bipbuffer->reservedSize = size;
                return &bipbuffer->buffer[0]; //buffer的其实位置
             //indexA-----------------------------------------------------
            }else if(bipbuffer->partitionAIndex == 0 || bipbuffer->partitionAIndex < size || size > bipbuffer->buffersize )
            {
                return NULL;
            }

            bipbuffer->reservedSizeIndex = 0;
            bipbuffer->reservedSize = size;

            return &bipbuffer->buffer[0];
        }
    }

    return NULL;
}

void bipBuffer_commit(Bipbuffer* bipbuffer, uint32_t size)
{
    //没有写成功
    if(0 == size)
    {
        bipbuffer->reservedSizeIndex = 0;
        bipbuffer->reservedSize = 0;
        return;
    }

    //防止尝试提交超出要求的空间
    if(size > bipbuffer->reservedSize)
    {
        size = bipbuffer->reservedSize;
    }

    //没有区域被使用
    if(bipbuffer->partitionAIndex == 0 && bipbuffer->partitionBIndex == 0)
    {
        //创建A区域
        bipbuffer->partitionAIndex = bipbuffer->reservedSizeIndex;
        bipbuffer->partitionASize = size;

        bipbuffer->reservedSize = 0;
        bipbuffer->reservedSizeIndex = 0;

        return;
    }

    //在A区域 后面添加
    if(bipbuffer->reservedSizeIndex == (bipbuffer->partitionAIndex + bipbuffer->partitionASize))
    {
        bipbuffer->partitionASize += size;
    }else
    {
        //在B区别后面添加
        bipbuffer->partitionBSize += size;
    }

    bipbuffer->reservedSize = 0;
    bipbuffer->reservedSizeIndex = 0;
}

void* bipBuffer_get(Bipbuffer* bipbuffer, uint32_t* countinousBlockSizePtr)
{
    //读永远从A区读
    if(bipbuffer->partitionASize == 0)
    {
        *countinousBlockSizePtr = 0;
        return NULL;
    }

    *countinousBlockSizePtr = bipbuffer->partitionASize;
    return &bipbuffer->buffer[bipbuffer->partitionAIndex];
}

//从缓冲区读取数据
void bipBuffer_decommit(Bipbuffer* bipbuffer, uint32_t size)
{
    //A区被读完
    if(bipbuffer->partitionAIndex < size && bipbuffer->partitionBIndex != 0)
    {
        //把B区变成A区
        bipbuffer->partitionAIndex = bipbuffer->partitionBIndex;
        bipbuffer->partitionASize = bipbuffer->partitionBSize;
        bipbuffer->partitionBIndex = 0;
        bipbuffer->partitionBSize = 0;
    }else
    {
        bipbuffer->partitionASize -= size;
        bipbuffer->partitionAIndex += size; 
    }
}

#include "ringbuffer.h"
 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void ringbuffer_init(ring_buffer_t* buffer, int size)
{
    if(NULL == buffer || size <= 0)
    {
        printf("input parameter is invalid\n");
        return;
    }

    memset(buffer, 0, sizeof(ring_buffer_t));
    buffer->buffer = (uint8_t*)malloc(size);
    if(NULL == buffer->buffer)
    {
        printf("malloc memory failed\n");
    }

    buffer->total_size = size;
}

void ringbuffer_descory(ring_buffer_t* ringbuffer)
{
    if(NULL == ringbuffer)
    {
        printf("input is null");
        return;
    }

    free(ringbuffer->buffer);
    memset(ringbuffer, 0, sizeof(ring_buffer_t));
}

int ring_buffer_write(const void *buffer_to_write, uint32_t size, ring_buffer_t *ring_buffer)
{
    if(NULL == ring_buffer || NULL == buffer_to_write)
    {
        printf("input parameter is invalid\n");
        return -1;
    }

    uint32_t write_offset = ring_buffer->writeIndex;
    uint32_t first_write_size = 0;
    uint32_t total_size = ring_buffer->total_size;

    if(ring_buffer->buffer_vaild_size + size > ring_buffer->total_size)
    {
        printf("ring buffer too short to save buffer\n");
        printf("total_size:%d valid_size:%d size:%d\n", ring_buffer->total_size, ring_buffer->buffer_vaild_size, size);
        return -1;
    }

    if(size + write_offset <= total_size)
    {
        memcpy(ring_buffer->buffer + write_offset, buffer_to_write, size);
    }else
    {
        first_write_size = total_size - write_offset;
        memcpy(ring_buffer->buffer + write_offset, buffer_to_write, first_write_size);
        memcpy(ring_buffer->buffer, (const uint8_t *)buffer_to_write + first_write_size, size - first_write_size);
    }

    ring_buffer->writeIndex = (write_offset + size)%total_size;
    ring_buffer->buffer_vaild_size += size;
    return 0;
}

int ring_buffer_read(ring_buffer_t *ring_buffer, void* buffer, uint32_t size)
{
    if(ring_buffer == NULL || buffer == NULL)
    {
        printf("input parameter is invalid\n");
        return -1;
    }

    uint32_t readIndex = ring_buffer->readIndex;
    uint32_t totalSize = ring_buffer->total_size;
    uint32_t firs_read_size =  0;

    if(size > ring_buffer->buffer_vaild_size)
    {
        printf("no enougt data to read\n");
        return -1;
    }

    //不会发送环绕
    if(readIndex + size <= totalSize)
    {
        memcpy(buffer, ring_buffer->buffer + readIndex, size);
    }else
    {
        firs_read_size = totalSize - readIndex;
        memcpy(buffer, ring_buffer->buffer + readIndex, firs_read_size);
        memcpy(buffer + firs_read_size, ring_buffer->buffer, size - firs_read_size);
    }

    ring_buffer->readIndex = (readIndex + size)%totalSize;
    ring_buffer->buffer_vaild_size -= size;

    return 0;
}
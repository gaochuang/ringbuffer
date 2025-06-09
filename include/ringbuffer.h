#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>

typedef struct ringbuffer
{
    uint8_t *buffer;
    uint32_t readIndex;
    uint32_t writeIndex;
    int32_t buffer_vaild_size;
    int32_t total_size;
}ring_buffer_t;

void ringbuffer_init(ring_buffer_t* buffer, int size);
void ringbuffer_descory(ring_buffer_t* buffer);
int ring_buffer_write(const void *buffer_to_write, uint32_t size, ring_buffer_t *ring_buffer);
int ring_buffer_read(ring_buffer_t *ring_buffer, void* buffer, uint32_t size);
#endif


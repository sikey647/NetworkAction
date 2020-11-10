#include <pthread.h>

typedef struct {
    int size;
    void *data;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} block_queue_st;

void block_queue_init(block_queue_st *block_queue, int size);

int block_queue_push(block_queue_st *block_queue, void *data);

int block_queue_pop(block_queue_st *block_queue, void **data);

void block_queue_destroy(block_queue_st *block_queue);
#include "block_queue.h"

#include <stdlib.h>

void block_queue_init(block_queue_st *block_queue, int size) {
    block_queue->size = size;
    block_queue->data = calloc(size, sizeof(void *));
    block_queue->front = block_queue->rear = 0;
    pthread_mutex_init(&block_queue->mutex, NULL);
    pthread_cond_init(&block_queue->cond, NULL);
}

void block_queue_push(block_queue_st *block_queue, void *data) {
    pthread_mutex_lock(&block_queue->mutex);
    block_queue->data[block_queue->rear] = data;
    if (++block_queue->rear == block_queue->size) {
        block_queue->rear = 0;
    }
    pthread_cond_signal(&block_queue->cond);
    pthread_mutex_unlock(&block_queue->mutex);
}

void block_queue_pop(block_queue_st *block_queue, void **data) {
    pthread_mutex_lock(&block_queue->mutex);
    while (block_queue->front == block_queue->rear)
        pthread_cond_wait(&block_queue->cond, &block_queue->mutex);
    *data = block_queue->data[block_queue->front];
    block_queue->data[block_queue->front] = NULL;
    if (++block_queue->front == block_queue->size) {
        block_queue->front = 0;
    }
    pthread_mutex_unlock(&block_queue->mutex);
}

void block_queue_destroy(block_queue_st *block_queue) {}
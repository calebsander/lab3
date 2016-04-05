/*
queue.c
Caleb Sander
Implements a contiguous queue of positions to deal with
*/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define DEFAULT_SIZE 1024

struct queue {
	Position **elements; //an array of position pointers storing the queue
	//The start index (inclusive) and end index (exclusive)
	unsigned int start, end;
	unsigned int allocatedSize; //the size of the elements array
};
/*
	Stored elements begin at index start and finish at index end,
	wrapping around if end < start; can store at most allocatedSize - 1 elements
*/

/**
 * Makes an empty queue of the specified size
 * @param size the initial size
 * @return a newly malloc'd queue
 */
Queue *makeEmptyQueueOfSize(unsigned int size) {
	Queue *queue = malloc(sizeof(*queue));
	queue->elements = malloc(sizeof(*(queue->elements)) * size);
	queue->start = queue->end = 0;
	queue->allocatedSize = size;
	return queue;
}
Queue *makeEmptyQueue() {
	return makeEmptyQueueOfSize(DEFAULT_SIZE);
}

/**
 * Resizes a queue to have twice its capacity
 * @param queue the queue to resize;
 */
void resizeQueue(Queue *queue) {
	Queue *newQueue = makeEmptyQueueOfSize(queue->allocatedSize << 1);
	while (!isEmpty(queue)) push(newQueue, pop(queue));
	free(queue->elements);
	queue->elements = newQueue->elements;
	queue->start = newQueue->start;
	queue->end = newQueue->end;
	queue->allocatedSize = newQueue->allocatedSize;
	free(newQueue);
}
/**
 * Add one to a value, wrapping around the queue size.
 * The value pointed to be value is directly modified for convenience.
 * @param value the value to increment
 * @param queue the queue whose size to use
 */
void addWrap(unsigned int *value, Queue *queue) {
	*value = (*value + 1) % queue->allocatedSize;
}
bool isEmpty(Queue *queue) {
	return queue->start == queue->end;
}
void push(Queue *queue, Position *element) {
	unsigned int end = queue->end;
	addWrap(&end, queue);
	if (end == queue->start) resizeQueue(queue); //queue cannot fit another element
	queue->elements[queue->end] = element;
	addWrap(&(queue->end), queue);
}
/**
 * Quit the program if the queue is empty (should never occur)
 * @param queue the queue in question
 */
void errorIfEmpty(Queue *queue) {
	if (isEmpty(queue)) {
		fputs("Cannot peek on empty queue\n", stderr);
		exit(EXIT_FAILURE);
	}
}
Position *peek(Queue *queue) {
	errorIfEmpty(queue);
	return queue->elements[queue->start];
}
Position *pop(Queue *queue) {
	Position *value = peek(queue);
	addWrap(&(queue->start), queue);
	return value;
}
void freeQueue(Queue *queue) {
	free(queue->elements);
	free(queue);
}
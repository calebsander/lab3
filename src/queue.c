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
	Position **elements; //contains an array of position pointers
	unsigned int start, end;
	unsigned int allocatedSize;
};
/*
	Stored elements begin at index start and finish at index end,
	wrapping around if end < start; can store at most allocatedSize - 1 elements
*/

//Make an empty queue of the specified size
Queue *makeEmptyQueueOfSize(unsigned int size) {
	Queue *queue = malloc(sizeof(*queue));
	queue->elements = malloc(sizeof(*(queue->elements)) * size);
	queue->start = queue->end = 0;
	queue->allocatedSize = size;
	return queue;
}
//Make an empty queue of a default size
Queue *makeEmptyQueue() {
	return makeEmptyQueueOfSize(DEFAULT_SIZE);
}

//Resize a queue to have twice the capacity
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
//Add one to a value, wrapping around the queue size
void addWrap(unsigned int *value, Queue *queue) {
	*value = (*value + 1) % queue->allocatedSize;
}
//Return whether the queue is empty
bool isEmpty(Queue *queue) {
	return queue->start == queue->end;
}
//Push a new position to the tail of the queue
void push(Queue *queue, Position *element) {
	unsigned int end = queue->end;
	addWrap(&end, queue);
	if (end == queue->start) resizeQueue(queue); //queue cannot fit another element
	queue->elements[queue->end] = element;
	addWrap(&(queue->end), queue);
}
//Quit the program if the queue is empty
void errorIfEmpty(Queue *queue) {
	if (isEmpty(queue)) {
		fputs("Cannot peek on empty queue\n", stderr);
		exit(EXIT_FAILURE);
	}
}
//Get the element at the head of the queue
Position *peek(Queue *queue) {
	errorIfEmpty(queue);
	return queue->elements[queue->start];
}
//Get and remove the element at the head of the queue
Position *pop(Queue *queue) {
	Position *value = peek(queue);
	addWrap(&(queue->start), queue);
	return value;
}
//Free a queue and its buffer
void freeQueue(Queue *queue) {
	free(queue->elements);
	free(queue);
}
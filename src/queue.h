/*
queue.h
Caleb Sander
Functions for manipulating queues of Positions to process
*/
#include <stdbool.h>
#include "position.h"

typedef struct queue Queue;

/**
 * Makes an empty queue of a default size
 * @return a newly malloc'd queue
 */
Queue *makeEmptyQueue();
/**
 * Returns whether the queue is empty
 * @param queue the queue in question
 * @return true iff no elements remain in the queue
 */
bool isEmpty(Queue *queue);
/**
 * Pushes a new Position to the tail of the queue
 * @param queue the queue to which to add
 * @param element the Position to add
 */
void push(Queue *queue, Position *element);
/**
 * Gets the element at the head of the queue without removing it
 * @param queue the queue in question
 * @return the Position at the head
 */
Position *peek(Queue *queue);
/**
 * Gets the element at the head of the queue.
 * If the element was malloc'd, it is now the caller's responsibility
 * to free it.
 * @param queue the queue from which to remove
 * @return the removed element
 */
Position *pop(Queue *queue);
/**
 * Frees a queue and its array of elements.
 * This will not free any of the elements remaining in it.
 * @param queue the queue to free
 */
void freeQueue(Queue *queue);
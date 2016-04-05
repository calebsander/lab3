/*
9-tiles.c
Caleb Sander
Contains the main function for the program
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "hashMap.h"
#include "position.h"

#define HEIGHT_ARG 1
#define WIDTH_ARG 2
#define MAX_LENGTH_ARG (argc - 3)
#define INITIAL_ARG (argc - 2)
#define GOAL_ARG (argc - 1)
#define MIN_DIMENSION 2
#define DEFAULT_DIMENSION 3

#define FOUND ' ' //character that shouldn't occur in position strings
#define INVALID_SYNTAX "Invalid argument syntax. \
Use ./Nine [HEIGHT WIDTH] MAXLENGTH INITIAL GOAL\n"

/**
 * If map doesn't contain the position, adds it to map and queue
 * (utility function used by main())
 * @param map the map of found positions being used
 * @param searchQueue the queue of positions to search
 * @param moved the position just discovered
 * @param from the position it was reached from
 * (in case it needs to be added to the map)
 * @param currentLength the number of moves to get to "from"
 * (in case it needs to be added to the map)
 */
void newPosition(HashMap *map,
	Queue *searchQueue,
	Position *moved,
	Position *from,
	const unsigned short currentLength) {
	if (contains(map, moved)) freePosition(moved); //this position is a duplicate
	else {
		put(map, moved, from, currentLength + 1);
		push(searchQueue, moved);
	}
}
int main(int argc, char **argv) {
	int height, width;
	if (argc == 4) height = width = DEFAULT_DIMENSION; //no dimensions specified
	else if (argc == 6) { //dimensions specified
		height = atoi(argv[HEIGHT_ARG]);
		width = atoi(argv[WIDTH_ARG]);
		if (height < MIN_DIMENSION || width < MIN_DIMENSION) {
			fputs("Height and width must be at least 2\n", stderr);
			exit(1);
		}
	}
	else {
		fputs(INVALID_SYNTAX, stderr);
		exit(1);
	}
	//Set the dimensions of all positions so we can check to ensure that
	//any new positions match the dimensions
	setDimensions((unsigned int)height, (unsigned int)width);

	const int maxLength = atoi(argv[MAX_LENGTH_ARG]);
	if (maxLength < 1) { //if string conversion fails, this will be 0
		fputs("MAXLENGTH must be positive\n", stderr);
		exit(1);
	}
	char *initialString = argv[INITIAL_ARG];
	char *goalString = argv[GOAL_ARG];
	char *goalCopy = strdupe(goalString);
	for (char *initialSearch = initialString; *initialSearch; initialSearch++) {
		char *goalSearch;
		for (goalSearch = goalCopy; *goalSearch != *initialSearch; goalSearch++) {
			if (!*goalSearch) {
				//reached the end of goalCopy without finding a matching character
				//so the input was invalid
				fputs("Characters in initial and goal don't match\n", stderr);
				exit(1);
			}
		}
		*goalSearch = FOUND; //register that this character has been matched
	}
	free(goalCopy);
	Position *initial = parsePosition(initialString);
	//This position is constructed using malloc'd strings for consistency
	Position *goal = parsePosition(strdupe(goalString));
	//Queue of positions to look through
	Queue *searchQueue = makeEmptyQueue();
	//Map of reached positions
	HashMap *map = makeEmptyMap();
	//Start looking with the goal position (trying to get back to initial position)
	push(searchQueue, goal);
	//Didn't reach the goal from a previous position, and no moves were necessary
	put(map, goal, NULL, 0);
	//If a path was found, the position that matched initial (needs to be freed)
	Position *newInitial = NULL;
	while (!isEmpty(searchQueue)) {
		Position *p = pop(searchQueue);
		if (equals(p, initial)) {
			assert(contains(map, p));
			newInitial = p;
			break;
		}
		else {
			const unsigned short currentLength = getLength(map, p);
			if (currentLength < maxLength) {
				const unsigned char possibilities = possibleMoves(p);
				if (up(possibilities)) {
					newPosition(map, searchQueue, moveUp(p), p, currentLength);
				}
				if (left(possibilities)) {
					newPosition(map, searchQueue, moveLeft(p), p, currentLength);
				}
				if (down(possibilities)) {
					newPosition(map, searchQueue, moveDown(p), p, currentLength);
				}
				if (right(possibilities)) {
					newPosition(map, searchQueue, moveRight(p), p, currentLength);
				}
			}
		}
	}
	freeQueue(searchQueue);
	//Iterate backwards through steps used to get to initial from goal
	//This will print out the steps in order
	if (contains(map, initial)) {
		for (Position *step = newInitial; step; step = getFrom(map, step)) {
			printPosition(step);
		}
	}
	//Free everything that had been used
	freeEntries(map);
	freeMap(map);
	//It is the copy (newInitial) that was in the map, so freed separately
	free(initial);
}
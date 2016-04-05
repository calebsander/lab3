/*
position.c
Caleb Sander
Implements the position structure (stored as a string) and its functions
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "position.h"

#define EMPTY '-' //character that represents the empty square

unsigned int height = 0, width = 0; //the height and width of all positions

struct position {
	char *values; //internally stores a malloc'd string
};

void setDimensions(unsigned int newHeight, unsigned int newWidth) {
	height = newHeight;
	width = newWidth;
}

Position *parsePosition(char *positionString) {
	if (strlen(positionString) == height * width) { //ensure right dimensions
		unsigned int dashCount = 0;
		char *positionChar = positionString;
		for (; *positionChar; positionChar++) { //count number of dashes in string
			if (*positionChar == EMPTY) dashCount++;
		}
		if (dashCount == 1) { //ensure that there is a dash and only one
			Position *position = malloc(sizeof(*position));
			position->values = positionString;
			return position;
		}
		else {
			fputs("There must be exactly one '-' in the input string\n", stderr);
			exit(1);
		}
	}
	else {
		fputs("Position string has wrong size\n", stderr);
		exit(1);
	}
}
#define PRIME 3141592661L
unsigned int hashCode(Position *position) {
	const char *s = position->values;
	unsigned long sum = 0;
	for (; *s; s++) sum = (sum << 4) ^ *s;
	return (unsigned int)(PRIME * sum);
}
bool equals(Position *position1, Position *position2) {
	return !strcmp(position1->values, position2->values);
}
/**
 * Calculates an index into the position string based on row and column.
 * Indices are ordered by row and then by column in the row.
 * Will crash the program if values exceed dimensions
 * @param y the y position (increasing downwards)
 * @param x the x position (increasing right)
 */
unsigned int rawIndex(unsigned int y, unsigned int x) {
	assert(y < height && x < width);
	return y * height + x;
}
//Values for bit positions of possibilities
//Must be distinct and less than 8 (so they fit in a byte)
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
typedef struct {
	unsigned int x, y; //the x and y positions on the board (as in rawIndex())
} Index;
/**
 * Finds the location of the empty square.
 * @param position the Position in question
 * @return the x and y coordinates
 */
Index *findEmpty(Position *position) {
	const char *values = position->values;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			if (values[rawIndex(y, x)] == EMPTY) {
				Index *index = malloc(sizeof(*index));
				index->x = x;
				index->y = y;
				return index;
			}
		}
	}
	assert(false); //there should always be an empty square
}
unsigned char possibleMoves(Position *position) {
	Index *emptyIndex = findEmpty(position);
	const unsigned int x = emptyIndex->x;
	const unsigned int y = emptyIndex->y;
	free(emptyIndex);
	return (unsigned char)(
		((y != 0) << UP) |
		((x != 0) << LEFT) |
		((y != height - 1) << DOWN) |
		((x != width - 1) << RIGHT)
	);
}
bool up(unsigned char possibilities) {
	return (possibilities >> UP) & 1;
}
bool left(unsigned char possibilities) {
	return (possibilities >> LEFT) & 1;
}
bool down(unsigned char possibilities) {
	return (possibilities >> DOWN) & 1;
}
bool right(unsigned char possibilities) {
	return (possibilities >> RIGHT) & 1;
}
char *strdupe(char *orig) {
	char *newString = malloc(sizeof(*newString) * (strlen(orig) + 1));
	strcpy(newString, orig);
	return newString;
}
Position *moveUp(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y - 1, emptyIndex->x)];
	newPositionString[rawIndex(emptyIndex->y - 1, emptyIndex->x)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
Position *moveLeft(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y, emptyIndex->x - 1)];
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x - 1)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
Position *moveDown(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y + 1, emptyIndex->x)];
	newPositionString[rawIndex(emptyIndex->y + 1, emptyIndex->x)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
Position *moveRight(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y, emptyIndex->x + 1)];
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x + 1)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
void freePosition(Position *position) {
	free(position->values);
	free(position);
}
void printPosition(Position *position) {
	puts(position->values);
}
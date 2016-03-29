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
	char *values;
};

//Set the height and width that all positions must have
void setDimensions(unsigned int newHeight, unsigned int newWidth) {
	height = newHeight;
	width = newWidth;
}
//Check to make sure that position is valid and wrap it in a position struct
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
//Hash a position string to an unsigned int (used for HashMap of positions)
unsigned int hashCode(Position *position) {
	const char *s = position->values;
	unsigned long sum = 0;
	for (; *s; s++) sum = (sum << 4) ^ *s;
	return (unsigned int)(PRIME * sum);
}
//Returns whether two positions are equivalent (if every square matches)
bool equals(Position *position1, Position *position2) {
	return !strcmp(position1->values, position2->values);
}
//Calculates an index into the position string based on row and column
//Indices are ordered by row and then by column in the row
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
	unsigned int x, y;
} Index;
//Finds the row and column of a position string of the empty square
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
//Returns a bitmask of the possible directions the empty square could go
//Process it using up(), left(), down(), and right()
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
	assert(false);
}
//Returns whether it is possible to move the empty square up
bool up(unsigned char possibilities) {
	return (possibilities >> UP) & 1;
}
//Returns whether it is possible to move the empty square left
bool left(unsigned char possibilities) {
	return (possibilities >> LEFT) & 1;
}
//Returns whether it is possible to move the empty square down
bool down(unsigned char possibilities) {
	return (possibilities >> DOWN) & 1;
}
//Returns whether it is possible to move the empty square right
bool right(unsigned char possibilities) {
	return (possibilities >> RIGHT) & 1;
}
//Essentially strdup() but this isn't in C99
char *strdupe(char *orig) {
	char *newString = malloc(sizeof(*newString) * (strlen(orig) + 1));
	strcpy(newString, orig);
	return newString;
}
//Returns a new position with the empty square moved up
Position *moveUp(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y - 1, emptyIndex->x)];
	newPositionString[rawIndex(emptyIndex->y - 1, emptyIndex->x)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
//Returns a new position with the empty square moved left
Position *moveLeft(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y, emptyIndex->x - 1)];
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x - 1)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
//Returns a new position with the empty square moved down
Position *moveDown(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y + 1, emptyIndex->x)];
	newPositionString[rawIndex(emptyIndex->y + 1, emptyIndex->x)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
//Returns a new position with the empty square moved right
Position *moveRight(Position *position) {
	Index *emptyIndex = findEmpty(position);
	char *newPositionString = strdupe(position->values);
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x)] =
		newPositionString[rawIndex(emptyIndex->y, emptyIndex->x + 1)];
	newPositionString[rawIndex(emptyIndex->y, emptyIndex->x + 1)] = EMPTY;
	free(emptyIndex);
	return parsePosition(newPositionString);
}
//Frees a position and its wrapped string
void freePosition(Position *position) {
	free(position->values);
	free(position);
}
void printPosition(Position *position) {
	printf("%s\n", position->values);
}
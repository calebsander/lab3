/*
position.h
Caleb Sander
Functions for manipulating position structs,
representing a state of the board
*/
#ifndef __POSITION_H__
	#define __POSITION_H__
	#include <stdbool.h>

	typedef struct position Position;

	/**
	 * Sets the height and width that all Positions must have
	 * @param height the height of the board
	 * @param width the width of the board
	 */
	void setDimensions(unsigned int height, unsigned int width);
	/**
	 * Checks to make sure that position is valid and wrap it in a position struct.
	 * Will crash the program if the position is invalid.
	 * @param positionString the string to wrap
	 * @return a newly malloc'd Position with the desired value
	 */
	Position *parsePosition(char *positionString);
	/**
	 * A hash function for a Position
	 * @param position the Position to hash
	 * @return its hash
	 */
	unsigned int hashCode(Position *position);
	/**
	 * Returns whether two Positions are equivalent
	 * @param position1 the first Position
	 * @param position2 the second Position
	 * @return true iff every square matches
	 */
	bool equals(Position *position1, Position *position2);
	/**
	 * Returns a bitmask of the possible directions the empty square could go.
	 * Process it using up(), left(), down(), and right().
	 * @param position the Position in question
	 * @return the possible directions
	 */
	unsigned char possibleMoves(Position *position);
	/**
	 * Returns whether it is possible to move the empty square up
	 */
	bool up(unsigned char possibilities);
	/**
	 * Returns whether it is possible to move the empty square left
	 */
	bool left(unsigned char possibilities);
	/**
	 * Returns whether it is possible to move the empty square down
	 */
	bool down(unsigned char possibilities);
	/**
	 * Returns whether it is possible to move the empty square right
	 */
	bool right(unsigned char possibilities);
	/**
	 * Returns a new position with the empty square moved up
	 */
	Position *moveUp(Position *position);
	/**
	 * Returns a new position with the empty square moved left
	 */
	Position *moveLeft(Position *position);
	/**
	 * Returns a new position with the empty square moved down
	 */
	Position *moveDown(Position *position);
	/**
	 * Returns a new position with the empty square moved right
	 */
	Position *moveRight(Position *position);
	/**
	 * Frees a position and its (malloc'd) wrapped string
	 * @param position the Position to be freed
	 */
	void freePosition(Position *position);
	/**
	 * Prints out a position by row and then column
	 * @param position the Position to print
	 */
	void printPosition(Position *position);
	/**
	 * Essentially strdup() but this isn't in C99
	 */
	char *strdupe(char *orig);
#endif
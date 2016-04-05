/*
hashMap.h
Caleb Sander
Functions for manipulating a map of nodes of the following format:
Position* -> (Position *from, unsigned short length)
The from Position is the Position that was used to arrive at the key Position
and the length is the minimum number of steps needed to reach it.
*/
#include <stdbool.h>
#include "position.h"

typedef struct hashMap HashMap;

/**
 * Makes a new map structure with no key-value pairs
 * @return an newly malloc'd empty HashMap
 */
HashMap *makeEmptyMap();
/**
 * Searches the map for the Position.
 * This effectively uses the map as a set of keys.
 * @param map the map to be searched
 * @param key the Position to look for
 * @return true iff the Position is in the map
 */
bool contains(HashMap *map, Position *key);
/**
 * Puts a new node into the map.
 * No checking is done to ensure that the key is not already in the map.
 * @param map the map to which to add the entry
 * @param key the key with which to associate the values
 * @param from the from value of the node
 * @param length the length value of the node
 */
void put(HashMap *map, Position *key, Position *from, unsigned short length);
/**
 * Gets the from value associated with a certain key.
 * The program will crash if the key is not in the map.
 * @param map the map to query
 * @param key the key to search for
 * @return the from value associated with the key
 */
Position *getFrom(HashMap *map, Position *key);
/**
 * Gets the length value associated with a certain key.
 * @param map the map to query
 * @param key the key to search for
 * @return the length value associated with the key
 */
unsigned short getLength(HashMap *map, Position *key);
/**
 * Frees the Position wrappers and strings of all entries' keys.
 * This does not remove the entries from the map,
 * so future get requests are invalid.
 * @param the map whose entries to free
 */
void freeEntries(HashMap *map);
/**
 * Frees the map - all the nodes, the buckets array, and the map structure.
 * @param the map to free
 */
void freeMap(HashMap *map);
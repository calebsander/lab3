/*
hashMap.c
Caleb Sander
Implements the hash table that contains reached Positions
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashMap.h"

#define LOAD_FACTOR 5 //maximum number of nodes per bucket allowed
#define DEFAULT_BUCKETS 1024 //the number of buckets to allocate initially
typedef struct bucketNode BucketNode; //a node in the map
struct bucketNode {
	Position *key; //the Position
	Position *from; //the Position that this was reached from
	//The number of moves required to get to this Position (max 31 for 3x3)
	unsigned short length;
	BucketNode *next; //NULL if the last node in the bucket, otherwise the next one
};
struct hashMap { //the map
	unsigned int bucketCount; //the number of buckets currently allocated
	unsigned int elementCount; //the number of nodes in the map
	BucketNode **buckets; //an array of pointers to buckets
};

/**
 * Finds the bucket index based on the key.
 * @param value the Position being looked up
 * @param bucketCount the number of buckets in the map
 * @return the index in the buckets array where the node would/should be located
 */
unsigned int hash(Position *value, unsigned int bucketCount) {
	return hashCode(value) % bucketCount;
}
/**
 * Adds an element to the map and resizes if necessary.
 * @param map the map to which to add
 * @param key the key of the new node
 * @param from the from value of the new node
 * @param length the length value of the new node
 * @param external true if called by put(), false if adding elements
 * during a resize (if false, skips size check and doesn't change elementCount)
 */
void addElementResize(HashMap *map,
	Position *key,
	Position *from,
	unsigned short length,
	bool external); //true if using put(), false if just moving in a resize
/**
 * Allocates the specified number of buckets in the map and moves old nodes.
 * @param map the map to resize
 * @param count the new number of buckets to have
 */
void allocateBuckets(HashMap *map, unsigned int count) {
	const unsigned int oldCount = map->bucketCount;
	BucketNode **oldBuckets = map->buckets;
	map->bucketCount = count;
	map->buckets = calloc(count, sizeof(*(map->buckets)));
	if (oldBuckets) { //if the map existed before, migrate entries
		for (unsigned int oldBucket = 0; oldBucket < oldCount; oldBucket++) {
			BucketNode *node = oldBuckets[oldBucket];
			while (node) {
				addElementResize(map, node->key, node->from, node->length, false);
				BucketNode *lastNode = node;
				node = node->next;
				free(lastNode);
			}
		}
		free(oldBuckets);
	}
}
/**
 * Makes a node with the specified key and values.
 * @param key the lookup key
 * @param from the from value
 * @param length the length value
 * @param the next node in the bucket
 * @return a newly malloc'd node
 */
BucketNode *makeNode(Position *key,
	Position *from,
	unsigned short length,
	BucketNode *next) {
	BucketNode *node = malloc(sizeof(*node));
	node->key = key;
	node->from = from;
	node->length = length;
	node->next = next;
	return node;
}

HashMap *makeEmptyMap() {
	HashMap *map = malloc(sizeof(*map));
	map->buckets = NULL;
	allocateBuckets(map, DEFAULT_BUCKETS);
	map->elementCount = 0;
	return map;
}

bool contains(HashMap *map, Position *key) {
	BucketNode *bucket = map->buckets[hash(key, map->bucketCount)];
	for (BucketNode *node = bucket; node; node = node->next) {
		if (equals(node->key, key)) return true;
	}
	return false;
}
void put(HashMap *map, Position *key, Position *from, unsigned short length) {
	addElementResize(map, key, from, length, true);
}
void addElementResize(HashMap *map,
	Position *key,
	Position *from,
	unsigned short length,
	bool external) {
	//If there is a possibility of exceeding the load factor, resize
	if (external && map->elementCount + 1 > map->bucketCount * LOAD_FACTOR) {
		allocateBuckets(map, map->bucketCount << 2);
	}
	BucketNode **node = map->buckets + hash(key, map->bucketCount);
	if (external) map->elementCount++;
	*node = makeNode(key, from, length, *node);
}
Position *getFrom(HashMap *map, Position *key) {
	BucketNode *bucket = map->buckets[hash(key, map->bucketCount)];
	for (BucketNode *node = bucket; node; node = node->next) {
		if (equals(node->key, key)) return node->from;
	}
	assert(false);
	return NULL;
}
unsigned short getLength(HashMap *map, Position *key) {
	BucketNode *bucket = map->buckets[hash(key, map->bucketCount)];
	for (BucketNode *node = bucket; node; node = node->next) {
		if (equals(node->key, key)) return node->length;
	}
	assert(false);
	return 0;
}
void freeEntries(HashMap *map) {
	for (unsigned int bucket = 0; bucket < map->bucketCount; bucket++) {
		for (BucketNode *node = map->buckets[bucket]; node; node = node->next) {
			freePosition(node->key);
		}
	}
}
void freeMap(HashMap *map) {
	for (unsigned int bucket = 0; bucket < map->bucketCount; bucket++) {
		for (BucketNode *node = map->buckets[bucket]; node;) {
			BucketNode *lastNode = node;
			node = node->next;
			free(lastNode);
		}
	}
	free(map->buckets);
	free(map);
}
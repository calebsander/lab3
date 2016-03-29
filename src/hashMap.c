/*
hashMap.c
Caleb Sander
Implements the hash table that contains reached positions
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
	Position *key; //the position
	Position *from; //the position that this was reached from
	//The number of moves required to get to this position (max 31)
	unsigned short length;
	BucketNode *next; //NULL if the last node in the bucket, otherwise the next one
};
struct hashMap { //the map
	unsigned int bucketCount;
	unsigned int elementCount;
	BucketNode **buckets;
};

//Finds the bucket index based on the key
unsigned int hash(Position *value, unsigned int bucketCount) {
	return hashCode(value) % bucketCount;
}
//Add an element to the map and check if it needs to be resized
void addElementResize(HashMap *map,
	Position *key,
	Position *from,
	unsigned short length,
	bool external); //true if using put(), false if just moving in a resize
//Allocate the specified number of buckets in the map, move existing nodes
//To new buckets
void allocateBuckets(HashMap *map, unsigned int count) {
	const unsigned int oldCount = map->bucketCount;
	BucketNode **oldBuckets = map->buckets;
	map->bucketCount = count;
	map->buckets = calloc(count, sizeof(*(map->buckets)));
	if (oldBuckets) { //if the map hadn't yet been created
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
//Make a node with the specified key and values
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

//Create an empty map
HashMap *makeEmptyMap() {
	HashMap *map = malloc(sizeof(*map));
	map->buckets = NULL;
	allocateBuckets(map, DEFAULT_BUCKETS);
	map->elementCount = 0;
	return map;
}

//Returns whether a node with the specified key is in the map
bool contains(HashMap *map, Position *key) {
	BucketNode *bucket = map->buckets[hash(key, map->bucketCount)];
	for (BucketNode *node = bucket; node; node = node->next) {
		if (equals(node->key, key)) return true;
	}
	return false;
}
//Put a new node into the map
//No checking is done to ensure that the key is not alreay in the map
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
//Get the position that led to the specified position
Position *getFrom(HashMap *map, Position *key) {
	BucketNode *bucket = map->buckets[hash(key, map->bucketCount)];
	for (BucketNode *node = bucket; node; node = node->next) {
		if (equals(node->key, key)) return node->from;
	}
	assert(false);
	return NULL;
}
//Get the minimum length of the path to the specified position
unsigned short getLength(HashMap *map, Position *key) {
	BucketNode *bucket = map->buckets[hash(key, map->bucketCount)];
	for (BucketNode *node = bucket; node; node = node->next) {
		if (equals(node->key, key)) return node->length;
	}
	assert(false);
	return 0;
}
//Free the position strings and wrappers of all entries
void freeEntries(HashMap *map) {
	for (unsigned int bucket = 0; bucket < map->bucketCount; bucket++) {
		for (BucketNode *node = map->buckets[bucket]; node; node = node->next) {
			freePosition(node->key);
		}
	}
}
//Free the map - all the nodes, the buckets array, and the map structure
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
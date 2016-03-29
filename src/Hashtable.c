// Hashtable.c
// Implements a hashtable for use in the Nine project.
// (Your name and date here, as per style guide)

// You may use this hash function in your implementation of a Hashtable.
// Returns a hashed value for the given string in [0, size)
static int hash(char *s, long size) {
    unsigned long sum;
    const long prime = 3141592661;
    for (sum = 0; *s; s++) {
        sum = (sum << 4) ^ *s;
    }
    return prime * sum % size;
}


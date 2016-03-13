// You may use this hash function in your Hashtable.c implementation.
// Return hash of string S into [0, SIZE)
static int hash(char *s, long size) {
	unsigned long sum;
	const long prime = 3141592661;

	for (sum = 0; *s; s++) {
		sum = (sum << 4) ^ *s;
	}

	return prime * sum % size;
}

#Lab 3: The Nine Puzzle
###Due before class, Friday, April 8

Reminder: do not under any circumstances copy another student’s code. After discussing the assignment with another student, please engage in a full hour of mind-numbing activity before you work on it again. If a piece of your code is inspired by another student (either conversation, or looking at their code), you must give them credit in a comment. If a significant portion of the lab is credited to another student, your grade will reflect this. Since it is very hard to "un-see" a working algorithm or way of organizing code, I strongly encourage you to attempt a complete solution on your own without looking at your classmates' solutions.

###Description of the problem

The nine puzzle is a square 3x3 tray in which we place 8 square 1x1 tiles numbered from 1 to 8. The remaining space is empty, so an adjacent tile can slide into that space, leaving its former location empty. For example:

    +---+---+---+       +---+---+---+    +---+---+---+    +---+---+---+
    | 1 | 2 | 3 |       | 1 | 2 | 3 |    | 1 | - | 3 |    | 1 | 2 | 3 |
    +---+---+---+       +---+---+---+    +---+---+---+    +---+---+---+
    | 4 | - | 5 |  ==>  | - | 4 | 5 | or | 4 | 2 | 5 | or | 4 | 5 | - | or ...
    +---+---+---+       +---+---+---+    +---+---+---+    +---+---+---+
    | 6 | 7 | 8 |       | 6 | 7 | 8 |    | 6 | 7 | 8 |    | 6 | 7 | 8 |
    +---+---+---+       +---+---+---+    +---+---+---+    +---+---+---+

The hyphen (-) denotes the empty space.

Given an initial configuration and a goal configuration, for example, 

            +---+---+---+           +---+---+---+
		    | 2 | 3 | 6 |           | 1 | 2 | 3 |
		    +---+---+---+           +---+---+---+
		    | 1 | 5 | 8 |    and    | 4 | 5 | 6 |
		    +---+---+---+           +---+---+---+
		    | - | 4 | 7 |           | 7 | 8 | - |
		    +---+---+---+           +---+---+---+

the object is to move from the former to the latter by sliding the tiles around.

###Goal

Write a program

    ./Nine [HEIGHT WIDTH] MAXLENGTH INITIAL GOAL

that solves the nine puzzle given an INITIAL position (236158-47 above) and a GOAL position (12345678- above). To make the task more challenging, Nine uses breadth-first search (see note below) to find some SHORTEST sequence of at most MAXLENGTH moves (if one exists) and prints out the complete sequence of positions separated by newlines; e.g.,

	$ ./Nine 100 236158-47 12345678-
	236158-47
	2361584-7
	23615847-
	23615-478
	23-156478
	2-3156478
	-23156478
	123-56478
	123456-78
	1234567-8
	12345678-

If no such move sequence exists, then nothing is printed, e.g.

	$ ./Nine 100 12345678- 12345687-

The label on each tile can be any single printing character (other than -, which denotes the empty square), but the labels need not be unique. The optional HEIGHT and WIDTH (whose default values are 3 and 3) allow other rectangular puzzles to be solved as well. Note that the square brackets in the command above indicate optional-ness; you wouldn’t actually include square brackets in the input.

###Algorithm

Your program will use breadth-first traversal/search using a hashtable of (POSITION, REACHED-FROM, LENGTH) triples and a queue of positions to visit. That is, you will create a hashtable (dictionary), where the keys are POSITIONs, and the values are structs containing a REACHED-FROM field (containing the position from which you reached POSITION), and a LENGTH field (containing the length of the path so far). 

Use the following algorithm:
	
	create a hashtable and a queue
	add the triple (GOAL, NULL, 0) to the hashtable
	add the GOAL position to the queue
	while the queue is not empty
		remove the position P at the head of the queue
		if P is the INITIAL position
			print the sequence of moves and exit
		let L be the length associated with P in the hash table
		if (L < MAXLENGTH)
			for each position P’ reachable from P by moving one tile
				if P’ is not in the hashtable
					add (P’, P, L + 1) to the hashtable
					add P’ to the queue
				endif
			endfor
		endif
	endwhile
	<IF YOU REACH HERE, GOAL NOT REACHABLE FROM INITIAL>

###About that hashtable

The hash table should be implemented as a separate module (i.e. in a separate file, with a header file specifying the interface) so that the code could be used in another application.  It may use either chaining or linear probing.

One way to modularize the hash table is to do what we’ve done in most of our data structures, writing functions to create, insert, and search, and possibly a function to destroy (i.e., free). In this case the header file should declare the Hash object as a pointer to some struct whose fields are undeclared. (typedef struct hash \*Hash). The names and types of the fields, which hold all information about the hash table, are only declared in the file that implements the module.

Another way is to create a module that implements one hash table (instead of allowing yout o create many), with functions to insert and search, and possibly a function to initialize. Static global variables hold all of the information about the hash table.

Both organizations ensure that the calling program cannot access the contents of the hash table other than through the functions defined in the header file.

Initially the hash table should have M = 1024 entries.  Let N be the number of triples in the hash table.  Whenever the load average L = N/M exceeds 5 (for chaining) or 3/4 (for linear probing), you should create a new hash table of four times the size (i.e., M' = 4M so that L' = N/M' is one-fourth as large); insert all triples in the old hash table into the new hash table; and delete the old hash table.

Aside: If you want to write a non-resizing version first, so that you can test your other code without worrying about resizing, make sure to start with a big hashtable if you want to solve most instances of the nine puzzle. The initial size, if you are not using auto-resizing, should be 300,000 entries (for chaining) and 2,000,000 (for linear probing).

###Hash function

Your hash function should depend on all of the bits in the representation yet be general enough to handle changes in the symbolic constants that define the size of the tray (WIDTH and HEIGHT).

Reducing a string to an unsigned integer (which you can then mod down to the correct size) can be done with a combination of shifts/multiplies (to move bits to the left) and adds/xors (to introduce new characters).  Good performance dictates that this integer not be invariant under permutations or depend on a subset of the characters (why?).  Together with the fact that it should have as many as seven digits unless the number of squares in the tray is very small (why?), this constrains the sizes of the shifts/multipliers. 

Here is a hash function for strings that you may use (also available in src/hashFunction.c):
```c
	// Return hash of string S into [0,SIZE)
	static int hash (char *s, long size)
	{
	    unsigned long sum;
	    const long prime = 3141592661;

	    for (sum = 0; *s; s++) {
		    sum = (sum<<4) ^ *s;
        }
	    return prime * sum % size;
	}
```
###Memory-efficiency

A 3x3 problem with no solution will lead to a hashtable with 181,440 entries when your algorithm terminates. You should make sure that each entry uses no more than about 100 bytes of memory (and ideally less) so that malloc(...) can always successfully return a sufficiently large memory block to you!

###Error reporting

Any invalid input should be detected by your program. You can then exit with a one-line error message.


###Log

As always, make sure to update your log as you work on the project, and to include a discussion of any difficulties you encounter along the way.

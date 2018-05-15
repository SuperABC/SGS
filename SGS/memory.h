#ifndef MEMORY_H
#define MEMORY_H
#include <iostream>

struct Block {
	void *cont[256];
	unsigned char offset;
	Block *next = NULL;
};

class SgsMemory {
private:
	Block *localMem;
	Block *globeMem;
public:
	SgsMemory();
	~SgsMemory();

	void *alloc(unsigned int size, bool local = true);
	void *alloc(void *ptr, bool local);
	void free(void *ptr);
	void clear();
};


#endif
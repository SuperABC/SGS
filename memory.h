#ifndef MEMORY_H
#define MEMORY_H
#include <iostream>

struct Block {
	void *cont[256];
	unsigned char offset;
	Block *next = NULL;
};

class Memory {
private:
	Block *localMem;
	Block *globeMem;
public:
	Memory();
	~Memory();

	void *alloc(unsigned int size, bool local = true);
	void free(void *ptr);
	void clear();
};


#endif
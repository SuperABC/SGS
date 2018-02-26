#include "memory.h"

Memory::Memory() {
	localMem = new Block();
	globeMem = new Block();
}
Memory::~Memory() {
	Block *tmp;
	tmp = localMem->next;
	while (localMem) {
		for (int i = 0; i < localMem->offset; i++)delete localMem->cont[i];
		delete localMem;
		localMem = tmp;
		if (tmp)tmp = tmp->next;
	}
	tmp = globeMem->next;
	while (globeMem) {
		for (int i = 0; i < globeMem->offset; i++)delete globeMem->cont[i];
		delete globeMem;
		globeMem = tmp;
		if (tmp)tmp = tmp->next;
	}
}

void *Memory::alloc(unsigned int size, bool local) {
	Block *tmp;
	if(local)tmp = localMem;
	else tmp = globeMem;

	while (tmp->offset == 255) {
		if(!tmp->next)
			tmp->next = new Block();
		tmp = tmp->next;
	}

	tmp->cont[tmp->offset] = new char[size];
	return tmp->cont[tmp->offset++];
}
void *Memory::alloc(void *ptr, bool local) {
	Block *tmp;
	if (local)tmp = localMem;
	else tmp = globeMem;

	while (tmp->offset == 255) {
		if (!tmp->next)
			tmp->next = new Block();
		tmp = tmp->next;
	}

	tmp->cont[tmp->offset] = ptr;
	return tmp->cont[tmp->offset++];
}
void Memory::clear() {
	Block *tmp;
	tmp = localMem->next;
	while (localMem) {
		for (int i = 0; i < localMem->offset; i++)delete localMem->cont[i];
		delete localMem;
		localMem = tmp;
		if (tmp)tmp = tmp->next;
	}
	localMem = new Block();
}
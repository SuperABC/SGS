#include "machine.h"

Machine::Machine() {

}
Machine::~Machine() {
	
}

Machine *Machine::input(stateSeq seq,
	vector<classType> classType, vector<funcNode> func,
	vector<varNode> var) {
	content = seq;
	globeClassType = classType;
	globeFunc = func;
	globeVar = var;
	return this;
}
void Machine::execute() {

}
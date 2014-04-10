#ifndef BACKENDMEM_H
#define BACKENDMEM_H

#include "newsgroup.h"
#include <string>
#include <vector>
#include <map>

class BackEnd : public BackEnd {
public:
private:
	std::map<int, NewsGroup> database;
	int ng_count;
};

#endif

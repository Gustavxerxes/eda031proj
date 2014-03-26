#include <string>
#include "article.h"
#include "newsgroup.h"

int main(int argc, char const *argv[])
{
	int index;
	std::std::map<int, Newsgroup> newsMap; 
	newsMap[1] = Newsgroup(1, "test");

	Newsgroup n = newsMap.find(1); 
	std::cout << "Name: " << n.getName() << std::endl();



	return 0;
}
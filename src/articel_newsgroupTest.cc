#include "article.h"
#include "newsgroup.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

int main(int argc, char const *argv[])
{
	int index;
	std::map<int, NewsGroup> newsMap; 
	std::string s = ("test");
	NewsGroup n = NewsGroup(1, s);
	newsMap.emplace(1,n);

	auto n2 = newsMap.find(1); 
	std::cout << "Name: " << n2->second.getTitle() << std::endl;



	return 0;
}
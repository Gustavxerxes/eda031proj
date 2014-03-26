#include "article.h"
#include "newsgroup.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char const *argv[])
{

	int index;
	std::map<int, NewsGroup> newsMap; 
	std::string s = ("test");
	NewsGroup n = NewsGroup(1, s);
	newsMap.insert( std::pair<int, NewsGroup>(1,n) );
	//newsMap.emplace(1,n);

	auto n2 = newsMap.find(1); 
	std::cout << n2->first << "Name: " << n2->second.getTitle() << std::endl;
	n2->second.addArticle(string("title"),string("author"),string("text") );

	std::cout << "Article 1 har id : " << n2->second.getArticle(1).getId() << std::endl;
	




	return 0;
}
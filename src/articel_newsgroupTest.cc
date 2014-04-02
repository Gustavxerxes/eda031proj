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
	std::cout << "id" << n2->first << "Name: " << n2->second.getTitle() << std::endl;
	
	n2->second.addArticle(string("title"),string("author"),string("text") );

	Article art;
	(n2->second).fetchArticle(1,art);

	std::cout << "Article 1 har id : " << art.getId() << ", titel: " << art.getTitle() << ", author: " << art.getAuthor() << ", text: " << art.getArticle() << std::endl;	
	std::vector< std::pair<int, std::string> > arts;
	if( (n2->second).listArticles(arts) ) cout << "gick att lista";
	if( (n2->second).removeArticle(1) ) cout << "gick att ta bort";

	




	return 0;
}
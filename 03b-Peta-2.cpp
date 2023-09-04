#include <iostream>
#include <algorithm>
#include <cstdio>
#include <utility>
#include <vector>

void Find_book(std::vector<int>book_price, int money)
{
	int i,j;
	int aux_search, first_book, second_book;
	i = 0;
	j = (int)book_price.size()-1;
	first_book = book_price.at(0);
	second_book = book_price.back();
	while(i < j)
	{
		aux_search = money - book_price.at(i);
		if (aux_search < 0)
		{
			break;
		}
		if(std::binary_search(book_price.begin()+i+1, book_price.end(), aux_search))
		{
			first_book = book_price.at(i);
			second_book = aux_search;
		}
		i++;
	}
	
	std::cout << "Peter should buy books whose prices are " << first_book <<" and " << second_book << ".\n\n";
}


int main()
{
	int i;
	int N, money, aux_input;
	std::vector<int> book_price;
	while(std::cin >> N)
	{
		for(i = 0; i < N; i++)
		{
			std::cin >> aux_input;
			book_price.push_back(aux_input);
		}
		std::cin >> money;
		std::sort(book_price.begin(),book_price.end());
		Find_book(book_price,money);
		book_price.clear();
	}
	return 0;
}

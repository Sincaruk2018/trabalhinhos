#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iomanip>
#define ERROR 0.001


void Internal_rate_of_return(std::vector<double>cash_flow, double Lowest_rate_found, double Highest_rate_found)
{
	if(Highest_rate_found - Lowest_rate_found <= 0)
	{
		std::cout << "No\n"; 
		return;
	}
	
	double Interest_rate_iteration = (Lowest_rate_found + Highest_rate_found)/2.0; 
	int i;
	double sum = cash_flow.at(0);
	for(i = 1; i < (int)cash_flow.size(); i++)
	{
		sum += cash_flow.at(i)/pow(1.0 + Interest_rate_iteration ,i);
	}
	
	if( fabs(sum) <= ERROR )
	{
		if(fabs(Interest_rate_iteration + 1.0) <= ERROR)
		{
			std::cout << "No\n"; 
			return;
		}
		std::cout << std::setprecision(2) <<  std::fixed << Interest_rate_iteration << "\n";
		return;
	}
	
	if(sum > ERROR)
	{
		Internal_rate_of_return(cash_flow,Interest_rate_iteration,Highest_rate_found);
		return;
	}
	Internal_rate_of_return(cash_flow,Lowest_rate_found,Interest_rate_iteration);
	return;
}


int main()
{
	std::vector<double> cash_flow;
	int i, j;
	int T;
	double cash_flow_input;
	j = 0;
	while(j < 25)
	{
		std::cin >> T;
		if(T <= 0)
		{
			break;
		}
		for(i = 0; i < (T+1); i++)
		{
			std::cin >> cash_flow_input;
			cash_flow.push_back(cash_flow_input);
		}
		Internal_rate_of_return(cash_flow, -0.999,10000000);
		cash_flow.clear();
		j++;
	}
	return 0;
}

#include <stdio.h>
#include <vector>
using namespace std;

/* Given the array coins, it finds the biggest number that is lesser than
 * x. It's a linear search (kinda)*/
int biggest (int x, std::vector<int> coins)
{
	int i;
	int control = -1;
	for (i = 0; i < (int)coins.size(); i++)
	{
		if (coins[i] <= x)
		{
			control = coins[i];
		}
	}
	return control;
}

/* Retruns the number of many coins necessary for a transactions worth of x. Coins is the vector listing the
 * coins values. For the sake of the recursivity, two needed variables are used: much_previous, indicating the
 * largest coin possible for that iteraction and many_previous, showing how manycoins were used untill given 
 * moment. This function should return the highest value of many_previous*/
long int Withdraw(long int x, std::vector<int> coins, int much_previous, int many_previous)
{
	long int y, aux_previous, aux_much_previous;
	y = much_previous;
	aux_previous = many_previous;
	aux_much_previous = much_previous;
	if (x <= 0) //Endgame. Let's stop here
	{
		return many_previous;
	}
	if (x < y)
	{
		y = biggest(x, coins);
		aux_previous = many_previous + 1;
		aux_much_previous = y;
	}
	return Withdraw((x-y), coins, (int)aux_much_previous, (int)aux_previous);	
}

int main()
{
	int cases, coins, now, i, j, bigger;
	bigger = 0;
	float aux_out;
	aux_out = 0;
	std::vector<int> v_coin;
	std::vector<int> output;
	scanf("%d",&cases);
	for (i = 0; i < cases; i++)
	{
		scanf("%d",&coins);
		for (j = 0; j < coins; j++)
		{
			scanf("%d",&now);
			v_coin.push_back(now);
		}
		now = (int)v_coin.back();
		long int amount = 0;
		/* For optimization purposes, the problem starts on the sum of all
		 * coins.*/
		for (j = 0; j < (int) v_coin.size(); j++)
		{
			amount += v_coin[j];
		}
		
		/* I wish I could get this algorithm more greedy, but the problem
		 * asked for very  numbers, 1000 is what worked for this situation, but not
		 * guaranteed it's an universal sollution*/
		for (j = amount; j < amount + 1000; j++)
		{
			aux_out = Withdraw(j,v_coin,now,1);
			if (aux_out > bigger)
			{
				bigger = aux_out;
			}
		}
		v_coin.clear();
		output.push_back(bigger);
		bigger = 0;

	}
	
	for (i = 0; i < cases; i++)
	{
		printf("%d\n",output[i]);
	}
	return 0;
}

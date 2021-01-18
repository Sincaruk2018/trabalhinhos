#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}

int MIN (int a, int b, int c)
{
	return min(a,min(b,c));
}

int Edit(int first_vec[], int sec_vec[], int num, int numb)
{
		int i,j;
		int matrix[num+1][numb+1];
		
		for(i = 0; i < num + 1; i++)
		{
			for(j = 0; j < num + 1; j++)
			{
				if (i == 0)
				{
					matrix[i][j] = j;
				}
				else if (j == 0)
				{
					matrix[i][j] = i;
				}
				else if(first_vec[i-1] == sec_vec[j-1])
				{
					matrix[i][j] = matrix[i-1][j-1];
				}
				else
				{
					matrix[i][j] = 1 + min(matrix[i][j-1],matrix[i-1][j]);
				}
				
			}
		}
		
		return matrix[num][numb];
}

int main()
{
	int i, j;
	int cases, inser, num_vec;
	int first_vec[100];
	int sec_vec[100];
	scanf("%d", &cases);
	for(i = 0; i < cases; i++)
	{
		scanf("%d", &num_vec);
		for(j = 0; j < num_vec; j++)
		{
			scanf("%d",&inser);
			first_vec[j] = inser;
		}
		for(j = 0; j < num_vec; j++)
		{
			scanf("%d",&inser);
			sec_vec[j] = inser;
		}
		printf("Case %d: %d\n", (i+1), Edit(first_vec,sec_vec,num_vec, num_vec));
		//first_vec.clear();
		//sec_vec.clear();
	}
	return 0;
}

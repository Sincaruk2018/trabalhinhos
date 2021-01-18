/* Made by: Luiz Gustavo Sincaruk Vieira
 * USP: 10374622
 * A.K.A: Sincaruk, SIncaruk2018, Russel, Coldfox, Bitter_Caracara, etc...
 * */
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

/* Pre-processing for the KMP algorithm. Padrao is the pattern string
 * size is padrao's size. *vec is a pointer to the auxiliar vector. This
 * function will generate a parameter for KSearch */
void Processing(char padrao[], int size, vector<int> *vec)
{
	int i,j;
	i = 0;
	j = -1;
	vec->push_back(j);
	while(i < size)// 'i' will awlays progride
	{
		while(padrao[j] != padrao[i] && j >= 0)// j won't progride untill a similar letter appears
		{
			j = vec->at(j);
		}
		i++;
		j++;
		vec->push_back(j);
	}
}

/* string1 is the string to be analyzed, padrao is the pattern to be recognized.
 * str1 size is the string1 size, str2size is padrao size. Vec is a vector with 
 * necessary info for checking the matches.
 */
void KSearch(char string1[],char padrao[], int str1size, int str2size, vector<int> vec)
{
	int i, j;
	i = 0;
	j = 0;
	while(i < str1size)
	{
		while(string1[i] != padrao[j] && j >= 0)
		{
			j = vec[j];
		}
		j++;
		i++;
		if (j == str2size - 1 && i < str1size)
		{
			printf("O padrao foi encontrado na posicao %d\n", (i-j)); // Pattern found at fosition i-j
		}
	}
}


int main(int argc, char **argv)
{	
	char string1[260000], padrao[1000];
	vector<int> vec;
	
	fgets(string1,260000,stdin);
	fgets(padrao,1000,stdin);
	string1[(int)strlen(string1)-1] ='\0'; // Suggested by teacher. Implemented. Also, you can't trust on those compilers....
	padrao[(int)strlen(padrao)-1] ='\0';
	Processing(padrao, (int)strlen(padrao), &vec);
	KSearch(string1,padrao,(int)strlen(string1),(int)strlen(padrao),vec);
	return 0;
}


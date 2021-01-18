/* 
 * Ratina Mase problem
 * Coded by Luiz Gustavo Sincaruk Vieira (A.K.A Russel)
 * 
 * This programs deals with backtracong concepts
 * */

#include <stdio.h>
#include <iostream>
#include <vector>

#define MAX 100 // Just to make the 2D array working properly

using namespace std;

/* Returns the number of the possible solutions for the labyrinth, it uses
 * brute force by checking every possible space nearby and returning it's possible
 * condition: a path, an obstacle or the end of the maze. THe 2D array is
 * the maze given by the proble, pos_actual_x and pos_actual_y are the current
 * x and y positions of said matrix, tammax is the size of the matrix */
int Result_Maze(int maze[][MAX],int pos_actual_x, int pos_actual_y, int tammax)
{
	int i,j;
	int control = 0;
	int mirror[tammax][MAX];
	/* Makes a coopy from maze, important to keep control and not altering
	 * the original 2D array*/
	for(i = 0; i < tammax; i++)
	{
		for(j = 0; j < MAX; j++)
		{
			mirror[i][j] = maze[i][j];
		}
	}
	/* If it is an obstacle or an invalid position, it returns 0*/
	if(maze[pos_actual_x][pos_actual_y] != 0 || pos_actual_x < 0 || pos_actual_y < 0 || pos_actual_x >= tammax || pos_actual_y > tammax)
	{
		return 0;
	}
	/* Returns 1 when the goal is achieved*/
	else if (pos_actual_x == tammax -1 && pos_actual_y == tammax -1)
	{
		return 1;
	}
	/* It's a path, search again in it's nearby spaces*/
	else
	{
		mirror[pos_actual_x][pos_actual_y] = 1;
		control += (Result_Maze(mirror,pos_actual_x + 1, pos_actual_y,tammax) + Result_Maze(mirror,pos_actual_x, pos_actual_y + 1,tammax) + Result_Maze(mirror,pos_actual_x - 1, pos_actual_y , tammax) + Result_Maze(mirror,pos_actual_x, pos_actual_y - 1,tammax)); // Recursive function for every adjacent position
		return control;
	}
}

int main()
{
	int n,i,j, result;
	/* Simple scanning*/
	cin >> n;
	int maze[n][MAX];
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			cin >> maze[i][j]; 
		}
	}
	result = Result_Maze(maze,0,0,n); // Does the required job
	cout << result;
	return 0;
}

/*
    Portuguese - As variáveis e os comentários foram escritos em inglês, pois tenho
    interesse em colocar esse projeto no github e ajudar pessoas do mundo inteiro.

    English - Hello there, this section is under construction. This program is made to solve UVA 1203
    a problem in competitive programming.

    Made by: Luiz Gustavo Sincaruk Vieira (Russel).
    Email: sincaruk@usp.br
    Email: sincarukl@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <algorithm>
using namespace std;

/* Recursive version of a Maximum Common Divisor of two variables "a" and "b" */
int mdc(int a, int b)
{
    int maximus, minimus;
    if (a >= b)
    {
        maximus = a;
        minimus = b;
    }
    else
    {
        minimus = a;
        maximus = b;
    }

    if (maximus%minimus == 0)
    {
        return minimus;
    }
    else
    {
        return mdc(minimus,(maximus%minimus));
    }
}

/* Maximum Common Divisor of an entire deque. It compares the MCD of the first result with the next until the end of the deque*/
int mdc_multiplo(deque<int> deck)
{
    sort(deck.begin(),deck.end());
    deque<int>::iterator it;
    int aux = *deck.begin();
    for (it = deck.begin()++; it < deck.end(); it++)
    {
        aux = mdc(aux,*it);
    }
    return aux;
}

int main()
{
    char input_a[200];
    char input_b[200];
    int number_a, time_a, loop_aux;
    /* For simplicity, two separated deques were used instead of an array of deques*/
    deque <int> time_b;
    deque <int> number_b;
    deque <int> aux_print;
    while (1)
    {
        fgets(input_a,200,stdin);//Gets the whole line. Check if there won't be any problems with the judge's compiler.
        sscanf(input_a,"%s%d%d",input_b,&number_a,&time_a);//Separates the line into commands
        if (strcmp(input_b,"Register") == 0)
        {
            number_b.push_back(number_a);
            time_b.push_back(time_a);
        }
        else if (strcmp(input_b,"#") == 0)
        {
            scanf("%d",&loop_aux);
            break;
        }
    }
    int i = 0;
    int j = 0;
    while (j < loop_aux)
    {
        /* For efficiency purposes, this program will only count the times a collision is possible.
            For each loop iteration, the program will make a linear search, putting i++ would make
            it denecessarily  slow. Works better in numbers that aren't relative primes.
         */
        i += mdc_multiplo(time_b);
        deque<int>::iterator it; //To run the numbers
        deque<int>::iterator she; // Just to print the correspondent ID
        deque<int>::iterator he;
        for (it = time_b.begin(), she = number_b.begin(); it < time_b.end(); it++, she++)
        {
            if (i % *it == 0)
            {
                aux_print.push_back(*she);
                //printf("%d\n",*she);
                j++;
                if (j >= loop_aux)
                {
                    sort(aux_print.begin(),aux_print.end());
                    for (he = aux_print.begin(); he < aux_print.end(); he++)
                    {
                        printf("%d\n", *he);
                    }
                    aux_print.clear();
                    break;
                }
            }
        }
            /* I'll fix that repeated code later*/
        sort(aux_print.begin(), aux_print.end());
        for (he = aux_print.begin(); he < aux_print.end(); he++)
        {
            printf("%d\n", *he);
        }
        aux_print.clear();
    }
    return 0;
}

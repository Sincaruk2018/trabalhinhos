#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <algorithm>
using namespace std;

int check_queue(deque<int>tobequeue,deque<int>comands)
{
    if (comands.size() == 0 || tobequeue.size() == 0)
    {
        return 0;
    }
    deque<int> aux;
    deque<int>::iterator it;
    deque<int>::iterator she;
    deque<int>::iterator he;
    int numberofone = 0, numberoftwo = 0;
    for (he = comands.begin(); he < comands.end(); he++)
    {
        if (*he == 1)
        {
            numberofone++;
        }
        if (*he == 2)
        {
            numberoftwo++;
        }
    }
    if (numberoftwo > numberofone)
    {
        return 0;
    }
    for (it = comands.begin(), she  = tobequeue.begin(); it < comands.end(); it++, she++)
    {
        if (*it == 1)
        {
            aux.push_back(*she);
        }
        if (*it == 2)
        {
            if (*aux.begin() != *she)
            {
                return 0;
            }
            aux.pop_front();
        }
    }
    return 1;
}

int check_stack(deque<int>tobestack, deque<int> comands)
{
    if (comands.size() == 0 || tobestack.size() == 0)
    {
        return 0;
    }
    deque<int> aux;
    deque<int>::iterator it;
    deque<int>::iterator she;
    deque<int>::iterator he;
    int numberofone = 0, numberoftwo = 0;
    for (he = comands.begin(); he < comands.end(); he++)
    {
        if (*he == 1)
        {
            numberofone++;
        }
        if (*he == 2)
        {
            numberoftwo++;
        }
    }
    if (numberoftwo > numberofone)
    {
        return 0;
    }
    for (it = comands.begin(), she  = tobestack.begin(); it < comands.end(); it++, she++)
    {
        if (*it == 1)
        {
            aux.push_front(*she);

        }
        if (*it == 2)
        {
            if (*aux.begin() != *she)
            {
                return 0;
            }
            aux.pop_front();
        }
    }
    return 1;
}

int check_priotityqueue(deque<int>tobepriority, deque<int> comands)
{
 if (comands.size() == 0 || tobepriority.size() == 0)
    {
        return 0;
    }
    deque<int> aux;
    deque<int>::iterator it;
    deque<int>::iterator she;
    deque<int>::iterator he;
    int numberofone = 0, numberoftwo = 0;
    for (he = comands.begin(); he < comands.end(); he++)
    {
        if (*he == 1)
        {
            numberofone++;
        }
        if (*he == 2)
        {
            numberoftwo++;
        }
    }
    if (numberoftwo > numberofone)
    {
        return 0;
    }
    for (it = comands.begin(), she  = tobepriority.begin(); it < comands.end(); it++, she++)
    {
        if (*it == 1)
        {
            aux.push_back(*she);
            sort(aux.begin(),aux.end());
        }
        if (*it == 2)
        {
            if (aux.back() != *she)
            {
                return 0;
            }
            aux.pop_back();
        }
    }
    return 1;
}

int main ()
{
    int entrada, comand, iten, i;
    int indice = 0;
    deque<int>::iterator it;
    deque<int>comando[200];
    deque<int>item[200];
    deque<int>outputs;
    while(scanf("%d",&entrada) != EOF)
    {
        for (i = 0; i < entrada; i++)
        {
            scanf("%d",&comand);
            scanf("%d",&iten);
            comando[indice].push_back(comand);
            item[indice].push_back(iten);
        }
        int teste = (check_queue(item[indice],comando[indice]) * 100) + (check_stack(item[indice],comando[indice]) * 10) + check_priotityqueue(item[indice],comando[indice]);
        outputs.push_back(teste);
        indice++;
    }
    for (it = outputs.begin(); it < outputs.end(); it++)
    {
        if (*it == 0)
        {
            printf("impossible\n");
        }
        else if (*it == 1)
        {
            printf("priority queue\n");
        }
        else if (*it == 10)
        {
            printf("stack\n");
        }
        else if (*it == 100)
        {
            printf("queue\n");
        }
        else if (*it == 111 || *it == 101 || *it == 110 || *it == 11)
        {
            printf("not sure\n");
        }
    }
    return 0;
}

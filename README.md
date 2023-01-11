#include <iostream>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

const string gene="NSEV";

int a[15][15];
int dim=5;
int pasi=2*dim;
int nr_populatie=100;

int rnd(int a,int b)
{
    return a+(rand()%(b-a+1));
}
void initializare_matrice()
{
    dim++;
    for(int i=0; i<=dim; i++)
        a[i][0]=a[0][i]=a[dim][i]=a[i][dim]=-1;
    a[1][1]=2;
    dim--;
    for(int i=1; i<=dim; i++)
    {
        int x=rnd(2,dim*dim);
        if(a[x%dim==0?x/dim:x/dim+1][x%dim==0?dim:x%dim]==1)
            i--;
        else
            a[x%dim==0?x/dim:x/dim+1][x%dim==0?dim:x%dim]=1;
    }


}

void afis_matrice()
{
    for(int i=0; i<=dim+1; i++)
    {
        for(int j=0; j<=dim+1; j++)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}

struct individ
{
    char adn[31];
    int fitness;

};
int valid(individ x)
{
    int b[15][15]= {0};
    int i=1,j=1;
    int p=pasi;
    while(p)
    {
        if(x.adn[2*dim-p]=='N')
            i--;
        if(x.adn[2*dim-p]=='S')
            i++;
        if(x.adn[2*dim-p]=='E')
            j++;
        if(x.adn[2*dim-p]=='V')
            j--;
        if(a[i][j]==-1||b[i][j]==2)
            return 0;
        else
            b[i][j]=2;
        p--;
    }
    return 1;
}
int fit(individ x)
{
    int i=1,j=1,mancare=0;
    for(int k=0; k<pasi; k++)
    {
        if(x.adn[k]=='N')
            i--;
        if(x.adn[k]=='S')
            i++;
        if(x.adn[k]=='E')
            j++;
        if(x.adn[k]=='V')
            j--;
        if(a[i][j]==1)
            mancare++;
    }
    return mancare;
}
void generare_parinti(individ populatie[10001])
{

    for(int x=0; x<nr_populatie; x++)
    {
        individ test;
        for(int i=0; i<pasi; i++)
        {
            int k=rnd(1,4);
            if(k==1)
                test.adn[i]='N';
            else if(k==2)
                test.adn[i]='S';
            else if(k==3)
                test.adn[i]='E';
            else if(k==4)
                test.adn[i]='V';


        }

        if(valid(test))
        {
            populatie[x]=test;
            populatie[x].fitness=fit(populatie[x]);
            populatie[x].adn[2*dim]='\0';

        }

        else
            x--;
    }

}
void afisare_parinti(individ populatie[10001])
{
    for(int i=0; i<nr_populatie; i++)
    {

        cout<<i<<" "<<populatie[i].adn<<" fitness:"<<populatie[i].fitness<<endl;
    }
}

int main()
{
    srand(time(NULL));

    individ populatie[10001];
    //int maxpop=10000;


    initializare_matrice();
    afis_matrice();


    generare_parinti(populatie);
    afisare_parinti(populatie);
    // cout<<endl<<srand(time(0));

}

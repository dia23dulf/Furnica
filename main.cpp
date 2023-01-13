#include <iostream>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

const string gene="NSEV"; ///genele individului

int a[100][100]; //mediul
int dim=6; //dimensiunea mediului
int pasi=2*dim+(dim+1)/2; //numarul de pasi
#define nr_populatie 1000 ///nr max de indivizi dintr-o populatie
#define maxGen 2000
#define procent_mutatie 5

struct individ
{
    char adn[1000]=""; //aici se va retine traseul parcurs
    int fitness; //aici se va retine numarul de mancare
};

individ populatie[10001]; //am definit aici populatia curenta
individ new_populatie[10001]; //urmatoarea populatie

int rnd(int a,int b) //randomul generat
{
    return a+(rand()%(b-a+1));
}

void initializare_matrice()
{
    dim++; //crestem dimensiunea ca sa putem borda matricea
    for(int i=0; i<=dim; i++)
        a[i][0]=a[0][i]=a[dim][i]=a[i][dim]=-1;
    a[1][1]=2;
    dim--; //revenim la dimensiunea initiala
    for(int i=1; i<=dim; i++)
    {
        int x=rnd(2,dim*dim); //generam un random in interiorul matricei
        if(a[x%dim==0?x/dim:x/dim+1][x%dim==0?dim:x%dim]==1) //daca se genereaza din nou acelasi random
            i--; //refacem iar pasul
        else //altfel
            a[x%dim==0?x/dim:x/dim+1][x%dim==0?dim:x%dim]=1; //marcam mancarea in matrice
    }
}

void afis_matrice() //afisam mediul
{
    for(int i=0; i<=dim+1; i++)
    {
        for(int j=0; j<=dim+1; j++)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}

int valid(individ x) //aceasta este functia care verifica daca un individ este valid
{ //adica verifica daca furnica nu iese din matrice si daca revine pe unde a mai fost
    int b[100][100]= {0};
    int i=1,j=1;
    int p=pasi;
    while(p)
    {
        if(x.adn[pasi-p]=='N')
            i--;
        if(x.adn[pasi-p]=='S')
            i++;
        if(x.adn[pasi-p]=='E')
            j++;
        if(x.adn[pasi-p]=='V')
            j--;
        if(a[i][j]==-1||b[i][j]==2)
            return 0;
        else
            b[i][j]=2;
        p--;
    }
    return 1;
}

int fit(individ x) //se face fitness-ul
{
    int b[100][100]= {0};
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
        if(a[i][j]==1&&b[i][j]==0)
            mancare++;
        if(a[i][j]==-1) return -1;
        b[i][j]=2;
    }
    return mancare; // altfel se returneaza mancarea culeasa
}

void generare_parinti() //genereaza populatia din generatie
{
    for(int x=0; x<nr_populatie; x++)
    {
        int ii=1,jj=1;
        individ test;
        for(int i=0; i<pasi; i++)
        {
jaja:
            int k=rnd(1,4); //determina o gena aleatoare
            //in functie de random se adauga gena
            if(k==1)
            {
                test.adn[i]='N';
                ii--;
            }
            else if(k==2)
            {
                test.adn[i]='S';
                ii++;
            }
            else if(k==3)
            {
                test.adn[i]='E';
                jj++;
            }
            else if(k==4)
            {
                test.adn[i]='V';
                jj--;
            }
            //daca se iese din mediu
            if(ii*jj==0||ii==dim+1||jj==dim+1)
            {
                //i--;
                if(k==1)
                    ii++;
                if(k==2)
                    ii--;
                if(k==3)
                    jj--;
                if(k==4)
                    jj++;
                goto jaja; //se reia de la generarea genei aleatorii
            }
        }
        populatie[x]=test;
        populatie[x].fitness=fit(populatie[x]);
        populatie[x].adn[pasi]='\0';
    }
}

char mutatie() //functia care determina mutatia
{
    return gene[rnd(0,3)];
}

individ recombinare(individ p1, individ p2) //recombinarea
{
    individ copil;
    int r=rnd(0,pasi-1); //taiem ADN-ul parintilor printr-un random
    for(int i=0; i<r; i++) //in prima parte vor aparea genele de la p1
    {
        int koko=rnd(0,100); //se genereaza un random
        if(koko>procent_mutatie) //daca randomul e mai mic decat procentul de mutatie
            copil.adn[i]=p1.adn[i];
        else
            copil.adn[i]=mutatie(); //se genereaza mutatia
    }
    for(int i=r;i<pasi;i++) //in a doua parte vor aparea genele de la p2
    {
        int jaja=rnd(0,100); //se genereaza un random
        if(jaja>procent_mutatie) //daca randomul e mai mic decat procentul de mutatie
            copil.adn[i]=p2.adn[i];
        else
            copil.adn[i]=mutatie(); //se genereaza mutatia
    }
    return copil; //returnam progenitura
}

void sort_populatie() //se sorteaza populatia in ordinea descrescatoare a fitness-ului
{
    int k;
    individ aux;
    do
    {
        k=1;
        for(int i=0; i<nr_populatie-1; i++)
            if(populatie[i].fitness<populatie[i+1].fitness)
            {
                aux=populatie[i];
                populatie[i]=populatie[i+1];
                populatie[i+1]=aux;
                k=0;
            }
    }
    while(!k);
}

void afisare_parinti() //se afiseaza populatia
{
    for(int i=0; i<nr_populatie; i++)
    {
        //cout<<valid(populatie[i])<<" ";
        cout<<i<<" "<<populatie[i].adn<<" fitness:"<<populatie[i].fitness<<endl;
    }
}

int main()
{
    srand(time(NULL));

    individ best;
    best.fitness=0;
    int best_gen=0;
    int nr_gen=0;

    initializare_matrice(); //initializam mediul
    afis_matrice(); //il afisam

    generare_parinti();
    //afisare_parinti();

    while(nr_gen<maxGen) //cat timp nu depasim numarul maxim de generatii
    {
        sort_populatie(); //sortam populatia pentru ca indivizii cei mai buni sa fie pe primele pozitii
        if(populatie[0].fitness>best.fitness)
        {
            best=populatie[0];
            best_gen=nr_gen;
        }

        if(best.fitness==dim) //daca am gasit fitness-ul maxim
        {
            cout<< "Generatie: " << nr_gen << "\t";
            cout<< "ADN: "<< populatie[0].adn <<"\t";
            cout<< "Fitness: "<< populatie[0].fitness << "\n";
            break; //il afisam si iesim din bucla
        }


        for(int i=0;i<nr_populatie/10; i++) //in 10% din populatie se gasesc indivizii cu firness-ul cel mai bun
        {
            int r=rnd(0,nr_populatie/10-1);
            individ p1=populatie[r];
            r=rnd(0,nr_populatie/10-1);
            individ p2=populatie[r];
            new_populatie[i]=recombinare(p1,p2); //generam un individ intre doi indivizi buni
            int f=fit(new_populatie[i]); //calculam fitness-ul
            new_populatie[i].fitness=f; //il memoram
        }
        int jum=nr_populatie/2;

        for(int i=nr_populatie/10; i<nr_populatie; i++)//pentru 90% din populatie facem recombinare intre indivizii
        {                                             //din prima jumatate a populatiei care sunt cei mai buni
            int r=rnd(0,jum);
            individ p1=populatie[r];
            r=rnd(0,jum);
            individ p2=populatie[r];
            individ copil=recombinare(p1,p2);
            int f=fit(copil);
            if(f>=0)
                {
                    copil.fitness=f;
                    new_populatie[i]=copil;
                }
            else
                i--;
        }
        cout<< "Generatie: " << nr_gen << "\t";
        cout<< "ADN: "<< populatie[0].adn <<"\t";
        cout<< "Fitness: "<< populatie[0].fitness << "\n";


        for(int i=0; i<nr_populatie; i++) //retinem noua populatia
            populatie[i]=new_populatie[i];

        nr_gen++;

    }

    cout<<endl<<"GOAT Furnica: "<<best.adn<<" fitness:"<<best.fitness<<"  Generatie:"<<best_gen;

}

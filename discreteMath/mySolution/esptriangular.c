#include<stdio.h>
#include<math.h>
void triangular(int num)
{

    int Inf = sqrt(num);
    int Sup = Inf + 1 ;
    int limiteInf = pow(Inf,2);
    int limiteSup = pow(Sup,2);
    int espaco = limiteSup - limiteInf;
    int meio = espaco/2 + limiteInf;
    int x;
    int y;
    if(limiteInf % 2 == 0)
    {
        
        y = sqrt(limiteInf)/-2;
        x = num - meio;               
            
    
    }
    int novoMeio1 = meio + (sqrt(limiteSup)/2);

    int novoMeio2 = meio - sqrt(limiteSup)/2;

    if(limiteInf % 2 != 0)
    {
        if(num >= meio)
        {
            y = novoMeio1 - num;
            x = meio - num;

        }
        if(num < meio)
        {
            y = num - novoMeio2;
            x = meio - num;
        }
    }

    
    printf("(%d,%d)\n",x,y);
}

int main()
{
    int num;
    scanf("%d", &num);
    triangular(num);
}
#include<stdio.h>
#include<math.h>
void quadrada(int num)
{
    int Inf = sqrt(num);
    int Sup = Inf + 1 ;
    int limiteSup = pow(Sup,2);
    int limiteInf = limiteSup - sqrt(limiteSup);
    int y;
    int x;
    int espaco = limiteSup - limiteInf;
    int meio = espaco/2 + limiteInf;
    
    //calcular y
    if (limiteSup % 2 != 0 && num <= limiteInf)
    {
        y  = sqrt(limiteInf)/-2;
    }
    if (limiteSup % 2 == 0 && num <= limiteInf)
    {
        y  = sqrt(limiteSup)/2;
    }
    if (limiteSup % 2 != 0 && num > limiteInf)
    {
        y = num - meio;
    }
    if (limiteSup % 2 == 0 && num > limiteInf)
    {
        y = meio - num ;
    }
    
    //calcular x
    meio = meio - sqrt(limiteSup);
    if (limiteSup % 2 != 0 && num >= limiteInf)
    {
        x  = (sqrt(limiteSup) - 1)/2;
    }
    if (limiteSup % 2 == 0 && num >= limiteInf)
    {
        x  = sqrt(limiteSup)/-2;
    }
    if (limiteSup % 2 != 0 && num < limiteInf)
    {
        x = num - meio - 1;
    }
    if (limiteSup % 2 == 0 && num < limiteInf)
    {
        x =  meio - num;
    }

    printf("(%d,%d)\n",x,y);
}    
int main()
{
    int num;
    scanf("%d", &num);
    quadrada(num);
}
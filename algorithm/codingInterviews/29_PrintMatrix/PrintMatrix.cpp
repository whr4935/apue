#include <iostream>
#include <stdio.h>
int printMatrix(int Matrix[][4], int n, int m)
{
    int i=0,e=0,r=0;
    int k,h;
    //int p[n][m]={0};
    //p = Matrix;
    k=n;
    h=m;
    while((k>0)&&(h>0)){
        //fisrt line
    for (i = e; i < m; ++i)
    {
        printf("%4d",Matrix[e][i]);
    }
    if(k==1) break;
    e=e+1;
     //last raw
    for(i =e;i<n;i++ ){
        printf("%4d",Matrix[i][m-1] );
    }
    if(h==1) break;
    //last line
    for (i = m-2; i >=r; i--)
    {
        printf("%4d", Matrix[n-1][i]);
    }
    r=r+1;
    //first raw 
    for ( i = n-2; i>=r; i--)
    {
        printf("%4d",Matrix[i][e-1]);
    }
    n=n-1;
    m=m-1;
    k=k-2;
    h=h-2;
    }
    return 0;    
}
int main()
{
    int Matrix[][4]={
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,16},
    };
    printMatrix(Matrix,4,4);
    return 0;
}
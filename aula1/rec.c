#include<stdio.h>
#include<stdlib.h>


void f1(int a){
    if (a >= 1){
        printf("%d %p %p \n",a,&f1,&a);
        f1(a-1);
    }
}
void f2(int a){
    if (a >= 1){
        f2(a-1);
        printf("%d %p %p \n",a,&f2,&a);
    }
}




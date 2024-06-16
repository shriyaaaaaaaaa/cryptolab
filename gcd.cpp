#include <stdio.h>  

int main()  

{  

int m, n; /* given numbers */ 
printf("SHREYA KHATRI\n");

printf("Enter-two integer numbers: ");  

scanf ("%d %d", &m, &n);  

while (n > 0)  

{  

int r = m % n;  

m = n;  

n = r;  

}  

printf ("GCD = %d \n",m);  

}  

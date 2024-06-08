#include<stdio.h>
#include<ctype.h>
#include<conio.h>
#include<string.h>
#include<process.h>
void vigenere(char *, char *);
void encipher();
void decipher();
int main()
{
int option;
while(1)
{
	printf("\n SHREYA KHATRI");
printf("\n 1. Encipher!!");
printf("\n 2. Decipher!!");
printf("\n 3. Exit \n");
printf("\n Enter your option: ");
scanf("%d",&option);
fflush(stdin);
if(option==3)
exit(0);
else if(option==1)
encipher();
else if(option==2)
decipher();
else
printf("\n Invalid selection!!Try again!! ");
}
}
void encipher()
{
unsigned int i,j; //unsigned integer can represent only non negative integers
char plain[127],key[15];
printf("\n Enter the plaintext (maximum 128 characters): ");
gets(plain);
printf("\n Enter the key (maximum 16 characters): ");
gets(key);
for(i=0,j=0; i<strlen(plain); i++,j++)
{
if(j>=strlen(key))
{
j=0;
}
printf("%c",65+(((toupper(plain[i])-65)+(toupper(key[j])-65))%26));
//plaintext+key%26 gives encrypted characters
}
}
void decipher()
{
unsigned int i,j;
char plain[127],key[15];
int value;
printf("\n Enter the ciphertext: ");
gets(plain);
printf("\n Enter the key: ");
gets(key);
for(i=0,j=0; i<strlen(plain); i++,j++)
{
if(j>=strlen(key))
{
j=0;
}
value=(toupper(plain[i])-64)-(toupper(key[j])-64);
if(value<0)
{
value=value+26;
}
printf("%c",65+(value%26));
}
}


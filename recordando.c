#include <stdlib.h>
#include <stdio.h>


int main(int argc,char *argv[]){
    /* binary operators */
    unsigned char byte = 0xFF;
    unsigned char byte2;
    unsigned char byte3;
    unsigned int bite;
    unsigned char letra;

    byte2 = byte << 1; 

    printf("B2:%X\n",byte2);

    byte3 = byte2;
    bite = (int)byte2;
    printf("bite:%d\n",bite);

    bite = 90;
    letra = (char)bite;
    printf("letra:%c\n",letra); /* ways to interpretate the data */

    byte2 = ((byte2>>1)|(byte2<<7));
    byte3 = ((byte3<<2)|(byte3>>6)); /* binary Rotation */

    printf("B1:%X\n",byte);
    printf("B2:%X\n",byte2);
    printf("B3:%X\n",byte3);

    return 0;
}




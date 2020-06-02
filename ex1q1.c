#include <stdio.h>

void exo1Q1() {

    int p=3, q=7, d=5, e=5, M = 2, c= 3 ,n, fi, cryptedM, decryptedM;


    printf("Exercice 1 (RSA)\n");

    n = 3 * 7;
    fi = (p-1)*(q-1);

    printf("1) \tn = %d\n",n);
    printf("\tfi(n) = %d\n",fi);

    cryptedM = (M^e)%n;
    decryptedM = (c^d)%n;
    printf("\tMessage crypté : %d\n", cryptedM);
    printf("\tMessage décrypté : %d\n", decryptedM);
}
int main() {

    exo1Q1();


    return 0;
}



#include <string.h>
#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define KEY_SIZE 4096   /* This is the number of bits we want in the public key */
#define PRIME_SIZE ((KEY_SIZE/8) / 2)  /* This is the number of bytes in q and p */

// Method that generates random prime numbers with a fixed length of bits.
void generatePrimeNumber(mpz_t value, mpz_t e) {
    mpz_t tmp1, tmp2;
    char buf[PRIME_SIZE];
    int i;

    mpz_inits(tmp1, tmp2, NULL);

    // Set the bits of tmp randomly
    for(i = 0; i < PRIME_SIZE; i++)
        buf[i] = rand() % 0xFF;

    // Set the top two bits to 1 to ensure int(tmp) is relatively large
    buf[0] |= 0xC0;

    // Set the bottom bit to 1 to ensure int(tmp) is odd (better for finding primes)
    buf[PRIME_SIZE - 1] |= 0x01;

    // Interpret this char buffer as an int
    mpz_import(tmp1, PRIME_SIZE, 1, sizeof(buf[0]), 0, 0, buf);

    // Pick the next prime starting from that random number
    mpz_nextprime(value, tmp1);

    /* Make sure this is a good choice */
    mpz_mod(tmp2, value, e);        /* If p mod e == 1, gcd(phi, e) != 1 */

    while(!mpz_cmp_ui(tmp2, 1))
    {
        mpz_nextprime(value, value);    /* so choose the next prime */
        mpz_mod(tmp2, value, e);
    }

    mpz_clears(tmp1, tmp2, NULL);
}

//Method that generates both public and private key, and also returns the "n" value
void generateKeys(mpz_t d, mpz_t e, mpz_t n) {
    mpz_t p, q, phi, pm1, qm1, gcd;
    mpz_inits(p,q,phi,pm1,qm1, gcd,NULL);

    //set e
    mpz_set_ui(e,3);

    //Generate random p & q
    generatePrimeNumber(p, e);
    generatePrimeNumber(q, e);

    // get n
    mpz_mul(n,p,q);
    // get p-1
    mpz_sub_ui(pm1, p,1);
    // get q-1
    mpz_sub_ui(qm1, q,1);

    // get phi
    mpz_mul(phi, pm1, qm1);

    //calculates d
    mpz_invert(d,e,phi);

    //setting the memory free
    mpz_clears(p, q, phi, pm1, qm1, NULL);

}

void encrypt(mpz_t encrypted, const mpz_t message, const mpz_t e, const  mpz_t n)
{
    //Method used to calculate message^e mod n and put it inside encrypted
    mpz_powm(encrypted, message, e, n);
}

void decrypt(mpz_t decrypted, const mpz_t message, const mpz_t d, const mpz_t n)
{
    //method used to calculate message^d mod n and put it inside decrypted
    mpz_powm(decrypted, message, d,n);
}

//Method that loops for x seconds and encrypt as many messages as possible.
void delay(int number_of_seconds)
{
    mpz_t n, e, d, message, encryptedM, decryptedM;

    //Seading the random based on the machine time.
    srand(time(NULL));

    mpz_inits(n,e,d,encryptedM,decryptedM, NULL);
    //Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
    int cpt = 0;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    do {
        generateKeys(d,e,n);
        mpz_init_set_ui(message, rand());
        encrypt(encryptedM, message, e, n);
        cpt++;
    } while (((clock() - start_time)*1000/CLOCKS_PER_SEC) <milli_seconds);

    printf("Number of crypted messages in %d seconds for a %d bits key : %d\n", number_of_seconds, KEY_SIZE, cpt);
}

int main() {

    mpz_t n, e, d, message, encryptedM, decryptedM;

    //Seading the random based on the machine time.
    srand(time(NULL));

    mpz_inits(n,e,d,encryptedM,decryptedM, NULL);

    generateKeys(d,e,n);
    gmp_printf("Private key : %Zd\n Public key : (%Zd , %Zd)\n", d, n, e);

    mpz_init_set_ui(message, rand());
    gmp_printf("Message : %Zd\n", message);

    encrypt(encryptedM, message, e, n);
    gmp_printf("Crypted message : %Zd\n", encryptedM);

    decrypt(decryptedM, encryptedM, d, n);
    gmp_printf("Decrypted message : %Zd\n", decryptedM);

    //Calling the 10 seconds loop to generate a maximum of encrypted message.
    delay(10);

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZ99-With-Data029 CrackAZ99-With-Data029.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Data029 > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$5sxZuW/BW7MhZpmsy8siMZkWWU5gslVlz5p54WWrvZTp0l8qiPG7oVlrBDJ0W6TKUg3zVS9yi2efTDTwgPVX50",
  "$6$KB$T4Rz4QACPy1J3egIL/P7vCkrpZzHX.YBH8MEJQkamYeGTv9byUhKRtRbAsy3.KRQ1DDAx.depDWgZ73V.Tec0.",
  "$6$KB$UIXrc3Pj2BdWAzoR5LdWvFJefiUBH04JEP.Pqh49LTxAtihqkD5gNMZWT.ylg1z3Sh1MHunpy6hGrWPUeTFz/1",
  "$6$KB$W4a6Zo4LQxTW50/GqIge.PFERTHRbL30zWlEWFnY4an8SvMqfPzRvphcs7Rv28SBKeO9pxIjLLG9whyK4Kts/0"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int p, q, r, s;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(p='A'; p<='Z'; p++){
    for(q='A'; q<='Z'; q++){
	for(r='A'; r<='Z'; r++){
      for(s=0; s<=99; s++){
        sprintf(plain, "%c%c%c%02d", p, q, r, s); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}


int time_difference(struct timespec *start, struct timespec *finish, 
                              long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {
int i;
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}

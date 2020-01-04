#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code.

  Compile with:
    cc -o muktithread muktithread.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./muktithread > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",
  "$6$KB$VDUCASt5S88l82JzexhKDQLeUJ5zfxr16VhlVwNOs0YLiLYDciLDmN3QYAE80UIzfryYmpR.NFmbZvAGNoaHW.",
  "$6$KB$9ECMQHYe.um7IQlO/hEV3pInwxRKppvI8OKZzzX2lyNbh0Ev3bfh3PnIw6rFZjT/QLGz/2Z89mDYXJN1dJB571",
  "$6$KB$CQ048eehl8D2Q4dYGBgQwOaqWHfQEjArOqcBpRALUt6SwQCem4mIW4H.irVf/ufBujEKlG/Mv8Zq.P/w6zu7C/"
};

/**
 Required by lack of standard function in C.  
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void pThread(char *paswd) {
  pthread_t thread_1, thread_2;

  void *kernel_function_1();
  void *kernel_function_2();
 
  pthread_create(&thread_1, NULL, kernel_function_1, paswd);
  pthread_create(&thread_2, NULL, kernel_function_2, paswd);

  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
}
/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the
 start of the line. Note that one of the most time consuming operations that
 it performs is the output of intermediate results, so performance experiments
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void *kernel_function_1(char *salt_and_encrypted){
  int r, o, h;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(r='A'; r<='M'; r++){
    for(o='A'; o<='Z'; o++){
     
      for(h=0; h<=99; h++){
        sprintf(plain, "%c%c%02d", r, o,h);
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
  printf("%d solutions explored\n", count);
}
void *kernel_function_2(char *salt_and_encrypted){
  int r, o, h;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(r='N'; r<='Z'; r++){
    for(o='A'; o<='Z'; o++){
     
      for(h=0; h<=99; h++){
        sprintf(plain, "%c%c%02d", r, o,h);
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


int main(){
  int i;
  struct timespec start, finish;  
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
  for(i=0;i<n_passwords;i<i++) {
    pThread(encrypted_passwords[i]);
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9));

  return 0;
}

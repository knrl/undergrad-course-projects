/*
  Mehmet Kaan Erol - BLM3010

  Compile on cli:
    gcc program.c && ./a.out 5

*/

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int tamSayilarinToplami(int);
int faktoriyelHesapla(int);
void usage(void);
int main(int argc, int *argv[])
{
  int N = 0;
  if (argc < 2)
  {
    usage();
    exit(1);
  }

  assert(argv[1] != NULL);
  N = atoi(argv[1]);
  assert(N >= 0);

  pid_t new_process = fork();
  if (new_process == 0) // child process
  {
    new_process = fork();
    if (new_process == 0) // child process
    {
      printf("\n[ID=%d] %d'in faktoriyel degeri %d'dir.", 
        getpid(), N, faktoriyelHesapla(N));    
    }
    else
    {
      printf("\n[ID=%d] %d'e kadar olan pozitif tam sayilarin toplami %d'tir.", 
        getpid(), N, tamSayilarinToplami(N));  
    }
  }
  else  // main (parent) process
  {
    wait(0);
    printf("\n[ID=%d]Bitti", getpid());
  }

  return 0;
}

void usage(void)
{
	fprintf(stderr, "Usage: odev2 [options]\n"
		"Options:\n"
		"  N <tam sayi> N tam sayi degeri\n"
		);
}

int tamSayilarinToplami(int N)
{
  if (N == 1)
  {
    return 1;
  }

  return N + tamSayilarinToplami(N - 1);
}

int faktoriyelHesapla(int N)
{
  if (N == 2)
  {
    return 2;
  }

  return N * faktoriyelHesapla(N - 1);
}
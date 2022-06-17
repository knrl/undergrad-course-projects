/*
...
*/

#include <stdio.h>
#include <stdlib.h>

void usage(void);
char readAndPrint(FILE *, const unsigned int, const unsigned int);
int main(int argc, char * argv[])
{
  // argc'ın (argument count) en az 3 argümanı olmalıdır. İlk argümanı varsayılan olarak gelmektedir. 
  // örnek: argc = 3 ve argv = {"./a.out", "-p", "home/usr/desktop/note.txt"}
  if (argc < 3)
  {
    usage();
    exit(1);
  }
  
  // dosya pointer'ı
  FILE *fp;
  // dosya adını kontrol et
  printf("\nGirilen dosya yolu: %s\n", argv[2]);
  // dosyayı okuma modunda açalım
  fp = fopen(argv[2], "r");
  
  // dosya açılamadı ise sonlandır
  if (fp == NULL)
  {
    printf("\n\tDosya açılamadı.\n");
    exit(1);
  }

  printf("Cikti:\n");
  // ilk 40 byte düz dosyanın geri kalanı 40'ar byte ve ters yazdırılacak
  // parametreler: dosya pointer'ı, okunacak boyut (40), ters(1) | düz(0) yazdır
  readAndPrint(fp, 40, 0);
  // dosyanın sonuna gelene kadar devam et
  while (readAndPrint(fp, 40, 1) != EOF);

  // açılan dosyayı kapat
  fclose(fp);
  return 0;
}

void usage(void)
{
	fprintf(stderr, "Usage: odev1 [options]\n"
		"Options:\n"
		"  -p <dosya_konumu>\t/dosya/yolu/note.txt\n"
		);
}

char readAndPrint(FILE *fp, const unsigned int boyut, const unsigned int yazdirMod)
{
  char ch;
  char *buffer;
  buffer = (char*)calloc(boyut, sizeof(char));

  // karakter EOF'den (End Of File) farklı ve i<boyut olduğu sürece devam et
  // her bir karakter 1 byte olduğudan toplan yazdırılan boyut 1 byte x boyut kadar olur.
  ch = fgetc(fp);
  for (int i=0;i<boyut; i++)
  {
    if (ch == EOF) break;
    buffer[i] = ch;
    ch = fgetc(fp);
  }

  // ters çevir ve yazdır
  if (yazdirMod == 1)
  {
    for (int i=boyut-1;i>=0; i--)
    {
      if (buffer[i] == 0) break; 
      printf("%c", buffer[i]);
    }
  }
  else
  {
    for (int i=0;i<boyut; i++)
    {
      if (buffer[i] == 0) break; 
      printf("%c", buffer[i]);
    }
  }

  // memory leak'i önlemek için dinamik bellek alanını serbest bırak
  free(buffer);
  return ch;
}

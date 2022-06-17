/**
 * @file      my_wc.c
 * @author    Mehmet Kaan Erol (kaanerol@marun.edu.tr)
 * @brief     System Programming 
 * @version   0.1
 * @date      2022-04-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
/* programın manual page'i man.txt olarak aynı dizinde bulunmaktadır. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_ERROR   2

/**
 * @brief wc programının algığı argüman flag'lerini gösteren fonksiyon
 * 
 */
void usage()
{
	fprintf(stderr, "Usage: ks [-c -l -w] [file ...]\n"
		"Options:\n"
		"  -c\t\tSatır sayısını yazdır\n"
		"  -l\t\tKelime sayısını yazdır\n"
		"  -w\t\tKarakter sayısını yazdır\n"
        );
}

/**
 * @brief wc programının algığı argüman flag'lerini gösteren fonksiyon
 * 
 * @param  file_name     dosya adı
 * @param  num_line      satır sayısının referansı
 * @param  num_word      kelime sayısının referansı
 * @param  num_character karakter sayısının referansı
 * @param  flag_stdin    girdi stdin ise 1 değilse 0 
 * @return hata durumunu döndürür
 */
int getNumOfLineWordCharacter(const char *file_name, unsigned int *num_line, unsigned int *num_word, unsigned int *num_character, int flag_stdin)
{
    char c;
    int inc_flag_counter=0;

    if (flag_stdin)
    {
        if (read(0, &c, 1) > 0)
        {            
            if (isspace(c) != 0 || c == '\t' || c == '\n' || c == '\r')
                inc_flag_counter++;

            if (!(isspace(c) != 0 || c == '\t' || c == '\n' || c == '\r'))
                *num_word+=1;

            if (c == '\n')
                *num_line+=1;

            *num_character+=1;
        }
            
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                *num_line+=1;

            if (isspace(c) != 0 || c == '\t' || c == '\n' || c == '\r')
                inc_flag_counter++;
            else if (c != EOF)
            {
                if (inc_flag_counter >= 1)
                {
                    *num_word+=1;
                }
                inc_flag_counter=0;
            }

            *num_character+=1;
        }
    }
    else
    {
        FILE *fp = fopen(file_name, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "HATA: \'%s\' dosyası açılamıyor.\n",file_name);
            return -1;
        }

        c = fgetc(fp);
        if (!feof(fp))
        {            
            if (isspace(c) != 0 || c == '\t' || c == '\n' || c == '\r')
                inc_flag_counter++;

            if (!(isspace(c) != 0 || c == '\t' || c == '\n' || c == '\r'))
                *num_word+=1;

            if (c == '\n')
                *num_line+=1;

            *num_character+=1;
        }

        while (!feof(fp))
        {
            c = fgetc(fp);

            if (c == '\n')
                *num_line+=1;

            if (isspace(c) != 0 || c == '\t' || c == '\n' || c == '\r')
                inc_flag_counter++;
            else if (c != EOF)
            {
                if (inc_flag_counter >= 1)
                {
                    *num_word+=1;
                }
                inc_flag_counter=0;
            }

            *num_character+=1;
        }

        // subtract 1 for EOF
        if (*num_character > 0)
            *num_character-=1;

        fclose(fp);
    }

    return 0;
}

int main(int argc, char * const* argv)
{
    int status;
    char opt;
    unsigned char flag_w=0, flag_l=0, flag_c=0;
    unsigned int num_line=0,num_word=0,num_character=0;
    unsigned int top_num_line=0,top_num_word=0,top_num_character=0;

    // c, l, w flag'lerinin hepsi aynı anda verilebilir
    while ((opt = getopt(argc, argv, ":cwl")) != -1) 
    {
        switch (opt) 
        {
            case 'l': flag_l     = 1; break; // satırlar
            case 'w': flag_w     = 1; break; // kelimeler
            case 'c': flag_c     = 1; break; // karakterler
            default:
                fprintf(stderr, "HATA: bilinmeyen seçenek -%c \n",optopt);
                //usage();
                exit(EXIT_ERROR);
        }
    }

    // eğer hiç flag girilmediyse tüm flag'leri aktif et.
    if (flag_c == 0 && flag_l == 0 && flag_w == 0)
    {
        flag_c=1; flag_l=1; flag_w=1;
    }

    // dosyalar ve stdin için sırasıyla çıktıları hesapla
    int counter=0;
    for (int i=1;argc>i;i++)
    {
        if (argv[i][0] != '-')      // flag değilse bir dosyayı temsil etmeli
        {
            if (argv[i][0] == '+')  // eğer + varsa stdin
            {
                printf("-stdin- ");
                status = getNumOfLineWordCharacter("stdin",&num_line,&num_word,&num_character,1);                
            }
            else
            {
                status = getNumOfLineWordCharacter(argv[i],&num_line,&num_word,&num_character,0);
                if (status == -1)
                    continue;
                printf("%s ",argv[i]);
            }

            if (flag_l == 1)
            {
                printf("%d ", num_line);
                top_num_line+=num_line;
                num_line=0;
            }

            if (flag_w == 1)
            {
                printf("%d ", num_word);
                top_num_word+=num_word;
                num_word=0;
            }

            if (flag_c == 1)
            {
                printf("%d ", num_character);
                top_num_character+=num_character;
                num_character=0;
            }

            printf("\n");
            counter++;
        }
    }

    if (counter == 0 && status != -1) // hiç (uygun ya da uygunsuz girişler dahil) dosya girilmezse stdin'den oku  
    {
        printf("-stdin- ");
        status = getNumOfLineWordCharacter("stdin",&num_line,&num_word,&num_character,1); 

        if (flag_l == 1)
            printf("%d ", num_line);
    
        if (flag_w == 1)
            printf("%d ", num_word);
        
        if (flag_c == 1)
            printf("%d ", num_character);
    }
    else if (counter > 1)
    {
        printf("toplam ");
        if (flag_l == 1)
            printf("%d ", top_num_line);

        if (flag_w == 1)
            printf("%d ", top_num_word);

        if (flag_c == 1)
            printf("%d ", top_num_character);
        printf("\n");
    }

    return EXIT_SUCCESS;
}

/**
 * @file      baklava_threadsync.c
 * @author    Mehmet Kaan Erol (kaanerol@marun.edu.tr)
 * @brief     System Programming 
 * @version   0.1
 * @date      2022-06-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
/* programın manual page'i man.txt olarak aynı dizinde bulunmaktadır. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_ERROR   2

#define USTA_SAYISI  6

struct sourceStruct {
    int id;
    char s1;
    char s2;
};

struct sourceStruct *sources;
struct sourceStruct *readySource;
int lineOfFile=0;
int isSourceReady=0;
sem_t semBaklavaReady, semSourceReady, semMalzemeAlindi, semAssignSource;

/**
 * @brief SIGUSR1 için özel signal tanımı
 * 
 * @param signum signal tipini temsil eden sayı
 */
void sig_handler(int signum)
{
    // thread'leri signal ile sonlandir
    exit(EXIT_SUCCESS);
}

/**
 * @brief main thread'de çalışan toptanci fonksiyonu
 * 
 */
void toptanci()
{
    int sourceIndex=lineOfFile-1;

    while(sourceIndex >= 0)
    {
        sem_wait(&semSourceReady);
        isSourceReady = 1;
        sem_post(&semSourceReady);

        // siradaki kaynagi hazirla
        sem_wait(&semAssignSource);
        readySource->s1 = sources[sourceIndex].s1;
        readySource->s2 = sources[sourceIndex].s2;
        sourceIndex--;
        sem_post(&semAssignSource);

        // kaynak hazir, dagitima cik
        printf("toptanci %c ve %c teslim etmektedir.\n",readySource->s1,readySource->s2);
        
        // ustanın malzemeyi alması bekleniyor
        sem_wait(&semMalzemeAlindi);
        printf("toptanci baklavayi bekliyor.\n"); 

        // bakalava yapımı tamamlanana kadar bekle
        sem_wait(&semBaklavaReady);
        printf("toptanci baklavayi aldi ve satmaya gitti.\n"); 
    }
}

/**
 * @brief ustaları temsil eden thread'lerin fonksiyonu
 * 
 * @param arg ustalara ait ihtiyaçları tutan referans
 * @return void* 
 */
void *usta(void *arg)
{
    struct sourceStruct *neededSources  = arg;
    int randomTime=0, status=0;
    // main thread'den kaynak bitti sinyali gelene kadar çalış
    while(1)
    {
        // usta baklava için malzeme bekliyor
        // printf("Usta %d, %c ve %c malzemeleri icin bekliyor.\n", neededSources->id,neededSources->s1,neededSources->s2);
        
        sem_wait(&semSourceReady);
        isSourceReady = 0; 
        sem_post(&semSourceReady);

        // malzemeler hazır mı?
       sem_wait(&semAssignSource);
        status = ((neededSources->s1 == readySource->s1 && neededSources->s2 == readySource->s2) || 
            (neededSources->s1 == readySource->s2 && neededSources->s2 == readySource->s1));
        usleep(50);
        sem_post(&semAssignSource);

        if (status)
        {
            // malzemeyi aldı
            sem_wait(&semAssignSource);
            printf("Usta %d, %c ve %c malzemelerini aldi.\n", neededSources->id, readySource->s1, readySource->s2);
            sem_post(&semAssignSource);
            sem_post(&semMalzemeAlindi);

            // baklava yapımı sırasında rastgele bir süre bekle 
            printf("Usta %d baklavayi hazirliyor.\n", neededSources->id);
            randomTime = rand() %5 + 1;
            sleep(randomTime);
            sem_post(&semBaklavaReady);

            // baklava yapımı tamamlandı
            printf("Usta %d baklavayi toptanciya teslim etti.\n", neededSources->id);                
        } 
    }
}

/**
 * @brief kaynak dosyasının okur geriye sourceSturct tipinde dinamik dizi döndürür 
 * 
 * @param fileName okunacak dosyanın adı
 * @return struct sourceStruct* kaynakları tutan dinamik dizinin referansı 
 */
struct sourceStruct* readFile(char const *fileName)
{
    FILE *filePtr;
    char emptyChar;
    int index=0;

    if ((filePtr = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Dosya okuma sirasinda hata olustu.\n");
        exit(EXIT_FAILURE);
    }

    // dosyadaki her satırı tek seferde alacak bir temp buffer
    char *tempData = (char*)malloc(sizeof(char)*3);

    // dinamik bellek boyutunu ayarlamak için dosyanın satır sayısını öğren
    while (fread(tempData, sizeof(char)*3, 1, filePtr) == 1)
        lineOfFile++;

    // fseek ile dosyanın başına gel
    fseek(filePtr, 0, SEEK_SET);

    struct sourceStruct *sources = (struct sourceStruct*)malloc(sizeof(struct sourceStruct)*lineOfFile); 
    while (fread(tempData, sizeof(char)*3, 1, filePtr) == 1)
    {
        sources[index].s1 = tempData[0];
        sources[index].s2 = tempData[1];
        index++;
    }

    fclose(filePtr);
    free(tempData);
    return sources;
}

/**
 * @brief programın kullanımı hakkında 
 * 
 */
void usage()
{
	fprintf(stderr, "Usage: ./a.out [-i] [filePath ...]\n"
		"Options:\n"
		"  -i\t\tKaynaklari tutan dosyanin yolu\n"
		);
}

int main(int argc, char *const argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Eksik arguman girisi.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    char opt;
    char const *fileName;
    struct sourceStruct ustaSources[USTA_SAYISI];
    pthread_t usta_th[USTA_SAYISI];

    signal(SIGUSR1,sig_handler);

    sem_init(&semBaklavaReady, 0, 0);
    sem_init(&semAssignSource, 0, 1);
    sem_init(&semSourceReady, 0, 1);
    sem_init(&semMalzemeAlindi, 0, 0);

    // ustaların ihtiyacı olan kaynakların atanması
    ustaSources[0].id = 1;      ustaSources[0].s1 = 'S';        ustaSources[0].s2 = 'G';    // usta 1
    ustaSources[1].id = 2;      ustaSources[1].s1 = 'U';        ustaSources[1].s2 = 'G';    // usta 2
    ustaSources[2].id = 3;      ustaSources[2].s1 = 'F';        ustaSources[2].s2 = 'G';    // usta 3
    ustaSources[3].id = 4;      ustaSources[3].s1 = 'S';        ustaSources[3].s2 = 'U';    // usta 4
    ustaSources[4].id = 5;      ustaSources[4].s1 = 'S';        ustaSources[4].s2 = 'F';    // usta 5
    ustaSources[5].id = 6;      ustaSources[5].s1 = 'U';        ustaSources[5].s2 = 'F';    // usta 6
   
    while ((opt = getopt(argc, argv, ":i")) != -1)
    {
        switch(opt)
        {
            case 'i': 
                fileName = argv[2];
                break;
            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

    // dosyadan kaynakları oku, heap'te bir array ile tut    
    sources = readFile(fileName);

    // hazır kaynakları temsil eden dizi
    readySource = (struct sourceStruct*)malloc(sizeof(struct sourceStruct));

    // thread'leri olustur
    for (int i=0;i<USTA_SAYISI;i++)
    {
        if ( (pthread_create(&usta_th[i], NULL, &usta, &ustaSources[i])) > 0)
        {
            fprintf(stderr, "Thread olusturulurken hata olustu.");
            exit(EXIT_FAILURE);
        }
    }

    // toptancıyı başlat
    toptanci();

    // toptanci fonksiyonu (main thread) tamamlandıysa kaynaklar bitmiştir
    printf("Kaynaklar bitti. Program kapaniyor...\n");

    // signal ile thread'lere kaynakların bittiğini bildir
    pthread_kill(usta_th[0], SIGUSR1);
    pthread_kill(usta_th[1], SIGUSR1);
    pthread_kill(usta_th[2], SIGUSR1);
    pthread_kill(usta_th[3], SIGUSR1);
    pthread_kill(usta_th[4], SIGUSR1);
    pthread_kill(usta_th[5], SIGUSR1);

    sem_destroy(&semBaklavaReady);
    sem_destroy(&semSourceReady);
    sem_destroy(&semAssignSource);
    sem_destroy(&semMalzemeAlindi);
    free(readySource);
    free(sources);
    return 0;
}

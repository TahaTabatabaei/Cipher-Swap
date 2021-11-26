#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    char mainDecoderFifo[] = "Main_Placer";
    int fd = open(mainDecoderFifo, O_RDONLY);
    char phrase[10000];
    int i = 0;
    read(fd, &phrase[i], sizeof(char));
    while(phrase[i] != '\0'){
        i++;
        read(fd, &phrase[i], sizeof(char));
    }
    close(fd);


    char finderDecoderFifo[] = "Finder_Placer";
    int fifo = open(finderDecoderFifo, O_RDONLY);

    char str[10000];
    i = 0;
    read(fd, &str[i], sizeof(char));
    while(str[i] != '\0'){
        i++;
        read(fd, &str[i], sizeof(char));
    }
    close(fd);

    char finalMessage[10000];
    int finalMessageCounter = 0;

    char * token = strtok(str, "=");
    for(int k = 0; k < strlen(phrase); k++){
        if(phrase[k] == '$'){
            for(int q = 0; q < strlen(token) ; q++){
                finalMessage[finalMessageCounter] = token[q];
                finalMessageCounter++;
            }
            token = strtok(NULL, "=");
        } else{
            finalMessage[finalMessageCounter] = phrase[k];
            finalMessageCounter++; 
        }
    }

    //printf("**%s\n", finalMessage);
    char placerMainFifo[] = "Placer_Main";
    mkfifo("Placer_Main" ,0666);
    int placerMain_fd = open(placerMainFifo,O_WRONLY);
    write(placerMain_fd,finalMessage, strlen(finalMessage));
    close(placerMain_fd);
    
    
    
  FILE *out=fopen("placer_resault.txt","w");
  fputs(finalMessage ,out);
  fclose(out);
    return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    //Reading keys from decoder:
    char mainDecoderFifo[] = "Main_Finder";
    int fd = open(mainDecoderFifo, O_RDONLY);
    char keys[1000];
    int i = 0;
    read(fd, &keys[i], sizeof(char));
    while(keys[i] != '\0'){
    i++;
    read(fd, &keys[i], sizeof(char));
    }
    close(fd);


    //reading from decoder:
    char decoderFinderFifo[] = "Decoder_Finder";
    int fd_decoder = open(decoderFinderFifo, O_RDONLY);
    char str[10000];
    i = 0;
    read(fd_decoder, &str[i], sizeof(char));
    while(str[i] != '\0'){
    i++;
    read(fd_decoder, &str[i], sizeof(char));
    }
    close(fd_decoder);

    
    int wordCounter = 0;
    for(int j = 0 ; j < strlen(keys) ; j++){
        if(keys[j] == '$'){
            wordCounter++;
        }
    }
    wordCounter++;

    char result[10000]= "";

    //finding words:    
    int counter = 0;
    int start = 0;
    int end = 0;
    for(int j = 0 ; j < strlen(keys) ; j++){
        if(keys[j] == '$'){
            end = j - 1;
            char keyNumbers[10];
            memcpy(keyNumbers,&keys[start],end-start+1);
            int firstNumber,secondNumber;
            sscanf(keyNumbers,"%d %d",&firstNumber,&secondNumber);
            char values[secondNumber+2];
            memcpy(values,&str[firstNumber],secondNumber);
            values[secondNumber] = '=';
            values[secondNumber+1] = '\0';
            strcat(result,values);
            start = j+1;
        }
    }
    char keyNumbers[10];
    memcpy(keyNumbers,&keys[start],strlen(keys)-start);
    int firstNumber,secondNumber;
    sscanf(keyNumbers,"%d %d",&firstNumber,&secondNumber);
    char values[secondNumber+1];
    memcpy(values,&str[firstNumber],secondNumber);
    values[secondNumber] = '\0';
    // printf("last: %s\n",values);
    strcat(result,values);


    char finderPlacerFifo[] = "Finder_Placer";
    mkfifo(finderPlacerFifo, 0666);
    int fd_placer = open(finderPlacerFifo,O_WRONLY);
    write(fd_placer, result , strlen(result)+1);
    close(fd_placer);

    while(wait(NULL) > 0);
    return 0;
}
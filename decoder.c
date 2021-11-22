#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    char mainDecoderFifo[] = "Main_Decoder";
    int fd = open(mainDecoderFifo, O_RDONLY);
    char str[1000];
    int i = 0;
    read(fd, &str[i], sizeof(char));
    while(str[i] != '\0'){
        i++;
        read(fd, &str[i], sizeof(char));
    }
    close(fd);

    char ch;
    for(i = 0; str[i] != '\0'; ++i){
        ch = str[i];
        if(ch >= 'a' && ch <= 'z'){
        ch = ch + 3;
        if(ch < 'a'){
            ch = ch + 'z' - 'a' + 1;
        }
        str[i] = ch;
        }
        else if(ch >= 'A' && ch <= 'Z'){
        ch = ch + 3;
        if(ch < 'A'){
            ch = ch + 'Z' - 'A' + 1;
        }
        str[i] = ch;
        }

        if(str[i] == '{'){
        str[i] = 'a';
        }else if(str[i] == '}'){
        str[i] = 'c';
        }
    }

    FILE *out=fopen("decoder.txt","w");
    fputs(str,out);
    fclose(out);

    char Decoder_Finder_Fifo[] = "Decoder_Finder";
    mkfifo(Decoder_Finder_Fifo,0666);
    int fd_fifo = open(Decoder_Finder_Fifo,O_WRONLY);
    write(fd_fifo, str , strlen(str)+1);
    close(fd_fifo);

    while(wait(NULL) > 0);
    return 0;
}
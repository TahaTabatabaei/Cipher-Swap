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
  char str[10000];
  char charFromMain;
  int i = 0;
  read(fd, &charFromMain, sizeof(char));
  while(charFromMain != '\0'){
    if(charFromMain != ' ' && charFromMain != '\n'){
     // read(fd, &str[i], sizeof(char));
     str[i] = charFromMain;
      i++;
    }
    read(fd, &charFromMain, sizeof(char));
  }
  str[i] = '\0';
  
  close(fd);

  // caesar cipher
   char ch;
  for(i = 0; str[i] != '\0'; ++i){
    ch = str[i];
    if(ch >= 'a' && ch <= 'w'){
      ch = ch + 3;
      }
    else if ( ch == 'x'){
        ch = 'a';
        }
    else if( ch == 'y'){
        ch = 'b';
        }
    else if(ch == 'z'){
       ch = 'c';
       }   
    else if(ch >= 'A' && ch <= 'W'){
      ch = ch + 3;
      }
    else if ( ch == 'X'){
        ch = 'A';
        }
    else if( ch == 'Y'){
        ch = 'B';
        }
    else if(ch == 'Z'){
       ch = 'C';
       }   
      
    str[i] = ch;
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

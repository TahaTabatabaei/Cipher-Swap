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
    if ( str[i] != ' ' && str[i] != '\n'){
    read(fd, &str[i], sizeof(char));
    }
  }
  close(fd);

  // caesar cipher
  char ch;
  for(i = 0; str[i] != '\0'; ++i){
    ch = str[i];
    if(ch >= 'd' && ch <= 'z'){
      ch = ch - 3;
      }
    else if ( ch == 'a'){
        ch = 'x';
        }
    else if( ch == 'b'){
        ch = 'y';
        }
    else if(ch == 'c'){
       ch = 'z';
       }   
    else if(ch >= 'D' && ch <= 'Z'){
      ch = ch - 3;
      }
    else if ( ch == 'A'){
        ch = 'X';
        }
    else if( ch == 'B'){
        ch = 'Y';
        }
    else if(ch == 'C'){
       ch = 'Z';
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

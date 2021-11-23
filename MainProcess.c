#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    int first_ID = fork();
    int second_ID = fork();

    if( first_ID == 0 && second_ID == 0 ){
        //finder

        char *args[]={"./finder",NULL};
        execvp(args[0],args);
    }else if( first_ID > 0 && second_ID == 0 ){
        //placer
        char *args[]={"./placer",NULL};
        execvp(args[0],args);
    }else if( first_ID == 0 && second_ID > 0 ){
        //decoder
        char *args[]={"./decoder",NULL};
        execvp(args[0],args);

        //while(wait(NULL) > 0);
    }else if( first_ID > 0 && second_ID > 0 ){
        //Main Process

        //Reading from file
        char *filename = "test.txt";
        FILE *fp = fopen(filename, "r");

        if (fp == NULL)
        {
            printf("Error: could not open file %s", filename);
            return 1;
        }

        const int Max_File_Size = 10000;
        char str[Max_File_Size];
        int counter = 0;
        char ch;
        while ((ch = fgetc(fp)) != EOF && counter < Max_File_Size ){
            str[counter] = ch;
            counter++;
        }
        fclose(fp);
        //end - Reading from file

        // find index of  each part's ending
        int partCounter = 0;
        int partDivider[2]; 
        for(int i = 0; i < counter ; i++){
            if(str[i] == '#' && str[i + 1] == '#' && str[i + 2] == '#'){
                partDivider[partCounter] = i - 1;
                partCounter++;
                i = i + 3;
            }
        }   

        //part 1;
        char str_Part1[partDivider[0] + 1];
        memcpy(str_Part1, &str[0], (partDivider[0]+ 1));
        str_Part1[partDivider[0] + 1] = '\0';

        //part 2;
        int part2_size = partDivider[1] - partDivider[0] - 3;
        char str_Part2[part2_size + 1];
        memcpy(str_Part2, &str[partDivider[0] + 4], part2_size);
        str_Part2[part2_size] = '\0';

        //part 3;
        int part3_size = (counter - 1) - partDivider[1] - 3;
        char str_Part3[part3_size + 1];
        memcpy(str_Part3, &str[partDivider[1] + 4], part3_size);
        str_Part3[part3_size] = '\0';

        //send data to decoder
        char mainDecoderFifo[] = "Main_Decoder";
        mkfifo(mainDecoderFifo, 0666);
        int fd_decoder = open(mainDecoderFifo,O_WRONLY);
        write(fd_decoder, str_Part1 , strlen(str_Part1)+1);
        close(fd_decoder);

        //send data to finder
        char mainFinderFifo[] = "Main_Finder";
        mkfifo(mainFinderFifo, 0666);
        int fd_finder = open(mainFinderFifo,O_WRONLY);
        write(fd_finder, str_Part2 , strlen(str_Part2)+1);
        close(fd_finder);


        //send data to finder
        char mainPlacerFifo[] = "Main_Placer";
        mkfifo(mainPlacerFifo, 0666);
        int fd_placer = open(mainPlacerFifo,O_WRONLY);
        write(fd_placer, str_Part3 , strlen(str_Part3)+1);
        close(fd_placer);

        char placerDecoder[] = "Placer_Main";
        int fd = open(placerDecoder, O_RDONLY);
        char finalResult[10000];
        int h = 0;
        read(fd, &finalResult[h], sizeof(char));
        while(str[h] != '\0'){
            h++;
            read(fd, &finalResult[h], sizeof(char));
        }
        close(fd);
        printf("\nfinal Result from MainProcess: %s",finalResult);

        while(wait(NULL) > 0);
    }

    return 0;
}




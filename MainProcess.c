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

    }else if( first_ID > 0 && second_ID == 0 ){
    
    }else if( first_ID == 0 && second_ID > 0 ){

    }else if( first_ID > 0 && second_ID > 0 ){


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
 
    }

    return 0;
}
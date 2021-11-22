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
 
    }

    return 0;
}
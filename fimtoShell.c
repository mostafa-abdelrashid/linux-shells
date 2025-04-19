#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include <sys/wait.h>
int main(){

        char buf[PATH_MAX];
        while(1){

                printf("I am a simple shell, enter your command $ ");
                if(!fgets(buf,PATH_MAX,stdin)){

                fprintf(stderr,"error in fgets");
                exit(-1);

                };


                buf[strlen(buf)-1]=0;
                if(strlen(buf)==0){
                        continue;
                }
                char* cmd1="echo ";
                char* cmd2="exit";


                if(strncmp(buf,cmd1,5)==0){



                printf("%s\n",buf+5);


                }


                else if(strncmp(buf,cmd2,4)==0){

                printf("Good Bye :)\n");
                exit(0);

                }
                else{

                printf("Invalid command\n");

                }

        }

        return 0;
}

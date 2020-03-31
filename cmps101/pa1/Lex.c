//MARK MORENO
//1742618
//pa1

#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include"string.h"
int main(int argc, char* argv[]){
    if (argc!=3){
        printf("Incorrect number of arguments: EXIT");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    //FILE *f = fopen("/Users/markmoreno/Desktop/test.txt", "r");
    
    int ch, lines = 0;
    do
    {
        ch = fgetc(f);
        if(ch == '\n')
            lines++;
    }
    while (ch != EOF);
    if((ch != '\n' )&( lines != 0))
        lines++;
    
    rewind(f);
    
    
    char *contents[lines];
    size_t len = 0;
    for(int i = 0; i < lines; i++)
    {
        contents[i] = NULL;
        len = 0;
        getline(&contents[i], &len, f);
    }
    fclose(f);
    
    
    List A = newList();
    
    for(int i = 0;i<lines;i++){
        if(isEmpty(A)){
            append(A, i);
        }
        else{
            int found = -1;
            moveFront(A);
            while(get(A)>=0){
                if(strcmp(contents[i],contents[get(A)])<0){
                    insertBefore(A, i);
                    found =1;
                    break;
                }
                moveNext(A);
            }
            if(found<0)append(A, i);
        }
        //printList(stdout,A);
    }
    printf("\n");
    //FILE* fp = fopen ("/Users/markmoreno/Desktop/empty.txt","w");
    FILE* fp = fopen (argv[2],"w");
    moveFront(A);
    while(get(A)>=0){
       
        if(get(A)==lines-1)fprintf (fp, "%s\n",contents[get(A)]);
        else fprintf (fp, "%s",contents[get(A)]);
        moveNext(A);
    }
    fclose (fp);
    return 0;
}

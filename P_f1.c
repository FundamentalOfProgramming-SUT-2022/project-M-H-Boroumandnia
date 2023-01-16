#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <unistd.h>
void rootFolder(){
    mkdir("root",777);
}
void strText(){
    printf("\n> This project developed by Mohammad Hossein Boroumandni in C.\n>\n");
}
void createfile(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int filePos,strLen;
    strLen=strlen(command);
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : createfile -–file <file name and address>\n");
        return;
    }
    //check for aditinal char between createfile and --file
    for(int i=0;*(filePtr-i)==' ';i++)
        filePos=i;
     if((filePtr-filePos-10)!=command ){
        printf("> Usage : createfile -–file <file name and address>\n");
        return;
    }
    //check for slash
    slashPtr=strstr(command,"/");
    if(slashPtr==NULL){
        printf("> dir : /root/... or \"/root/...\"\n");
        return;
    }
    //check for qout
    if(*(slashPtr-1)=='"')
        qoutPtr=slashPtr-1;
    if(qoutPtr!=NULL && *(command+strLen-1)!='"'){
        printf("> dir : \"/root/...\"\n");
        return;
    }
    //check for aditinal char between / or " and --file
    if(qoutPtr!=NULL){
        for(int i=0;(qoutPtr-1-i)!=(filePtr+7);i++){
            if(*(qoutPtr-1-i)!=' '){
                printf("> Usage : createfile -–file <file name and address>\n");
                return;
            }
        }  
    }
    else{
        for(int i=0;(slashPtr-1-i)!=(filePtr+7);i++){
            if(*(slashPtr-1-i)!=' '){
                printf("> Usage : createfile -–file <file name and address>\n");
                return;
            }
        }
        for(int i=0;*(slashPtr+i)!=0;i++){
            if(*(slashPtr+i)==' '){
                printf("> Usage : aditinal space detected\n");
                return;
            }
        }
    }
    //make address file
    if(qoutPtr!=NULL){
        for(int i=0;(slashPtr+i+1)!=(command+strLen-1);i++){
            *(address+i)=*(slashPtr+i+1);
        }
    }
    else{
        for(int i=0;(slashPtr+i+1)!=(command+strLen);i++){
            *(address+i)=*(slashPtr+i+1);
        }
    }
    //check for file existence
    if(access(address,F_OK)==0){
        printf("> File exists\n");
        return;
    }
    //make new directories and file
    for(int i=0;*(address+i)!=0;i++){
        if(*(address+i)=='/'){
            char *folder=(char*)malloc(sizeof(char)*200);
            for(int j=0;j<i;j++)
                *(folder+j)=*(address+j);
            mkdir(folder,777);
            free(folder);
        }
    }
    fopen(address,"w+");
    free(address);
}
void firstEndSpaceRemoval(char *command){
    int len;
    for(int i=0;*(command+i)==' ';i++){
        for(int j=0;*(command+j)!=0;j++)
            *(command+j)=*(command+j+1);
        i=-1;
    }
    len=strlen(command);
    for(int i=len-1;*(command+i)==' ';i--)
        *(command+i)=0;
}
void commandDetector(char *command){
    while(1){
        if(!strcmp(command,"end")){
            printf("> Exiting  Programm ...\n\n");
            exit(0);
            break;
        }
        if(strstr(command,"createfile ")==command || !strcmp(command,"createfile")){
            createfile(command);
            break;
            
        }
        else{
            printf("> Invalid command\n");
            break;
        }
    }
}
int main(){
    rootFolder();
    strText();
    char *command;
    while(1){
        command=(char*)malloc(sizeof(char)*300);
        printf("> ");
        fgets(command,300,stdin);
        command[strcspn(command, "\n")] = 0;
        firstEndSpaceRemoval(command);
        //printf("%s",command);
        commandDetector(command);
        free(command);
    }
    return 0;
}

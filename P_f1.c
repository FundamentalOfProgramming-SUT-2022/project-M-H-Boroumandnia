#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <unistd.h>
void rootFolder(){
    mkdir("root",0777);
}
void strText(){
    printf("\n> This project developed by Mohammad Hossein Boroumandni in C.\n>\n");
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
            mkdir(folder,0777);
            free(folder);
        }
    }
    fopen(address,"w+");
    free(address);
}
void insertstr(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL,*lastQoutPtr,*lastCharPtr,*stringPos;
    char *address=(char*)malloc(sizeof(char)*100);
    int filePos;
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : insertstr --file <file name> –str <str> —pos <line no>:<start position>\n");
        return;
    }
    //check for aditinal char between insertstr and --file
    for(int i=0;*(filePtr-i)==' ';i++)
        filePos=i;
    if((filePtr-filePos-9)!=command ){
        printf("> Usage : insertstr --file <file name> –str <str> —pos <line no>:<start position>\n");
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
    //check for aditinal char between " or / and --file
    if(qoutPtr!=NULL){
        for(int i=0;(qoutPtr-1-i)!=(filePtr+7);i++){
            if(*(qoutPtr-1-i)!=' '){
                printf("> Usage : insertstr --file <file name> –str <str> —pos <line no>:<start position>\n");
                return;
            }
        }  
    }
    else{
        for(int i=0;(slashPtr-1-i)!=(filePtr+7);i++){
            if(*(slashPtr-1-i)!=' '){
                printf("> Usage : insertstr --file <file name> –str <str> —pos <line no>:<start position>\n");
                return;
            }
        }
    }
    //check for final qout and -str pos
    long int strCount=0,strCheck=0;
    while(1){
        stringPos=strstr(command+strCount," -str ");
        if(stringPos==NULL){
            printf("> Usage : –str <str> —pos <line no>:<start position> Or dir : /root/... or \"/root/...\"\n");
            return;
        }
        strCheck=0;
        if(qoutPtr!=NULL){
            for(int i=0;*(stringPos-i)!='"';i++){
                if(*(stringPos-i)!=' '){
                    strCount=(stringPos-command)+5;
                    strCheck=-1;
                    break;  
                }
                lastQoutPtr=stringPos-i;
            }
        }
        if(strCheck==0)
            break;
    }
    //check for -str without qout
    if(qoutPtr==NULL){
        stringPos=strstr(command," -str ");
        if(stringPos==NULL){
            printf("Usage : –str <str> —pos <line no>:<start position>\n");
            return;
        }
        for(int i=0;*(stringPos-i)==' ';i++){
            lastCharPtr=stringPos-i-1;
        }
        for(int i=0;(slashPtr+i)<=(lastCharPtr);i++){
            if(*(slashPtr+i)==' '){
                printf("> Usage : aditinal space detected\n");
                return;
            }
        }
    }
    //make address file
    if(qoutPtr!=NULL){
        for(int i=0;(slashPtr+i+1)!=lastQoutPtr-1;i++){
            *(address+i)=*(slashPtr+i+1);
        }
    }
    else{
        for(int i=0;(slashPtr+i+1)!=(lastCharPtr+1);i++){
            *(address+i)=*(slashPtr+i+1);
        }
    }
    //check for file existence
    if(access(address,F_OK)!=0){
        printf("> File dose not exists\n");
        return;
    }
    char *charPtr,*posPos,*signPtr,*lastTextPtr;
    //check the location of first char in string
    for(int i=0;*(stringPos+i+5)!=0;i++){
        if(*(stringPos+i+5)!=' '){
            charPtr=stringPos+i+5;
            break;
        }
    }
    //check for -pos
    long int posCount=0,posCheck=0;
    while(1){
        posPos=strstr(stringPos+5+posCount," -pos ");
        if(posPos==NULL){
            printf("> Usage : -pos <line no>:<start position>\n");
            return;
        }
        posCheck=0;
        for(int i=0;*(posPos+5+i)!=0;i++){
            if((*(posPos+i+5)<48 || *(posPos+i+5)>58) && *(posPos+i+5)!=' '){
                posCount=(posPos-stringPos);
                posCheck=-1;
                break;  
            }
        }
        if(posCheck==0)
            break;
    }
    //check the location of last char in string
    for(int i=0;*(posPos-i)==' ';i++)
        lastTextPtr=posPos-i-1;
    //save text in a dynamic arrey
    char *text=(char*)malloc(sizeof(char)*300);
    if((*lastTextPtr)=='"' && *(charPtr)=='"'){
        for(int i=0;(charPtr+i+1)!=lastTextPtr;i++)
            *(text+i)=*(charPtr+i+1);
    }
    else{
        for(int i=0;(charPtr+i)!=lastTextPtr+1;i++)
            *(text+i)=*(charPtr+i);
    }
    //check for aditinal space foe text without qout
    if(*(lastTextPtr)!='"' ||  *(charPtr)!='"'){
        for(int i=0;(charPtr+i)!=lastTextPtr;i++){
            if(*(charPtr+i)==' '){
                printf("> Usage : aditinal space detected\n");
                return;
            }
        }
    }
    char *lineNum=(char*)malloc(sizeof(char)*30);
    char *charNum=(char*)malloc(sizeof(char)*30);
    int signNum=0,line=0,character=0;
    //number of sign : check
    for(int i=0;*(posPos+i+5)!=0;i++){
        if(*(posPos+i+5)==':'){
            signPtr=(posPos+i+5);
            signNum++;
        }
    }
    if(signNum!=1){
        printf("> Usage : -pos <line no>:<start position>\n");
        return;
    }
    //save line number and char number
    for(int i=0;(posPos+i+5)!=signPtr;i++)
       *(lineNum+i)=*(posPos+i+5);
    for(int i=0;*(signPtr+i+1)!=0;i++)
       *(charNum+i)=*(signPtr+i+1);
    firstEndSpaceRemoval(lineNum);
    firstEndSpaceRemoval(charNum);
    //check for aditinal spaces in line number and char number
    for(int i=0;*(lineNum+i)!=0;i++){
        if(*(lineNum+i)==' '){
            printf("> Usage : -pos <line no>:<start position>\n");
            return;
        }
    }
    for(int i=0;*(charNum+i)!=0;i++){
        if(*(charNum+i)==' '){
            printf("> Usage : -pos <line no>:<start position>\n");
            return;
        }
    }
    //change sring to numbers
    line=atoi(lineNum);
    character=atoi(charNum);
    //file making 
    FILE *letsWrite;
    //int endText=0,lastLine=0,lastChar=0;
    char *buffer=(char*)malloc(sizeof(char)*1000);
    letsWrite=fopen(address,"w+");
    fseek(letsWrite, 0, SEEK_SET);
    fread(buffer,1000,1,letsWrite);
    int addLine=1,addChar=0,whereToWrite=0;
    for(int i=0;;i++){
        if(*(buffer+i)=='\n')
            addLine++;
        if(addLine==line)
            addChar++;
        if(addChar==character){
            whereToWrite=i;
            break;     
        }
    }
    printf("%d",whereToWrite);
    fseek(letsWrite, 0, SEEK_SET);
    for(int i=0;i<=whereToWrite;i++)
        fputc(*(buffer+i),letsWrite);
        
    fclose(letsWrite); 
}
void cat(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int filePos,strLen;
    strLen=strlen(command);
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : cat -–file <file name and address>\n");
        return;
    }
    //check for aditinal char between cat  and --file
    for(int i=0;*(filePtr-i)==' ';i++)
        filePos=i;
     if((filePtr-filePos-3)!=command ){
        printf("> Usage : cat -–file <file name and address>\n");
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
                printf("> Usage : cat --file <file name and address>\n");
                return;
            }
        }
    }
    else{
        for(int i=0;(slashPtr-1-i)!=(filePtr+7);i++){
            if(*(slashPtr-1-i)!=' '){
                printf("> Usage : cat -–file <file name and address>\n");
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
    if(access(address,F_OK)!=0){
        printf("> File dose not exists\n");
        return;
    }
    //split file name
    char* fileName=(char*)malloc(sizeof(char)*200);
    int lastSlash=0;
    for(int i=0;*(address+strlen(address)-1-i)!='/';i++)
        lastSlash=strlen(address)-1-i;
    for(int i=0;*(address+lastSlash+i)!=0;i++)
        *(fileName+i)=*(address+lastSlash+i);
    //open file and put the read function in buffer
    FILE *letsSee;
    char* buffer=(char*)malloc(sizeof(char)*1000);
    letsSee=fopen(address,"r");
    fseek(letsSee,0,SEEK_SET);
    fread(buffer,1000,1,letsSee);
    printf("// %s\n%s\n",fileName,buffer);

}
void commandDetector(char *command){
    if(!strcmp(command,"end")){
         printf("> Exiting  Programm ...\n\n");
         exit(0);
         return;
     }
     if(strstr(command,"createfile ")==command || !strcmp(command,"createfile")){
         createfile(command);
         return;
     }
    if(strstr(command,"insertstr ")==command || !strcmp(command,"insertstr")){
         insertstr(command);
         return;
     }
    if(strstr(command,"cat ")==command || !strcmp(command,"cat")){
         cat(command);
         return;
     }
     else{
        printf("> Invalid command\n");
        return;;
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

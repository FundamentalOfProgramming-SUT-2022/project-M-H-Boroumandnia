#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <unistd.h>
#include<dirent.h>
void rootFolder(){
    mkdir("root",0777);
    FILE *file;
    mkdir(".system",0777);
    file=fopen(".system/Clipboard","w+");
    fclose(file);
}
void strText(){
    printf("\n> This project developed by Mohammad Hossein Boroumandni in C.\n>\n");
}
void beforChanges(char *address){
    char *buffer=(char*)malloc(sizeof(char)*1000);
    char *newAddress=(char*)malloc(sizeof(char)*100);
    *(newAddress)='.';*(newAddress+1)='s';*(newAddress+2)='y';*(newAddress+3)='s';*(newAddress+4)='t';*(newAddress+5)='e';*(newAddress+6)='m';*(newAddress+7)='/';
    for(int i=0;*(address+i)!=0;i++)
        *(newAddress+8+i)=*(address+i);
    FILE *beforChanges,*Undo;
    beforChanges=fopen(address,"r");
    fseek(beforChanges,0,SEEK_SET);
    fread(buffer,1000,1,beforChanges);
    fclose(beforChanges);
    Undo=fopen(newAddress,"w+");
    fseek(beforChanges,0,SEEK_SET);
    fputs(buffer,Undo);
    fclose(Undo);
    free(newAddress);
    free(buffer);
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
char * commandMaker(char *command,char *position,char *inputString){
    char *newCommand=(char*)malloc(sizeof(char)*500);
    int counter=0,bufferCounter=0;
    for(int i=0;;i++){
        *(newCommand+i)=*(command+i);
        bufferCounter=i+1;
        if(command+i==position){
            *(newCommand+i+1)=' ';
            *(newCommand+i+2)='-';
            *(newCommand+i+3)='s';
            *(newCommand+i+4)='t';
            *(newCommand+i+5)='r';
            *(newCommand+i+6)=' ';
            *(newCommand+i+7)='"';
            for(int j=0;*(inputString+j)!=0;j++){
                *(newCommand+i+j+8)=*(inputString+j);
                counter=i+j+9;
            }
            *(newCommand+counter)='"';
            *(newCommand+counter+1)=' ';
            break;
        }
    }
    for(int i=0;*(command+bufferCounter+i)!=0;i++)
        *(newCommand+i+counter+2)=*(command+i+bufferCounter);
    return newCommand;
}
int  PathCheck(char *address,int grep){
    int check=1;
    int lastSlash=0;
    char *dirPath=(char*)malloc(sizeof(char)*100);
    for(int i=0;*(address+i)!=0;i++)
        if(*(address+i)=='/')
            lastSlash=i;
    for(int i=0;i<lastSlash;i++)
        *(dirPath+i)=*(address+i);
    if(access(dirPath,X_OK)!=0){
        if(grep==0){
            printf("> Path dose not exists\n");
            return 0;
        }
        if(grep==1)
            return 0;
    }
    if(access(address,F_OK)!=0){
        if(grep==0){
            printf("> File dose not exists\n");
            return 0;   
        }
        if(grep==1)
            return -1;
    }
    return check;
}
int specialCharHandeling(char *string){
    //check for special character
     for(int i=0;*(string+i+1)!=0;i++){
        if(*(string+i)=='\\' && ((*(string+i+1)!='t') &&  (*(string+i+1)!='n') && (*(string+i+1)!='"') && (*(string+i+1)!='*'))){
            printf("> Usage : undefined character detecteds\n");
            return 0;
        }
        if((*(string+i+1)=='"' && *(string+i)!='\\') || *(string)=='"'){
            printf("> Usage : undefined character detecteds\n");
            return 0;
        }
    }
    for(int i=0;*(string+i+1)!=0;i++){
        //check for \t
        if(*(string+i)=='\\' && (*(string+i+1)=='t')){
            *(string+i)='\t';
            for(int j=i+1;*(string+j)!=0;j++){
                *(string+j)=*(string+j+1);
            }
            i=0;
        }
        //check for \n
        if(*(string+i)=='\\' && (*(string+i+1)=='n')){
            *(string+i)='\n';
            for(int j=i+1;*(string+j)!=0;j++){
                *(string+j)=*(string+j+1);
            }
            i=0;
        }
        //check for \"
        if(*(string+i)=='\\' && (*(string+i+1)=='"')){
            *(string+i)='"';
            for(int j=i+1;*(string+j)!=0;j++){
                *(string+j)=*(string+j+1);
            }
            i=0;
        }
        //check for \*
        if(*(string+i)=='\\' && (*(string+i+1)=='*')){
            *(string+i)='*';
            for(int j=i+1;*(string+j)!=0;j++){
                *(string+j)=*(string+j+1);
            }
            i=0;
        }
    }
    return 1;
}
void createfile(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int strLen;
    strLen=strlen(command);
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : createfile -–file <file name and address>\n");
        return;
    }
    //check for aditinal char between createfile and --file
    for(int i=0;(command+i+10)<=filePtr;i++)
        if(*(command+i+10)!=' '){
            printf("> Usage : createfile -–file <file name and address>\n");
            return;
    }
    //check for slash
    slashPtr=strstr(command,"/");
    if(slashPtr==NULL || *(slashPtr+1)==0){
        printf("> dir : /root/... or \"/root/...\"\n");
        return;
    }
    //check for qout
    if(*(slashPtr-1)=='"')
        qoutPtr=slashPtr-1;
    if((qoutPtr!=NULL && *(command+strLen-1)!='"') || (*(command+strLen-2)=='\\' && *(command+strLen-1)=='"')){
        printf("> dir : \"/root/...\"\n");
        return;
    }
    //check for aditinal char between / or " and --file and special character
    if(qoutPtr!=NULL){
        for(int i=0;(qoutPtr)>(filePtr+7+i);i++)
            if(*(filePtr+7+i)!=' '){
                printf("> Usage : createfile -–file <file name and address>\n");
                return;
            }
    }
    else{
        for(int i=0;(slashPtr)>(filePtr+7+i);i++){
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
    //check for special character
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    //check for file existence
    if(access(address,F_OK)==0){
        printf("> File exists\n");
        return;
    }
    //make new directories and file
    char *newAddress=(char*)malloc(sizeof(char)*100);
      *(newAddress)='.';*(newAddress+1)='s';*(newAddress+2)='y';*(newAddress+3)='s';*(newAddress+4)='t';*(newAddress+5)='e';*(newAddress+6)='m';*(newAddress+7)='/';   
      for(int i=0;*(address+i)!=0;i++)
        *(newAddress+8+i)=*(address+i);
    for(int i=0;*(address+i)!=0;i++){
        if(*(address+i)=='/'){
            char *folder=(char*)malloc(sizeof(char)*200);
            for(int j=0;j<i;j++)
                *(folder+j)=*(address+j);
            mkdir(folder,0777);
            free(folder);
        }
    }
    //make undo mode file
    for(int i=0;*(newAddress+i)!=0;i++){
        if(*(newAddress+i)=='/'){
            char *folder=(char*)malloc(sizeof(char)*200);
            for(int j=0;j<i;j++)
                *(folder+j)=*(newAddress+j);
            mkdir(folder,0777);
            free(folder);
        }
    }
    fopen(newAddress,"w+");
    fopen(address,"w+");
    free(address);
}
void insertstr(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL,*lastChar=NULL,*strQoutPtr=NULL,*lastCharPtr=NULL,*stringPos=NULL,*posPos=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    char *text=(char*)malloc(sizeof(char)*300);
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
    if(qoutPtr!=NULL)
        for(int i=0;*(slashPtr+i+1)!=0;i++){
            if(*(slashPtr+i+1)=='"' && *(slashPtr+i+2)!=0 && *(slashPtr+i)!='\\'){
                lastChar=(slashPtr+i+2);
                break;
            }
            *(address+i)=*(slashPtr+i+1);
        }
    else
        for(int i=0;*(slashPtr+i+1)!=0;i++){
            if(*(slashPtr+i+1)==' '){
                lastChar=(slashPtr+i+1);
                break;
            }
            *(address+i)=*(slashPtr+i+1);
        }
    //check for the command after address
    if(lastChar==NULL){
        printf("> Usage : –str <str> —pos <line no>:<start position> Or dir : /root/... or \"/root/...\"\n");
        return;
    }
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    //find str pos
    stringPos=strstr(lastChar," -str ");
    if(stringPos==NULL){
            printf("> Usage : –str <str> —pos <line no>:<start position> Or dir : /root/... or \"/root/...\"\n");
            return;
    }
    //find last char of string
    for(int i=0;*(stringPos+5+i)!=0;i++)
        if(*(stringPos+5+i)!=' '){
            strQoutPtr=(stringPos+5+i);
            break;
        }
    //check for the command after string
    if(strQoutPtr==NULL){
        printf("> Usage : –str <str> —pos <line no>:<start position>");
        return;
    }
    if(*(strQoutPtr)=='"')
        for(int i=0;*(strQoutPtr+i+1)!=0;i++){
            if(*(strQoutPtr+i+1)=='"' && *(strQoutPtr+i+2)!=0 && *(strQoutPtr+i)!='\\'){
                lastCharPtr=(strQoutPtr+i+2);
                break;
            }
            *(text+i)=*(strQoutPtr+i+1);
        }
    else
        for(int i=0;*(strQoutPtr+i)!=0;i++){
            if(*(strQoutPtr+i)==' '){
                lastCharPtr=(strQoutPtr+i);
                break;
            }
            *(text+i)=*(strQoutPtr+i);
        }
    if(lastCharPtr==NULL){
        printf("> Usage : –str <str> —pos <line no>:<start position>\n");
        return;
    }
    if(specialCharHandeling(text)==0)
        return;
    //check for pos
    posPos=strstr(lastCharPtr," -pos ");
    if(posPos==NULL){
        printf("> Usage : –str <str> —pos <line no>:<start position>\n");
        return;
    }
    //check for aditional spaces
    for(int i=0;(lastCharPtr+i)<=posPos;i++)
        if(*(lastCharPtr+i)!=' '){
            printf("> Usage : –str <str> —pos <line no>:<start position>\n");
            return;
        }
    //check for numbers and :
    for(int i=0;*(posPos+5+i)!=0;i++)
        if((*(posPos+i+5)<48 || *(posPos+i+5)>58) && *(posPos+i+5)!=' '){
            printf("> Usage : —pos <line no>:<start position>\n");
            return;
        }
    char *signPtr;
    int signNum=0,line=0,character=0;
    char *lineNum=(char*)malloc(sizeof(char)*30);
    char *charNum=(char*)malloc(sizeof(char)*30);
    //check for number of :
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
    for(int i=0;(posPos+i+5)<signPtr;i++)
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
    if(line==0){
        printf("> Usage : Line dose not exists\n");
        return;
    }
    //check for file existence
    if(PathCheck(address,0)==0)
        return;
    //make file
    beforChanges(address);
    FILE *letsWrite;
    char *buffer=(char*)malloc(sizeof(char)*1000);
    char *newBuffer=(char*)malloc(sizeof(char)*1000);
    letsWrite=fopen(address,"r+");
    fread(buffer,1000,1,letsWrite);
    //make a new pointer to hold edited test
    int lineCount=0,charCount=0,counter=0,finF=0;
    //put the string until the specific line
    for(int i=0;;i++){
        if(*(buffer+i)=='\n')
            lineCount++;
        if(*(buffer+i)!=0)
            *(newBuffer+i)=*(buffer+i);
        counter=i;
        if(line==1)
            counter--;
        if(lineCount==line-1)
            break;
        if(*(buffer+i)==0){
            finF=1;
            for(int j=i;lineCount<line-1;j++){
                *(newBuffer+j)='\n';
                lineCount++;
                counter=j;
            }
            for(int j=counter+1;charCount<=character-1;j++){
                *(newBuffer+j)=' ';
                charCount++;
                counter=j;
            }
            for(int j=0;*(text+j)!=0;j++)
                *(newBuffer+j+counter+1)=*(text+j);
            break;
        }
    }
    //put the string until specific char
    int bufferCounter=0;
    if(finF==0){
        for(int i=counter+1;;i++){
            if(charCount<character+1 && (*(buffer+i)!='\n' && *(buffer+i)!=0)){
                *(newBuffer+i)=*(buffer+i);
                charCount++;
                bufferCounter=i;
            }
            if(charCount<character+2 && (*(buffer+i)=='\n'|| *(buffer+i)==0)){
                 bufferCounter=i;
                if(*(buffer+i)==0)
                    finF=1;
                for(int j=0;charCount<=character+1;j++){
                    if(charCount==character || charCount==character+1){
                        for(int k=0;*(text+k)!=0;k++){
                            *(newBuffer+i+j+k)=*(text+k);
                            counter=i+j+k;
                        }
                        break;
                    }
                    *(newBuffer+i+j)=' ';
                    charCount++;
                }
                break;
            }
            if(charCount==character+1){
                for(int j=0;*(text+j)!=0;j++){
                    *(newBuffer+i+j)=*(text+j);
                    counter=i+j;
                }
                break;
            }
        }
    }
    //put the last part of buffer
    if(finF==0)
        for(int i=0;*(buffer+i+bufferCounter)!=0;i++)
            *(newBuffer+counter+i+1)=*(buffer+i+bufferCounter);
    //overwrite and close file
    fseek(letsWrite,0,SEEK_SET);
    fputs(newBuffer,letsWrite);
    fclose(letsWrite);
}
void pastestr(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL,*lastChar=NULL,*posPos=NULL;
    int filePos;
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : pastestr –file <file name> –pos <line no>:<start position>\n");
        return;
    }
    //check for aditinal char between insertstr and --file
    for(int i=0;*(filePtr-i)==' ';i++)
        filePos=i;
    if((filePtr-filePos-8)!=command ){
        printf("> Usage : pastestr –file <file name> –pos <line no>:<start position>\n");
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
                printf("> Usage : pastestr –file <file name> –pos <line no>:<start position>\n");
                return;
            }
        }
    }
    else{
        for(int i=0;(slashPtr-1-i)!=(filePtr+7);i++){
            if(*(slashPtr-1-i)!=' '){
                printf("> Usage : pastestr –file <file name> –pos <line no>:<start position>\n");
                return;
            }
        }
    }
    //check for final qout
    if(qoutPtr!=NULL){
        for(int i=0;*(slashPtr+i+1)!=0;i++)
            if(*(slashPtr+i+1)=='"' && *(slashPtr+i+2)!=0 && *(slashPtr+i)!='\\'){
                lastChar=(slashPtr+i+2);
                break;
            }
    }
    else{
        for(int i=0;*(slashPtr+i+1)!=0;i++)
            if(*(slashPtr+i+1)==' '){
                lastChar=(slashPtr+i+1);
                break;
            }
    }
    //check for the command after address
    if(lastChar==NULL){
        printf("> Usage : –pos <line no>:<start position>\n");
        return;
    }
    //check for -pos position
    posPos=strstr(lastChar," -pos ");
    if(posPos==NULL){
        printf("> Usage : –pos <line no>:<start position>\n");
        return;
    }
    for(int i=0;(lastChar+i)<=posPos;i++)
        if(*(lastChar+i)!=' '){
        printf("> Usage : –pos <line no>:<start position>\n");
        return;
        }
    //make a file 
    FILE *file;
    char *buffer=(char*)malloc(sizeof(char)*1000);
    file=fopen("system/Clipboard","r+");
    fread(buffer,1000,1,file);
    if(*(buffer)==0){
        printf("> Usage : Clipboard is empty\n");
        return;
    }
    //change command to insertstr
    char *commandHolder;
    char *newBuffer=(char*)malloc(sizeof(char)*500);
    commandHolder=commandMaker(command,lastChar,buffer);
    *(newBuffer)='i';*(newBuffer+1)='n';*(newBuffer+2)='s';*(newBuffer+3)='e';*(newBuffer+4)='r';*(newBuffer+5)='t';
    for(int i=0;*(commandHolder+i+5)!=0;i++)
        *(newBuffer+i+6)=*(commandHolder+i+5);
    insertstr(newBuffer);
    fclose(file);
    free(newBuffer);
    free(buffer);
}
void cat(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int strLen;
    strLen=strlen(command);
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : cat -–file <file name and address>\n");
        return;
    }
    //check for aditinal char between cat  and --file
    for(int i=0;(command+3+i)<=filePtr;i++)
        if(*(command+3+i)!=' '){
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
        for(int i=0;(qoutPtr)>(filePtr+7+i);i++){
            if(*(filePtr+7+i)!=' '){
                printf("> Usage : cat --file <file name and address>\n");
                return;
            }
        }
    }
    else{
        for(int i=0;(slashPtr)>(filePtr+7+i);i++){
            if(*(filePtr+7+i)!=' '){
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
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    //check for file existence
    if(PathCheck(address,0)==0)
        return;
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
    free(buffer);
    free(fileName);
    free(address);
}
void copystr(char* address,int line,int character,int size,int type){
    FILE *letsCopy;
    char *buffer=(char*)malloc(sizeof(char)*1000);
    char *newBuffer=(char*)malloc(sizeof(char)*1000);
    int lineCount=0,charCount=0,counter=0;
    letsCopy=fopen(address,"r+");
    fseek(letsCopy,0,SEEK_SET);
    fread(buffer,1000,1,letsCopy);
    //check for the position and make a newBuffer
    for(int i=0;;i++){
        if(*(buffer+i)=='\n')
            lineCount++;
        if(*(buffer+i)==0 && lineCount<line-1){
            printf("> Usage : Position is out of order\n");
            return;
        }
        if(line==1)
            i--;
        if(lineCount==line-1){
            for(int j=1;;j++){
                if((*(buffer+i+j)==0 || (*(buffer+i+j)=='\n' && character!=charCount)) && character>=charCount){
                    printf("> Usage : Position is out of order\n");
                    return;
                }
                if(charCount==character){
                    counter=i+j;
                    break;
                }
                charCount++;
            }
            break;
        }
    }
    //check for bigger sizes
    if(counter+size*type>(strlen(buffer)-1)){
        printf("> Usage : Size is out of order\n");
        return;
    }
    if((counter+size*type)<0){
        printf("> Usage : Size is out of order\n");
        return;
    }
    //copy another part of buffer to newBuffer
    if(type==1)
        for(int i=0;i<size;i++)
            *(newBuffer+i)=*(buffer+counter+i+1);
    else
        for(int i=0;i<size;i++)
            *(newBuffer+i)=*(buffer+counter+i-size);
    fclose(letsCopy);
    letsCopy=fopen("system/Clipboard","w+");
    fputs(newBuffer,letsCopy);
    fclose(letsCopy);
    free(newBuffer);
    free(buffer);
}
void cutstr(char* address,int line,int character,int size,int type){
    copystr(address,line,character,size,type);
}
void removestr_copy_cut(char *command,int n){
    char *filePtr,*slashPtr,*qoutPtr=NULL,*lastChar=NULL,*signPtr=NULL,*sizePtr=NULL,*typeBPos=NULL,*typeFPos=NULL,*posPos=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int filePos;
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        if(n==9)
            printf("> Usage : removestr --file <file name> –pos <line no>:<start position> -size <number of characters to remove> -f -b <forward or backward>\n");
        if(n==7)
             printf("> Usage : copystr --file <file name> –pos <line no>:<start position> -size <number of characters to copy> -f -b <forward or backward>\n");
        if(n==6)
             printf("> Usage : cutstr --file <file name> –pos <line no>:<start position> -size <number of characters to cut> -f -b <forward or backward>\n");
        return;
    }
    //check for aditinal char between insertstr and --file
    for(int i=0;*(filePtr-i)==' ';i++)
        filePos=i;
    if((filePtr-filePos-n)!=command ){
        if(n==9)
            printf("> Usage : removestr --file <file name> –pos <line no>:<start position> -size <number of characters to remove> -f -b <forward or backward>\n");
        if(n==7)
             printf("> Usage : copystr --file <file name> –pos <line no>:<start position> -size <number of characters to copy> -f -b <forward or backward>\n");
        if(n==6)
             printf("> Usage : cutstr --file <file name> –pos <line no>:<start position> -size <number of characters to cut> -f -b <forward or backward>\n");
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
                if(n==9)
                    printf("> Usage : removestr --file <file name> –pos <line no>:<start position> -size <number of characters to remove> -f -b <forward or backward>\n");
                if(n==7)
                    printf("> Usage : copystr --file <file name> –pos <line no>:<start position> -size <number of characters to copy> -f -b <forward or backward>\n");
                if(n==6)
                    printf("> Usage : cutstr --file <file name> –pos <line no>:<start position> -size <number of characters to cut> -f -b <forward or backward>\n");
                return;
            }
        }
    }
    else{
        for(int i=0;(slashPtr-1-i)!=(filePtr+7);i++){
            if(*(slashPtr-1-i)!=' '){
                 if(n==9)
                    printf("> Usage : removestr --file <file name> –pos <line no>:<start position> -size <number of characters to remove> -f -b <forward or backward>\n");
                if(n==7)
                    printf("> Usage : copystr --file <file name> –pos <line no>:<start position> -size <number of characters to copy> -f -b <forward or backward>\n");
                if(n==6)
                    printf("> Usage : cutstr --file <file name> –pos <line no>:<start position> -size <number of characters to cut> -f -b <forward or backward>\n");
                return;
            }
        }
    }
    //check for final qout
    if(qoutPtr!=NULL)
        for(int i=0;*(slashPtr+i+1)!=0;i++){
            if(*(slashPtr+i+1)=='"' && *(slashPtr+i+2)!=0 && *(slashPtr+i)!='\\'){
                lastChar=(slashPtr+i+2);
                break;
            }
            *(address+i)=*(slashPtr+i+1);
        }
    else
        for(int i=0;*(slashPtr+i+1)!=0;i++){
            if(*(slashPtr+i+1)==' '){
                lastChar=(slashPtr+i+1);
                break;
            }
            *(address+i)=*(slashPtr+i+1);
        }
    //check for the command after address
    if(lastChar==NULL){
        printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
        return;
    }
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    //check for -pos position
    posPos=strstr(lastChar," -pos ");
    if(posPos==NULL){
        printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
        return;
    }
    for(int i=0;(lastChar+i)<=posPos;i++)
        if(*(lastChar+i)!=' '){
        printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
        return;
        }
    //check for : sign
    int line=0,character=0,type=0,size=0;
    char *lineNum=(char*)malloc(sizeof(char)*30);
    char *charNum=(char*)malloc(sizeof(char)*30);
    char *sizeOfMove=(char*)malloc(sizeof(char)*30);
    signPtr=strstr(posPos+5,":");
    if(signPtr==NULL){
        printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
        return;
    }
    //check for aditinal char between numbers
    for(int i=0;(posPos+5+i)<signPtr;i++){
        if((*(posPos+5+i)<48 || *(posPos+5+i)>57) && *(posPos+5+i)!=' '){
            printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
            return;
        }
        *(lineNum+i)=*(posPos+5+i);
    }
    //check for -size position
    sizePtr=strstr(posPos+5," -size ");
    if(sizePtr==NULL){
        printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
        return;
    }
    for(int i=0;(signPtr+i+1)<=sizePtr;i++){
        if((*(signPtr+1+i)<48 || *(signPtr+1+i)>57) && *(signPtr+1+i)!=' '){
            printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
            return;
        }
        *(charNum+i)=*(signPtr+1+i);
    }
    firstEndSpaceRemoval(lineNum);
    firstEndSpaceRemoval(charNum);
    //check for additional spaces
    for(int i=0;*(lineNum+i)!=0;i++)
        if(*(lineNum+i)==' '){
            printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
            return;
        }
    for(int i=0;*(charNum+i)!=0;i++)
        if(*(lineNum+i)==' '){
            printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
            return;
        }
    //make number out of strings
    line=atoi(lineNum);
    character=atoi(charNum);
    if(line==0){
        printf("> Usage : Line dose not exists\n");
        return;
    }
    typeBPos=strstr(sizePtr+6," -b\0");
    typeFPos=strstr(sizePtr+6," -f\0");
    if(typeBPos==NULL && typeFPos==NULL){
        printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
        return;
    }
    //check for -b or -f
    if(typeBPos!=NULL)
        type=-1;
    else
        type=1;
    //check for aditinal char between tpye and number
    if(typeBPos!=NULL){
        for(int i=0;(sizePtr+6+i)<=typeBPos;i++){
            if((*(sizePtr+6+i)<48 || *(sizePtr+6+i)>57) && *(sizePtr+6+i)!=' '){
            printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
            return;
        }
        *(sizeOfMove+i)=*(sizePtr+6+i);
        }
    }
    else{
        for(int i=0;(sizePtr+6+i)<=typeFPos;i++){
            if(*(sizePtr+6+i)<48 && *(sizePtr+6+i)>57 && *(sizePtr+6+i)!=' '){
                printf("> Usage : –pos <line no>:<start position> -size <number of characters> -f -b <forward or backward>\n");
                return;
        }
        *(sizeOfMove+i)=*(sizePtr+6+i);
        }
    }
    //take size number
    firstEndSpaceRemoval(sizeOfMove);
    for(int i=0;*(sizeOfMove+i)!=0;i++)
        if(*(sizeOfMove+i)==' '){
            printf("> Usage : size <number of characters> -f -b <forward or backward>\n");
            return;
        }
    size=atoi(sizeOfMove);
    if(PathCheck(address,0)==0)
        return;
    if(n==7){
        copystr(address,line,character,size,type);
        return;
    }
    if(n==6){
        cutstr(address,line,character,size,type);
    }
    beforChanges(address);
    //make file and open it in read mode
    FILE *letsDelete;
    char *buffer=(char*)malloc(sizeof(char)*1000);
    char *newBuffer=(char*)malloc(sizeof(char)*1000);
    int lineCount=0,charCount=0,counter=0;
    letsDelete=fopen(address,"r+");
    fseek(letsDelete,0,SEEK_SET);
    fread(buffer,1000,1,letsDelete);
    //check for the position and make a newBuffer
    for(int i=0;;i++){
        if(*(buffer+i)=='\n')
            lineCount++;
        if(*(buffer+i)!=0)
            *(newBuffer+i)=*(buffer+i);
        if(*(buffer+i)==0 && lineCount<line-1){
            printf("> Usage : Position is out of order\n");
            return;
        }
        if(line==1)
            i--;
        if(lineCount==line-1){
            for(int j=1;;j++){
                if((*(buffer+i+j)==0 || (*(buffer+i+j)=='\n' && character!=charCount)) && character>=charCount){
                    printf("> Usage : Position is out of order\n");
                    return;
                }
                *(newBuffer+i+j)=*(buffer+i+j);
                if(charCount==character){
                    counter=i+j;
                    if(counter<0){
                        printf("> Usage : Size is out of order\n");
                        return;
                    }
                    break;
                }
                charCount++;
            }
            break;
        }
    }
    //check for bigger sizes
    if(counter+size*type>(strlen(buffer)-1)){
        printf("> Usage : Size is out of order\n");
        return;
    }
    if((counter+size*type)<0){
        printf("> Usage : Size is out of order\n");
        return;
    }
    int lastPar=0;
    //atatch another part of buffer to newBuffer
    if(type==1){
        for(int i=0;*(buffer+i+counter+size*type+1)!=0;i++)
            *(newBuffer+i+counter+1)=*(buffer+counter+size*type+i+1);
    }
    else{
        for(int i=0;*(buffer+i+counter)!=0;i++){
            *(newBuffer+i+counter+size*type)=*(buffer+counter+i);
            lastPar=i+counter+size*type;
        }
         *(newBuffer+lastPar+1)=0;
    }
    fclose(letsDelete);
    //open in write mode and overwrite it
    letsDelete=fopen(address,"w+");
    fseek(letsDelete,0,SEEK_SET);
    fputs(newBuffer,letsDelete);
    fclose(letsDelete);
    free(address);
    free(lineNum);
    free(charNum);
    free(buffer);
    free(newBuffer);
}
int wildCard(char *buffer,char *search){
    int check=1,counter=0,counterP=0,counterPO=0,start=0;
    for(int i=0;;i++){
        if(*(search+i)==0){
            if(*(buffer+counter)!=0 && *(buffer+counter)!='\n' &&  *(buffer+counter)!=' '){
                i=0;
            }
            else if((start!=0 &&  *(buffer+start-1)!=' ' && *(buffer+start-1)!='\n')){
                i=0;
            }
            else{
                check=1;
                break;
            }
        }
        if(*(buffer+counter)==0){
            check=0;
            break;
        }
        if(*(search+i)=='*' && (*(search+i+1)==' ' || *(search+i+1)==0)){
            for(int j=0;*(buffer+j+counter)!=0;j++){
                if(*(buffer+j+counter)==' ' || *(buffer+j+counter)=='\n' || *(buffer+j+counter)==0)
                    break;
                counterP=j+counter;
            }
            counter=counterP+1;
            continue;
        }
        if(*(search+i)=='*' && (i==0 || *(search+i-1)==' ') && ((*(search+i+1)!=' ' && *(search+i+1)!=0))){
            for(int j=0;*(buffer+j+counter)!=0;j++){
                if(*(buffer+j+counter)==' ' || *(buffer+j+counter)=='\n' || *(buffer+j+counter)==0){
                    for(int k=0;;k++){
                        counterPO=i+k-1;
                        if(*(search+i+k)==0 || *(search+i+k)==' ' || *(search+i+k)=='\n')
                            break;
                    }
                    for(int k=0;*(search+counterPO-k)!='*';k++){
                        if(*(buffer+j+counter-1-k)!=*(search+counterPO-k)){
                            i=0;
                            start=counter;
                            break;
                        }
                    }
                    break;
                }
                counterP=j+counter;
            }
            counter=counterP+1;
            i=counterPO;
            continue;
        }
        if(*(search+i)!=*(buffer+counter)){
            i=0;
            start=counter;
        }
        counter++; 
    }
    if(check==1)
        return start;
    else
        return -1;
}
void find(char *command){
    char *filePos=NULL,*stringPos=NULL,*firstChar=NULL,*stringQoutPtr=NULL,*posDistance=NULL;
    int typePos=0;
    char *text=(char*)malloc(sizeof(char)*300);
    char *address=(char*)malloc(sizeof(char)*300);
    stringPos=strstr(command," -str ");
    if(stringPos==NULL){
        printf("> Usage : find –str <str> --file <file name>\n" );
        return;
    }
    for(int i=0;(command+4+i)<=stringPos;i++)
        if(*(command+4+i)!=' '){
            printf("> Usage : find –str <str> --file <file name>\n" );
            return;
        }
    for(int i=0;*(stringPos+5+i)!=0;i++){
        if(*(stringPos+5+i)!=' ' && *(stringPos+5+i)!='"'){
            firstChar=(stringPos+5+i);
            break;
        }
        if(*(stringPos+5+i)=='"'){
            stringQoutPtr=(stringPos+5+i);
            break;
        }
    }
    if(stringQoutPtr==NULL && firstChar==NULL){
        printf("> Usage : find –str <str> --file <file name>\n" );
        return;
    } 
    if(firstChar!=NULL)
        for(int i=0;*(firstChar+i)!=0;i++){
            if(*(firstChar+i)==' '){
                posDistance=(firstChar+i);
                break;
            }
            *(text+i)=*(firstChar+i);
        }
    if(stringQoutPtr!=NULL)
        for(int i=0;*(stringQoutPtr+i+1)!=0;i++){
            if(*(stringQoutPtr+i+1)=='"'){
                posDistance=(stringQoutPtr+i+1);
                break;
            }
        *(text+i)=*(stringQoutPtr+i+1);
        }
    if(posDistance==NULL){
        printf("> Usage : find –str <str> --file <file name>\n" );
        return;
    }
    filePos=strstr(posDistance," --file ");
    if(filePos==NULL){
        printf("> Usage : find –str <str> --file <file name>\n" );
        return;
    }
    for(int i=0;(posDistance+i+1)<=filePos;i++)
        if(*(posDistance+i+1)!=' '){
            printf("> Usage : find –str <str> --file <file name>\n" );
            return;
        }
    char *qoutPtr=NULL,*slashPtr=NULL,*typeDistance=NULL;
    for(int i=0;*(filePos+5+i)!=0;i++){
        if(*(filePos+7+i)!=' ' && *(filePos+7+i)!='"' && *(filePos+7+i)!='/'){
            printf("> Usage : find –str <str> --file <file name>\n" );
            return;
        }
        if(*(filePos+7+i)=='"'){
            qoutPtr=(filePos+7+i);
            break;
        }
        if(*(filePos+7+i)=='/'){
            slashPtr=(filePos+7+i);
            break;
        }
    }
    if(qoutPtr==NULL && slashPtr==NULL){
        printf("> dir : /root/... or \"/root/...\"\n" );
        return;
    }
    if(qoutPtr!=NULL && *(qoutPtr+1)=='/')
        slashPtr=(qoutPtr+1);
    else if(qoutPtr!=NULL){
            printf("> dir : /root/... or \"/root/...\"\n" );
            return;
        }
    if(qoutPtr!=NULL)
        for(int i=0;*(slashPtr+i+1)!=0;i++){
            if(*(slashPtr+i+1)=='"'){
                typeDistance=(slashPtr+i+1);
                break;
            }
            *(address+i)=*(slashPtr+i+1);
        }
    else
        for(int i=0;*(slashPtr+i+1)!=0;i++){
            if(*(slashPtr+i+1)==' '){
                typeDistance=(slashPtr+i+1);
                break;
            }
        *(address+i)=*(slashPtr+i+1);
        }
    if(qoutPtr!=NULL && typeDistance==NULL){
        printf("> dir : /root/... or \"/root/...\"\n" );
        return;
    }
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    if(qoutPtr!=NULL && (*(typeDistance)==0))
        typePos=1;
    if(qoutPtr==NULL && typeDistance==NULL)
        typePos=1;
    int countNumber=0;
    char *countPos=NULL,*wordPos=NULL,*atPos=NULL,*allPos=NULL,*bufferAlter=NULL;
    if(typeDistance!=NULL && *(typeDistance)!=0){
        countPos=strstr(typeDistance," -count");
        wordPos=strstr(typeDistance," -byword");
        atPos=strstr(typeDistance," -at");
        allPos=strstr(typeDistance," -all");
    }
    if(countPos!=NULL && atPos!=NULL){
        printf("> Usage : opetions can not be excuted\n");
        return;
    }
    if(atPos!=NULL && allPos!=NULL){
        printf("> Usage : opetions can not be excuted\n");
        return;
    }
    FILE *find;
    char *buffer=(char *)malloc(sizeof(char)*1000);
    char *number=(char *)malloc(sizeof(char)*30);
    int tonum=0;
    find=fopen(address,"r");
    fread(buffer,1000,1,find);
    fclose(find);
    if(typePos==1){
        printf("%d\n",wildCard(buffer,text));
    }
    if(countPos!=NULL && atPos==NULL){
        bufferAlter=buffer;
        while(1){
            if(wildCard(bufferAlter,text)==-1)
                break;
            countNumber++;
            bufferAlter=bufferAlter+wildCard(bufferAlter,text)+1;
        }
        printf("%d\n",countNumber);
    }
    if(wordPos!=NULL && countPos==NULL && atPos==NULL && allPos==NULL){
        int wordNum=0;
        for(int i=0;i<=wildCard(buffer,text);i++){
            if((*(buffer+i)==' ' || *(buffer+i)=='\n') && (*(buffer+i+1)!=' ' && *(buffer+i+1)!='\n' ))
                wordNum++;
        }
        printf("%d\n",wordNum);
    }
    if(allPos!=NULL && countPos==NULL && atPos==NULL){
        bufferAlter=buffer;
        int coma=0;
        while(1){
            if(wildCard(bufferAlter,text)==-1)
                break;
            if(wordPos==NULL){
                if(coma!=0)
                    printf(" , %ld",bufferAlter-buffer+wildCard(bufferAlter,text));
                else 
                    printf("%ld",bufferAlter-buffer+wildCard(bufferAlter,text));
            }
            else{
                int wordNum=0;
                for(int i=0;i<=bufferAlter-buffer+wildCard(bufferAlter,text);i++){
                    if((*(buffer+i)==' ' || *(buffer+i)=='\n') && (*(buffer+i+1)!=' ' && *(buffer+i+1)!='\n' ))
                        wordNum++;
                }
                if(coma!=0)
                    printf(" , %d",wordNum);
                else
                    printf("%d",wordNum);                          
            }
            bufferAlter=bufferAlter+wildCard(bufferAlter,text)+1;
            coma++;
        }
        printf("\n");
    }
    if(atPos!=NULL &&  allPos==NULL && countPos==NULL){
        for(int i=0;*(atPos+5+i)!=0;i++){
            if(*(atPos+5+i)==' ')
                break;
            *(number+i)=*(atPos+5+i);
        }
        tonum=atoi(number);
        bufferAlter=buffer;
        int coma=0;
        while(1){
            if(wildCard(bufferAlter,text)==-1)
                break;
            coma++;
            if(coma==tonum)
                break;
            bufferAlter=bufferAlter+wildCard(bufferAlter,text)+1;
        }
        if(coma<tonum)
            printf("-1\n");
        else if(wordPos!=NULL){
            int wordNum=0;
            for(int i=0;i<=bufferAlter-buffer+wildCard(bufferAlter,text);i++){
            if((*(buffer+i)==' ' || *(buffer+i)=='\n') && (*(buffer+i+1)!=' ' &&  *(buffer+i+1)!='\n' ))
                wordNum++;
        }
        printf("%d\n",wordNum);
        }
        else
            printf("%ld\n",bufferAlter-buffer+wildCard(bufferAlter,text));
    }
    if(atPos!=NULL && wordPos!=NULL){

    }
    free(text);
    free(address);
    free(buffer);
}
void undo(char *command){
    char *filePtr,*slashPtr,*qoutPtr=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int strLen;
    strLen=strlen(command);
    filePtr=strstr(command," --file ");
    //check for --file
    if(filePtr==NULL){
        printf("> Usage : undo -–file <file name and address>\n");
        return;
    }
    //check for aditinal char between createfile and --file
    for(int i=0;(command+i+4)<=filePtr;i++)
        if(*(command+i+4)!=' '){
            printf("> Usage : undo -–file <file name and address>\n");
            return;
    }
    //check for slash
    slashPtr=strstr(command,"/");
    if(slashPtr==NULL || *(slashPtr+1)==0){
        printf("> dir : /root/... or \"/root/...\"\n");
        return;
    }
    //check for qout
    if(*(slashPtr-1)=='"')
        qoutPtr=slashPtr-1;
    if((qoutPtr!=NULL && *(command+strLen-1)!='"') || (*(command+strLen-2)=='\\' && *(command+strLen-1)=='"')){
        printf("> dir : \"/root/...\"\n");
        return;
    }
    //check for aditinal char between / or " and --file and special character
    if(qoutPtr!=NULL){
        for(int i=0;(qoutPtr)>(filePtr+7+i);i++)
            if(*(filePtr+7+i)!=' '){
                printf("> Usage : undo -–file <file name and address>\n");
                return;
            }
    }
    else{
        for(int i=0;(slashPtr)>(filePtr+7+i);i++){
            if(*(slashPtr-1-i)!=' '){
                printf("> Usage : undo -–file <file name and address>\n");
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
    //check for special character
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    //check for correct path and file
    if(PathCheck(address,0)==0)
        return;
    //make tow file and put the contects of them into the other
    char *buffer=(char*)malloc(sizeof(char)*1000);
    char *newbuffer=(char*)malloc(sizeof(char)*1000);
    char *newAddress=(char*)malloc(sizeof(char)*100);
    *(newAddress)='.';*(newAddress+1)='s';*(newAddress+2)='y';*(newAddress+3)='s';*(newAddress+4)='t';*(newAddress+5)='e';*(newAddress+6)='m';*(newAddress+7)='/';
    for(int i=0;*(address+i)!=0;i++)
        *(newAddress+8+i)=*(address+i);
    FILE *afterChanges,*Undo;
    afterChanges=fopen(address,"r");
    fseek(afterChanges,0,SEEK_SET);
    fread(buffer,1000,1,afterChanges);
    fclose(afterChanges);
    Undo=fopen(newAddress,"r");
    fread(newbuffer,1000,1,Undo);
    fseek(Undo,0,SEEK_SET);
    fclose(Undo);
    Undo=fopen(newAddress,"w+");
    fseek(Undo,0,SEEK_SET);
    fputs(buffer,Undo);
    fclose(Undo);
    afterChanges=fopen(address,"w+");
    fseek(afterChanges,0,SEEK_SET);
    fputs(newbuffer,afterChanges);
    fclose(afterChanges);
    free(buffer);
    free(newbuffer);
    free(newAddress);
    free(address);
}
void grep(char *command){
    char *filePos=NULL,*stringPos=NULL,*firstChar=NULL,*stringQoutPtr=NULL,*posDistance=NULL,*typeLPos=NULL,*typeCPos=NULL;
    char *text=(char*)malloc(sizeof(char)*300);
    char *typeCheck=(char*)malloc(sizeof(char)*30);
    stringPos=strstr(command," -str ");
    if(stringPos==NULL){
        printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n");
        return;
    }
    for(int i=0;(command+4+i)<=stringPos;i++)
        *(typeCheck+i)=*(command+4+i);
    typeLPos=strstr(typeCheck," -l ");
    typeCPos=strstr(typeCheck," -c ");
    if(typeLPos==NULL && typeCPos==NULL)
        for(int i=0;*(typeCheck+i)!=0;i++){
            if(*(typeCheck+i)!=' '){
                printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n");
                return;
            }
        }
    for(int i=0;*(stringPos+5+i)!=0;i++){
        if(*(stringPos+5+i)!=' ' && *(stringPos+5+i)!='"'){
            firstChar=(stringPos+5+i);
            break;
        }
        if(*(stringPos+5+i)=='"'){
            stringQoutPtr=(stringPos+5+i);
            break;
        }
    }
    if(stringQoutPtr==NULL && firstChar==NULL){
        printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n" );
        return;
    }
    if(firstChar!=NULL)
        for(int i=0;*(firstChar+i)!=0;i++){
            if(*(firstChar+i)==' '){
                posDistance=(firstChar+i);
                break;
            }
            *(text+i)=*(firstChar+i);
        }
    if(stringQoutPtr!=NULL)
        for(int i=0;*(stringQoutPtr+i+1)!=0;i++){
            if(*(stringQoutPtr+i+1)=='"'){
                posDistance=(stringQoutPtr+i+1);
                break;
            }
        *(text+i)=*(stringQoutPtr+i+1);
        }
    if(posDistance==NULL){
        printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n" );
        return;
    }
    filePos=strstr(posDistance," --file ");
    if(filePos==NULL){
        printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n" );
        return;
    }
    for(int i=0;(posDistance+i+1)<=filePos;i++)
        if(*(posDistance+i+1)!=' '){
            printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n" );
            return;
        }

    //from here
   char *qoutPtr=NULL,*slashPtr=NULL,*Distance=NULL;
   Distance=filePos+7;
   int findCounter=0;
   while(*(Distance)!=0){
        int counter=0;
        char *address=(char*)malloc(sizeof(char)*300);
        for(int i=0;*(Distance+i)!=0;i++){
            if(*(Distance+i)!=' ' && *(Distance+i)!='"' && *(Distance+i)!='/'){
                printf("> Usage : grep [options] –str <pattern> --files [<file1> <file2> <file3> ...]\n" );
                return;
            }
            if(*(Distance+i)=='"'){
                qoutPtr=(Distance+i);
                break;
            }
            if(*(Distance+i)=='/'){
                slashPtr=(Distance+i);
                break;
            }
        }
        if(qoutPtr==NULL && slashPtr==NULL){
            printf("> dir : /root/... or \"/root/...\"\n" );
            return;
        }
        if(qoutPtr!=NULL && *(qoutPtr+1)=='/')
            slashPtr=(qoutPtr+1);
        else if(qoutPtr!=NULL){
                printf("> dir : /root/... or \"/root/...\"\n" );
                return;
            }
        if(qoutPtr!=NULL)
            for(int i=0;*(slashPtr+i+1)!=0;i++){
                if(*(slashPtr+i+1)=='"'){
                    Distance=(slashPtr+i+2);
                    counter=1;
                    break;
                }
                *(address+i)=*(slashPtr+i+1);
            }
        else
            for(int i=0;*(slashPtr+i+1)!=0;i++){
                if(*(slashPtr+i+1)==' '){
                    Distance=(slashPtr+i+1);
                    counter=1;
                    break;
                }
                *(address+i)=*(slashPtr+i+1);
            }
        if(specialCharHandeling(address)==0)
            return;
        else
            for(int i=0;*(address+i)!=0;i++){
                if(*(address+i)=='\n'){
                    printf("> Usage : undefined character detected\n");
                    return;
                }
            }
        char *buffer=(char*)malloc(sizeof(char)*1000);
        char* fileName=(char*)malloc(sizeof(char)*200);
        int lastSlash=0;
        for(int i=0;*(address+strlen(address)-1-i)!='/';i++)
            lastSlash=strlen(address)-1-i;
        for(int i=0;*(address+lastSlash+i)!=0;i++)
            *(fileName+i)=*(address+lastSlash+i);
        if(PathCheck(address,1)==0){
            printf("Path dose not exists : %s\n",fileName);
        }else
        if(PathCheck(address,1)==-1){
            printf("File dose not exists : %s \n",fileName);
        }else
        if(typeCPos!=NULL){
            FILE *file;
            file=fopen(address,"r");
            while(fgets(buffer,512,file)!=NULL){
                if(strstr(buffer,text)!=NULL){
                    findCounter++;
                    free(buffer);
                }
                buffer=(char*)malloc(sizeof(char)*1000);
            }
            fclose(file);
        }else
        if(typeLPos!=0){
            FILE *file;
            file=fopen(address,"r");
            fread(buffer,1000,1,file);
            if(strstr(buffer,text)!=NULL)
                printf("%s\n",fileName);
            fclose(file);
        }else{
            FILE *file;
            file=fopen(address,"r");
            while(fgets(buffer,512,file)!=NULL){
                if(strstr(buffer,text)!=NULL){
                    buffer[strcspn(buffer, "\n")] = 0;
                    printf("%s : %s\n",fileName,buffer);
                    free(buffer);
                }
                buffer=(char*)malloc(sizeof(char)*1000);
            }
            fclose(file);
        }
        free(address);
        free(buffer);
        free(fileName);
        slashPtr=NULL;
        qoutPtr=NULL;
        if(counter!=1)
            break;
    }
    if(typeCPos!=NULL)
        printf("Output number : %d\n",findCounter);
}
void auto_indent(char *command){
    char *slashPtr,*qoutPtr=NULL;
    char *address=(char*)malloc(sizeof(char)*100);
    int strLen;
    strLen=strlen(command);
    //check for slash
    slashPtr=strstr(command,"/");
    if(slashPtr==NULL || *(slashPtr+1)==0){
        printf("> dir : /root/... or \"/root/...\"\n");
        return;
    }
    //check for qout
    if(*(slashPtr-1)=='"')
        qoutPtr=slashPtr-1;
    if((qoutPtr!=NULL && *(command+strLen-1)!='"') || (*(command+strLen-2)=='\\' && *(command+strLen-1)=='"')){
        printf("> dir : \"/root/...\"\n");
        return;
    }
    if(qoutPtr!=NULL){
        for(int i=0;(qoutPtr)>(command+11+i);i++)
            if(*(command+11+i)!=' '){
                printf("> Usage : auto-indent <file>\n");
                return;
            }
    }
    else{
        for(int i=0;(slashPtr)>(command+11+i);i++){
            if(*(command+11+i)!=' '){
                printf("> Usage : auto-indent <file>\n");
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
    //check for special character
    if(specialCharHandeling(address)==0)
        return;
    else
        for(int i=0;*(address+i)!=0;i++){
            if(*(address+i)=='\n'){
                printf("> Usage : undefined character detected\n");
                return;
            }
        }
    //check for correct path and file
    if(PathCheck(address,0)==0)
        return;
    beforChanges(address);
    //make file
    FILE *file;
    char *bufferS=(char*)malloc(sizeof(char)*1000);
    char *buffer=(char*)malloc(sizeof(char)*1000);
    char *newBuffer=(char*)malloc(sizeof(char)*1000);
    //open in read mode and get the content of file
    file=fopen(address,"r");
    fseek(file,0,SEEK_SET);
    fread(bufferS,1000,1,file);
    fclose(file);
    //delte spaces after } or {
    int count=0;
    for(int i=0;*(bufferS+i)!=0;i++){
        *(buffer+count)=*(bufferS+i);
        count++;
        if(*(bufferS+i)=='{'){
            for(int j=0;*(bufferS+i+j+1)!=0;j++)
                if(*(bufferS+i+j+1)!=' '){
                    i=i+j;
                    break;
                }
        }
        if(*(bufferS+i)=='}'){
            for(int j=0;*(bufferS+i+j+1)!=0;j++)
                 if(*(bufferS+i+j+1)!=' '){
                    i=i+j;
                    break;
                }
        }
    }
    //delte spaces before { }
    for(int i=0;*(buffer+i)!=0;i++){
        if(*(buffer+i)=='{' || *(buffer+i)=='}'){
            for(int j=i-1;;j--){
                if(*(buffer+j)=='\n' || j==0)
                    break;
                else 
                if(*(buffer+j)==' '){
                    for(int k=j;*(buffer+k)!=0;k++)
                        *(buffer+k)=*(buffer+k+1);
                    i=0;
                    break;
                }else
                    break;
            }
        }
        if(*(buffer+i)=='\n' && *(buffer+i+1)==' '){
            for(int j=i+1;*(buffer+j)!=0;j++){
                *(buffer+j)=*(buffer+j+1);
            }
            i=0;
        }
    }
    //indenting
    int braces=0,counter=0;
    for(int i=0;*(buffer+i)!=0;i++){
        if(*(buffer+i)=='{')
            braces++;
        if(*(buffer+i)=='}')
            braces--;
        if(braces<0){
            printf("> Invalid use of braces\n");
            return;
        }
        *(newBuffer+counter)=*(buffer+i);
        if(*(buffer+i)=='{'){
            if(count>=1 && *(newBuffer+counter-1)=='\n')
                for(int j=0;j<(braces-1)*4;j++){
                    *(newBuffer+counter)=' ';
                    counter++;
                }
            if(*(newBuffer+counter-1)!='\n'  && *(newBuffer+counter-1)!=' ' && counter>1){
                *(newBuffer+counter)=' ';
                counter++;
            }
            *(newBuffer+counter)='{';
                if(*(buffer+i+1)!='\n'){
                    *(newBuffer+counter+1)='\n';
                    counter=counter+2;
                }
                else{
                    counter++;
                }
        }else
        if(*(buffer+i)=='}'){
            for(int j=0;j<(braces)*4;j++){
                *(newBuffer+counter)=' ';
                counter++;
            }
            if(*(newBuffer+counter-(braces)*4-1)!='\n'){
                *(newBuffer+counter)='\n';
                counter++;
                for(int j=0;j<(braces)*4;j++){
                    *(newBuffer+counter)=' ';
                    counter++;
                }
                *(newBuffer+counter)='}';
                *(newBuffer+counter+1)='\n';
                counter=counter+2;
            }else{
                *(newBuffer+counter)='}';
                if(*(buffer+i+1)!='\n'){
                    *(newBuffer+counter+1)='\n';
                    counter=counter+2;   
                }
                else{
                    counter++;
                }
            }
        }
        else{
            if(*(newBuffer+counter-1)=='\n'){
                for(int j=0;j<(braces)*4;j++){
                    *(newBuffer+counter)=' ';
                    counter++;
                }
            }
            *(newBuffer+counter)=*(buffer+i);
                counter++;
        }
    }
    if(braces!=0){
        printf("> Warning : Left and right braces are not equal\n");
    }
    //rewrite the file 
    file=fopen(address,"w+");
    fseek(file,0,SEEK_SET);
    fputs(newBuffer,file);
    fclose(file);
    free(address);
    free(buffer);
    free(bufferS);
    free(newBuffer);
}
void compare(char *command){
    char *slashPtr=NULL,*snSlashPtr=NULL,*qoutPtr=NULL,*snQoutPtr=NULL;
    char *address1=(char*)malloc(sizeof(char)*100);
    char *address2=(char*)malloc(sizeof(char)*100);
    int fileDistance=0;
    //check for slash
    slashPtr=strstr(command,"/");
    if(slashPtr==NULL){
        printf("> dir : /root/... or \"/root/...\"\n");
        return;
    }
     //check for qout
    if(*(slashPtr-1)=='"')
        qoutPtr=slashPtr-1;
    //check for aditinal char between createfile and --file
    if(qoutPtr!=NULL)
        for(int i=0;(command+7+i)<qoutPtr;i++){
            if(*(command+7+i)!=' '){
                printf("Usage : compare <file1> <file2>\n");
                return;
            }
        }
    else
        for(int i=0;(command+7+i)<slashPtr;i++){
            if(*(command+7+i)!=' '){
                printf("Usage : compare <file1> <file2>\n");
                return;
            }
        }
    //check for first address
    if(qoutPtr==NULL){
        for(int i=0;*(slashPtr+1+i)!=' ';i++){
            *(address1+i)=*(slashPtr+1+i);
            fileDistance=2+i;
        }
        if(specialCharHandeling(address1)==0)
            return;
        else
            for(int i=0;*(address1+i)!=0;i++){
                if(*(address1+i)=='\n'){
                    printf("> Usage : undefined character detected\n");
                    return;
                }
            }
    }
    else{
        int qoutCheck=0;
        for(int i=0;*(slashPtr+1+i)!=0;i++){
            if(*(slashPtr+1+i)=='"' &&  *(slashPtr+i+2)!=0 && *(slashPtr+i)!='\\'){
                qoutCheck=1;
                break;
            }
            *(address1+i)=*(slashPtr+1+i);
            fileDistance=3+i;
        }
        if(qoutCheck!=1){
            printf("> dir : \"/root/...\"\n");
            return;
        }
        if(specialCharHandeling(address1)==0)
            return;
        else
            for(int i=0;*(address1+i)!=0;i++){
                if(*(address1+i)=='\n'){
                    printf("> Usage : undefined character detected\n");
                    return;
                }
            }
    }
    //check for second file slash
    snSlashPtr=strstr(slashPtr+fileDistance,"/");
    if(snSlashPtr==NULL){
        printf("> dir : /root/... or \"/root/...\"\n");
        return;
    }
    //check for qout
    if(*(snSlashPtr-1)=='"')
        snQoutPtr=snSlashPtr-1;
    //check for aditinal char between file1 and file2
    if(snQoutPtr!=NULL)
        for(int i=0;(slashPtr+fileDistance+i)<snQoutPtr;i++){
            if(*(slashPtr+fileDistance+i)!=' '){
                printf("> Usage : compare <file1> <file2>\n");
                return;
            }
        }
    else
        for(int i=0;(slashPtr+fileDistance+i)<snSlashPtr;i++){
            if(*(slashPtr+fileDistance+i)!=' '){
                printf("> Usage : compare <file1> <file2>\n");
                return;
            }
        }
    //save second address
    if(snQoutPtr==NULL){
        for(int i=0;*(snSlashPtr+1+i)!=0;i++){
            *(address2+i)=*(snSlashPtr+1+i);
        }
        if(specialCharHandeling(address2)==0)
            return;
        else
            for(int i=0;*(address2+i)!=0;i++){
                if(*(address2+i)=='\n'){
                    printf("> Usage : undefined character detected\n");
                    return;
                }
            }
    }
    else{
        if((*(command+strlen(command)-1)!='"') || ((*(command+strlen(command)-1)=='"')  &&  (*(command+strlen(command)-2)='\\' ))){
            printf("> dir : \"/root/...\"\n");
            return;
        }
        for(int i=0;*(snSlashPtr+1+i)!='"';i++){
            *(address2+i)=*(snSlashPtr+1+i);
        }
        if(specialCharHandeling(address2)==0)
            return;
        else
            for(int i=0;*(address2+i)!=0;i++){
                if(*(address2+i)=='\n'){
                    printf("> Usage : undefined character detected\n");
                    return;
                }
        }
    }
    //check for aditional space at the end of file
    if(snQoutPtr==NULL)
        for(int i=0;*(address2+i)!=0;i++)
            if(*(address2+i)==' '){
                printf("> Usage : aditinal space detected\n");
                return;
            }
    //check for address existence
   if(PathCheck(address1,0)==0)
        return;
    if(PathCheck(address2,0)==0)
        return;
   //open files
    FILE *file1,*file2;
    char *filePtr1,*filePtr2,*buffer1,*buffer2;
    file1=fopen(address1,"r");
    file2=fopen(address2,"r");
    int lineNumber=1;
    do{
        //use fgets() to read lines seperatly
        buffer1=(char*)malloc(sizeof(char)*500);
        buffer2=(char*)malloc(sizeof(char)*500);
        filePtr1=fgets(buffer1,512,file1);
        filePtr2=fgets(buffer2,512,file2);
        if(*(buffer1+strlen(buffer1)-1)!='\n' && filePtr1!=NULL){
            *(buffer1+strlen(buffer1))='\n';
            *(buffer1+strlen(buffer1)+1)=0;
        }
        if(*(buffer2+strlen(buffer2)-1)!='\n'&& filePtr2!=NULL){
            *(buffer2+strlen(buffer2))='\n';
            *(buffer2+strlen(buffer2)+1)=0;
        }
        //check witch file is longer and check for same lines
        if(strcmp(buffer1,buffer2) && filePtr1!=NULL && filePtr2!=NULL)
            printf("============ #%d ============\n%s%s",lineNumber,buffer1,buffer2);
        if(filePtr1!=NULL && filePtr2==NULL)
            printf("<<<<<<<<<< #%d - #%d <<<<<<<<<<\n%s",lineNumber,lineNumber,buffer1);
        if(filePtr1==NULL && filePtr2!=NULL)
            printf(">>>>>>>>>> #%d - #%d >>>>>>>>>>\n%s",lineNumber,lineNumber,buffer1);
        free(buffer1);
        free(buffer2);
        lineNumber++;
    }while(filePtr2!=NULL ||  filePtr1!=NULL);
    fclose(file1);
    fclose(file2);
    free(address1);
    free(address2);
}
void treeDepthPrint(int depth,int startFrom,char dirAdd[]){
    DIR * directories;
    struct dirent *dir;
    char *add3=(char*)malloc(sizeof(char)*1000);
    directories = opendir(dirAdd);
    //check for directories with dirent library
    if(directories){
        while((dir=readdir(directories))!=NULL){
            //check for depth
            if(startFrom==depth)
                break;
            //check for hidden files
            if(dir->d_name[0]!='.' && dir->d_name[strlen(dir->d_name)-1]!='~'){
                for(int i=0;i<startFrom;i++)
                    printf("    ");
                printf("└── %s",dir->d_name);
                //print the tpye of files or folders
                if(dir->d_type==4)
                    printf(" (dir)\n");
                else
                    printf(" (file)\n");
                //make address to open new directory
                if(dir->d_type!=8){
                    for(int i=0;;i++){
                        if(i<strlen(dirAdd))
                            *(add3+i)=*(dirAdd+i);
                        else if(i==strlen(dirAdd))
                                *(add3+i)='/';
                             else
                                *(add3+i)=*(dir->d_name+i-strlen(dirAdd)-1);
                        if(i==strlen(dirAdd)+strlen(dir->d_name)){
                            *(add3+i+1)=0;
                            break;
                        }
                    }
                    treeDepthPrint(depth,startFrom+1,add3);
                }
            }        
        }
        closedir(directories);
    }
}
void treeCommandReader(char* command){
    char *depthString=(char*)malloc(sizeof(char)*30);
    //check for aditinal char
    for(int i=0;*(command+i+4)!=0;i++){
        *(depthString+i)=*(command+i+4);
        if((*(command+i+4)>57 || *(command+i+4)<48) && *(command+i+4)!=' ' && *(command+i+4)!='-'){
            printf("> Usage : tree <depth>\n");
            return;
        }
    }
    //check for aditinal space and -
    firstEndSpaceRemoval(depthString);
    int depthDash=0;
    int depth=0;
    if(*(depthString)=='-')
        depthDash=1;
    if(depthDash==1){
        for(int i=1;*(depthString+i)!=0;i++)
            if(*(depthString+i)==' ' || *(depthString+i)=='-'){
                printf("> Usage :tree <depth>\n");
                return;
            }
        depth=-atoi(depthString+1);
    }
    else{
        for(int i=0;*(depthString+i)!=0;i++)
            if(*(depthString+i)==' ' || *(depthString+i)=='-'){
                printf("> Usage :tree <depth>\n");
                return;
            }
        depth=atoi(depthString);
    }
    //check for Invalid depth number
    if(depth<-1){
        printf("> Usage : Invalid depth\n");
        return;
    }
    printf("root\n");
    //use recursive function to print every directories
    treeDepthPrint(depth,0,"root");
    free(depthString);
}
void commandDetector(char *command){
     if(!strcmp(command,"end")){
        if(access(".system",X_OK)!=0){
            mkdir(".system",0777);
         }
         system("rm -r .system");
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
    if(strstr(command,"removestr ")==command || !strcmp(command,"removestr")){
         removestr_copy_cut(command,9);
         return;
    }
    if(strstr(command,"copystr ")==command || !strcmp(command,"copystr")){
         removestr_copy_cut(command,7);
         return;
    }
    if(strstr(command,"cutstr ")==command || !strcmp(command,"cutstr")){
         removestr_copy_cut(command,6);
         return;
    }
    if(strstr(command,"pastestr ")==command || !strcmp(command,"pastestr")){
         pastestr(command);
         return;
    }
    if(strstr(command,"find ")==command || !strcmp(command,"find")){
         find(command);
         return;
    }
    if(strstr(command,"grep ")==command || !strcmp(command,"grep")){
         grep(command);
         return;
    }
    if(strstr(command,"undo ")==command || !strcmp(command,"undo")){
         undo(command);
         return;
    }
    if(strstr(command,"auto-indent ")==command || !strcmp(command,"auto-indent")){
         auto_indent(command);
         return;
    }
    if(strstr(command,"compare ")==command || !strcmp(command,"compare")){
         compare(command);
         return;
    }
    if(strstr(command,"tree ")==command || !strcmp(command,"tree")){
         treeCommandReader(command);
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
        commandDetector(command);
        free(command);
    }
    return 0;
}

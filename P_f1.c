#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <unistd.h>
#include<dirent.h>
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
int  PathCheck(char *address){
    int check=1;
    int lastSlash=0;
    char *dirPath=(char*)malloc(sizeof(char)*100);
    for(int i=0;*(address+i)!=0;i++)
        if(*(address+i)=='/')
            lastSlash=i;
    for(int i=0;i<lastSlash;i++)
        *(dirPath+i)=*(address+i);
    if(access(dirPath,X_OK)!=0){
        printf("> Path dose not exists\n");
        return 0;
    }
    if(access(address,F_OK)!=0){
        printf("> File dose not exists\n");
        return 0;
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
    //check for file existence
    if(PathCheck(address)==0)
        return;
    //make file
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
            for(int j=counter+1;charCount<character-1;j++){
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
        for(int i=0;*(buffer+i)!=0;i++)
            *(newBuffer+counter+i+1)=*(buffer+i+bufferCounter+1);
    //overwrite and close file
    fseek(letsWrite,0,SEEK_SET);
    fputs(newBuffer,letsWrite);
    fclose(letsWrite);
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
    if(PathCheck(address)==0)
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
}
/*void wildCard(char *buffer){
    int curentPtr=0;
    char *newBuffer;
    char wildCard[100][100];
    while(1){
        newBuffer=(char*)malloc(sizeof(char)*1000);
        for(int i=0;*(buffer+i)!=*) 
    
    }   
}*/
void find(char *command){
    char *filePos=NULL,*stringPos=NULL,*firstChar=NULL,*stringQoutPtr=NULL,*posDistance=NULL,*typePos=NULL;
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
    if(qoutPtr!=NULL && (*(typeDistance+1)=0))
        typePos=NULL;
    if(qoutPtr==NULL && typeDistance==NULL)
        typePos=NULL;
    FILE *find;
    char *buffer=(char *)malloc(sizeof(char)*1000);
    char *findPos=NULL;
    if(typePos==NULL){
        find=fopen(address,"r");
        fread(buffer,1000,1,find);
        //wildCard(buffer);
        fclose(find);
    }
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
   if(PathCheck(address1)==0)
        return;
    if(PathCheck(address2)==0)
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
    if(strstr(command,"find ")==command || !strcmp(command,"find")){
         find(command);
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

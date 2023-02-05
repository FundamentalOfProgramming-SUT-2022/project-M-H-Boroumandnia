#include<ncurses.h>
#include<curses.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<unistd.h>
WINDOW *firstPage[6];
int lineCounter=0,saveCheck=-1,nameCheck=-1,addressCheck=0;
void  Window(){
  int X_dir,Y_dir;
  init_color(COLOR_CYAN,110,110,110);
  init_color(COLOR_MAGENTA,250,250,250);
  init_color(COLOR_BLUE,400,900,900);
  init_color(COLOR_GREEN,0,700,600);
  init_pair(1,COLOR_WHITE,COLOR_CYAN);
  init_pair(2,COLOR_MAGENTA,COLOR_BLUE);
  init_pair(3,COLOR_WHITE,COLOR_MAGENTA);
  init_pair(6,COLOR_WHITE,COLOR_CYAN);
  init_pair(10,COLOR_GREEN,COLOR_CYAN);
  getmaxyx(stdscr,Y_dir,X_dir);
  firstPage[5]=newwin(Y_dir-2,X_dir-6,0,6);
  firstPage[0]=newwin(Y_dir-2,X_dir-6,0,6);
  firstPage[1]=newwin(1,8,Y_dir-2,0);
  firstPage[2]=newwin(1,X_dir-8,Y_dir-2,8);
  firstPage[3]=newwin(1,X_dir,Y_dir-1,0);
  firstPage[4]=newwin(Y_dir-2,6,0,0);
  refresh();
  wbkgd(firstPage[0], COLOR_PAIR(1));
  wbkgd(firstPage[1], COLOR_PAIR(2));
  wbkgd(firstPage[2], COLOR_PAIR(3));
  wbkgd(firstPage[3], COLOR_PAIR(1));
  wbkgd(firstPage[4], COLOR_PAIR(1));
  wattron(firstPage[4], COLOR_PAIR(10));
  wattron(firstPage[0],COLOR_PAIR(6));
  mvwprintw(firstPage[1], 0, 1, "NORMAL");
  mvwprintw(firstPage[2], 0, 2, "[NO NAME]");
  mvwprintw(firstPage[5],0,0,"%c",'@');
  wrefresh(firstPage[5]);
  wrefresh(firstPage[0]);
  wrefresh(firstPage[1]);
  wrefresh(firstPage[2]);
  wrefresh(firstPage[3]);
  wrefresh(firstPage[4]);
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
      wclear(firstPage[3]);
      mvwprintw(firstPage[3],0,0,"Path dose not exists");
      return 0;
  }
  if(grep==1)
    return 0;
  }
  if(access(address,F_OK)!=0){
    if(grep==0){
      wclear(firstPage[3]);
      mvwprintw(firstPage[3],0,0,"File dose not exists");
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
int navigation(char c,int mode){
  int curY=0,curX=0,newCurent=0;
  getyx(firstPage[0],curY,curX);
  if(c==104 || c==4){
    if(mode==-1)
      return 0;
    if(mode==1 && c==104)
      return 3;
    else if(mode==1 && c!=104)
          return 2;
    else
    if(curX!=0 && mode==0)
      wmove(firstPage[0],curY,curX-1);
    return 1;
  }
  if(c==108 || c==5){
    if(mode==-1)
      return 0;
    if(mode==1 && c==108)
      return 3;
    else if(mode==1 && c!=108)
          return 2;
    else
    if((mvwinch(firstPage[5],curY,curX) & A_CHARTEXT)!='@')
      wmove(firstPage[0],curY,curX+1);
    return 1;
  }
  if(c==106 || c==3){
    if(mode==-1)
      return 0;
    if(mode==1 && c==106)
      return 3;
    else if(mode==1 && c!=106)
          return 2;
    else
    if(curY!=0){
      if(mode==-1)
        return 0;
      while((mvwinch(firstPage[5],curY-1,newCurent) & A_CHARTEXT)!='@'){
        if(newCurent==curX)
          break;
        newCurent++;
      }
      wmove(firstPage[0],curY-1,newCurent);
    }
    return 1;
  }
  if(c==107 || c==2){
    if(mode==-1)
      return 0;
    if(mode==1 && c==107)
      return 3;
    else if(mode==1 && c!=107)
          return 2;
    else
    if(curY!=lineCounter){
      while((mvwinch(firstPage[5],curY+1,newCurent) & A_CHARTEXT)!='@'){
        if(newCurent==curX)
          break;
        newCurent++;
      }
      wmove(firstPage[0],curY+1,newCurent);
    }
    return 1;
  }
  return 0;
}
void fileClose(char *command,char *fileName){
  addressCheck=0;
  lineCounter=0;
  nameCheck=0;
  saveCheck=0;
  fileName=(char*)malloc(sizeof(char)*200);
  command=(char*)malloc(sizeof(char)*1000);
  wclear(firstPage[0]);
  wclear(firstPage[5]);
  wclear(firstPage[2]);
  wclear(firstPage[4]);
  mvwprintw(firstPage[2], 0, 2, "[NO NAME]");
  wrefresh(firstPage[5]);
  wrefresh(firstPage[0]);
  wrefresh(firstPage[2]);
  wrefresh(firstPage[4]);
}
void open(char *command,char *fileName,char *address){
  char *slashPtr,*qoutPtr=NULL;
  int strLen;
  strLen=strlen(command);
  slashPtr=strstr(command,"/");
  if(slashPtr==NULL || *(slashPtr+1)==0){
    wclear(firstPage[3]);
    mvwprintw(firstPage[3],0,0,"dir : /root/... or \"/root/...\"");
    return;
  }
  if(*(slashPtr-1)=='"')
    qoutPtr=slashPtr-1;
  if((qoutPtr!=NULL && *(command+strLen-1)!='"') || (*(command+strLen-2)=='\\' && *(command+strLen-1)=='"')){
    wclear(firstPage[3]);
    mvwprintw(firstPage[3],0,0,"dir : /root/... or \"/root/...\"");
    return;
  }
  if(qoutPtr!=NULL){
    for(int i=0;(qoutPtr)>(command+11+i);i++)
      if(*(command+11+i)!=' '){
        wclear(firstPage[3]);
        mvwprintw(firstPage[3],0,0,"Usage : open <file>");
        return;
      }
  }
  else{
    for(int i=0;(slashPtr)>(command+11+i);i++){
      if(*(command+11+i)!=' '){
        wclear(firstPage[3]);
        mvwprintw(firstPage[3],0,0,"Usage : open <file>");
        return;
      }
    }
    for(int i=0;*(slashPtr+i)!=0;i++){
      if(*(slashPtr+i)==' '){
        wclear(firstPage[3]);
        mvwprintw(firstPage[3],0,0,"Usage : aditinal space detected");
        return;
      }
    }
  }
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
        wclear(firstPage[3]);
        mvwprintw(firstPage[3],0,0,"Usage : aditinal space detected");
      }
    }
  if(PathCheck(address,0)==0)
    return;
  FILE *file;
  char *buffer=(char*)malloc(sizeof(char)*1000);
  int curY=0,curX=0;
  file=fopen(address,"r");
  fread(buffer,1000,1,file);
  mvwprintw(firstPage[4],0,0 ,"%4d",1);
  wrefresh(firstPage[4]);
  for(int i=0;*(buffer+i)!=0;i++){
    if(*(buffer+i)=='\n'){
      getyx(firstPage[0],curY,curX);
      mvwprintw(firstPage[5],curY,curX ,"%c",'@');
      lineCounter++;
      mvwprintw(firstPage[4],lineCounter,0 ,"%4d",lineCounter+1);
      wrefresh(firstPage[4]);
    }
    waddch(firstPage[0],*(buffer+i));
    if(*(buffer+i+1)==0){
      getyx(firstPage[0],curY,curX);
      mvwprintw(firstPage[5],curY,curX,"%c",'@');
    }
  }
  if(*(buffer)==0)
    mvwprintw(firstPage[5],0,0,"%c",'@');
  wrefresh(firstPage[0]);
  fclose(file);
  int lastSlash=0;
  for(int i=0;*(address+strlen(address)-1-i)!='/';i++)
    lastSlash=strlen(address)-1-i;
  for(int i=0;*(address+lastSlash+i)!=0;i++){
    *(fileName+i)=*(address+lastSlash+i);
    *(fileName+i+1)=0;
  }
  nameCheck=1;
  wclear(firstPage[2]);
  mvwprintw(firstPage[2],0,2,"%s",fileName);
  wrefresh(firstPage[2]);
  addressCheck=1;
}
void save(char *address){
  FILE *file;
  char *read=(char*)malloc(sizeof(char)*1000);
  file=fopen(address,"w");
  int line=0,charackter=0,lineChar=0,curX=0,curY=0;
  while(1){
    if((mvwinch(firstPage[5],line,lineChar) & A_CHARTEXT)=='@' && lineCounter!=line){
      *(read+charackter)='\n';
      line++;
      charackter++;
      lineChar=0;
      continue;
    }
    if((mvwinch(firstPage[5],line,lineChar) & A_CHARTEXT)=='@' &&  lineCounter==line)
      break;
    *(read+charackter)=(mvwinch(firstPage[0],line,lineChar) & A_CHARTEXT);
    charackter++;
    lineChar++;
  }
  getyx(firstPage[0],curY,curX);
  wmove(firstPage[0],curY,curX+1);
  fputs(read,file);
  fclose(file);
  saveCheck=1;
  addressCheck=1;
}
void expression(char *command){
  int line=0,charackter=0,lineChar=0,curX=0,curY=0;
  char* buffer=(char*)malloc(sizeof(char)*1000);
  char* search=(char*)malloc(sizeof(char)*1000);
  mvwprintw(firstPage[3], 0, 0, "/");
  echo();
  wgetstr(firstPage[3],search);
  wclear(firstPage[3]);
  noecho();
  while(1){
    if((mvwinch(firstPage[5],line,lineChar) & A_CHARTEXT)=='@' && lineCounter!=line){
      *(buffer+charackter)='\n';
      line++;
      charackter++;
      lineChar=0;
      continue;
    }
    if((mvwinch(firstPage[5],line,lineChar) & A_CHARTEXT)=='@' &&  lineCounter==line)
      break;
    *(buffer+charackter)=(mvwinch(firstPage[0],line,lineChar) & A_CHARTEXT);
    charackter++;
    lineChar++;
  }
  char c;
  char *pos;
  int posCounter=0;
  pos=strstr(buffer,search);
  while(1){
    if(posCounter==0){
    }
    else{
      c=wgetch(firstPage[0]);
      if(c!='n')
        break;
    }
    if(pos==NULL && posCounter==0){
      while((c=wgetch(firstPage[0]))=='n'){
      }
      break;
    }
    if(pos!=NULL){
      for(int i=0;buffer+i<pos;i++){
        if(*(buffer+i)=='\n'){
          curY++;
          curX=0;
        }
        if(*(buffer+i)!='\n')
          curX++;
     }
      wchgat(firstPage[0],-1,A_NORMAL,0,NULL);
      wbkgd(firstPage[0], COLOR_PAIR(1));
      wmove(firstPage[0],curY,curX);
      mvwchgat(firstPage[0],curY,curX,strlen(search),A_STANDOUT,0,NULL);
      wrefresh(firstPage[0]);
    }
    if((c=='n' && pos!=NULL ) || posCounter==0){
      pos=(pos+strlen(search));
      pos=strstr(pos,search);
      posCounter++;
    }
    if(pos==NULL)
      pos=strstr(buffer,search);
    curX=0;
    curY=0;
  }
  wchgat(firstPage[0],-1,A_NORMAL,0,NULL);
  wbkgd(firstPage[0], COLOR_PAIR(1));
}
void getname(char *address,char *fileName){
    echo();
    mvwprintw(firstPage[3],0,0,"Please choose a name for your file : ");
    wmove(firstPage[3],0,37);
    nameCheck=1;
    wgetstr(firstPage[3],fileName);
    *(address)='r';*(address+1)='o';*(address+2)='o';*(address+3)='t';*(address+4)='/';
    for(int i=0;*(fileName+i)!=0;i++){
      *(address+i+5)=*(fileName+i);
      *(address+i+6)=0;
    }
    mvwprintw(firstPage[3],0,0,"Please choose a name for your file : ");
    wclear(firstPage[3]);
    wclear(firstPage[3]);
    noecho();
    FILE *file;
    file=fopen(address,"w");
    fclose(file);
    save(address);
    wclear(firstPage[2]);
    mvwprintw(firstPage[2],0,2,"%s",fileName);
    wrefresh(firstPage[2]);
}
void saveas(char *command,char *fileName,char *address){
  FILE *file;
  for(int i=0;*(command+7+i)!=0;i++){
    *(fileName+i)=*(command+i+7);
    *(fileName+i+1)=0;
  }
  int check=0;
  int len=strlen(address);
  if(addressCheck==1){
    remove(address);
    for(int i=0;len-1-i>=0;i++){
      if(*(address+len-1-i)=='/'){
        for(int j=0;*(fileName+j)!=0;j++){
          *(address+len-i+j)=*(fileName+j);
          *(address+len-i+j+1)=0;
        }
        check=1;
        break;
      }
      if(check==1)
        break;
    }
    nameCheck=1;
    file=fopen(address,"w");
    fclose(file);
    save(address);
  }
  else{
    *(address)='r';*(address+1)='o';*(address+2)='o';*(address+3)='t';*(address+4)='/';
    for(int i=0;*(fileName+i)!=0;i++){
      *(address+i+5)=*(fileName+i);
      *(address+i+6)=0;
    }
    nameCheck=1;
    file=fopen(address,"w");
    fclose(file);
    save(address);
  }
  wclear(firstPage[2]);
  mvwprintw(firstPage[2],0,2,"%s",fileName);
  wrefresh(firstPage[2]);
}
void commandDetecter(char *command,char *fileName,char*address){
  if(!strcmp(command,"q")){
    wclear(firstPage[3]);
    exit(0);
    return;
  }
  if(!strcmp(command,"open") || strstr(command,"open ")==command){
    wclear(firstPage[3]);
    fileClose(command,fileName);
    open(command,fileName,address);
    return;
  }
   if(strstr(command,"saveas ")==command){
    wclear(firstPage[3]);
    saveas(command,fileName,address);
    if(saveCheck==1){
      wclear(firstPage[2]);
      mvwprintw(firstPage[2],0,2,"%s",fileName);
      wrefresh(firstPage[2]);
    }
    return;
  }
  if(!strcmp(command,"save")){
    wclear(firstPage[3]);
    if(nameCheck==-1)
      return;
    if(nameCheck==0)
      getname(address,fileName);
    else
      save(address);
    if(saveCheck==1){
      wclear(firstPage[2]);
      mvwprintw(firstPage[2],0,2,"%s",fileName);
      wrefresh(firstPage[2]);
    } 
    return;
  }
  else{
    init_color(COLOR_RED,800,400,400);
    init_pair(8,COLOR_RED,COLOR_CYAN);
    wattron(firstPage[3],COLOR_PAIR(8));
    mvwprintw(firstPage[3], 0, 0, "Not and editor command : %s",command); 
    wattroff(firstPage[3],COLOR_PAIR(8));
  }
}
void visualMode(){
  FILE *file;
  char c,charFinder;
  char *buffer=(char *)malloc(sizeof(char)*1000);
  char *buffer1=(char *)malloc(sizeof(char)*1000);
  int curY=0,curX=0,selectNUM=0;
  while((c=wgetch(firstPage[0]))!=27){
      getyx(firstPage[0],curY,curX);
      if(c==104 || c==4){
        if(curX!=0){
          if(selectNUM>=0){
            wmove(firstPage[0],curY,curX);
            wchgat(firstPage[0],1,A_COLOR,1,NULL);
          }
          wmove(firstPage[0],curY,curX-1);
          wchgat(firstPage[0],1,A_STANDOUT,0,NULL);
          selectNUM--;
        }
        continue;
      }
    if(c==108 || c==5){
      if(selectNUM<=0){
        wmove(firstPage[0],curY,curX);
        wchgat(firstPage[0],1,A_COLOR,1,NULL);
      }
      if((mvwinch(firstPage[5],curY,curX) & A_CHARTEXT)!='@'){
        wmove(firstPage[0],curY,curX+1);
        wchgat(firstPage[0],1,A_STANDOUT,0,NULL);
          selectNUM++;
      }
      continue;
    }
    if(c=='y' || c=='c'){
      int check=0;
      if(selectNUM<0){
        check=1;
        selectNUM=-selectNUM;
        for(int i=0;i<=selectNUM;i++)
          *(buffer+i)=(mvwinch(firstPage[0],curY,curX+i) & A_CHARTEXT);
      }
      else{
        for(int i=0;i<selectNUM;i++)
          *(buffer+i)=(mvwinch(firstPage[0],curY,curX-selectNUM+i+1) & A_CHARTEXT);
      }
      *(buffer+selectNUM+1)=0;
      file=fopen("system/Clipboard","w");
      fputs(buffer,file);
      fclose(file);
      wchgat(firstPage[0],-1,A_NORMAL,0,NULL);
      wbkgd(firstPage[0], COLOR_PAIR(1));
      wrefresh(firstPage[0]);
      if(check==1)
        wmove(firstPage[0],curY,curX+selectNUM);
      else
        wmove(firstPage[0],curY,curX-selectNUM);
      if(c=='y')
        selectNUM=0;
      else{
        if(check==1){
          wmove(firstPage[0],curY,curX-selectNUM);
          selectNUM=-selectNUM;
        }
        else
          wmove(firstPage[0],curY,curX+selectNUM+1);
      }
    }
    if(c=='d' || c=='c'){
      int check=0;
      wchgat(firstPage[0],-1,A_NORMAL,0,NULL);
      wbkgd(firstPage[0], COLOR_PAIR(1));
      wrefresh(firstPage[0]);
      if(selectNUM<0){
        check=1;
        selectNUM=-selectNUM;
        for(int i=0;(mvwinch(firstPage[5],curY,curX+selectNUM+i) & A_CHARTEXT)!='@';i++){
          charFinder=(mvwinch(firstPage[0],curY,curX+selectNUM+i) & A_CHARTEXT);
          mvwaddch(firstPage[0],curY,curX+i,charFinder);
        }
      }
      else if(selectNUM>0)
        for(int i=0;(mvwinch(firstPage[5],curY,curX+i+1) & A_CHARTEXT)!='@';i++){
          charFinder=(mvwinch(firstPage[0],curY,curX+i+1) & A_CHARTEXT);
          mvwaddch(firstPage[0],curY,curX-selectNUM+i+1,charFinder);
        }
      int adCounter=0;
      for(int i=0;(mvwinch(firstPage[5],curY,i) & A_CHARTEXT)!='@';i++)
        adCounter=i+1;
      for(int i=adCounter-selectNUM;(mvwinch(firstPage[5],curY,i) & A_CHARTEXT)!='@';i++)
         mvwprintw(firstPage[0],curY,i,"%c",' ');
      mvwprintw(firstPage[5],curY,adCounter,"%c",' ');
      mvwprintw(firstPage[5],curY,adCounter-selectNUM,"%c",'@');
      if(check==1)
        wmove(firstPage[0],curY,curX);
      else
        wmove(firstPage[0],curY,curX-selectNUM);
      selectNUM=0;
    }
    if(c=='p'){
      file=fopen("system/Clipboard","r");
      fread(buffer1,1000,1,file);
      winsstr(firstPage[0],buffer);
      int adCounter=0;
      int len=strlen(buffer1);
      for(int i=0;(mvwinch(firstPage[5],curY,i) & A_CHARTEXT)!='@';i++)
        adCounter=i+1;
      mvwprintw(firstPage[5],curY,adCounter,"%c",' ');
      mvwprintw(firstPage[5],curY,adCounter+len,"%c",'@');
      fclose(file);
      selectNUM=0;
      wchgat(firstPage[0],-1,A_NORMAL,0,NULL);
      wbkgd(firstPage[0], COLOR_PAIR(1));
      wrefresh(firstPage[0]);
    }
  }
  wchgat(firstPage[0],-1,A_NORMAL,0,NULL);
  wbkgd(firstPage[0], COLOR_PAIR(1));
  wrefresh(firstPage[0]);
}
void insertMode(char *fileName){ 
  char c;
  int curY=0,curX=0,lastCHAR=0,lastCChar=0;
  keypad(firstPage[0],TRUE);
  mvwprintw(firstPage[4],0,0 ,"%4d",1);
  wrefresh(firstPage[4]);
  if(saveCheck==-1)
    saveCheck=0;
  if(nameCheck==-1)
    nameCheck=0;
  saveCheck=0;
  if(nameCheck==0){
    mvwprintw(firstPage[2],0,2,"[NO NAME]   +");
    wrefresh(firstPage[2]);
  }
  else{
    mvwprintw(firstPage[2],0,2,"%s   +",fileName);
    wrefresh(firstPage[2]);
  }
  getyx(firstPage[0],curY,curX);
  wmove(firstPage[0],curY,curX);
  while((c=wgetch(firstPage[0]))!=27){
    lastCHAR=0;
    for(int i=0;(mvwinch(firstPage[5],lineCounter,i) & A_CHARTEXT)!='@';i++)
        lastCHAR=i+1;
    getyx(firstPage[0],curY,curX);
    if(c=='\n' && curY==lineCounter && curX==lastCHAR){
      getyx(firstPage[0],curY,curX);
      mvwprintw(firstPage[5],curY,curX ,"%c",'@');
      mvwprintw(firstPage[5],curY+1,0 ,"%c",'@');
      lineCounter++;
      wmove(firstPage[0],curY+1,0);
      mvwprintw(firstPage[4],lineCounter,0 ,"%4d",lineCounter+1);
      wrefresh(firstPage[4]);
      continue;
    }
    if(navigation(c,1)==2)
      continue;
    if(c==127){
      continue;
    }
    else if(curY==lineCounter && curX==lastCHAR){
      waddch(firstPage[0],c);
      getyx(firstPage[0],curY,curX);
      mvwprintw(firstPage[5],curY,curX,"%c",'@');
      if(curX!=0)
        mvwprintw(firstPage[5],curY,curX-1,"%c",' ');
    }
    else if(c!='\n'){
      getyx(firstPage[0],curY,curX);
      for(int i=0;(mvwinch(firstPage[5],curY,i) & A_CHARTEXT)!='@';i++)
        lastCChar=i+1;
      mvwprintw(firstPage[5],curY,lastCChar,"%c",' ');
      mvwprintw(firstPage[5],curY,lastCChar+1,"%c",'@');
      mvwinsch(firstPage[0],curY,curX,c);
      wmove(firstPage[0],curY,curX+1);
      lastCChar=0;
    }else if(c=='\n'){
      char charFinder;
      getyx(firstPage[0],curY,curX);
      winsertln(firstPage[0]);
      wmove(firstPage[5],curY,curX);
      winsertln(firstPage[5]);
      getyx(firstPage[0],curY,curX);
      lineCounter++;
      mvwprintw(firstPage[4],lineCounter,0 ,"%4d",lineCounter+1);
      wrefresh(firstPage[4]);
      for(int i=0;i<curX;i++){
        charFinder=(mvwinch(firstPage[0],curY+1,i) & A_CHARTEXT);
        mvwaddch(firstPage[0],curY,i,charFinder);
      }
      mvwprintw(firstPage[5],curY,curX,"%c",'@');
      int uptolast=0;
      for(int i=0;(mvwinch(firstPage[5],curY+1,i+curX) & A_CHARTEXT)!='@';i++){
        charFinder=(mvwinch(firstPage[0],curY+1,i+curX) & A_CHARTEXT);
        mvwaddch(firstPage[0],curY+1,i,charFinder);
        uptolast++;
      }
      mvwprintw(firstPage[5],curY+1,uptolast,"%c",'@');
      for(int i=uptolast;i<=512;i++){
        mvwprintw(firstPage[5],curY+1,i+1,"%c",' ');
        mvwprintw(firstPage[0],curY+1,i,"%c",' ');
        wrefresh(firstPage[0]);
      }
      wmove(firstPage[0],curY+1,0);
    }
  }
}
void getString(char *command,char *fileName,char *address){
  init_pair(6,COLOR_WHITE,COLOR_CYAN);
  wattron(firstPage[3],COLOR_PAIR(6));
  mvwprintw(firstPage[3], 0, 0, ":");
  echo();
  wgetstr(firstPage[3],command);
  firstEndSpaceRemoval(command);
  wclear(firstPage[3]);
  commandDetecter(command,fileName,address);
  noecho();
  wattroff(firstPage[3],COLOR_PAIR(6));
}
void modeIdentifier(int mode,char *fileName){
  init_pair(4,COLOR_MAGENTA,COLOR_YELLOW);
  init_pair(5,COLOR_MAGENTA,COLOR_GREEN);
   switch(mode){
    case 0: wbkgd(firstPage[1], COLOR_PAIR(2));mvwprintw(firstPage[1], 0, 1, "NORMAL");break;
    case -1: wbkgd(firstPage[1], COLOR_PAIR(4));mvwprintw(firstPage[1], 0, 1, "VISUAL");break;
    case 1: wbkgd(firstPage[1], COLOR_PAIR(5));mvwprintw(firstPage[1], 0, 1, "INSERT");break;
  }
  wrefresh(firstPage[4]);
  wrefresh(firstPage[1]);
  wrefresh(firstPage[2]);
  wrefresh(firstPage[3]);
  wrefresh(firstPage[0]);
  if(mode==1)
    insertMode(fileName);
  if(mode==-1)
    visualMode();
}
int main(){
  FILE *file;
  mkdir("system",0777);
  file=fopen("system/Clipboard","w+");
  fclose(file);
  char *fileName=(char*)malloc(sizeof(char)*200);
  char *address=(char*)malloc(sizeof(char)*200);
  char startText;
  int mode=0;
  char*command=(char*)malloc(sizeof(char)*1000);
  initscr();
  start_color();
  init_pair(7,COLOR_BLUE,COLOR_CYAN);
  noecho();
  Window();
  keypad(firstPage[0],TRUE);
  startText=wgetch(firstPage[0]);
  while(1){
    if(navigation(startText,mode)){
        startText=wgetch(firstPage[0]);
        continue;
        } 
    if(mode==0 || startText==27){
      switch(startText){
        case 'i':wclear(firstPage[3]);mode=1;wattron(firstPage[3],COLOR_PAIR(7));mvwprintw(firstPage[3], 0, 0, "-- INSERT --");wattroff(firstPage[3],COLOR_PAIR(7));break;
        case 'v':wclear(firstPage[3]);mode=-1;wattron(firstPage[3],COLOR_PAIR(7));mvwprintw(firstPage[3], 0, 0, "-- VISUAL --");wattroff(firstPage[3],COLOR_PAIR(7));break;
        case ':':wclear(firstPage[3]);getString(command,fileName,address);break;
        case '/':wclear(firstPage[3]);expression(command);break;
        case 27:wclear(firstPage[3]);mode=0;break; 
      } 
      modeIdentifier(mode,fileName);
    }
    if(mode==1 || mode==-1){
      startText=27;
      mode=0;
    }
    else
      startText=wgetch(firstPage[0]);
  }
  endwin();
}

#include<bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
 #include <sys/types.h>
  #include <unistd.h>
 #include <termios.h>
#include <errno.h>
      

  #include <stdio.h>
#include <iostream> 
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 
using namespace std;
int parse(char **input, char **commands, int pipe_count, int inputLen){
  int i = 0, j = 0;
  while(i < (pipe_count + 1)){
    for(; j < inputLen; j++){
      if(strcmp(input[j], "|") != 0){
        strcat(commands[i], input[j]);
        if(!((j + 1) >= inputLen)){
          strcat(commands[i], " ");
        }
      }else{
        j++;
        break;
      }
    }
    i++;
  }
  return 0;
}
int pipecount(char **input, int len){

  int i = 0, count = 0;
  for(i = 0; i <len; i++){
    if(strcmp(input[i], "|") == 0){
      count+= 1;
    }
  }
  return count;
}
  void trim(char *str)
{

        int i;
    int begin = 0;

    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}
int redirectionCheck(char *command)
{  
    // cout<<command<<endl;
  char *out = strstr(command, ">");
  char *in = strstr(command, "<");

  if((out != NULL) && (in != NULL)){
    //both inut and output redirection
    return 3;
  }else if(out != NULL){
    //output redirection only
    return 2;
  }else if(in != NULL){
    //input redirection only
    return 1;
  }else{
    return -1;
  }
}
//counts number of whitespaces in a given input string
int whitespaceCount(char *in, int len){
  int i = 0;
  int count = 0;
  for(i = 0; i < len; i++){
    if(in[i] == ' '){
      count++;
    }
  }
  return count;
}


//takes an input array and returns a char ** that points to
//the array of strings delimited by a space
int stitch(char *in, char **out){
  int i = 1;
  char *temp, cpy[100];

  sprintf(cpy, "%s", in);
  
  temp = strtok(in, " ");
  sprintf(out[0], "%s", temp);

  while((temp = strtok( NULL, " ")) != NULL){
    sprintf(out[i], "%s", temp);
    i++;
  }
  out[i] = NULL;
  return i;
}


//implements redirection
int redirection(char *input, int state){

  char cpy[1000], path[1000], command[1000], filename[1000], **args, *temp;
  int spacecount, i = 0, fd;

  sprintf(cpy, "%s", input);

  temp = strtok(cpy, " ");
  sprintf(path, "%s", temp);

  sprintf(cpy, "%s", input);

  if(state == 1){

    temp = strtok(cpy, "<");
    sprintf(command, "%s", temp);
    command[strlen(command) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';

    temp = strtok(NULL, "\0");
    sprintf(filename, "%s", temp);
    trim(filename);

    spacecount = whitespaceCount(command, strlen(command));

    args = (char**)calloc((spacecount)+2, sizeof(char *));
    for(i = 0; i < spacecount + 1; i ++){
      args[i] = (char*)calloc(strlen(command)+10, sizeof(char));
    }

    i = stitch(command, args);

    if((fd = open(filename, O_RDONLY, 0644)) < 0){
      perror("open error");
      return -1;
    }

    dup2(fd, 0);
    close(fd);

    execvp(path, (char *const *)args);
    perror("execvp error");
    _exit(EXIT_FAILURE);

  }else if(state == 2){
    // cout<<"two is called"<<endl;
    temp = strtok(cpy, ">");
    sprintf(command, "%s", temp);
    command[strlen(command) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';

    temp = strtok(NULL, "\0");
    sprintf(filename, "%s", temp);
    trim(filename);

    spacecount = whitespaceCount(command, strlen(command));

    args = (char**)calloc((spacecount)+2, sizeof(char *));

    for(i = 0; i < spacecount + 1; i ++){
      args[i] = (char*)calloc(strlen(command)+10, sizeof(char));
    }

    i = stitch(command, args);

    if((fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0){
      perror("open error");
      return -1;
    }

    dup2(fd, 1);
    close(fd);
    
    execvp(path, (char *const *)args);
    perror("execvp error");
    _exit(EXIT_FAILURE);    

  }else if(state == 3){
    char filename2[1000];
    int fd2;

    temp = strtok(cpy, "<");
    sprintf(command, "%s", temp);
    command[strlen(command) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';

    temp = strtok(NULL, ">");
    sprintf(filename, "%s", temp);
    filename[strlen(filename) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';
    trim(filename);

    temp = strtok(NULL, "\0");
    sprintf(filename2, "%s", temp);
    trim(filename2);

    spacecount = whitespaceCount(command, strlen(command));

    args =(char**) calloc((spacecount)+2, sizeof(char *));
    for(i = 0; i < spacecount + 1; i ++){
      args[i] = (char*)calloc(strlen(command)+10, sizeof(char));
    }

    i = stitch(command, args);

    if((fd = open(filename, O_RDONLY, 0644)) < 0){
      perror("open error");
      return -1;
    }

    if((fd2 = open(filename2, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0){
      perror("open error");
      return -1;
    }

    dup2(fd, 0);
    close(fd);

    dup2(fd2, 1);
    close(fd2);

    execvp(path, (char *const *)args);
    perror("execvp error");
    _exit(EXIT_FAILURE);

  }
  return 1;
}
//implements piping
int do_pipe(char **in, int *pipecount, int i){
  if(i == *pipecount - 1){
    //last process

    int x, spacecount, c;
    char ** args, cpy[1000];
    sprintf(cpy, "%s", in[i]);

    spacecount = whitespaceCount(in[i], strlen(in[i]));


    args =(char**)calloc((spacecount)+2, sizeof(char *));
    for(c = 0; c < spacecount + 1; c++){
      args[c] = (char*)calloc(strlen(in[i])+10, sizeof(char));
    }
    stitch(cpy, args);

    if((x = redirectionCheck(in[i])) < 0){
        execvp(args[0], args);
        perror("execvp error");
        _exit(EXIT_FAILURE);
      }
      redirection(in[i], x);
      return 1;
  }
  if(i < *pipecount){
    int fd[2], x, status, spacecount, c;
    pid_t pid;
    char ** args, cpy[1000];

    if(pipe(fd) < 0){
      perror("pipe");
      _exit(EXIT_FAILURE);
    }

    if((pid = fork()) < 0){
      perror("fork");
      _exit(EXIT_FAILURE);
    }

    if(pid != 0){
      dup2(fd[1], 1);
      close(fd[0]);
      in[i+1] = NULL;

      sprintf(cpy, "%s", in[i]);

      spacecount = whitespaceCount(in[i], strlen(in[i]));

      args = (char**)calloc((spacecount)+2, sizeof(char *));
      for(c = 0; c < spacecount + 1; c++){
        args[c] = (char*)calloc(strlen(in[i])+10, sizeof(char));
      }
      stitch(cpy, args);

      if((x = redirectionCheck(in[i])) < 0){
        execvp(args[0], args);
        perror("execvp error");
        _exit(EXIT_FAILURE);
      }

      redirection(in[i], x);
      waitpid(-1, &status, 0); //wait for all child processes to finish

    }else{
      if(i != *pipecount-1){
        dup2(fd[0], 0);
      }
      close(fd[1]);
      
      i+=1;
      do_pipe(in, pipecount, i);
    }
  }
  return 1;
}
// int main(int argc, char const *argv[])
int main()
{
	/* code */
    int i=0,status;
	// char cmd[1000];
 //    char* arg[100];
 //    char*parsed;
    // string s;
    pid_t p;
    // getline(cin,s);
    // cout<<"child"<<endl;
  while(true)
  {
     
  	// cout<<"iswhile"<<endl;
  	char cmd[1000];
    char * cmd1;
    char* arg[100];
    char*parsed;
    string s;
    i=0;

    getline(cin,s);
	strcpy(cmd,s.c_str());

	cmd1=(char*)calloc(s.size()+1,sizeof(char));
	
    strcpy(cmd1,s.c_str());

	parsed=strtok(cmd," ");
	
	int agc=0;
	// cout<<agc<<endl;
	while(parsed!=NULL)
	{
         // cout<<"hiii"<<endl;
         arg[i]=parsed;
         parsed=strtok(NULL," ");
         agc++;
         // cout<<agc<<endl;
         i++;

	}
	// argc--;
	// cout<<agc<<endl;


	arg[i]=NULL;
	// int numPipes = pipecount(arg, agc);
	// cout<<"pipetest"<<endl;
	// cout<<numPipes<<endl;
	// cout<<s<<endl;
	if(strcmp(arg[0],"exit")==0)
    {
    	// return 0;
    	// cout<<"exit entered"<<endl;
    	break;
    }

     if(strcmp(arg[0],"cd")==0)
    {   
         if(agc==1)
         {  
         	cout<<"single"<<endl;
         	char*home=getenv("HOME");
         	if(chdir(home)!=0)
         		perror("directory not changed");

         }
         else
         {
         	// cout<<"multiple"<<endl;
         	if(chdir(arg[1])!=0)
         		perror("directory not changed");
         	// chdir(arg[1]);
         }

    	
    }
    p=fork();

    if(p==0){
    // cout<<arg[0]<<endl;
    // cout<<*arg<<endl;

    int numPipes = pipecount(arg, agc);
    // cout<<numPipes<<endl;
    int x = 0;
    x = redirectionCheck(cmd1);
	
	//for pipe start from here.
	if(numPipes!=0)
	{
       char **commands = (char**) calloc(numPipes +1, sizeof(char*));
        for(i = 0; i < (numPipes +1); i++)
       {
        commands[i] = (char*)calloc((2*agc + 1), sizeof(char));
       }

      
    parse(arg, commands, numPipes, agc);

    for(i = 0; i < (numPipes + 1); i++)
      {
        trim(commands[i]);
      }

      int count = numPipes+1;
      do_pipe(commands, &count, 0);

	}
   else if(x>0)
   {
      redirection(cmd1, x);

   }
   else if(x<0)
   {     
   	// cout<<"last if"<<endl;
         // cout<<arg[0]<<endl;
         // cout<<arg[1]<<endl;
          execvp(arg[0],arg);
          perror("execvp error");
          _exit(EXIT_FAILURE);

   }
  
            


    }

    //parent process starts here.
    else
    {
    	// wait(NULL);
    	waitpid(-1, &status, 0);
    	continue;
    }
   
   i=0;
  }
  //while ends here.

	return 0;
}
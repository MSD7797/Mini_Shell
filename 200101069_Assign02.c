#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(){
    
    //COPIES OF A LOT OF VARIABLES ARE MADE BECAUSE STRTOK MODIFIES THE ORIGINAL INPUT , 
    //HENCE COPYING IS DONE TO KEEP ORIGINAL INPUT SAFE FOR FURTHER USAGE(IF ANY).

    //EXPLANATION OF SOME OF THE FUNCTIONS USED HERE IS GIVEN IN README FILE TO MAKE THE CODE MORE COMPREHENSIVE.
    //HERE ONLY WHAT A PARTICULAR PART OF A CODE DOES IS EXPLAINED(NOT THE USAGE OF THE FUNCTIONS).
    
    //AFTER EVERY COMMAND CHECK , IF THE COMMAND CHECK IS CORRECT , THEN AFTER EXECUTING THE COMMAND , 
    //THE MEMORY ALLOCATED TO THE INPUT IS FREED AND EITHER CONTINUE OR BREAK IS USED TO SKIP THAT ITERATION OF LOOP 
    //OR TERMINATE IT AS PER THE NEED.
    
    //array to store current directory
    
    char curr_dir[1000];
    
    //creating history.txt to store the commands given
    
    FILE *fp = fopen("/tmp/history.txt","a+");
    
    //the variable line store value of PATH environment variable needed for execv() implementation
    
    char *line=getenv("PATH");
    char *linecopy=(char *)malloc(strlen(line) +1);
    char *linecopys=(char *)malloc(strlen(line) +1);
    
    //whereever integer variables are needed , variables are created in alphabetical order to avoid confusion.
    
    int a=0;
    int b=0;
    while(1){
    	
    	// dynamic memory allocation to store input command.
        
        char *input = (char*)malloc(sizeof(char)*1000);
        char *inputcopy = (char*)malloc(sizeof(char)*1000);
        char *inputagain = (char*)malloc(sizeof(char)*1000);
        char *inputthirdcopy = (char*)malloc(sizeof(char)*1000);
        
        //gets current working directory and storing it in curr_dir and printing it.
        
        getcwd(curr_dir,1000);
        printf("%s\n>>> ",curr_dir);
        
        //scanning input command
        
        scanf(" %[^\n]s",input);
        strcpy(inputcopy,input);
        strcpy(inputagain,input);
        strcpy(inputthirdcopy,input);
        
        //breaking input into words to check for presence of environment variable and internal commands like cd,exit,x,history,quit
        
        char *wordss=strtok(inputthirdcopy," \t");
        
        //wordarray is the array of words of the string got using strtok.
        
        char **wordarray=(char **)malloc(sizeof(char)*1000);
        
        //q denotes the word count in the input.
        
        int q=0;
        while(wordss != NULL){
            wordarray[q]=wordss;
            q++;
            wordss=strtok(NULL," \t");
        }
        
        //if you give the name of the environment variable , it will print its value. 
        //Eg: if input is USER,then output is value of the environment variable USER.(uppercase)
        
        if(getenv(input)){
            printf("%s\n",getenv(input));
            fprintf(fp,"%s\n",input);
            free(input);
            continue;
        }
        
        //checks whether echo command is given for printing the value of a environment variable or not. 
        //(checked for only 5 environment variables)
        
        if(q==2 && wordarray[0][0]=='e' && wordarray[0][1]=='c' && wordarray[0][2]=='h' && wordarray[0][3]=='o' && wordarray[1][0]=='$'){
            if(wordarray[1][1]=='U' && wordarray[1][2]=='S' && wordarray[1][3]=='E' && wordarray[1][4]=='R'){
            	printf("%s\n",getenv("USER"));
            }
            if(wordarray[1][1]=='P' && wordarray[1][2]=='A' && wordarray[1][3]=='T' && wordarray[1][4]=='H'){
            	printf("%s\n",getenv("PATH"));
            }
            if(wordarray[1][1]=='S' && wordarray[1][2]=='H' && wordarray[1][3]=='E' && wordarray[1][4]=='L' && wordarray[1][5]=='L'){
            	printf("%s\n",getenv("SHELL"));
            }
            if(wordarray[1][1]=='H' && wordarray[1][2]=='O' && wordarray[1][3]=='M' && wordarray[1][4]=='E'){
            	printf("%s\n",getenv("HOME"));
            }
            if(wordarray[1][1]=='T' && wordarray[1][2]=='E' && wordarray[1][3]=='R' && wordarray[1][4]=='M'){
            	printf("%s\n",getenv("TERM"));
            }
            fprintf(fp,"%s\n",input);
            free(input);
            continue;
        }
        
        // to set user defined environment variables.
        
        if(q==4 && !strcmp(wordarray[0],"setenv")){
            setenv(wordarray[1],wordarray[3],1);
            fprintf(fp,"%s\n",input);
            free(input);
            continue;
        }
        
        //Checks if the command is for changing the directory.if only cd is given , 
        //then directory changed to /home/user otherwise it is changed to the specified directory.
        
        else if(wordarray[0][0]=='c' && wordarray[0][1]=='d' && strlen(wordarray[0])==2){
            if(q==1){
                chdir("..");
                fprintf(fp,"%s\n",input);
                free(input);
                continue;
            }
            else if(q>1){
                chdir(wordarray[1]);
                fprintf(fp,"%s\n",input);
                free(input);
                continue;
            }
        }
        
        //checks if input command is history and prints the history of commands given earlier on.
        
        else if(wordarray[0][0]=='h' && wordarray[0][1]=='i' && wordarray[0][2]=='s' && wordarray[0][3]=='t'  && wordarray[0][4]=='o' && wordarray[0][5]=='r' && wordarray[0][6]=='y' && strlen(wordarray[0])==7 && q==1){
            fprintf(fp,"%s\n",input);
    	    fseek(fp,0,SEEK_SET);
    	    char c;
    	    c=fgetc(fp);
    	    while(c != EOF){
    	        printf("%c",c);
    	        c=fgetc(fp); 
    	    }
    	    free(input);
    	    continue;
        }
        
        //checks if the input is x or quit or exit , if yes , then program is terminated.
        
        else if((wordarray[0][0]=='x' && strlen(wordarray[0])==1 && q==1)||(wordarray[0][0]=='e' && wordarray[0][1]=='x' && wordarray[0][2]=='i' && wordarray[0][3]=='t'  && strlen(wordarray[0])==4 && q==1)){
    	    if(remove("/tmp/history.txt")==0){
    	    	free(input);
    	        break;
    	    }
        }
        else if(q==1 && wordarray[0][0]=='q' && wordarray[0][1]=='u' && wordarray[0][2]=='i' && wordarray[0][3]=='t'  && strlen(wordarray[0])==4){
    	    if(remove("/tmp/history.txt")==0){
    	    	free(input);
    	        break;
    	    }
        }
        int j;
        int k=0;
        
        //checking for presence of pipe in input.
        
        for(j=0;j<strlen(input);j++){
            if(input[j]=='|'){
            	k=1;
            }
        }
        
        //if pipe is absent.
        
        if(k==0){
            
            //breaking input into words to check for linux external command.
            
            char *word=strtok(inputcopy," \t");
            char **required=(char **)malloc(sizeof(char)*1000);;
            int e=0;
            int f=0;
            int g=0;
            while(word != NULL){
            	required[e]=word;
            	e++;
            	word=strtok(NULL," \t");
            }
            
            // checking for presence of redirection in input.
            //variable g denotes the type of redirection present.
            
            for(f=0;f<e;f++){
            	if(required[f][0]=='>' && strlen(required[f])==1){
            	    if(g==0){
            	    	g=1;
            	    }
            	    else if(g==2){
            	    	g=4;
            	    }
            	    break;
            	}
            	if(required[f][0]=='<' && strlen(required[f])==1){
            	    g=2;
            	}
            	if(required[f][0]=='>' && required[f][1]=='>' && strlen(required[f])==2){
            	    if(g==0){
            	    	g=3;
            	    }
            	    else if(g==2){
            	    	g=5;
            	    }
            	    break;
            	}
            }
            
            //storing all the paths in the PATH variable in an array of strings. paths are separated by ':'
            
            strcpy(linecopy,line);
            
            //strtok breaks string into parts based on the delimiter given.
            
            char *token=strtok(linecopy,":");
            char **newvar=(char **)malloc(sizeof(char)*1000);;
            int h=0;
            while(token != NULL){
            	newvar[h]=token;
            	h++;
    	    	token = strtok(NULL,":");
            }
            int i;
            int z=0;
            
            //searching for the presence of input command in the paths defined in PATH variable.
            
            for(i=0;i<h;i++){
    	    	char *dest=(char*)malloc(sizeof(char)*1000);;
    	    	strcpy(dest,newvar[i]);
    	    	strcat(dest,"/");
    	    	strcat(dest,required[0]);
    	    	if(access(dest,F_OK)==0){
    	    	    z=1;
    	    	    
    	    	    //if input command is found and no redirection then executing command using g==0 if block.
    	    	    //here forking is used so that parent program does not terminate after execv call
    	    	    // a child process is executed
    	    	    
    	    	    if(g==0){
    	            	required[e]=NULL;
    	            	pid_t pid;
    	            	pid = fork();
    	            	if (pid==0){
    	            	    execv(dest,required);
    	            	}
    	            	else if(pid>0){
    	            	    wait(NULL);
    	            	}
    	            	fprintf(fp,"%s\n",input);
    	            	free(input);
    	            	break;
    	            }
    	            
    	            //if input command is found and we have to write output to a file, then executing command using g==1 if block.
    	            //creates new file if it is not existing.
    	            
    	            if(g==1){
    	            	required[f]=NULL;
    	            	pid_t abc;
    	            	abc=fork();
    	            	if(abc==0){
    	            	    int outfile=open(required[e-1], O_WRONLY | O_CREAT);
    	            	    dup2(outfile,1);
    	            	    close(outfile);
    	            	    required[f+1]=NULL;
    	            	    execv(dest,required);
    	            	}
    	            	else if(abc>0){
    	            	    wait(NULL);
    	            	}
    	            	fprintf(fp,"%s\n",input);
    	            	free(input);
    	            	break;
    	            }
    	            
    	            //if input command is found and we have to read input from a file, then executing command using g==2 if block.
    	            
    	            if(g==2){
    	            	required[e-2]=NULL;
    	            	pid_t def;
    	            	def=fork();
    	            	if(def==0){
    	            	    int inputfile=open(required[e-1], O_RDONLY);
    	            	    dup2(inputfile,0);
    	            	    close(inputfile);
    	            	    required[f+1]=NULL;
    	            	    execv(dest,required);
    	                }
    	                else if(def>0){
    	            	    wait(NULL);
    	                }
    	                fprintf(fp,"%s\n",input);
    	            	free(input);
    	            	break;
    	            }
    	            
    	            //if input command is found and we have to append output to a file, then executing command using g==3 if block.
    	            //creates the file if the file does not exist already.
    	            
    	            if(g==3){
    	            	required[f]=NULL;
    	            	pid_t fgh;
    	            	fgh=fork();
    	            	if(fgh==0){
    	            	    int appendfile=open(required[e-1], O_APPEND | O_RDWR | O_CREAT);
    	            	    dup2(appendfile,1);
    	            	    close(appendfile);
    	            	    required[f+1]=NULL;
    	            	    execv(dest,required);
    	            	}
    	               else if(fgh>0){
    	            	    wait(NULL);
    	               }
    	               fprintf(fp,"%s\n",input);
    	               free(input);
    	               break;
    	            }
    	            
    	            //g==4 if block
    	            //if we have to take input from a file and print the output to another file,creates output file if it doesn't exist.
    	            
    	            if(g==4){
    	            	required[e-4]=NULL;
    	            	required[e-2]=NULL;
    	            	pid_t newpid;
    	            	newpid=fork();
    	            	if(newpid==0){
    	            	    int inputfile=open(required[e-3], O_RDONLY);
    	            	    dup2(inputfile,0);
    	            	    close(inputfile);
    	            	    int outfile=open(required[e-1], O_WRONLY | O_CREAT);
    	            	    dup2(outfile,1);
    	            	    close(outfile);
    	            	    required[f-1]=NULL;
    	            	    required[f+1]=NULL;
    	            	    execv(dest,required);
    	                }
    	                else if(newpid>0){
    	            	    wait(NULL);
    	                }
    	                fprintf(fp,"%s\n",input);
    	            	free(input);
    	            	break;
    	            }
    	            
    	            //g==5 if block
    	            //if we have to take input from a file and append output to another file,creates the output file if it doesn't exist.
    	            
    	            if(g==5){
    	            	required[e-4]=NULL;
    	            	required[e-2]=NULL;
    	            	pid_t apppid;
    	            	apppid=fork();
    	            	if(apppid==0){
    	            	    int inputfile=open(required[e-3], O_RDONLY);
    	            	    dup2(inputfile,0);
    	            	    close(inputfile);
    	            	    int appendfile=open(required[e-1], O_APPEND | O_RDWR | O_CREAT);
    	            	    dup2(appendfile,1);
    	            	    close(appendfile);
    	            	    required[f-1]=NULL;
    	            	    required[f+1]=NULL;
    	            	    execv(dest,required);
    	                }
    	                else if(apppid>0){
    	            	    wait(NULL);
    	                }
    	                fprintf(fp,"%s\n",input);
    	            	free(input);
    	            	break;
    	            }
    	        }
            }
            
            //checks for presence  of a invalid command.
            
            if(z==0 && g==0){
            	printf("Invalid Command\n");
            	fprintf(fp,"%s\n",input);
    	        free(input);
            }
            continue;
        }
        else{
            
            //this part is executed if piping is present.
            
            FILE *fp2=fopen("pipe.txt","w+");
            
            //breaking the input string into words.
            
            char *word=strtok(inputagain," \t");
            char **required1=(char **)malloc(1000*sizeof(char));
            char **required2=(char **)malloc(1000*sizeof(char));
            int k=0;
            int l=0;
            int m=100;
            while(word != NULL){
            	if(word[0]=='|' && strlen(word)==1){
            	    m=k;
            	}
            	if(k<=m){
            	    required1[k]=word;
            	    k++;
            	}
            	else{
            	    required2[l]=word;
            	    l++;
            	}
            	word=strtok(NULL," \t");
            }
            
            //required1 contains command preceding the pipe and required2 contains command present after the pipe.
            // both are arrays of strings
            
            strcpy(linecopys,line);
            
            //storing all the paths in the PATH environment variable in an array of arrays newvars.
            
            char *tokens=strtok(linecopys,":");
            char **newvars=(char **)malloc(1000*sizeof(char));
            int j=0;
            while(tokens != NULL){
            	newvars[j]=tokens;
            	j++;
    	    	tokens = strtok(NULL,":");
            }
            int i;
            
            //finding input commands in the paths and executing them.
            //this loop is for the command before the pipe.
            
            for(i=0;i<j;i++){
    	    	char *dest=(char *)malloc(100*sizeof(char));;
    	    	strcpy(dest,newvars[i]);
    	    	strcat(dest,"/");
    	    	strcat(dest,required1[0]);
    	    	if(access(dest,F_OK)==0){
    	    	    required1[k-1]=NULL;
    	    	    pid_t pipe1;
    	            pipe1=fork();
    	            if (pipe1==0){
    	            	int pipefile=open("pipe.txt", O_RDWR | O_CREAT);
    	            	dup2(pipefile,1);
    	            	close(pipefile);
    	            	execv(dest,required1);
    	            }
    	            else if(pipe1>0){
    	            	wait(NULL);
    	            }
    	            break;
    	        }
    	    }
    	    
    	    //this loop is for the command after the pipe. reads the output of the first command before the pipe and 
    	    //is feeded as input of the second command after the pipe.the pipe output is printed on the terminal.
    	    
    	    for(i=0;i<j;i++){
    	    	char *dest2=(char *)malloc(100*sizeof(char));;
    	    	strcpy(dest2,newvars[i]);
    	    	strcat(dest2,"/");
    	    	strcat(dest2,required2[0]);
    	    	if(access(dest2,F_OK)==0){
    	    	    required2[l]=NULL;
    	    	    pid_t pipe2;
    	            pipe2=fork();
    	            if (pipe2==0){
    	            	int pipefile=open("pipe.txt", O_RDWR | O_CREAT);
    	            	dup2(pipefile,0);
    	            	close(pipefile);
    	            	execv(dest2,required2);
    	            }
    	            else if(pipe2>0){
    	            	wait(NULL);
    	            }
    	            break;
    	        }
    	    }
    	    remove("pipe.txt");
    	    fprintf(fp,"%s\n",input);
    	    free(input);
    	    continue;
        }
    }
}


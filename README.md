# Mini_Shell

This is the readme file for the C code written for mini shell wit some specifications and warnings regarding the input given in the terminal.

In the actual program file , just short comments are added to tell the purpose of that section of the program and as the program is already too long , so short comments are added. Proper explanation is provided at the end of this file which will help a lot in understanding , how everything is done in my program.

PLEASE READ THE ENTIRE README FILE ONCE BEFORE EXECUTING THE PROGRAM.

Commands to be given in terminal to execute the program:
1)g++ 200101069_Assign02.c
2)./a.out

The first command will compile the file and make it an executable.
The second command will execute the executable thus formed.

Specifications of the input:
1)Give atleast a single space before and after the pipe(|) sign and the redirections sign(<,>,>>).
Eg: cat file1.txt | grep xyz
    cat file1.txt > file2.txt
    cat < file1.txt
    cat file1.txt >> file2.txt
2)For printing value of the environment variable , you can use the following commands.
  Remember to give the name of the environment variable in capital letters.
  i)only give environment variable name
  ii)printenv environment variable
  iii)echo $environment_variable(as mentioned by sir , by using echo , i made only five environment variables)
      They are : SHELL , USER , PATH , TERM , HOME
Eg: USER
    printenv SHELL
    evho $PATH
3)Other than environment variables , all the input commands should be in small letters.
4)I have made a provision of handling extra whitespaces if given by mistake but because of the shortcomings of the programming language , you might get unexpected results sometimes after giving extra spaces. So try to give only single space whenever it is required. (You can try and chaeck my provision of extra spaces and tabs if you want). For commands that i used , it worked well.
5)After using the '>' operation , if the file did not exist earlier on , it will get created and after its creation , check its permissions. Sometimes , its the issue that the permission of the file are neither read nor write , so if you use '> again , instead of getting the output in the file , you might get the output on the terminal.
6)If you are using any command like grep for matching patterns or echo for printing , dont use quotes.
7)Among the list of commands provided by sir in a mail , the set of commamds that my program can execute are:
man,which,chsh,whereis,passwd,date,cal,clear,sleep,history,apropos,exit,x,quit,shutdown,echo,ls,cat,more,less,touch,cp,mv,rm,script,find, mkdir,cd,pwd,rmdir,chmod,grep,printenv,env,setenv,df.
8)This program has the provision of setenv.Note that there should be a space before and after equal to sign for command to work well.
  var_value should be continuous i.e. it should not have spaces within it.
  Format of setenv:
  setenv var_name = var_value
9)As confirmed from sir , as all the internal commnads cannot be executed by making different loops from them , so only the following internal commands are run by making loops and rest of the internal commands using execv.
They are : processing of environment variable(1st and 3rd way),cd,history,setenv,exit,x and quit.
10)Single level piping is there and the following redirections are possible:
  i)   command > filename           (writing output to a file)
  ii)  command < filename           (reading input from a file)
  iii) command >> filename          (appending output to a file)
  iv)  command < file1 > file2      (reading input from file1 and writing output in file2)
  v)   command < file1 >> file2     (reading input from file1 and appending output in file2)
11)As mentioned by sir , there is no provision for combination of redirection and piping.
12)Take extra care that there are no spaces in between the name of a file.

Explanation of the Program to help in comprehending the program :
----------------------------------------------------------------
----------------------------------------------------------------

Using the curr_dir variable, we store the current working directory in it by using getcwd function of c. The variable 'input' is used to store the command given in the terminal. Its several copiess are made to keep input safe from strtok. The variable line stores the value of the environment variable path. Input is broken into words by strtok based on given delimiter(whitespaces here). Then by using those words and comparing them to internal commands cd,echo $env_variable,history,x,quit or exit.cd is executed using chdir function of C used to change directory to the path given.A provision for creating new environment variables is provided by using the setenv command with command given in the correct format. If input is x,quit or exit , then history file is removed and program is terminated.Then we are checking for the presence of pipe. Variable 'k' denotes the presence or absence of pipe. If there is no pipe , then another copy of input is broken into words by strtok.Then among the 5 types of redirection , we  are checking the type of redirection present and variable 'g' denotes the type of redirection in input or there is no redirection. Then , we are breaking a copy of the PATH variable into different paths based on the delimiter ':' and are checking where the input command is present.Then , by using fork() command for all types of redirection to create child process for execution so that our program does not terminate and this is done using execv().During or after execution of each command, the input is written into the history and the memory allocated for the input variable is freed. An invalid command check is made to confirm whether the input command is valid or not.Now , if the pipe is present , then the command is broken into two arrays each consisting of words , the first of the words before the pipe and the second of the words after the pipe. Then as per the concept of the pipe , the output of the first command is fed as the input of the second command and the output of the terminal is printed on the screen. This is achieved using storing output of 1st command in a temporary file and then taking that file as input of the 2nd command and hence the program does its job.

The link to the Command Spell Checker using **Perl** is : https://github.com/MSD7797/Perl_Command_Rectifier

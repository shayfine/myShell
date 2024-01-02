// authors : shay finegold 311165609 , dan monsonego 313577595
 


#include "myShell.h"


void printDir() //printing the current dir
{
	char cwd[MAX_LEN];
	getcwd(cwd,sizeof(cwd));
	printf("%s ",cwd);
}

char *readLine(char *p)  // reading the given line from the user
{
	char *buffer = malloc(MAX_LEN);
	printDir();
	printf("%s " , p);
	fgets(buffer,MAX_LEN,stdin);
	buffer[strlen(buffer) -1 ] = '\0';
	return buffer;
}

void myPipe(char** arr) // pipe operator for tow commands
{
	int i =0;
	int sizeOfFirst = 0;
	int sizeOfSecond = 0;
	char **temp = arr;

	while(**arr != '|') // counting the words of the first command
	{
		sizeOfFirst++;
		i++;
		arr++;
	}

	i = 0;

	while(*arr) // counting the words of the second command
	{
		sizeOfSecond++;
		i++;
		arr++;
	}
	sizeOfSecond = sizeOfSecond - 1;

	char *arrFirst[sizeOfFirst];
	char *arrSecond[sizeOfSecond];
	i = 0;

	while(**temp != '|' ) // making the first command list
	{
		arrFirst[i] = *temp;
		temp++;
		i++;
	}
	arrFirst[i] = NULL;
    
	temp++;
	i=0;

	while(*temp) // making the second command list
	{
		arrSecond[i] = *temp;
		temp++;
		i++;
	}
	arrSecond[i] = NULL;

	int p[2];
	pipe(p);

	if(fork() == 0)
	{
		close(p[0]);
		dup2(p[1],1);
		execvp(arrFirst[0],arrFirst);
	}
	
	if(fork() == 0)
	{
		close(p[1]);
		dup2(p[0],0);
		execvp(arrSecond[0],arrSecond);
	}
}

void helpFunction() {
    printf("------------------------\n");
    printf("The following commands are available:\n");
    printf("pwd : Display current working directory\n");
    printf("nano : Open the nano text editor\n");
    printf("clear : Clear the screen\n");
    printf("grep : Search for a string in a file\n");
    printf("wc : Count the number of lines, words, and characters in a file\n");
    printf("cp : Copy a file or directory\n");
    printf("wc : Count the number of lines, words, and characters in a file\n");
    printf("ls : List files and directories in the current directory\n");
    printf("ls -l > output_file : List files and directories in the current directory and save output to a file\n");
    printf("myShell supports pipe operator for two commands\n");
    printf("Enter your commands and arguments, then press enter.\n");
    printf("Use '|' for pipe operator and '>' for output redirection.\n");
}

void executeCommand(char **info) // execute function
{
	

	pid_t childPid;
	int status; 
	int countLetters = 0;
	int countWords = 0;
	char **info2 = info;
	char **info3 = info;
	char **info4 = info;
	int arraySize;

		
	// making the parameter list
	while(*info2)
	{
		countLetters = countLetters + (int)strlen(*info2);
		++info2;
	}

	while(*info3)
	{
		countWords++;
		++info3;
	}

	char *newArray[countWords];
	int i = 0;

	while(*info4)
	{
		newArray[i] = *info4;
		++info4;
		i++;
	}

	newArray[i] = NULL;
	int j = 0;
    childPid = fork();

	
	while(newArray[j] != NULL && childPid == 0) // in case we need a pipe operation
	{
		if(newArray[j][0] == '|')
		{		
			myPipe(newArray);
			exit(0);
		}
		j++;
	}


	int k = 0;

	while(newArray[k] != NULL && childPid == 0) // for the ls -l > output_file
	{
		if(newArray[k][0] == '>' && (newArray[0][0] == 'l' && newArray[0][1] == 's'))
		{
			printf("new file named %s with ls -l content has created\n",newArray[k + 1]);	
			int fd;
			char *args[] = {"ls","-l",NULL};

   			fd = open(newArray[k + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
   			if (fd == -1) 
			{
       			perror("open");
    		}

    		if (dup2(fd, STDOUT_FILENO) == -1) {
        		perror("dup2");
    		}
			
    		if (execvp(args[0], args) == -1)
			{
       			perror("execvp");
    		}
		}
		k++;
	}



	if(childPid == 0) // child procces
	{
		if(newArray[0][0] == 'p' && newArray[0][1] == 'w' && newArray[0][2] == 'd'
		|| newArray[0][0] == 'n' && newArray[0][1] == 'a' && newArray[0][2] == 'n' && newArray[0][3] == 'o'
		|| newArray[0][0] == 'c' && newArray[0][1] == 'l' && newArray[0][2] == 'e' && newArray[0][3] == 'a'
		|| newArray[0][0] == 'g' && newArray[0][1] == 'r' && newArray[0][2] == 'e' && newArray[0][3] == 'p'
		|| newArray[0][0] == 'w' && newArray[0][1] == 'c'
		|| newArray[0][0] == 'c' && newArray[0][1] == 'p')
		{
			execvp(newArray[0],newArray);
			exit(0);		
		}
		else if(newArray[0][0] == 'l' && newArray[0][1] == 's') //ls commeand
		{
			execvp(newArray[0],newArray);
			exit(0);
		}
		else if(newArray[0][0] == 't' && newArray[0][1] == 'r' && newArray[0][2] == 'e' && newArray[0][3] == 'e') //tree commeand
		{
			char cwd[MAX_LEN];
			getcwd(cwd,sizeof(cwd));
			char *a[] ={"./tree",cwd,NULL};
			execvp(a[0],a);
		}
		else if(newArray[0][0] == 'e' && newArray[0][1] == 'x' && newArray[0][2] == 'i' && newArray[0][3] == 't') //exit commeand
		{
			printf("---------- Exit bye bye ----------");
			fflush(stdout);
			kill(0,SIGINT);
		}
		else if(newArray[0][0] == 'h' && newArray[0][1] == 'e' && newArray[0][2] == 'l' && newArray[0][3] == 'p') //help commeand
		{
			helpFunction();
		}		
		else if(newArray[0][0] == 'c' && newArray[0][1] == 'a' && newArray[0][2] == 't')
		{
			if(newArray[1][0] == '>') //create a file
			{
				execlp("nano","nano",newArray[2],NULL);
				exit(0);
			}
			else  //view the contents of the file
			{
				execvp(newArray[0],newArray);
			    exit(0);
			}			
		}
		else
		{
			if(!(newArray[0][0] == 'c' && newArray[0][1] == 'd'))
			{
				printf("Command not found\n");
			}
			exit(0);
		}	
	}
	else
	{
		if(newArray[0][0] == 'c' && newArray[0][1] == 'd')
		{
			chdir(newArray[1]);
		}
        waitpid(childPid,&status,0); 
	}
		
}



char **parse(char *buf)
{
	int wc = 0;
	char *buf2 = buf;
	int state = 0;
	char *token;
	int i = 0;

	while(*buf)  //count the words
	{
		if(*buf == ' ' || *buf == '\n' || *buf == '\t' )
		{
			state = 0;
		}
		else if(state == 0)
		{
			state = 1;
			++wc;
		}
		++buf;
	}

	char **buffer = malloc(MAX_LEN*sizeof(char*));
	token = strtok(buf2," "); //split into tokens

	while(token != NULL)  //copy the tokens to an array
	{
		buffer[i++] = token;
		token = strtok(NULL," ");	
	}

	i = 0;


return buffer;


}



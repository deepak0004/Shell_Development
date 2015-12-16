// author : Deepak Thukral 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<time.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>

#define MOD 1000000007
#define I_MAX 9223372036854775807
#define I_MIN -9223372036854775807

#define rep(i,s,e) for(i=s;i<=e;i++)
#define rrep(i,s,e) for(i=s;i>=e;i--)
#define endl "\n"

#define ll long long
#define mid(a,b) ((a)+((b-a)/2))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define pb push_back
#define mp make_pair
#define f first
#define s second
#define si set<int>
#define vi vector<int>
#define ii pair<int,int>
#define sii set<ii>
#define vii vector<ii>
#define all(c) c.begin(),c.end()
#define tr(c,it) for(typeof(c.begin()) it=c.begin();it!=c.end();++it)

#define DEBUG
// debugging
#ifdef DEBUG
#define trace1(x)                    cerr << #x << ": " << x << endl;
#define trace2(x, y)                 cerr << #x << ": " << x << " | " << #y << ": " << y << endl;
#define trace3(x, y, z)              cerr << #x << ": " << x << " | " << #y << ": " << y << " | " << #z << ": " << z << endl;
#define trace4(a, b, c, d)           cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c << ": " << c << " | " << #d << ": " << d << endl;
#define trace5(a, b, c, d, e)        cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c << ": " << c << " | " << #d << ": " << d << " | " << #e << ": " << e << endl;
#define trace6(a, b, c, d, e, f)     cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c << ": " << c << " | " << #d << ": " << d << " | " << #e << ": " << e << " | " << #f << ": " << f << endl;
#else
#define trace1(x)
#define trace2(x, y)
#define trace3(x, y, z)
#define trace4(a, b, c, d)
#define trace5(a, b, c, d, e)
#define trace6(a, b, c, d, e, f)
#endif

int check_if_valid(char *tokens,char *valid[],int n);
void break_into_comp(char words[],char token[][2000]);
void Execio(char token[][2000], int pos);
void Execute(char token[][2000]);
void Exec(char token[][2000],int pos);

///////////////////////////////////////////////////////////////
int pfds[2];
struct his
{
	char command[2000];
	struct his *move_to_next;
};
///////////////////////////////////////////////////////////////

int main()
{
	signal(SIGINT, SIG_IGN);
	time_t current_time;
	
	char tokens[10][2000];
	char *All_valid_comm[17] =                                 
        {  
           "cat", "cp", "cd", "gedit", "ls", "pwd", "mkdir", "rmdir", "rm", "top", "exit", "help", "clear", "history", "kill", "grep", "fgrep"
        };
	
        char *c_string;
	char word[2000];

        int n = 17;
	FILE *fp;
	struct his *history = NULL;
	
        while(1)
	{
		char dir[100];
		getcwd(dir, 100*sizeof(char));
		
                printf("%s>>", dir);
		printf("deepak@RShell$ ");
		
                fgets(word, 2000, stdin);
		fp = fopen("Hist.txt","a");
		
                if(history==NULL)
		{
			history = (struct his *)malloc(sizeof(struct his));
			strcpy(history->command, word);
			current_time = time(NULL);
			c_string = ctime(&current_time);
			fprintf(fp, "%s->%s\n", c_string, history->command);
			history->move_to_next = NULL;
		}
		else
		{
			struct his *temp;
			temp = (struct his *)malloc(sizeof(struct his));
			strcpy(temp->command, word);
			current_time = time( NULL );
			c_string = ctime(&current_time);
			fprintf(fp, "%s->%s\n", c_string, temp->command);
			temp->move_to_next = history;
			history = temp;
		}
		
                 
                // Break into components  
                break_into_comp(word,tokens);		
                

                int i, len = strlen(word);                 
 
                // For piplining 
                int flag=0, pos = 0;
		rep(i, 0, len-1)
		{
                        if( word[i]==' ' )
                        {
                           pos++;
                        }   
			else if(word[i]=='|')
			{
			   flag = 1;
	                   break;
			}
		}
		
                // For redirection  
                int flag1 = 0, pos1 = 0;
		rep(i, 0, len-1)
		{
			if( word[i]==' ' )
			{
			     pos1++;
			} 
                        else if( (word[i]=='>') || (word[i]=='<') )
			{
			     flag1 = 1;
			     break;
			}
		}
		

                if((flag==0) && (flag1==0))
		{
                        // Useless tokens  
			if( (strcmp(tokens[0],"\0")==0) || (strcmp(tokens[0],"\n")==0) || (strcmp(tokens[0],"\t")==0) )
			{
                           continue;
 			}
                        // For exiting  
                        else if(strcmp(tokens[0],"exit") == 0)
			{
                           exit(0);
			}
                        // Invalid commands 
                        if( check_if_valid(tokens[0],All_valid_comm,n)==-1 )
			{
	                    printf("Command is not valid. See help section for commands valid for this shell");
			    continue;
			}  
                        // cd
			else if( strcmp(tokens[0],"cd")==0 )
			{
			    chdir( tokens[1] );
			}
                        // history
			else if( strcmp(tokens[0], "history")==0 )
			{
			    printf("\nPreviously used commands are :\n");

			    if(fork()==0)
			    {
				execl("/home/user/Desktop/Shell/cmd/cat","cat","Hist.txt",NULL);
			    }
			    else
			    {
				wait(NULL);
		            }
			}
                        // help section 
			else if( strcmp(tokens[0],"help")==0 )
			{
			    rep(i, 0, n-1)
			    {
				printf("%d. %s\n", i+1, All_valid_comm[i]);
		            }
			    printf("\n");
			    continue;
			}
                        // Rest of commands 
			else
			{
			    Execute(tokens);
			}
		}
                // Redirection 
		else if( (flag==0) && (flag1==1) )
		{
		    Execio(tokens, pos1);
		}
                // Piplining
                else if( (flag==1) && (flag1==0) )
		{
	            Exec(tokens, pos);
		}
 



                strcpy(tokens[5], "");
		strcpy(tokens[6], "");
		strcpy(tokens[7], "");
		strcpy(tokens[8], "");
		strcpy(tokens[9], "");
		strcpy(tokens[0], "");
		strcpy(tokens[3], "");
		strcpy(tokens[4], "");
 		strcpy(tokens[1], "");
		strcpy(tokens[2], "");
                strcpy(word, "");
		
                fclose( fp );
	}


	return 0;
}



////////////////////////////////////////////////////////////////
int check_if_valid(char *tokens,char *valid[],int n)
{
	int deepak_flag = 0, i;
	rep(i, 0, n-1)
	{
		if(strcmp(tokens, valid[i])==0)
		{
			deepak_flag = 1;
			break;
		}
	}
	if(deepak_flag!=1)
	{
          return -1;
	}
        return deepak_flag; 
}

//////////////////////////////////////////////////////////////
void break_into_comp(char words[],char token[][2000])
{
	ll i, len = strlen(words), j = 0, k = 0;
        
        rep(i, 0, len-1)
        {
            if( (words[i]=='\n') || (words[i]==' ') )
	    {
		token[j][k] = '\0';
		k = 0;
         	j++;
	    }
	    else
	    {
		token[j][k] = words[i];
         	k++;
	    }
        }      
	token[j][k++] = '\0';
}
////////////////////////////////////////////////////////////////

// Redirection
////////////////////////////////////////////////////////////////
void Execio(char token[][2000], int pos)
{
	int fd, fd1, pid = fork();
	if(pid==0)
	{
                if( strcmp(token[pos],"<")==0 )
		{
			fd1 = open(token[pos+1],0);
			dup2(fd1,STDIN_FILENO);
			close(fd1);
		} 
		else if( strcmp(token[pos],">")==0 )
		{
			fd = creat(token[pos+1], 0644);
			dup2(fd,STDOUT_FILENO);
			close(fd);
		}
		
		if(pos==1)
		{
		        char path[50]="/home/user/Desktop/Shell/cmd/";
			strcat(path, token[0]);
			if(execl(path, token[0], NULL)<0)
			{
			   printf("Problem : Failed execution\n");
			   exit(1);
			}
	        }
                else if(pos==3)
	        {
		        char path[50]="/home/user/Desktop/Shell/cmd/";
			strcat(path,token[0]);
		        if(execl(path, token[0], token[1], token[2], NULL)<0)
			{
			   printf("Problem : Failed execution\n");
			   exit(1);
			}
	        }
		else if(pos==2)
		{
			char path[50]="/home/user/Desktop/Shell/cmd/";
			strcat(path, token[0]);
			if(execl(path, token[0], token[1], NULL)<0)
			{
			   printf("Problem : Failed execution\n");
                           exit(1);
			}
	        }
	}
	else
	{
	        wait(NULL);
	}
}
////////////////////////////////////////////////////////////

// For other commands
//////////////////////////////////////////////////////////////
void Execute(char token[][2000])
{
	int pid = fork();
	if(pid==0)
	{
		if( (strcmp(token[1],"")==0) && (strcmp(token[2],"")==0) )
		{
			char dest[50] = "/home/user/Desktop/Shell/cmd/";
			strcat(dest, token[0]);
			if( execlp(dest, token[0], NULL)<0 )
			{
				printf("Problem : Failed execution\n");
				exit(1);
			}
		}
		else if( (strcmp(token[1],"")!=0) && (strcmp(token[2],"")==0) )
		{
			char dest[50] = "/home/user/Desktop/Shell/cmd/";
			strcat(dest, token[0]);
			if( execlp(dest, token[0], token[1], NULL)<0 )
			{
				printf("Problem : Failed execution\n");
				exit(1);
			}
		}
		else if( (strcmp(token[1],"")!=0) && (strcmp(token[2],"")!=0) )
		{
			char dest[50]="/home/user/Desktop/Shell/cmd/";
			strcat(dest, token[0]);
			if(execlp(dest, token[0], token[1], token[2], NULL)<0)
			{
				printf("Problem : Failed execution\n");
				exit(1);
			}
		}
	}
	else
	{
		wait(NULL);
	}
}
/////////////////////////////////////////////////////////////

// Piplining
/////////////////////////////////////////////////////////////
void Exec(char token[][2000],int pos)
{
	int pid1, pid2, status, status2;
	pipe(pfds);
	if((pid1=fork())<0)
	{
		printf("Problem in forking\n");
	}
	if(pid1==0)
	{
		close(1);
		dup(pfds[1]);
		close(pfds[0]);
		{
			{
				if(pos==1)
				{
					char path[50]="/home/user/Desktop/Shell/cmd/";
					strcat(path,token[0]);
					if(execl(path,token[0],NULL)<0)
					{
						printf("Problem : Failed execution\n");
						exit(1);
					}
				}
				else if(pos==3)
				{
					char path[50]="/home/user/Desktop/Shell/cmd/";
					strcat(path,token[0]);
					if(execl(path,token[0],token[1],token[2],NULL)<0)
					{
						printf("Problem : Failed execution\n");
						exit(1);
					}
				}
                                else if(pos==2)
				{
					char path[50]="/home/user/Desktop/Shell/cmd/";
					strcat(path,token[0]);
					if(execl(path,token[0],token[1],NULL)<0)
					{
						printf("Problem : Failed execution\n");
						exit(1);
					}
				} 
			}
		}
	}
	else if((pid2=fork())==0)
	{
		close(0);
		dup(pfds[0]);
		close(pfds[1]);
		{
			{
				if( (strcmp(token[1+(pos+1)],"")==0) && (strcmp(token[2+(pos+1)],"")==0) )
				{
					char path[50]="/home/user/Desktop/Shell/cmd/";
					strcat(path,token[0+(pos+1)]);
					if(execl(path,token[0+(pos+1)],NULL)<0)
					{
						printf("Problem : Failed execution\n");
						exit(1);
					}


				}
                                else if( (strcmp(token[1+(pos+1)],"")!=0) && (strcmp(token[2+(pos+1)],"")!=0) )
				{
					char path[50]="/home/user/Desktop/Shell/cmd/";
					strcat(path,token[0+(pos+1)]);
					if(execl(path,token[0+(pos+1)],token[1+(pos+1)],token[2+(pos+1)],NULL)<0)
					{
						printf("Problem : Failed execution\n");
						exit(1);
					}
				} 
				else if( (strcmp(token[1+(pos+1)],"")!=0) && (strcmp(token[2+(pos+1)],"")==0) )
				{
					char path[50]="/home/user/Desktop/Shell/cmd/";
					strcat(path,token[0+(pos+1)]);
					if( execl(path,token[0+(pos+1)],token[1+(pos+1)],NULL)<0 )
					{
						printf("Problem : Failed execution\n");
						exit(1);
					}

				}
			}
		}
	}
	else
	{
		close(pfds[0]);
		close(pfds[1]);
		while(wait(&status)!=pid1);
		while(wait(&status2)!=pid2);
	}
	return ;
}
//////////////////////////////////////////////////////////////


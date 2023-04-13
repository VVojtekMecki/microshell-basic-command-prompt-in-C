#include <unistd.h> //chdir, exec*, exit(), fork, itd
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> //waitpid(), wait(NULL)
#include <sys/stat.h> //chmod, mkdir, mode_t

//program designed for Unix-like systems !!!

#define MAX_DL 400

//colors
#define CYAN_ANSI "\x1b[36m"
#define RED_ANSI "\x1b[31m"
#define YELLOW_ANSI "\x1b[33m"
#define GREEN_ANSI "\x1b[32m"
#define WHITE_ANSI "\e[0;37m"
#define BLACK_ANSI "\e[0;30m"
#define RESET_COLOR "\x1b[0m"


char *exit_p="exit";
char path[MAX_DL]={0};
char prevPath[MAX_DL]={0};
char host[MAX_DL]={0};
char user[MAX_DL]={0};
char input[MAX_DL]={0};


char *partition[20];
char *str_tool;



void cleanBUF(char *buf, int size)
{
    memset(buf, 0, size);
}

void clean()
{
    cleanBUF(input, MAX_DL);
    cleanBUF(path, MAX_DL);
}
int changeDir(char *pathchange)
{
    cleanBUF(prevPath, MAX_DL);
    strcpy(prevPath, path);
    strcpy(path, pathchange);
    return chdir(pathchange);

}

void makeDir (char *location)
{
    int zm;
    zm = mkdir(location, 0777);
    /* 0777 == owner, group, others can execute, read and write; chmod 777 */
    if (zm != 0)
    {
        perror("mkdir error");
    }
}

void setPath()
{
    getcwd(path, sizeof(path));
}

void setHost()
{
    gethostname(host, sizeof(host));
}
void setUser()
{
    strcpy(user, getenv("USER"));
}

void clear()
{
    //source: ANSI escape code
    printf("\033[H\033[J");
    /* \033[ == escape code, \033[H == cursor top left, \033[J clears form cursor to the end */
    /* \x1b == \033 */
}

void cp(char *from, char *to)
{
    FILE *ft1, *ft2;
    ft1 = fopen(from, "r");
    if (ft1 == NULL)
    {
        printf("%sError: %sfile %s%s%s does not exist\n", RED_ANSI, RESET_COLOR, GREEN_ANSI, from, RESET_COLOR);
    }
    else
    {
        ft2 = fopen(to, "a");
    char c;
    while((c=fgetc(ft1)) != EOF)
    {
        fputc(c, ft2);
    }
    fclose(ft1);
    fclose(ft2);
    }

}





int main()
{
    setPath();
    setUser();
    setHost();

    while(1)
    {
        clean();
        setPath();
        setHost();

        printf("[%s%s@%s:%s%s%s] $ ", YELLOW_ANSI, user, host, CYAN_ANSI, path, RESET_COLOR);

        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        str_tool = strtok(input, " ");
        int k = 0;
        while(str_tool != NULL)
        {
            partition[k]=str_tool;
            str_tool = strtok(NULL, " ");
            k++;
        }
        partition[k]=NULL;

        if(partition[0]==NULL)
        {
            continue;
        }
        else if(strcmp(partition[0], exit_p)==0)
        {
            return 0;
            //exit
        }
        else if(strcmp(partition[0], "pwd")==0)
        {
            printf("%s%s%s\n", CYAN_ANSI, path, RESET_COLOR);
        }
        //exit and pwd in bash ignore further arguments, so I gave up on the error message

        else if(strcmp(partition[0], "mkdir")==0)
        {
                if(partition[1] == NULL)
                {
                    printf("%sError: %slack of the directory name\n", RED_ANSI, RESET_COLOR);
                }
                else
                {
                    char *dir_tool = partition[1];
                    int zx=1;
                    while(dir_tool != NULL)
                    {
                        makeDir(partition[zx]);
                        zx++;
                        dir_tool = partition[zx];
                    }

                }
        }

        else if(strcmp(partition[0], "cd")==0)
        {
            if(partition[2] != NULL)
            {
                printf("%sError: %stoo many arguments!\n", RED_ANSI, RESET_COLOR);
            }
            else
            {
                if((partition[1]==NULL) || (strcmp(partition[1],"~")==0))
                {
                    changeDir(getenv("HOME"));

                }
                else if(strcmp(partition[1],".")==0)
                {
                    changeDir(path);
                }
                else if(strcmp(partition[1],"-")==0)
                {
                        printf("%s%s%s\n", CYAN_ANSI, prevPath, RESET_COLOR);
                        char tmp[MAX_DL]={0};
                        chdir(prevPath);
                        strcpy(tmp, path);
                        strcpy(path, prevPath);
                        strcpy(prevPath, tmp);

                }
                else
                {
                    if(changeDir(partition[1]) != 0)
                    {
                        printf("%sError: %sDirectory %s%s%s does not exist \n", RED_ANSI, RESET_COLOR, YELLOW_ANSI, partition[1], RESET_COLOR);
                    }


                }
            }
        }

        else if(strcmp(partition[0], "clear")==0)
        {
            if(partition[1] != NULL)
            {
                printf("%sError: %stoo many arguments!\n", RED_ANSI, RESET_COLOR);
            }
            else
            {
                clear();
            }
        }
        else if(strcmp(partition[0], "cp")==0)
        {
            if(partition[3] != NULL)
            {
                printf("%sError: %stoo many arguments!\n", RED_ANSI, RESET_COLOR);
            }
            else
            {
                if(partition[1] == NULL)
                {
                    printf("%sError: %sLack of the source file, try again\n", RED_ANSI, RESET_COLOR);
                }
                else
                {
                    if(partition[2] == NULL)
                    {
                        printf("%sError: %sLack of the destination file, try again\n", RED_ANSI, RESET_COLOR);
                    }
                    else
                    {
                        cp(partition[1], partition[2]);
                    }
                }
            }
        }
        else if(strcmp(partition[0], "help")==0)
        {
            if(partition[1] != NULL)
            {
                printf("%sError: %shelp%s doesn't have any arguments\n", RED_ANSI, GREEN_ANSI, RESET_COLOR);
            }
            else
            {
                printf("\n");
                printf("Witam w sekcji %shelp%s mojego microshella\n", GREEN_ANSI, RESET_COLOR);
                printf("Nazywam się: %sWojciech Medenecki%s \n", CYAN_ANSI, RESET_COLOR);
                printf("Mój program obsługuje następujące %spolecenia%s: \n", GREEN_ANSI, RESET_COLOR);
                printf("- %scd%s z parametrami: %s~%s lub %s-%s \n", GREEN_ANSI, RESET_COLOR, YELLOW_ANSI, RESET_COLOR, YELLOW_ANSI, RESET_COLOR);
                printf("- %spwd%s \n", GREEN_ANSI, RESET_COLOR);
                printf("- %sclear%s \n", GREEN_ANSI, RESET_COLOR);
                printf("- %sexit%s \n", GREEN_ANSI, RESET_COLOR);
                printf("- %smkdir%s \n", GREEN_ANSI, RESET_COLOR);
                printf("W razie problemów zapraszam do sekcji %shelp%s z powrotem C; \n",GREEN_ANSI, RESET_COLOR);
                printf("\n");
            }
        }
        else
        {
            printf("%s%s%s is an out-of-shell function\n", GREEN_ANSI, partition[0], RESET_COLOR);
            int pPID = fork();
            if (pPID == -1)
            {
                perror("fork error");
            }
            else if (pPID>0)
            {
                wait(NULL);
            }
            else if(pPID ==0)
            {

                if(execvp(input, partition) == -1)
                /* (execvp(partition[0], partition) == -1)
                    name of the file; arguments  */
                {
                    perror("execvp error");
                }
                exit(0);
            }
        }
    }
    return 0;
}
#include "myShell.h"
#include "Job.h"
#include "DataListVariable.h"
#include "Histories.h"

void welcome()
{
    char *logo[] = {"                     aRTI//BARK             ____\n",
                    "             simpleSH//////////Shl         |B//S|", "    |\n",
                    "          SH//////YS           shell//Sh   |A@@I|", "    | Welcome to Advance shell\n",
                    " Sim aSystemSH//Sh              sys//B     |R$$M|", "    | Version 1.0\n",
                    " BarABBBaraKk///Sh               sM//E     |A^^P|", "    |\n",
                    "         pCCCCY//h          eSS@@ y//E     |K**L|", "    | https://github.com/BSharabi\n",
                    "         SPPPP///a          pP///AC//E     |&&&E|", "    |\n",
                    "              A//A            smP////S     |A**S|", "    | Have fun!\n",
                    "              p///Ac            sE///a     |R##H|", "    |\n",
                    "              P////YCpc           L//L     |T!!E|", "    | Wanna support advance shell?\n",
                    "       scccccp///pSP///p          p//l     |I%%L|", "    |\n",
                    "      sY/////////y  caa           S//h     |U--L|", "    |\n",
                    "       shsSellH//Ya              pY/Sh     |MIGT|", "    |\n",
                    "        sH/ShS////YCc          aC//Yp      |____|", "    |\n",
                    "         Si  shlll//SHsimpleSH//LSs\n",
                    "                  shell//////IPSHs\n",
                    "                       baraks\n", NULL};
    int i = 0;
    purple();
    printf("\n------------------------------------\033[1m Barak Sharabi------------------------------------\n");
    printf("\033[1m yes master?\033[0m\n\n");
    green();
    do
    {
        printf("%s", logo[i++]);
        if (i % 2 != 0 || i > 26)
            green();
        else
            blue();
    } while (logo[i]);
    reset();
    puts("\n");
}

int changeDirectory(char *path)
{

    return chdir(path);
}

void getInput(char *command)
{
    fgets(command, MAX_INPUT, stdin);
    command[strlen(command) - 1] = '\0';
}

void getLocation(char *prompt)
{
    if (getcwd(prompt, MAX_PROMPT) == NULL)
    {
        printf("error in finding the location, check again later...\n");
    }
}

int replaceVar(char **data)
{
    int i = 0;
    Data *d;
    while (data[i])
    {
        if (data[i][0] == '$')
        {
            if ((d = search(data[i] + 1)))
            {
                data[i] = realloc(data[i], strlen(d->value) + 1);
                strcpy(data[i], d->value);
            }
            else
                data[i][0] = '\0';
        }
        i++;
    }
    return 1;
}

void changePrompt(char *prompt, char *newPrompt)
{
    strcpy(prompt, newPrompt);
}

void initPipe(int piping, int in, int swap)
{
    if (piping && in)
    {
        if (swap)
        {
            dup2(fd1[1], STDOUT_FILENO);
            dup2(fd2[0], STDIN_FILENO);
        }
        else
        {
            dup2(fd1[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);
        }
    }
    else if (piping && swap)
    {
        dup2(fd1[1], STDOUT_FILENO);
    }
    else if (piping)
    {
        dup2(fd2[1], STDOUT_FILENO);
    }
    else if (in)
    {
        if (swap)
        {
            dup2(fd2[0], STDIN_FILENO);
        }
        else
        {
            dup2(fd1[0], STDIN_FILENO);
        }
    }
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
}

void execJob(int swap, int in, int piping, Process *p)
{
    replaceVar(p->argv.data);
    if (p->argv.size == 0)
    {
        getInput(command);
        initJob(command);
    }
    if (strcmp("quit", p->argv.data[0]) == 0)
    {
        quit = 0;
        return;
    }
    if (strcmp("cd", p->argv.data[0]) == 0)
    {
        changeDirectory((p->argv.data[1] != NULL) ? p->argv.data[1] : "/");
        return;
    }
    pid_t pid;
    if ((pid = fork()) == 0)
    {
        if (piping || in)
            initPipe(piping, in, swap);
        execvp(p->argv.data[0], p->argv.data);
        exit(EXIT_FAILURE);
    }
    else
        p->pid = pid;
}

void appendToFile(Process *p)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        int size = p->argv.size - 1;

        int fd = open(p->argv.data[size], O_WRONLY | O_APPEND | O_CREAT, 0660);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        p->argv.data[size] = NULL;
        execvp(p->argv.data[0], p->argv.data);
        exit(0);
    }
    else
        p->pid = pid;
}

void redirectOut(Process *p)
{
    pid_t pid;
    if ((pid = fork()) == 0)
    {
        int size = p->argv.size - 1;
        int fd = creat(p->argv.data[size], 0660);
        dup2(fd, (p->action == Write) ? STDOUT_FILENO : STDERR_FILENO);
        close(fd);
        p->argv.data[size] = NULL;
        execvp(p->argv.data[0], p->argv.data);
        exit(0);
    }
    else
        p->pid = pid;
}

void placement(Process *p)
{

    if (strcmp(p->argv.data[0], "prompt") == 0)
    {
        if (strcmp(p->argv.data[1], "getcwd") == 0)
            getLocation(prompt);
        else
            changePrompt(prompt, p->argv.data[1]);
    }
    else if (p->argv.data[0][0] == '$')
    {
        if (!add(p->argv.data[0] + 1, p->argv.data[1]))
            perror("error");
    }
    else
        printf("-myShell: syntax error near unexpected token `%s'\n", p->argv.data[0]);
}

void reversePipe(int swap, int in, int piping, Process *p)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        int size = p->argv.size - 1;

        int fd = open(p->argv.data[size], O_RDONLY);
        if (fd == -1)
        {
            printf("-myShell: %s: No such file or directory\n", p->argv.data[size]);
            exit(0);
        }

        dup2(fd, STDIN_FILENO);
        close(fd);
        p->argv.data[size] = NULL;
        if (piping || in)
            initPipe(piping, in, swap);
        execvp(p->argv.data[0], p->argv.data);
        exit(0);
    }
    else
        p->pid = pid;
}

void readExec(Process *p)
{
    getInput(command);
    if (!add(p->argv.data[1], command))
        perror("error");
}

int buildCondition(Job **job)
{

    return 1;
}

void startJob()
{
    Job *current = getFirstJob();
    int piping;
    int closeFD;
    int in = 0;
    int swap = 1;
    pid_t pid;
    int status;

    if ((closeFD = piping = (current->next) ? 1 : 0))
    {
        if (pipe(fd1) == -1 || pipe(fd2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    while (current)
    {
        piping = (current->next) ? 1 : 0;
        Process *p = current->head;

        while (p)
        {
            switch (p->action)
            {
            case Append:
                appendToFile(p);
                break;
            case Write:
            case WriteError:
                redirectOut(p);
                break;
            case ReversePipe:
                reversePipe(swap, in, piping, p);
                break;
            case Placement:
                placement(p);
                break;
            case Normal:
                execJob(swap, in, piping, p);
                break;
            case Read:
                readExec(p);
                break;
            default:
                break;
            }
            in = 0;
            p = p->next;
        }

        in = (current->next) ? 1 : 0;
        current = current->next;
        swap = !swap;
    }
    if (closeFD)
    {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
    }
    int i = 1;
    while ((pid = wait(&status)) != -1)
    {
        updateStatusProcess(pid, WIFEXITED(status), 0, WEXITSTATUS(status));
        printf("[%d] %d\n", i, pid);
        i++;
    }
}

void printPrompt(char *prompt)
{
    green();
    printf("%s>", prompt);
    blue();
    printf("$ ");
}

void freeAllAllocateMemory()
{
    freeJob();
}

void replaceStrings(char *str1, char *str2, char *delim)
{
    char *pos = str1;
    while ((pos = strstr(pos, delim)) != NULL)
    {
        if ((pos == str1 || *(pos - 1) == ' ') && (*(pos + 2) == ' ' || *(pos + 2) == '\0'))
        {
            size_t len1 = strlen(str1);
            size_t len2 = strlen(str2);
            size_t pos_offset = pos - str1;
            memmove(pos + len2, pos + 2, len1 - (pos_offset + 2)); // shift remaining string to make room for str2
            memcpy(pos, str2, len2);                               // copy str2 to str1
            pos += len2;                                           // move pointer after the replaced string
        }
        else
        {
            pos += 2; // move pointer after the non-replaced string
        }
    }
}

int initJob(char *command)
{
    char *token;
    int ampr = (getFirstJob() == NULL) ? 0 : 1;
    int isquit = 0;
    int newJobFlag = 0;
    while ((token = strtok(command, " ")) != NULL)
    {
        command = NULL;
        // After quit or '='can only be Action
        if ((newJobFlag || isquit) && regexec(&regex, token, 0, NULL, 0) != 0)
        {
            printf("-myShell: syntax error near unexpected token `%s'\n", token);
            return 0;
        }

        if (!isEmpty() && regexec(&regex, token, 0, NULL, 0) == 0)
        {
            if (ampr)
            {
                printf("-myShell: syntax error near unexpected token `%s'\n", token);
                return 0;
            }
            if (strcmp(token, "&") == 0)
            {
                if (!addProcess())
                    return 0;
                isquit = 0;
                ampr = 1;
                newJobFlag = 0;
            }
            else if (strcmp(token, "|") == 0)
            {
                if (!addJob() || !addProcess())
                    return 0;
                newJobFlag = 0;
                isquit = 0;
            }
            else if (!newJobFlag)
            {
                for (int i = 0; i < MAX_SYMBOL; i++)
                    if (strcmp(token, symbol[i]) == 0)
                    {
                        getTailJob()->tail->action = (Action)i;
                        break;
                    }
                if ((token = strtok(command, " ")) != NULL)
                {
                    if (!setProcessArgument(getTailJob()->tail, token))
                        return 0;
                }
                else
                    return 0;
                newJobFlag = 1;
            }
            else
            {
                printf("-myShell: syntax error near unexpected token `%s'\n", token);
                return 0;
            }
        }
        else
        {
            if (strcmp(token, "quit") == 0)
                isquit = 1;
            if (isEmpty() && (!addJob() || !addProcess()))
                return 0;
            if (!setProcessArgument(getTailJob()->tail, token))
                return 0;
            if (strcmp(token, "read") == 0 && getTailJob()->tail->argv.size == 1)
            {
                getTailJob()->tail->action = Read;
                if ((token = strtok(command, " ")) != NULL)
                {
                    if (!setProcessArgument(getTailJob()->tail, token))
                        return 0;
                }
                else
                    return 0;
                newJobFlag = 1;
            }
            // if(strcmp(token, "if")==0){

            // }
            ampr = 0;
        }
    }
    return 1;
}

void sigHandler(int signum)
{

    switch (signum)
    {
    case SIGINT: // dad is dead
        if (getpid() == pid_m)
        {
            blue();
            system("echo -n '\b\b  \b\b'");
            if (getFirstJob())
            {
                printPrompt(prompt);
                printf("You typed Control-C!\n");
            }
            else
            {
                printf("You typed Control-C!\n");
                printPrompt(prompt);
            }
        }
        else if (getFirstJob())
        {
            kill(getpid(), 0);
            exit(0);
        }
        fflush(0);
    default:
        return;
    }
}

void initSignal()
{
    signal(SIGINT, sigHandler);
}

int buildRegex()
{
    char *pattern = "^(\\||>|>>|2>|&|=|<)$";
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
    {
        printf("Failed to compile regex.\n");
        return 0;
    }
    return 1;
}

void trimCommand()
{
    int i;
    for (i = strlen(command) - 1; i >= 0; i--)
        if (!isspace(command[i]))
            break;
    command[i + 1] = '\0';
}

void Substitution(char *command)
{
    if (getTailHistories())
    {
        replaceStrings(command, getTailHistories()->command, "!!");
        replaceStrings(command, getTailHistories()->status, "$?");
    }
    else
    {
        replaceStrings(command, "  ", "!!");
        replaceStrings(command, "0 ", "$?");
    }
}

void updateStatusHistories()
{
    Job *job = getFirstJob();
    while (job)
    {
        Process *p = job->head;
        while (p)
        {
            switch (p->status)
            {
            case 0:
                strcpy(getTailHistories()->status, "0 \n");

                break;
            case 1:
                strcpy(getTailHistories()->status, "1 \n");

                break;
            case 2:
                strcpy(getTailHistories()->status, "2 \n");

                break;

            default:
                break;
            }
            p = p->next;
        }
        job = job->next;
    }
}

int run()
{
    while (quit)
    {
        printPrompt(prompt);
        getInput(command);
        Substitution(command);
        addHistories(command);
        trimCommand();
        if (strlen(command) < 2)
        {
            if (strcmp(command, "") != 0)
                printf("-myShell: syntax error near unexpected token `%s'\n", command);
            strcpy(getTailHistories()->status, "2 Exit");
            continue;
        }
        if (!(initJob(command)))
        {
            perror("Error in the structure of the operation");
            freeJob();
            continue;
        }
        startJob();
        updateStatusHistories();
        // puts(getTailHistories()->status);
        // showList();
        // printJob();
        // printAllHistory();
        freeJob();
    }
    freeAllAllocateMemory();
    regfree(&regex);
    puts("logout");
    return 0;
}

int main()
{
    buildRegex();
    pid_m = getpid();
    welcome();
    initSignal();
    return run();
}

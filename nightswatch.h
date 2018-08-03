void dirty()
{
    FILE* file = fopen("/proc/meminfo","r");
    int count = 1;
    if (file != NULL)
    {
        char line[100];
        while(fgets(line,sizeof line,file) != NULL)
        {
            if(count == 17)
            {
                char* token = strtok(line,": ");
                token = strtok(NULL,": ");
                char* size = strtok(NULL," ");
                strcat(token,size);
                printf("%s",token);
                break;
            }
            else count++;
        }
        fclose(file);
    }
    return;
}
void interrupt()
{
    FILE* file = fopen("/proc/interrupts","r");
    int count = 1;
    if (file != NULL)
    {
        char line[100];
        while(fgets(line,sizeof line,file) != NULL)
        {

            if(count == 3)
            {
                printf("%s",line);
                break;
            }
            else count++;
        }
        fclose(file);
    }
    return;
}
void nightswatch(int time,char* command)
{
    int n;
    struct termios old;
    struct termios new;
    unsigned char key;
    if(strcmp(command,"interrupt") != 0 && strcmp(command,"dirty") !=0)
    {
        fprintf(stderr,"Invalid Command.\n");
        return;
    }
    if(strcmp(command,"interrupt") == 0)
    {
        FILE* file = fopen("/proc/interrupts","r");
        int count = 1;
        char line[100];
        if (file != NULL)
        {
            fgets(line,sizeof line,file);
            printf("%s",line);
        }
    }

    tcgetattr(0,&old);
    
    new = old;
    new.c_lflag &= ~ICANON;
    new.c_lflag &= ~ECHO;
    new.c_lflag &= ~ISIG;
    new.c_cc[VMIN] = 0;
    new.c_cc[VTIME] = 0;
    tcsetattr(0,TCSANOW,&new);
    while(1)
    {
        n = getchar();
        if (n != EOF)
        {
            key = n;
            if(key == 'q')
            {
                break;
            }
        }
        if(strcmp(command,"interrupt") == 0)
            interrupt();
        if(strcmp(command,"dirty") == 0)
            dirty();
        sleep(time);
        fflush(stdout);
    }
    tcsetattr(0,TCSANOW, &old);
    return ;
}

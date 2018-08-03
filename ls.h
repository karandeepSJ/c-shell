void getPermissions(struct stat info,char* perm)
{
    int i;
    for(i=0;i<10;i++) perm[i] = '-';

    if(S_ISLNK(info.st_mode)) perm[0]='l';
    else if(S_ISDIR(info.st_mode)) perm[0]='d';
    
    if(info.st_mode & S_IRUSR) perm[1]='r';
    if(info.st_mode & S_IWUSR) perm[2]='w';
    if(info.st_mode & S_IXUSR) perm[3]='x';

    if(info.st_mode & S_IRGRP) perm[4]='r';
    if(info.st_mode & S_IWGRP) perm[5]='w';
    if(info.st_mode & S_IXGRP) perm[6]='x';

    if(info.st_mode & S_IROTH) perm[7]='r';
    if(info.st_mode & S_IWOTH) perm[8]='w';
    if(info.st_mode & S_IXOTH) perm[9]='x';
    return ;
    
}
char* getUser(struct stat info)
{
    char* ans = malloc(sizeof(char) * 25);
    struct passwd* buf;
    buf = getpwuid(info.st_uid);
    ans =  buf->pw_name;
    return ans;
}
char* getGroup(struct stat info)
{
    char* ans = malloc(sizeof(char) * 25);
    struct passwd* buf;
    buf = getpwuid(info.st_gid);
    ans =  buf->pw_name;
    return ans;
}
char* getLastModTime(struct stat info)
{
    int i,k=0;
    struct timespec t;
    t = info.st_mtim;
    char* time = ctime(&t.tv_sec);
    char* ans = malloc(sizeof(char)*13);
    for(i=4;i<=15;i++)
        ans[k++] = time[i];
    ans[k] = '\0';
    return ans;
}
int ls(char **args,int argn, char *HOME)
{
    int i;
    char *dir = malloc(100);
    dir = ".";
    struct stat a;
    int flag=0;
    for(i=0;i<argn;i++)
    {
        if(args[i][0]=='-')
        {
            if(strcmp(args[i],"-a")==0)
            {
                if(flag<2)
                    flag=1;
                else
                    flag=3;
            }
            else if(strcmp(args[i],"-l")==0)
            {
                if(flag%2==0)
                    flag=2;
                else
                    flag=3;
            }
            else if(strcmp(args[i],"-al")==0 || strcmp(args[i],"-la")==0)
                flag=3;
        }
        else
        {
            if(strcmp(args[i],"~") == 0)
            {
                dir = HOME;
            }
            else
            {
                if(strstr(args[i],"~") != NULL)
                {
                    char* fullPath = malloc(sizeof(char) * 120);
                    strcpy(fullPath,HOME);
                    strcat(fullPath,&args[i][1]);
                    dir = fullPath;
                }
                else
                    dir = args[i];
            }
        }
    }
    if(stat(dir,&a)==-1)
    {
        if(!S_ISDIR(a.st_mode))
        {
            perror("ls");
        }
    }
    i = 0;
    struct dirent* readir;
    struct dirent** scdir;
    int dirNo = scandir(dir,&scdir,NULL,alphasort);
    while(i < dirNo)
    {
        readir = scdir[i];
        char *item = malloc(100);
        strcpy(item,dir);
        strcat(item,"/");
        strcat(item,readir->d_name);
        struct stat statBuf;
        lstat(item,&statBuf);
        if(flag==3 || (flag==2 && readir->d_name[0]!='.'))
        {
            char perm[11];
            perm[10] = '\0';
            getPermissions(statBuf,perm);
            printf("%s ",perm);
            
            printf("%*lu ",5,statBuf.st_nlink);
            char* user;
            user = getUser(statBuf);
            printf("%*s ",10,user);
            
            char* group;
            group = getGroup(statBuf);
            printf("%*s ",10,group);
            
            printf("%*ld ",10,statBuf.st_size);
            
            char* time;
            time = getLastModTime(statBuf);
            printf("%s ",time);
            
        }
        if (S_ISLNK(statBuf.st_mode))
        {
            size_t linkBufSize = 100;
            char* linkBuf = malloc(sizeof(char) * linkBufSize);
            readlink(item,linkBuf, linkBufSize);
            if(readir->d_name[0]=='.')
            {
                if(flag&1)
                    printf("\033[1;36m%s\033[0m -> %s\n",readir->d_name,linkBuf);
            }
            else
                printf("\033[1;36m%s\033[0m -> %s\n",readir->d_name,linkBuf);
        }
        else if(S_ISDIR(statBuf.st_mode))
        {
            if(readir->d_name[0]=='.')
            {
                if(flag&1)
                    printf("\033[1;34m%s\033[0m/\n",readir->d_name);
            }
            else
                printf("\033[1;34m%s\033[0m/\n",readir->d_name);

        }
        else 
        {
            if(readir->d_name[0]=='.')
            {
                if(flag&1)
                    printf("%s\n",readir->d_name);
            }
            else
                printf("%s\n",readir->d_name);
        }
        i++;
    }
    return 0;
}
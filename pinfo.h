char* pinfo(int pid)
{
	char* spath=malloc(100),*apath = malloc(sizeof(char)*50),*expath = malloc(sizeof(char)* 1000),*status=malloc(5),*mem=malloc(100);
	char buf[100010];
	sprintf(spath,"/proc/%d/stat",pid);
	sprintf(apath,"/proc/%d/exe",pid);
	int fd;
	readlink(apath,expath,1000);
	fd = open(spath,O_RDONLY);
	if(fd < 0)
    {
        printf("Invalid PID\n");
        return "";
    }
    read(fd,buf,100010);
	char * token = strtok(buf," ");
	int i=1;
	while(token!=NULL)
	{
		i++;
		token = strtok(NULL," ");
		if(i==3)
			status=token;
		if(i==23)
			mem=token;
	}
    char* res = malloc(sizeof(char) * 250);
	sprintf(res,"pid             -  %d\nProcess Status  -   %s\nMemory          -   %s{virtual memory}\nExecutable Path -  %s\n",pid,status,mem,expath);
	return res;
}

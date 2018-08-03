char* getUserName()
{
    struct passwd* passwd; 
    passwd = getpwuid(getuid());
    return passwd->pw_name;
}
char* getHostName()
{
    char* name = malloc(100*sizeof(char));
    if(!gethostname(name,100))
        //print: Error getting hostname//
    ;
    return name;
}

void cd(const char* path)
{
    if(chdir(path) == 0)
        return ;
    else
        perror("cd");
    return ;
}

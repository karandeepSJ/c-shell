char* getPWD()
{
    char* buf=NULL;
    buf = getcwd(buf,100);
    if (buf == NULL)
        perror("getPWD");
    return buf;
}

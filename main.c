#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

FILE *fp_out=NULL;

int readFileList(char *basePath)
{
    DIR *dir=NULL;
    struct dirent *ptr=NULL;
    char base[1000];
    char file[100][1000];
    char tmpfile[1000];
    char delfilepath[1000];
    int count = 0;
    int i = 0;
    int j = 0;
    int flag = 0;

    if ((dir = opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
    memset(file, 0, sizeof(file));
    memset(tmpfile, 0, sizeof(tmpfile));
    printf("%s\n",basePath); //显示到终端上
    fprintf(fp_out,"dir:%s\n",basePath);//打印目录到文件中
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) /// current dir OR parrent dir
        {
            continue;
        }
        else if (ptr->d_type == 8) //file
        { 
            strcpy(file[count], ptr->d_name);
            count++;
            if (count >= 100)
                count = 0;
        }
        else if (ptr->d_type == 10) //link file
            printf("link name:%s/%s\n", basePath, ptr->d_name);
        else if (ptr->d_type == 4) // dir
        {
            memset(base, '\0', sizeof(base));
            strcpy(base, basePath);
            strcat(base, "/");
            strcat(base, ptr->d_name);
            readFileList(base);
        }
    }
    for (i = 0; i < count; i++)
    {
    
        if (file[i][strlen(file[i]) - 2] == '.' && file[i][strlen(file[i]) - 1] == 'c')
        {
            flag=0;
            memset(tmpfile, 0, sizeof(tmpfile));
            strcpy(tmpfile, file[i]);
            tmpfile[strlen(tmpfile) - 2] = 0;
            strcat(tmpfile, ".o");
            for (j = 0; j < count; j++)
            {

                if (0 == strcmp(tmpfile, file[j]))
                {
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                sprintf(delfilepath, "%s/%s", basePath, file[i]);
                fprintf(fp_out,"  %s\n",delfilepath);
                printf("delfilepath:%s\n", delfilepath);
                unlink(delfilepath);
            }
        }
    }
    closedir(dir);
    return 1;
}

int main(void)
{
    DIR *dir;
    char basePath[1000];
    FILE *fp=fopen("file_list.txt","r");
    fp_out=fopen("del_list.txt","w");
    if(!fp){
        printf("can't open file\n");
        return -1;
    }
    while(!feof(fp))
    {
        memset(basePath, '\0', sizeof(basePath));
        fscanf(fp,"%s",basePath);
        
        readFileList(basePath);
    }
    fclose(fp);
    fclose(fp_out);
    printf("hello ltt you are task finish!\n");
    return 0;
}

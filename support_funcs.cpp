#include "support_funcs.h"
#include <stdio.h>
#include <stdlib.h>

char *trimWhiteSpace(char *str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    // Write new null terminator character
    end[1] = '\0';
    return str;
}


int startsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

std::vector<char *> splitString(const char * str, char *key)
{
    std::vector<char *> resVec;
    char * resStr1 = (char *) malloc (strlen(str) * sizeof (char));
    char * strCopy= (char *) malloc (strlen(str) * sizeof (char));
    strcpy(strCopy,str);

    char *pch = strtok (strCopy,key);
    if(pch != NULL){
        strcpy(resStr1,pch);
        resVec.push_back(resStr1);
    }
    while (pch != NULL)
    {
        //printf ("%s\n",pch);
        pch = strtok (NULL, key);
        if(pch != NULL){
            char * resStr2 = (char *) malloc (strlen(pch) * sizeof (char));
            strcpy(resStr2,pch);
            resVec.push_back(resStr2);
        }
    }
    return resVec;
}


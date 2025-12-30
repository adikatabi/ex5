#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *getString(){
    char *strName = NULL;
    int oneChar;
    int countChar = 0;
    printf("Enter the name of the show: ");
    oneChar = getchar();
    while(oneChar != '\n'){
        countChar = countChar + 1;
        char *temp;
        temp =(char*)realloc(strName, (countChar +1)*sizeof(char));
        if(temp == NULL){
            printf("Error");
            free(strName);
            return NULL;
        }
        strName = temp;
        strName[countChar - 1] = (char)oneChar;
        oneChar =getchar();
    }
    if(strName != NULL){
        strName[countChar] = '\0';
    }
    return strName;
}

int main() {
    printf(getString());
}
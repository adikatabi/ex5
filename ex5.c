/***********
 ID: 322906728 
 NAME: Adi Katabi
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Episode {
    char *name;
    char *length;
    struct Episode *next;
} Episode;

typedef struct Season {
    char *name;
    Episode *episodes;
    struct Season *next;
} Season;

typedef struct TVShow {
    char *name;
    Season *seasons;
} TVShow;

typedef struct{
    int iRow;
    int jCol;
}position;

TVShow ***database = NULL;
int dbSize = 0;

char *getString();
int getInt();

char *getString(){
    char *strName = NULL;
    int oneChar;
    int countChar = 0;
    oneChar = getchar();
    while(oneChar == '\n' || oneChar == ' '){
        oneChar = getchar();
    }
    while(oneChar != '\n'){
        countChar = countChar + 1;
        char *temp;
        temp =(char*)realloc(strName, (countChar +1)*sizeof(char));
        if(temp == NULL){
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

int validLength(char *s);
int countShows();

int countShows(){
    int count =0;
    for (int i=0; i<dbSize ;i++){
        for(int j =0; j<dbSize; j++){
            if(database[i][j] != NULL){
                count = count + 1;
            }
        }
    }
    return count;
}

int validLength(char *s){
    if(s == NULL || strlen(s) != 8){
        return 1;//Invalid length input
    }

    if((s[0] >='0' && s[0] <= '9') && (s[1] >='0' && s[1] <= '9') && (s[2] == ':') &&(s[3] >= '0' && s[3] <='9') && 
    (s[4] >= '0' && s[4] <= '9') && (s[5]== ':') &&(s[6] >= '0' && s[6] <= '9') && (s[7] >= '0' && s[7] <= '9')){
        int firstDigit = s[0] - '0';
        int secondDigit = s[1]- '0';
        int thirdDigit = s[3]- '0';
        int fourthDigit = s[4]- '0';
        int fiveDigit = s[6]- '0';
        int sixthDigit = s[7]- '0';
        if((firstDigit * 10 + secondDigit > 99) || (s[2] != ':') || (thirdDigit * 10+ fourthDigit > 59) 
        || (s[5] != ':') || (fiveDigit * 10 + sixthDigit > 59) ){
            return 1;
        }
        return 0;
    }
    else{
        return 1;
    }
} 
        

        /*if(firstDigit * 10 + secondDigit > 99){
            return 1;
        }
        if(s[2] != ':'){
            return 1;
        }
        if(thirdDigit * 10+ fourthDigit > 59){
            return 1;
        }
        if(s[5] != ':'){
            return 1;
        }
        if(fiveDigit * 10 + sixthDigit > 59){
            return 1;
        }*/




void shrinkDB();
void expandDB();
void fixGaps();

void expandDB(){
    
    //to do: temp
    TVShow ***temp = (TVShow ***)realloc(database,(dbSize+1)*sizeof(TVShow **));
    if(temp == NULL){
        return;
    }
    database = temp;
    dbSize++;
    for(int i=0;i<dbSize -1;i++){
        TVShow **tempTry = (TVShow **)realloc(database[i], dbSize * sizeof(TVShow *));
        if(tempTry != NULL){
            database[i] = tempTry;
            database[i][dbSize-1] = NULL;
        }
        
    }
    database[dbSize-1] = (TVShow **)malloc(dbSize*sizeof(TVShow*));
    for(int i=0; i<dbSize; i++){
        database[dbSize - 1][i]= NULL;
    }
    fixGaps();
    //database = (TVShow ***)realloc(database, dbSize*dbSize*sizeof(TVShow **));
}

void fixGaps(){
    for(int i=0;i<dbSize;i++){
        for(int j=0;j<dbSize;j++){
            if(database[i][j] == NULL){
                if(j+1<dbSize){
                    if(database[i][j+1] != NULL){
                        database[i][j] = database[i][j+1];
                        database[i][j+1] = NULL;
                        j--;//to check if the next is also a NULL
                    }
                }
                else{
                    if(i+1<dbSize){
                        if(database[i+1] != NULL){
                            database[i][j]=database[i+1][0];
                            database[i+1][0] =NULL;
                            j--;
                        }
                        else{
                            return;
                        }

                    }
                }
            }

        }
    }
}

void shrinkDB(){
    free(database[dbSize-1]);
    dbSize--;
    TVShow ***temp = (TVShow ***)realloc(database, sizeof(TVShow **)*dbSize);
    if(temp == NULL){
        return;
    }
    database = temp;
    for(int i=0;i<dbSize;i++){
        database[i] = (TVShow **)realloc(database[i], sizeof(TVShow*)*dbSize);
    }
}

void freeEpisode(Episode *e);
void freeSeason(Season *s);
void freeShow(TVShow *show);

void freeEpisode(Episode *e){
    if(e == NULL){
        return;
    }
    freeEpisode(e->next);
    free(e->name);
    free(e->length);
    free(e);
}

void freeSeason(Season *s){
    if(s == NULL){
        return;
    }
    freeSeason(s->next);
    freeEpisode(s->episodes);
    free(s->name);
    free(s);
}

void freeShow(TVShow *show){
    if(show == NULL){
        return;
    }
    freeSeason(show->seasons);
    free(show->name);
    free(show);
}

void freeAll();

void freeAll(){
    for(int i=0;i<dbSize;i++){
        for(int j=0;j<dbSize;j++){
            if(database[i][j] != NULL){
                freeShow(database[i][j]);
                database[i][j] = NULL;
            }
        }
        free(database[i]);
    }
    free(database);

}



TVShow *findShow(char *name);
Season *findSeason(TVShow *show, char *name);
Episode *findEpisode(Season *season, char *name);

TVShow *findShow(char *name){
    for(int i = 0; i< dbSize; i++){
        for(int j = 0; j< dbSize; j++){
            if(database[i][j] != NULL){
                if(strcmp(database[i][j]->name,name) == 0){
                    return database[i][j];
            }
            }
        }
    }
    return NULL; //new show
}

Season *findSeason(TVShow *show, char *name){
    Season *seasonName = show->seasons;
    while(seasonName != NULL){
        if(strcmp(seasonName->name, name) == 0){
            return seasonName;
        }
        seasonName = seasonName->next;
    }
    return NULL;
}

Episode *findEpisode(Season *seasonName, char *name){
    Episode *episodeName = seasonName->episodes;
    while(episodeName != NULL){
        if(strcmp(episodeName->name, name) == 0){
            return episodeName;
        }
        episodeName = episodeName->next;
    }
    return NULL;
}
void addShow();
void addSeason();
void addEpisode();
void initDataBase(char *ptrNewShow);
void inOrder(TVShow *newShow);
void moveArr(int iRow, int jCol);

void addShow(){
    //while (getchar() != '\n');
    printf("Enter the name of the show:\n");
    char *ptrNewShow = getString();
    if(findShow(ptrNewShow) != NULL){
        printf("Show already exists.\n");
        free(ptrNewShow);
        return;//back to main menu
    }
    initDataBase(ptrNewShow);
}

void initDataBase(char *ptrNewShow){
    TVShow *newShow =  (TVShow *)malloc(sizeof(TVShow));
    if(newShow == NULL){
        free(ptrNewShow);
        return;
    }
    newShow->name= ptrNewShow;
    newShow->seasons = NULL;
    if(countShows()+1 > dbSize*dbSize){
        expandDB();
    }
    inOrder(newShow);
}

void inOrder(TVShow *newShow){
    int freeRow = -1, freeCol = -1, countTvShows = 0;
    for(int i = 0; i<dbSize; i++){
        for(int j = 0; j<dbSize; j++){
            if(database[i][j]!= NULL){
                countTvShows++;
                if(strcmp(newShow->name,database[i][j]->name) < 0){
                    moveArr(i, j);
                    database[i][j] = newShow;
                    return;
                }
            }
            else{
                if(freeRow == -1 && freeCol ==-1 && countTvShows == countShows()){
                    freeRow = i;
                    freeCol = j;
                }

            }
        }
    }
    database[freeRow][freeCol] = newShow;
    
}

void moveArr(int iRow, int jCol){
    //int j =0;
    //int i = 0;
    for(int i = dbSize-1; i>=iRow ;i--){
        for(int j= dbSize-1; j>=0; j--){
            if(database[i][j]!=NULL){
                if(j+1 <dbSize){
                    database[i][j+1] = database[i][j];
                    database[i][j] = NULL;
                }
                else{
                    if(i+1<dbSize){
                        database[i+1][0]= database[i][j];
                    }
                    database[i][j] = NULL;
                }
            }
            if(i == iRow && j==jCol){
                return;
            }
        }
    }
}

void addNewSeason(TVShow *nameOfShow, char *seasonName, int pos);

void addSeason(){
     printf("Enter the name of the show:\n");
     char *name =getString();
     TVShow *nameOfShow = findShow(name);
     if(nameOfShow == NULL){
        printf("Show not found.\n");
        free(name);
        return;
     }
     printf("Enter the name of the season:\n");
     char *seasonName = getString();
     Season *newSeason = findSeason(nameOfShow, seasonName);
     if(newSeason != NULL){
        printf("Season already exists.\n");
        free(seasonName);
        free(name);
        return;
     }
     int pos;
     printf("Enter the position:\n");
     scanf("%d", &pos);
     while(getchar() != '\n');
     addNewSeason(nameOfShow, seasonName, pos);
     free(name);

}

void addNewSeason(TVShow *nameOfShow, char *seasonName, int pos){
    Season *newSeason = (Season *)malloc(sizeof(Season));
    if(newSeason == NULL){
        free(seasonName);
        return;
    }
    newSeason->name = seasonName;
    newSeason->episodes = NULL;
    newSeason->next = NULL;
    if(pos == 0 || nameOfShow->seasons == NULL){
        Season *temp = nameOfShow->seasons;
        nameOfShow->seasons = newSeason;
        newSeason->next = temp;
        return;
    }
    Season *currentSeason = nameOfShow->seasons;
    int counterToPos = 0;
    while(currentSeason->next != NULL && counterToPos < pos-1){
        currentSeason = currentSeason->next;
        counterToPos++;
    }
    Season *temp = currentSeason->next;
    currentSeason->next = newSeason;
    newSeason->next = temp;
}

void addNewEpisode(Season *nameOfSeason, char *episodeName, char *length, int pos);

void addEpisode(){
    printf("Enter the name of the show:\n");
    char *name = getString();
    TVShow *nameOfShow = findShow(name);
    if(nameOfShow == NULL){
        printf("Show not found.\n");
        free(name);
        return;
    }
    printf("Enter the name of the season:\n");
    char *seasonName= getString();
    Season *nameOfSeaseon = findSeason(nameOfShow, seasonName);
    if(nameOfSeaseon == NULL){
        printf("Season not found.\n");
        free(seasonName);
        free(name);
        return;
    }
    printf("Enter the name of the episode:\n");
    char *episodeName = getString();
    Episode *nameOfEpisode = findEpisode(nameOfSeaseon, episodeName);
    if(nameOfEpisode != NULL){
        printf("Episode already exists.\n");
        free(episodeName);
        free(seasonName);
        free(name);
        return;
    }
    printf("Enter the length (xx:xx:xx):\n");
    char *length = getString();
    while(validLength(length) != 0){
        printf("Invalid length, enter again:\n");
        free(length);
        length = getString();
    }
    int pos;
    printf("Enter the position:\n");
    scanf("%d", &pos);
    while(getchar() != '\n');
    addNewEpisode(nameOfSeaseon, episodeName, length, pos);
    free(seasonName);
    free(name);
}

void addNewEpisode(Season *nameOfSeason, char *episodeName, char *length, int pos){
    Episode *newEpisode = (Episode *)malloc(sizeof(Episode));
    if(newEpisode == NULL){
        free(episodeName);
        free(length);
        return;
    }
    newEpisode->name = episodeName;
    newEpisode->length = length;
    newEpisode->next = NULL;


    Episode *currentEpisode = nameOfSeason->episodes;
    if(currentEpisode == NULL || pos == 0){
        nameOfSeason->episodes = newEpisode;
        newEpisode->next = currentEpisode;
        return;
    }
    int countToPos = 0;
    while(currentEpisode->next != NULL && countToPos < pos-1){
        currentEpisode = currentEpisode->next;
        countToPos++;
    }
    Episode *temp = currentEpisode->next;
    currentEpisode->next = newEpisode;
    newEpisode->next = temp;
    return;
}
void deleteShow();
position getIndex(TVShow *show);
void moveArrBackwards(int iRow, int jCol);



void deleteShow(){
    printf("Enter the name of the show:\n");
    char *ptrDelShow = getString();
    if(ptrDelShow == NULL){
        return;
    }
    TVShow *toDel = findShow(ptrDelShow);
    if(toDel == NULL){
        printf("Show not found.\n");
        free(ptrDelShow);
        return;
    }
    position index = getIndex(toDel);
    if(index.iRow != -1){
        moveArrBackwards(index.iRow, index.jCol);
        freeShow(toDel);
    if(dbSize>1 && countShows() <= (dbSize- 1)*(dbSize-1)){
        shrinkDB();
    }
    }
    free(ptrDelShow);
}


position getIndex(TVShow *show){
    position indexes;
    indexes.iRow = -1;
    indexes.jCol = -1;
    for(int i = 0; i< dbSize; i++){
        for(int j = 0; j< dbSize; j++){
            if(database[i][j] != NULL){
                if(strcmp(database[i][j]->name,show->name) == 0){
                    indexes.iRow = i;
                    indexes.jCol = j;
                    return indexes;
            }
            }
        }
    }
    return indexes;
}

void moveArrBackwards(int iRow, int jCol){
    database[iRow][jCol] = NULL;
    for(int i= iRow; i<dbSize; i++){
        //to start in the deleted cell and not to delete the cells befor it
        int jPosition;
        if(i == iRow){
            jPosition = jCol;
        }
        else jPosition = 0;
        for(int j=jPosition; j<dbSize; j++){
            if(j+1 < dbSize){
                database[i][j] = database[i][j+1];
                database[i][j+1]=NULL;
            }
            else{
                if(i+1<dbSize){
                database[i][j] = database[i+1][0];
                database[i+1][0]=NULL;
                }
            }
        }
    }
}
void deleteSeason();
void ToDelSeason(TVShow *nameOfShow, char *seasonName);

void deleteSeason(){
    printf("Enter the name of the show:\n");
    char *name = getString();
    TVShow *nameOfShow = findShow(name);
    if(nameOfShow == NULL){
        printf("Show not found.\n");
        free(name);
        return;
    }
    printf("Enter the name of the season:\n");
    char *seasonName = getString();
    Season *SeasonToDel = findSeason(nameOfShow, seasonName);
    if(SeasonToDel == NULL){
        printf("Season not found.\n");
        free(seasonName);
        free(name);
        return;
    }
    ToDelSeason(nameOfShow, seasonName);
    free(seasonName);
    free(name);
}

void ToDelSeason(TVShow *nameOfShow, char *seasonName){
    Season *currentSeason = nameOfShow->seasons;

    if(currentSeason == NULL){
        return;
    }
    //if it's the first season
    if(strcmp(currentSeason->name,seasonName) == 0){
        Season *toDelete = currentSeason;
        nameOfShow->seasons = toDelete->next;
        toDelete->next = NULL;
        freeSeason(toDelete);
        return;
    }
    
    while(currentSeason->next != NULL){
        if((strcmp(currentSeason->next->name, seasonName) == 0)){
            Season *temp = currentSeason->next;
            currentSeason->next = temp->next;
            temp->next = NULL;
            freeSeason(temp);
            return;
        }
        currentSeason = currentSeason->next;
    }
    return;
}

void deleteEpisode();
void toDelEpisode(Season *nameOfSeason, char *episodeName);

void deleteEpisode(){
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *nameOfShow = findShow(showName);
    if(nameOfShow == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }
    printf("Enter the name of the season:\n");
    char *seasonName = getString();
    Season *nameOfSeason = findSeason(nameOfShow, seasonName);
    if(nameOfSeason ==  NULL){
        printf("Season not found.\n");
        free(seasonName);
        free(showName);
        return;
    }
    printf("Enter the name of the episode:\n");
    char *episodeName = getString();
    Episode *nameOfEpisode = findEpisode(nameOfSeason, episodeName);
    if(nameOfEpisode == NULL){
        printf("Episode not found.\n");
        free(episodeName);
        free(seasonName);
        free(showName);
        return;
    }
    toDelEpisode(nameOfSeason, episodeName);
    free(episodeName);
    free(seasonName);
    free(showName);

}

void toDelEpisode(Season *nameOfSeason, char *episodeName){
    Episode *currentEpisode = nameOfSeason->episodes;
    if(currentEpisode == NULL){
        return;
    }
    if(strcmp(currentEpisode->name, episodeName) == 0){
        nameOfSeason->episodes = currentEpisode->next;
        currentEpisode->next = NULL;
        freeEpisode(currentEpisode);
        //currentEpisode = nameOfSeason->episodes;
        return;
    }
    while(currentEpisode->next != NULL) {
        if(strcmp(currentEpisode->next->name, episodeName) == 0){
            Episode *temp = currentEpisode->next->next;
            currentEpisode->next->next=NULL;
            freeEpisode(currentEpisode->next);
            currentEpisode->next = temp;
            return;
        }
        currentEpisode = currentEpisode->next;
    }
    
    
}

void printEpisode();
void printShow();
void printArray();

void printArray(){
    for(int i=0;i<dbSize;i++){
        for(int j=0;j<dbSize;j++){
            if(database[i][j] == NULL){
                printf("[NULL] ");
            }
            else printf("[%s] ",database[i][j]->name);
        }
        printf("\n");
    }
}

void printSeasonArg(TVShow *name);
void printEpisodeArg(Season *nameOfSeason);

void printShow(){
    printf("Enter the name of the show:\n");
    char *name = getString();
    TVShow *show = findShow(name);
    if(show == NULL){
        printf("Show not found.\n");
        free(name);
        return;
    }
    printf("Name: %s\n", show->name);
    printf("Seasons:\n");
    printSeasonArg(show);
    free(name);
}

void printSeasonArg(TVShow *name){
    if(name == NULL){
        return;
    }
    Season *nameOfSeason = name->seasons;
    int i=0;
    while(nameOfSeason != NULL){
        printf("\tSeason %d: %s\n", i, nameOfSeason->name);
        printEpisodeArg(nameOfSeason);
        nameOfSeason = nameOfSeason->next;
        i++;
    }
}

void printEpisodeArg(Season *nameOfSeason){
    if(nameOfSeason == NULL){
        return;
    }
    Episode *nameOfEpisode = nameOfSeason->episodes;
    int i=0;
    while(nameOfEpisode != NULL){
        printf("\t\tEpisode %d: %s (%s)\n", i, nameOfEpisode->name, nameOfEpisode->length);
        nameOfEpisode = nameOfEpisode->next;
        i++;
    }
}

void printEpisode(){
    printf("Enter the name of the show:\n");
    char *name = getString();
    TVShow *nameOfShow = findShow(name);
    if(nameOfShow == NULL){
        printf("Show not found.\n");
        free(name);
        return;
    }
    printf("Enter the name of the season:\n");
    char *nameOfSeason = getString();
    Season *seasonToPrint = findSeason(nameOfShow, nameOfSeason);
    if(seasonToPrint == NULL){
        printf("Season not found.\n");
        free(nameOfSeason);
        free(name);
        return;
    }
    printf("Enter the name of the episode:\n");
    char *nameOfEpisode = getString();
    Episode *EpisodeToPrint = findEpisode(seasonToPrint, nameOfEpisode);
    if(EpisodeToPrint == NULL){
        printf("Episode not found.\n");
        free(nameOfEpisode);
        free(nameOfSeason);
        free(name);
        return;
    }
    printf("Name: %s\nLength: %s\n", EpisodeToPrint->name, EpisodeToPrint->length);
    free(name);
    free(nameOfSeason);
    free(nameOfEpisode);
}

void addMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Add a TV show\n");
    printf("2. Add a season\n");
    printf("3. Add an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: addShow(); break;
        case 2: addSeason(); break;
        case 3: addEpisode(); break;
    }
}

void deleteMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Delete a TV show\n");
    printf("2. Delete a season\n");
    printf("3. Delete an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: deleteShow(); break;
        case 2: deleteSeason(); break;
        case 3: deleteEpisode(); break;
    }
}

void printMenuSub() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Print a TV show\n");
    printf("2. Print an episode\n");
    printf("3. Print the array\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: printShow(); break;
        case 2: printEpisode(); break;
        case 3: printArray(); break;
    }
}

void mainMenu() {
    printf("Choose an option:\n");
    printf("1. Add\n");
    printf("2. Delete\n");
    printf("3. Print\n");
    printf("4. Exit\n");
}

int main() {
    int choice;
    do {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: addMenu(); break;
            case 2: deleteMenu(); break;
            case 3: printMenuSub(); break;
            case 4: freeAll(); break;
        }
    } while (choice != 4);
    return 0;
}

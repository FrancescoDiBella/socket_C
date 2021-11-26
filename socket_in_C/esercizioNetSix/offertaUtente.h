#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SERIE 6
#define SE_MAX_LEN 64

struct __series_t{
    char SERIES_NAME[SE_MAX_LEN];
    unsigned int EPISODE_NUMB;
};

typedef struct __series_t series_t;

series_t catalog[] = {
    {"Friends",200} , {"Better Call Saul", 52} , {"What if...",8} , {"Boris",52} , {"Breaking Bad",100} , {"The Office",125} 
};

char *listaSerie[MAX_SERIE] = {
    "Friends", "The Office", "Breaking Bad", "What if...", "Boris", "Better Call Saul"
};
int listaEpisodi[MAX_SERIE] = {
    200, 216, 56, 8, 33, 42
};

//in termini di eff. non cambia molto

char series_name[SE_MAX_LEN];
unsigned int episode_number;

int validString(char* str){//@validString restituisce l'indice al quale si trova la virgola, restituisce 0 se la stringa non è valida
    int comma_index = -1;
    for(int i = 0; str[i] != 0; i++){

        if(str[i]==','){
            comma_index = i;
            break;
        } 
    }
    if(comma_index == -1){
        printf("Comma index -1\n");
        return 0;
    }
    for(int i = comma_index+1; str[i] != '\r'; i++){
        if(str[i]< '0' || str[i] > '9'){
            printf("elemento non numero : %c\n", str[i]);
            return 0;
        } 
    }
    return comma_index;
}

char* episodioCheck(char* str){
    int is_valid = validString(str);
    int comma_index = is_valid;

    if(is_valid == 0) return "Errore nella formattazione della richiesta.\nFormattazzione corretta: Nome_Serie,Numero_Episodio ";
    sscanf(str, "%[^,],%u", series_name, &episode_number); //fa uno 

    for(int i = 0; i < MAX_SERIE; i++){
        if(strcmp(series_name,catalog[i].SERIES_NAME)==0 && episode_number>=0 && episode_number<=catalog[i].EPISODE_NUMB) 
            return "Episodio Presente!\n";
    }
    return "Coming Soon!\n";
}
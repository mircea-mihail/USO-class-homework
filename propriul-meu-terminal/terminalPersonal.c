// 3. Implementaţi un shell cu următoarele facilităţi:
// - acceptă comanda internă ’exit’;
// - acceptă comenzi externe cu un număr oarecare de argumente;
// - acceptă rularea de script-uri; script-ul poate conţine o listă de comenzi
// utilitare şi poate accepta argumente ı̂n linia de comandă.

//bug cunoscut: nu pot da whitespace sau endline din prima ca imi
//da sec fault si nu prea stiu de ce...

//pare sa mearga bine

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define marimeArbitrara 1000

char *citesteLinia(){
    int marimeCitita = marimeArbitrara, pozitie = 0, litera;
    char *buffer;
    buffer = (char*)malloc(sizeof(char)*marimeCitita);
    //EOF e un int, nu un char, asa ca stochez
    //literele intr-un int pt a evita eroarea 
    //de la laboratorul trecut

    //verific alocarea
    if(buffer == NULL){
        fprintf(stderr, "eroare terminal personal: alocare buffer\n");
        exit(EXIT_FAILURE);
    }

    while(1 == 1){
        litera = fgetc(stdin);

        //daca intalnesc EOF sau comanda trece
        //pe urmatorul rand (\n) inlocuiesc ultimul
        //caracter cu un \0
        if(litera == EOF || litera == '\n'){
            buffer[pozitie] = '\0';
            return buffer;
        }
        else{
            buffer[pozitie] = litera;
        }
        pozitie ++;

        //verific mereu daca mai e loc pe buffer
        if(pozitie >= marimeCitita){
            buffer = (char *)realloc(buffer, sizeof(char) * (marimeCitita + marimeArbitrara));
            marimeCitita += marimeArbitrara;
            //verific daca a realocat ok
            if(!buffer){
                fprintf(stderr, "eroare terminal personal: realocare esuata\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **imparte(char *linie){
    int marimeCitita = marimeArbitrara, pozitie = 0;
    char *token;
    char **tokenuri = (char **)malloc(sizeof(char*) * marimeCitita);
    
    if(tokenuri == NULL){
        fprintf(stderr, "eroare terminal personal: alocare tokenuri\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(linie, " \t");

    if(token == NULL){
        strcpy(tokenuri[0], "s-au_introdus_doar_whitespaceuri...");
        return tokenuri;
    }

    while(token != NULL){
        tokenuri[pozitie] = token;
        pozitie ++;

        if(pozitie >= marimeCitita){
            tokenuri = (char **)realloc(tokenuri, (marimeCitita + marimeArbitrara) * sizeof(char*));
            marimeCitita += marimeArbitrara;
            //verific daca a realocat ok
            if(tokenuri == NULL){
                fprintf(stderr, "eroare terminal personal: realocare tokenuri\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t");
    }
    tokenuri[pozitie] = NULL;
    return tokenuri;
}

//pentru a incepe un proces avem doua cai:
//fie esti un init, care e incarcat cand se 
//booteaza unixul, 
//fie cu ajutorul apelului de sistem fork()
//sistemul de operare face o duplicare a procesului
//si le pune pe ambele in functiune (procesul original
//este procesul tata si cel secunar, procesul copil)
//fork(): returneaza 0 la copil si PID la tata
//procese noi se formeaza prin duplicarea proceselor
//actuale
//exec(): alt apel de sistem care inlocuieste procesul
//duplicat de fork() cu unul complet nou.
//aceste 2 functii reprezinta modul de functionare
//a tuturor programelor de pe UNIX:
//wait(): procesul tata poate controla procesul copil
int lanseaza(char **tokenuri){
    pid_t pid, wpid;
    //wpid --> waitpid
    int stare;

    //pid = procesul 
   
    pid = fork();
   
    //frokuiesc procesul
    //verific procesul copil
    //v - execv(), execvp(), execvpe()
    //stringurile argument sunt date
    //printr-un vector (array)
    
    if(pid == 0){
        //copil:
        //la execvp trebuie dat un nume de program
        //si un array cu argumente. Se foloseste 
        //de PATH pentru a cauta executabile
        if(execvp(tokenuri[0], tokenuri) == -1){
            //verific daca e vreo eroare la execvp
            perror("eroare terminal personal: execvp()");
        }
        exit(EXIT_FAILURE);
    }
    else
        if(pid < 0){
            //eroare la fork
            perror("eroare terminal personal: fork()");
        }
        else
        //parinte:
        //copilul va executa procesul dat asa ca parintele
        //trebuie sa astepte ca acesta sa se termine
            do{
                //cu waitpid astept sa vad o schimbare
                //la statusul procesului

                //WUNTRACED returnează dacă copilul
                //a fost stopat sau terminat
                wpid = waitpid(pid, &stare, WUNTRACED);
            }while(!WIFEXITED(stare) && !WIFSIGNALED(stare));
        //WIFEXITED returneaza true daca copilul s-a
        //terminat normal: exit()

        //WIFSIGNALED() returneaza true daca copilul
        // a fost terminat de un semnal
    
    //return 1 ii spune functiei apelante ca poate
    //introduce o alta comanda
    return 1;
}

int executa(char **argumente){

    //asta face sa nu mai dea sec fault cand se introduc doar spatii sau taburi
    if(strcmp(argumente[0], "s-au_introdus_doar_whitespaceuri...") == 0){
        //printf("s-au introdus doar whitespaceuri...\n");
        return 1;
    }

    //se ocupa de comanda interna exit

    if(strcmp(argumente[0], "exit") == 0)
        return 0;

    //am adaugat si comanda interna cd pt ca am crezut ca ar fi cool
    if(strcmp(argumente[0], "cd") == 0){
        if(argumente[1] == NULL)
            printf("trebuie specificat un director pentru cd...\n");
        else
            if(argumente[2] != NULL)
                printf("mircea: cd: too many arguments\n");
            else
            if(chdir(argumente[1]) != 0){
                perror("eroare la terminalul personal: cd");
            }
            
        return 1;
    }

    if((argumente[1] == NULL) && (strcmp(argumente[0], "help") == 0)){
        printf("\nterminal codat de Ionescu Mircea Mihail, anul 1, grupa 163.\nPentru a iesi, apeleaza comanda internta \"exit\"\n\n");
        return 1;
    }
    return lanseaza(argumente); 
}

void bucla(void){
    char *linia;
    char **argumentele;
    //daca statusul este diferit de 0
    //bucla continua
    int stare;

    //citire comanda data
    //transmite comanda 
    //executa comanda

    do{
        printf("mircea> ");
        linia = citesteLinia();
        //argumente reprezinta un array de stringuri
        //printf("%s\n", linia);
        //impart argumentele dupa spatiu
        argumentele = imparte(linia);
        
        //acum ca avem un vector de comenzi, 
        //le execut
        stare = executa(argumentele);
        //in urma executarii comenzii
        //pun rezultatul executarii in status

        free(linia);
        free(argumentele);

    }while(stare != 0);
}

int main(int argc, char **argv){
    //structura:
    if(argc != 1){
        perror("functia nu necesita decat apelul ei");
            return 1;
    }
    
    //cod
    //incarc fisiere de configurare
    //run an REPL
    //(read execute print loop)

    bucla();

    //dupa ar trebui sa ma ocup de shutdown sau cleanup
    return 0;
}

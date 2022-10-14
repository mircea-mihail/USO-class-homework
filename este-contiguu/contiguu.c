// 5. Scriet¸i un program care determin˘a dac˘a cont¸inutul unui fi¸sier este inclus
// (contiguu) ˆın cont¸inutul altui fi¸sier ¸si de cˆate ori (num˘arul aparit¸iilor). Se vor
// considera ¸si aparit¸iile care se suprapun. Specificatorii fi¸sierelor sunt dat¸i ca
// argumente ˆın linia de comand˘a. Se vor folosi funct¸ii de nivel superior (tipul
// ’FILE’).

#include <string.h> 
#include <stdlib.h>
#include <stdio.h>

//să se numerede câteori apare un șir t într-un șir s

// este contiguu?
// de cate ori?

//ATENTIE! functia contiguu de mai jos nu este apelata si are doar rol istoric

void contiguu(FILE *f1, FILE *f2){
    char dummy[1000];
    
    //nu citesc fisierele in memorie
    //fisierele sa fie oricat de lungi cu memorie limitata

    char *s, *t;
    //s si t alocare dinamica
    //dummy ramane static pana ma gandesc la ceva mai bun
    

    //citirile clar sunt bazate
    
    fgets(dummy, 1000, f1);
    t = (char *)malloc(strlen(dummy)*sizeof(char));
    //testez daca malloc esueaza si dezalocat in caz ca nu merge
    //mai bine cu calloc ca initializeaza cu 0
    strcpy(t, dummy);

    //concatenez restul linilor
    while(fgets(dummy, 1000, f1)){
        t = (char *)realloc(t, (strlen(dummy) + strlen(t)+1)*sizeof(char));
        //si realloc trebuie testat si dezalocat in caz ca nu merge
        strcat(t, dummy);
    }

    //uneori vede un endl la finalul fisierelor cand nu e acolo xd
    //tratat cazul in care fisierul e vid
    
    //program gaunos
    if(t[strlen(t) - 1] == '\n')
        t[strlen(t) - 1] = '\0';
        //sa nu mai sterg capul de linie

    fgets(dummy, 1000, f2);
    s = (char *)malloc(strlen(dummy)*sizeof(char));
    strcpy(s, dummy);

    while(fgets(dummy, 1000, f2)){
        s = (char *)realloc(s, (strlen(dummy) + strlen(s) + 1)*sizeof(char));
        strcat(s, dummy);
    }

    //restul programului e bun

    //      pentru verificare
    // printf("din f1 am luat:\n%s\n", t);
    // printf("din f2 am luat:\n%s\n", s);

    int nrAparitii = 0;
    char *p;

    p = strstr(s, t);
    if(p != NULL)
        printf("\"%.100s\"\n", p);

    while(p){
        nrAparitii++;
        p = strstr(p+1, t);
        if(p != NULL)
            printf("\"%.100s\"\n", p);
    }

    if(nrAparitii == 0)
        printf("fisierul nu e contiguu\n");
    else{
        if(nrAparitii == 1)
            printf("\nfisierul este contiguu si apare o data\n");
        else
            printf("\nfisierul este contiguu si apare de %d ori\n", nrAparitii);
    }    
    
    free(s);
    free(t);

}


void contiguuButBetter(FILE *f1, FILE *f2){
	//trebuie sa iau caracterele separat
	//fgetc();
	//while(ceva != EOF)
	//fseek
	
    int nrAparitii = 0, are_rost=0;
	long int pozitie_curenta = 0; 
	
    //255 devine -1 si se confunda cu EOF
    //confund lipsa char cu char 377
    //fgetc intoarce int, pe care il fac char
    //si dupa il fac int

    //FGETC intoarce INT!!

    //FOARTE MARE ATENTIE LA CE INTORC FUNCTIILE

    int chMic, chMare;
    int verificEgalitatea = 0;
	chMare = fgetc(f2);

	while(chMare != EOF){
            //debug
        //printf("\nchMare inceput: %c\n", chMare);

		if(fseek ( f2, pozitie_curenta, SEEK_SET ) != 0){
            printf("a avut loc o eroare la pozitie cu al doilea fisier");
        }

        if(fseek ( f1, 0, SEEK_SET ) != 0){
            printf("a avut loc o eroare la pozitie cu primul fisier");
        }

        verificEgalitatea = 0;
		while((chMic = fgetc(f1)) != EOF){
            if((chMare = fgetc(f2)) == EOF){
                are_rost = 1;

                    //debug
                //printf("\n!!!!s-a terminat!!!!\n");
                
                break;
            }

                //debug
            //printf("f1:%c ", chMic);
            //printf("f2:%c ", chMare);

            if(chMic != chMare){
                verificEgalitatea = 1;
                    //debug
                //printf("\n!!!Egalitatea!!! f1: %c f2: %c\n", chMic, chMare);
            }
        }  
        //daca fisierul mare devine mai mic decat primul
        //opresc cautarea ca e clar ca nu mai e contiguu
        if(are_rost == 1)
            break;

        if(verificEgalitatea == 0){
            nrAparitii ++;
            //printf("\nAm gasit! (mai sus)\n");
        }

        //dupa ce termin cu primul caracter
        pozitie_curenta ++;

        //pozitionez cursorul unde trebuie sa fie in fisierul
        //mare ca apo sa pot pune in chMare urmatoarea litera
        //de la care sa incep

        if(fseek ( f2, pozitie_curenta, SEEK_SET ) != 0){
            printf("a avut loc o eroare la pozitie");
        }

        chMare = fgetc(f2);
	}

    if(nrAparitii == 0)
        printf("fisierul nu e contiguu\n");
    else{
        if(nrAparitii == 1)
            printf("fisierul este contiguu si apare o data\n");
        else
            printf("fisierul este contiguu si apare de %d ori\n", nrAparitii);
    }
    
}


int main(int argc, char * argv[]){

    //Din file1 iau stringul si in file2 il caut
    char *file1, *file2;

    //argc:     refers to the number of arguments passed

    //argv[]:   is a pointer array which points to each
    //          argument passed to the program


    if(argc != 3){
        //argv[0] e pointer la "-o program" (numele dat executabilului)
		fprintf(stderr, "Utilizare: %s fisier_din_care_caut fisier_in_care_caut\n", argv[0]);
		return 1;
	}
	
	file1 = argv[1];
	file2 = argv[2];

    FILE* f1 = fopen(file1, "r");
    
    //verific daca f1 a fost adresat corect
    if(f1 == NULL){
		perror(file1);
		return 1;
	}
	
	FILE* f2 = fopen(file2, "r");
	
    //verific daca f2 a fost adresat corect
    if(f2 == NULL){
		perror(file2);
		fclose(f1);
		return 1;
	}
	
	//contiguu(f1, f2);
    contiguuButBetter(f1, f2);

    fclose(f1);
	fclose(f2);
    
    return 0;

}

// gcc -Wall -o contiguu  ex5.c
// contiguu deLuat.txt deCautat.txt 

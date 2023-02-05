/*KNIZNICE*/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# include <time.h>

/*MAKRA*/
# define FILTER 2            /* Urcuje definiciu "slov", - 1 - ZACHOVANIE INTERPUNKCIE, - 2 - VYMAZANIE INTERPUNKCIE. */
# define TEST 80         /* Urcuje velkost nahodneho testovacieho retazca ! POZOR ! NESMIE BYT VATCSI AKO MAX_SKEN_ZAPIS! */
# define MAX_SKEN_ZAPIS 10000   /* Urcuje maximalnu dlzku naskenovaneho retazca, ktory sa cita od pouzivatela alebo zo suboru. */
# define MAX_SKEN 50  /* Urcuje maximalnu dlzku naskenovaneho retazca, ktory sa cita od pouzivatela alebo zo suboru. */

/* PRE UCEL MERANIA CASU */
#define TICK(X) clock_t X = clock() //SPUSTENIE MERANIA CASU
#define TOCK(X) printf("\n\t time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC) //ZASTAVENIE A VYPIS ZMERANEHO CASU

/* PRE UCEL MERANIA CASU DUPLIKACIOU VOLANIA */
#define TIMES 1 // urcuje kolko krat sa maju funkcie zopakovat vrámci 1 spustenia programu.
#define TIMES_WHOLE 1 //urcuje kolko krat sa má cely program opekovat, aj s vypisom.

/*------------------------------------------------- DEKLARACIA FUNKCII-------------------------------------------------------------*/

void vymaz_extra_whitespaces(char* retazec);
int pole_org(char *pole_char,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN]);
void sorting(int space_count,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN]);
int vyskyt(int space_count,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN],char dvrozvys[MAX_SKEN_ZAPIS][MAX_SKEN]);
int vyskyt_vypis(int space_count, int pocitadlo,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN],char dvrozvys[MAX_SKEN_ZAPIS][MAX_SKEN]);
int switch_volba(void);
void export_novy_retazec(int space_pocet, char *pole_char, char *nazsub1, char *nazsub2,FILE *filePointer,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN]);
void nahr_interpunkcie(char *pole_char);

/*--------------------------------------------------------MAIN --------------------------------------------------------------------*/

int main()
{
    char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN], dvrozvys[MAX_SKEN_ZAPIS][MAX_SKEN];
    char pole_char[MAX_SKEN_ZAPIS],data_citanie[MAX_SKEN_ZAPIS],nazsub1[261],nazsub2[261],*testy[MAX_SKEN_ZAPIS];
    int  i, pocitadlo = 0, space_count = 0; // SPACE_COUNT : pocet medzier
    FILE *Subor_pointer ;

    printf("\n\t\t\t\t\t  Semestralne zadanie c. 13 \n");
    printf("\n\t Program, ktory v subore najde prvky (slova), nasledne ich zoradi podla abecedy a vypise ich pocet.\n\n");
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    printf(" Zadajte - 1 - Vytvorenie vlastneho textoveho suboru ako vstup. \n");
    printf(" Zadajte - 2 - Pouzitie existujuceho textoveho suboru ako vstup.\n");
    printf(" Zadajte - 3 - Testovanie\n");
    printf("-----------------------------------------------------------------------------------------------------------------\n => ");

    switch(switch_volba())
    {

//-----------------------------------------------------------------------------------CASE 1-------------------------------------------------------------------------------------------
    case 1: /* VYTVORENIE VLASTNEHO TEXTOVEHO SUBORU AKO VSTUP */
    {
        /* NACITANIE VSTUPNEHO RETAZCA OD POUZIVATELA */

        printf("\n\t Zadajte retazec, ktory sa zapise do suboru, maximalna velkost je %d \n\n\n => ",MAX_SKEN_ZAPIS); // MAX_SKEN_ZAPIS znakov + \0 znak
        fflush(stdin);
        fgets(pole_char,MAX_SKEN_ZAPIS+1,stdin);//AK POUZIVATEL ZADA VIACEJ ZNAKOV AKO MAX_SKEN_ZAPIS FGETS ICH ZAHODI.


        /* NACITANIE NAZVU SUBORU KDE SA ULOZI VSTUNY RETAZEC */

        printf("\n\n\t Prosim zadajte nazov noveho textoveho suboru v formate ' nazov.txt ', ktory bude obsahovat zadany retazec.\n\n => ");
        do
        {
            fflush(stdin);
            gets(nazsub1);
            Subor_pointer = fopen(nazsub1, "w") ;
            if ( Subor_pointer == NULL )
            {
                printf( "\n\t Subor %s sa nepodarilo vytvorit, zadajte nazov znovu \n\n => ",nazsub1 ) ;
            }
        }
        while( Subor_pointer == NULL );

        /* VKLADANIE VSTUNEHO RETAZECA DO SUBORU */

        if ( strlen ( pole_char ) > 0 )
        {
            fputs(pole_char, Subor_pointer) ;
            fputs(" ", Subor_pointer) ;
        }

        fclose(Subor_pointer) ;
        printf( "\n\t Subor %s sa podarilo vytvorit, data su uspesne ulozene v subore.\n\n",nazsub1 ) ;

        if(FILTER == 2)
        {
            nahr_interpunkcie(pole_char);
        }

        /* VYMAZANIE EXTRA MEDZIER, TZV. "WHITESPACES */

        TICK(t_vymaz_extra_whitespaces);

        for(i=0; i<TIMES; i++)
        {
            vymaz_extra_whitespaces(pole_char);
        }

        TOCK(t_vymaz_extra_whitespaces);

        /* ORGANIZACIA VSTUPU DO DVOJROZMERNEHO POLA */

        TICK(t_pole_org);

        for(i=0; i<TIMES; i++)
        {
            space_count = pole_org(pole_char,dvroz);
        }

        TOCK(t_pole_org);

        /* ASCII SORTING */

        TICK(t_Bubble_sort);

        for(i=0; i<TIMES; i++)
        {
            sorting(space_count,dvroz);
        }

        TOCK(t_Bubble_sort);

        /* VYPISANIE UPRAVENEHO RETAZCA */

        printf("\t\n\n\n\n\t Po abecednom zoradeni kazdeho slova v subore string vyzera takto: \n\n\n");


        printf("-----------------------------------------------------------------------------------------------------------------\n\t  ");
        for (i = 0; i <= space_count; i++)
        {
            printf(" %s",dvroz[i]);
            pocitadlo++;
        }
        printf("\n-----------------------------------------------------------------------------------------------------------------\n  ");

        /* VYPISANIE VYSKYTU SLOV */

        vyskyt_vypis(space_count,pocitadlo,dvroz,dvrozvys);

        /* EXPORT UPRAVENEHO RETAZCA */

        export_novy_retazec(space_count, pole_char, nazsub1, nazsub1, Subor_pointer,dvroz);

        break;

        /* KONIEC  */
    }
//-----------------------------------------------------------------------------------CASE 2 -------------------------------------------------------------------------------------------
    case 2:  /* POUZITIE EXISTUJUCEHO TEXTOVEHO SUBORU AKO VSTUP */
    {
        /* NACITANIE NAZVU SUBORU KDE JE ULOZENY VSTUNY RETAZEC */

        printf("\n\t Zadajte nazov textoveho suboru v formate ' nazov.txt '.\n\n\n => ");
        do
        {
            fflush(stdin);
            gets(nazsub2);
            Subor_pointer = fopen(nazsub2, "r") ;
            if ( Subor_pointer == NULL )
            {
                printf("\n\n\t Subor ' %s ' sa nepodarilo otvorit! Pozadovany subor a program musia byt ulozene v rovnakej lokacii.", nazsub2);
                printf("\n\t Skontrolujte taktiez syntax a zadajte nazov znovu. \n\n => ") ;

            }
        }
        while(Subor_pointer == NULL);

        /* OBSAH VSTUPNEHO SUBORU PRED UPRAVOU */
        TICK(t_celok);
        int h = 0;
        for(h=0; h<TIMES_WHOLE; h++)
        {


            printf("\n\n\t V danom textovom subore je zapisane:\n\n\n") ;
            printf("-----------------------------------------------------------------------------------------------------------------\n\t");
            while( fgets ( data_citanie, MAX_SKEN_ZAPIS, Subor_pointer) != NULL )
            {
                printf( " %s", data_citanie );
                strncpy(pole_char, data_citanie, MAX_SKEN_ZAPIS);
            }
            printf("\n-----------------------------------------------------------------------------------------------------------------\n");
            fclose(Subor_pointer) ;

            if(FILTER == 2)
            {
                nahr_interpunkcie(pole_char);

            }

            /* VYMAZANIE EXTRA MEDZIER, TZV. "WHITESPACES */

            TICK(t_vymaz_extra_whitespaces);

            for(i=0; i<TIMES; i++)
            {
                vymaz_extra_whitespaces(pole_char);
            }

            TOCK(t_vymaz_extra_whitespaces);

            /* ORGANIZACIA VSTUPU DO DVOJROZMERNEHO POLA */

            TICK(t_pole_org);

            for(i=0; i<TIMES; i++)
            {
                space_count = pole_org(pole_char,dvroz);
            }

            TOCK(t_pole_org);

            /* ASCII SORTING */

            TICK(t_Bubble_sort);

            for(i=0; i<TIMES; i++)
            {
                sorting(space_count,dvroz);
            }

            TOCK(t_Bubble_sort);

            /* VYPISANIE UPRAVENEHO RETAZCA */

            printf("\n\n\t Po abecednom zoradeni kazdeho slova v subore string vyzera takto: \n\n\n");

            printf("-----------------------------------------------------------------------------------------------------------------\n\t");
            for (i = 0; i <= space_count; i++)
            {
                printf(" %s",dvroz[i]);
                pocitadlo++;
            }
            printf("\n-----------------------------------------------------------------------------------------------------------------\n");

            /* VYPISANIE VYSKYTU SLOV */

            vyskyt_vypis(space_count, pocitadlo,dvroz,dvrozvys);

            /* EXPORT UPRAVENEHO RETAZCA */
        }
        TOCK(t_celok);

        export_novy_retazec(space_count, pole_char,nazsub1,nazsub2,Subor_pointer,dvroz);

        break;

        /* KONIEC  */
    }
//-----------------------------------------------------------------------------------CASE 3 -------------------------------------------------------------------------------------------
    case 3:  /* TESTOVANIE*/
    {
        srand((unsigned int)(time(NULL)));
        int i= 0;
        char u[TEST+1];
        char char1[] = "abcd , ";
        for(i = 0; i < (TEST); i++)
        {
            u[i] = (char1[rand() % (sizeof char1 - 1)]);
        }

        strcpy(nazsub1, "original.txt");

        Subor_pointer = fopen(nazsub1, "w") ;

        if ( Subor_pointer == NULL )
        {
            printf( "\n\t Subor %s sa nepodarilo vytvorit \n",nazsub1 );
            break;
        }

        if ( strlen ( u ) > 0 )
        {
            fputs(u, Subor_pointer) ;
        }

        fclose(Subor_pointer) ;

        Subor_pointer = fopen(nazsub1, "r") ;
        if ( Subor_pointer == NULL )
        {
            printf("\n\n\t Subor ' %s ' sa nepodarilo otvorit! \n",nazsub1) ;
            break;
        }

        printf("\n\n\t V textovom subore je zapisane:\n\n\n") ;
        printf("-----------------------------------------------------------------------------------------------------------------\n\t");
        while( fgets ( data_citanie, MAX_SKEN_ZAPIS, Subor_pointer ) != NULL )
        {
            printf( " %s", data_citanie ) ;
        }
        printf("\n-----------------------------------------------------------------------------------------------------------------\n\n");
        fclose(Subor_pointer) ;

        if(FILTER == 2)
        {
            nahr_interpunkcie(u);
        }

        /* VYMAZANIE EXTRA MEDZIER, TZV. "WHITESPACES */

        TICK(t_vymaz_extra_whitespaces);

        for(i=0; i<TIMES; i++)
        {
            vymaz_extra_whitespaces(u);
        }

        TOCK(t_vymaz_extra_whitespaces);


        /* ORGANIZACIA VSTUPU DO DVOJROZMERNEHO POLA */

        TICK(t_pole_org);

        for(i=0; i<TIMES; i++)
        {
            space_count = pole_org(u,dvroz);
        }

        TOCK(t_pole_org);

        /* ASCII SORTING */

        TICK(t_Bubble_sort);

        for(i=0; i<TIMES; i++)
        {
            sorting(space_count,dvroz);
        }

        TOCK(t_Bubble_sort);

        /* VYPISANIE UPRAVENEHO RETAZCA */

        printf("\t\n\t Po abecednom zoradeni kazdeho slova v subore string vyzera takto: \n\n\n");


        printf("-----------------------------------------------------------------------------------------------------------------\n\t  ");
        for (i = 0; i <= space_count ; i++)
        {
            printf("%s ",dvroz[i]);
            pocitadlo++;
        }
        printf("\n-----------------------------------------------------------------------------------------------------------------\n  ");

        /* VKLADANIE VYSLEDEHO RETAZECA DO SUBORU */
        strcpy(nazsub2, "upraveny.txt");

        Subor_pointer = fopen(nazsub2, "w") ;

        if ( strlen ( u ) > 0 )
        {
            for (i = 0; i <= space_count; i++)
            {
                fputs(dvroz[i], Subor_pointer) ;
                fputs(" ", Subor_pointer) ;
            }
        }

        fclose(Subor_pointer) ;

        /* VYPISANIE VYSKYTU SLOV */

        vyskyt_vypis(space_count,pocitadlo,dvroz,dvrozvys);

        printf( "\n\n\n\t ' %s ' a subor ' %s ' boli vytvorene\n\n\n",nazsub1, nazsub2) ;
        break;
    }
        /* KONIEC MAIN SWITCHU  */
    }

    /* VYPIS MOZNOSTI NA KONCI PROGRAMU  */

    printf("\n-----------------------------------------------------------------------------------------------------------------\n");
    printf(" Zadajte - 1 - Ak chcete program ukoncit. \n");
    printf(" Zadajte - 2 - Ak chcete program opakovat.\n");
    printf(" Zadajte - 3 - Ak chcete videt info o autorovi.\n");
    printf("-----------------------------------------------------------------------------------------------------------------\n => ");
    switch(switch_volba())
    {
    case 1:  /* PROGRAM SA UKONCI CEZ BREAK NA RETURN 0 */
    {
        break;
    }
    case 2: /* PROGRAM SA OPAKUJE = VRATI SA K MAIN */
    {
        system("cls");
        return main();
    }
    case 3: /* INFO O AUTOROVI */
    {
        system("cls");
        printf("\n\n\n\t Autor: Pavol Lukacka \n");
        time_t t;
        time(&t);
        printf("\n\t %s \n",ctime(&t));
        printf("\t Email: pavollukacka.l@gmail.com\n\n\n\n");
        break;
    }
        /* KONIEC VEDLAJSIEHO SWITCHU  */
    }
    return 0;

    /* KONIEC PROGRAMU */
}


void vymaz_extra_whitespaces(char* pole_char)
{
    int i, x;
    for(i=x=0; pole_char[i]; ++i)
        if(!isspace(pole_char[i]) || (i > 0 && !isspace(pole_char[i-1])))
            pole_char[x++] = pole_char[i];
    pole_char[x] = '\0';
}

/*------------------------------------------- ORGANIZACIA-POLE -----------------------------------------------------*/
int pole_org(char *pole_char,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN])
{
    int i, j = 0, k, p = strlen(pole_char)-1, count_medzery =0;

    for (i = 0, j = 0, k = 0; j < p+1; j++) //PRECHADZA PO JEDNOTLIVYCH CHAR KYM NEPREJDE CELE POLE
    {
        if (isspace(pole_char[j]))
        {
            dvroz[i][k] = '\0'; //SLOVA SU POD "i" A OBSAH SLOV POD "k" OBE SA UKONCIA AK SA NAJDE MEDZERA

            i++; //POSTUPUJE NA DRUHE SLOVO
            k = 0; //OBSAH PRE DALSIE SLOVO SA NASTAVI NA INDEX 0
            count_medzery ++;
        }
        else
        {
            dvroz[i][k++] = pole_char[j]; //POSTUPUJE SA NA DALSI CHAR A ZAPISE SA DO SLOVA NA DANOM INDEXE

        }
    }

    if(FILTER == 1) //AK POUZIVATEL ZADA WHITESPACE ZA KONECNYM CHARAKTEROM V SUBORE ALEBO V SKENOVANOM RETAZCI, NACITA SA NADPOCETNA MEDZERA A VYPISE VYSKYT "NULL"
    {
        if (isspace(pole_char[p]))
        {
            count_medzery--;
        }

    }
    else if(FILTER == 2) //AK POUZIVATEL ZADA WHITESPACE/INTERPUNKCIOU ZA KONECNYM CHARAKTEROM V SUBORE ALEBO V SKENOVANOM RETAZCI, NACITA SA NADPOCETNA MEDZERA A VYPISE VYSKYT "NULL"
    {
        if ((isspace(pole_char[p])) || (ispunct (pole_char[p])))
        {
            count_medzery--;
        }
    }
    return count_medzery;
}
/*-------------------------------------------ASCII SORTOVANIE -----------------------------------------------------*/
void sorting(int space_count,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN])
{
    int i,j = 0;
    char tmp[MAX_SKEN_ZAPIS];
    for (i = 0; i < space_count; i++)  // I SA MUSI = POCTU MEDZIER, POTOM SA SKONCI
    {
        for (j = i + 1; j <= space_count; j++) // "j" MUSI BYT > POCTET MEDZIER, POTOM SA SKONCI
        {
            if ((strcmp(dvroz[i], dvroz[j]) > 0)) //AK SLOVO MA VATCSIU HODNOTU ASCII, TAK SI VYMENIA MIESTA, PRECHADZA SA VSETKYMI SLOVAMI
            {
                strcpy(tmp, dvroz[i]);
                strcpy(dvroz[i], dvroz[j]);
                strcpy(dvroz[j], tmp);
            }
        }
    }
}
/*-------------------------------------------------VYSKYT ----------------------------------------------------------*/
int vyskyt(int space_count,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN],char dvrozvys[MAX_SKEN_ZAPIS][MAX_SKEN])
{
    TICK(t_vyskyt);
    int pocet = 0, i, j=0,k=0;

    for (i = 0; i <= space_count; i++) // I MUSI BYT VATCSIE AKO POCET MEDZIER, POTOM SA SKONCI - KED SA SKONCI BUDU POROVNANE VSETKY SLOVA
    {
        for (j = 0; j <= space_count; j++) // "j" MUSI BYT VATCSIE AKO POCET MEDZIER, POTOM SA SKONCI - KED SA SKONCI BUDE POROVNANE SLOVO NA INDEXE "i" ZO VSETKYMI SLOVAMI
        {
            if (i == j)  //AK SA ROVNAJU PREKOPIRUJKU SA NA "i" DVOJROZMERNEHO POLA DVROZVYS
            {
                strcpy(dvrozvys[k], dvroz[i]);
                k++;
                pocet++; //POCET SA NAVYSI
                break;
            }
            else
            {
                if (strcmp(dvrozvys[j], dvroz[i]) != 0) // AK SA NEROVNAJU TAK SA SLOVO ZAPISE A INDEX SA POSUNIE
                    continue;
                else
                    break;
            }
        }
    }
    return pocet; //POCET SA EXPORTUJE NA POUZITIE
    TOCK(t_vyskyt);
}
/*------------------------------------------------VYSKYT TEXT -----------------------------------------------------*/
int vyskyt_vypis(int space_count, int pocitadlo,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN],char dvrozvys[MAX_SKEN_ZAPIS][MAX_SKEN])
{
    int c = 0, prenos, i, j = 0;
    prenos = vyskyt(space_count,dvroz,dvrozvys);
    printf("\n\n\t Vyskyt jednotlivych slov v zadanom retazci: \n\n\n"); //VYPISUJE SA VYSKYT
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < prenos; i++)
    {
        for (j = 0; j <= space_count; j++)
        {
            if (strcmp(dvrozvys[i], dvroz[j]) == 0)
                c++;
        }
        printf("\t  %s -> %d krat\n", dvrozvys[i], c);
        c = 0;
    }
    printf("\n\t Celkovy pocet slov je %d \n",pocitadlo); //POCET SLOV
    if(pocitadlo>prenos)
    {
        printf("\t Celkovy pocet slov bez duplicity je %d \n",prenos); //POCET SLOV BEZ DUPLICITY
    }
    printf("-----------------------------------------------------------------------------------------------------------------\n");
}
/*-------------------------------------------SWITCH 3 MOZNOSTNA VOLBA-----------------------------------------------------*/
int switch_volba(void)
{
    char vyber_ako_char[100]; //OSETRENE VYBEROM
    int vyber;
    do
    {
        gets(vyber_ako_char);
        vyber = *vyber_ako_char - '0';
        if((vyber !=1)&&(vyber !=2)&&(vyber !=3))
            printf("\n\t\t\t\t %s nieje validna moznost! Zadajte znovu.\n => ",vyber_ako_char);
    }
    while((vyber != 1)&&(vyber != 2)&&(vyber != 3));
    return vyber;
}
/*---------------------------------EXPORT UPRAVENEHO RETAZCA DO SUBORU ALEBO ZAHODENIE -----------------------------------------------------*/

void export_novy_retazec(int space_pocet, char *pole_char, char *nazsub1, char *nazsub2,FILE *filePointer,char dvroz[MAX_SKEN_ZAPIS][MAX_SKEN])
{
    int i;
    printf("\n\n\t  Vystupny string je pripraveny na export \n\n");
    printf("\n-----------------------------------------------------------------------------------------------------------------\n");
    printf(" Zadajte - 1 - Ak chcete ulozit upraveny string do noveho suboru. \n");
    printf(" Zadajte - 2 - Ak chcete nahradit obsah suboru ' %s '. \n",nazsub2);
    printf(" Zadajte - 3 - Ak chcete zahodit upraveny string. \n");
    printf("-----------------------------------------------------------------------------------------------------------------\n => ");
    switch(switch_volba())
    {
    case 1:  // ULOZENIE VYSTUPNEHO RETAZCA DO NOVEHO SUBORU
    {
        system("cls");
        printf("\n\n\t Prosim zadajte nazov noveho textoveho dokumentu v formate ' nazov.txt ', ktory bude obsahovat zoradene slova.\n\n\n => ");
        do
        {
            fflush(stdin);
            gets(nazsub1); // OSETRENE LIMITOM 260 ZNAKOV PRE VYTVORENIE NAZVU VO WINDOWS 10
            filePointer = fopen(nazsub1, "w") ;
            if ( filePointer == NULL )
            {
                printf( "\n\t\t\t\t Subor %s sa nepodarilo vytvorit, zadajte nazov znovu",nazsub1 ) ;
            }
        }
        while( filePointer == NULL );

        if ( strlen ( pole_char ) > 0 )
        {
            for (i = 0; i <= space_pocet; i++)
            {
                fputs(dvroz[i], filePointer) ;
                fputs(" ", filePointer) ;
            }
        }

        fclose(filePointer) ;
        printf( "\n\t Subor %s sa podarilo vytvorit, data su uspesne ulozene v subore \n",nazsub1 ) ;

        break;
    }
    case 2:  // ULOZENIE VYSTUPNEHO STRINGU DO VSTUPNEHO SUBORU NAHRADENIM (PREPISANIM POVODNEHO OBSAHU)
    {
        system("cls");
        do
        {
            filePointer = fopen(nazsub2, "w") ;
            if ( filePointer == NULL )
            {
                printf( "\n\t Subor ' %s ' sa nepodarilo otvorit, program konci(fatalna chyba)",nazsub2 );
                break;
            }
        }
        while( filePointer == NULL );

        if ( strlen ( pole_char ) > 0 )
        {
            for (i = 0; i <= space_pocet; i++)
            {
                fputs(dvroz[i], filePointer) ;
                fputs(" ", filePointer) ;
            }
        }
        fclose(filePointer) ;
        printf( "\n\t data su uspesne ulozene v subore ' %s '\n\n",nazsub2 ) ;
        break;
    }
    case 3:  //ZAHODENIE VYSTUPNEHO STRINGU
    {
        system("cls");
        break;
    }
    }
}

/*---------------------------------NAHRADA INTERPUNKCIER -----------------------------------------------------*/

void nahr_interpunkcie(char *pole_char) //NAHRADA INTERPUNKCIE ZA MEDZERY
{
    char interpunkcia[] = { '.', '?', '!', ':', ';',
                            '-', '(', ')', '[', ']',
                            ',', '"', '/'
                          };

    int i, j, k, ch, ozn;
    i = j = k = ozn = 0;

    while (pole_char[i] != '\0') //KYM SA POLE_CHAR NENACHAZDA NA KONCI
    {
        ozn = 0;
        ch = pole_char[i];

        for (j = 0; j < 13; j++) // KYM SA NEPREJDU VSETKY ZNAKY INTERPUNKCIE
        {
            if (ch == interpunkcia[j]) //AK SA NACHADZA NA INDEXE "i=ch" INTERPUNKCIA, NAHRADI SA MEDZEROU
            {
                pole_char[i] = ' ';
                break;
            }
        }
        if (!ozn) //AK SA NACHADZA NA INDEXE "i=ch" NENACHADZA INTERPUNKCIA ULOZI SA VYGENEROVANE SLOVO BEZ INTERPUNKCIE A POSUNIE SA DALEJ NA DALSIE SLOVO
        {
            pole_char[k++] = pole_char[i];
        }
        i++;
    }
    pole_char[k] = '\0';
}

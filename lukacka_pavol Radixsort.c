/*KNIZNICE*/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# include <time.h>

/*MAKRA*/
# define FILTER 2   /* Urcuje definiciu "slov", - 1 - ZACHOVANIE INTERPUNKCIE, - 2 - VYMAZANIE INTERPUNKCIE. */
# define TEST 800   /* Urcuje velkost nahodneho testovacieho retazca ! POZOR ! NESMIE BYT VATCSI AKO MAX_SKEN_ZAPIS! toto makro vyuzijete v volbe c.3 TESTING*/
# define MAX_SKEN_ZAPIS 10000  /* VELKOST POLA  */
# define MAX_SKEN 50  /* Dlzka slov */

/* PRE UCEL MERANIA CASU */
#define TICK(X) clock_t X = clock()
#define TOCK(X) printf("\n\t time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)

/* PRE UCEL MERANIA CASU DUPLIKACIOU VOLANIA */
#define TIMES 1 //urcuje kolko krat sa ma kazda funkcia duplicitne opakovat.
#define TIMES_WHOLE 1 //urcuje kolko krat sa ma case 2 na nacitanie zo suboru opakovat

/*------------------------------------------------- DEKLARACIE FUNKCII-------------------------------------------------------------*/

void vymaz_extra_whitespaces(char* retazec); //Sluzi na vymazanie extra medzier, ktore su prebytocne, zjednoduhci to celkovo prehladnost programu a debuging funkcionality pocitania medzier

int pole_org(char *pole_char,char **jednoroz); //Sluzi na rozdelenie vety ktora je zapisana v pole_char  na jednotlive slova, ktore su ulozene v dvroz a jednoroz

void radixSort(int n,  char **pole_sort, int k); //Sortovaci algorimus RADIX MSD, ktory je viacej optimalny pre dlhe stringy

static void VymenStringy( char **a,  char **b);  /* V Zime tu bola tato fukcionalita realizovana cez strcopy, to je fukcia ktora skopiruje kazdy bajt z jedneho miesta v pamäti do
                                                   druhého, ak chceme vymenit obsah, musíme to urobit 3-krat, ale ak vyuzijeme pointre, iba prehodime kam maju ukazovat */

int vyskyt(int space_count, char **jednoroz,char pole_vyskyt_slov[MAX_SKEN_ZAPIS][MAX_SKEN]); // Urcuje vyskyt slov

int vyskyt_vypis(int space_count, char *pole_char, int pocitadlo,char pole_vyskyt_slov[MAX_SKEN_ZAPIS][MAX_SKEN],char **jednoroz); //Vypisuje vyskyt slov

int switch_volba(void); //Pomocna funkcia pre rozhodovanie switchu

void export_novy_retazec(int space_pocet, char *pole_char, char *nazsub1, char *nazsub2,FILE *filePointer,char **jednoroz); //Urcuje co sa ma stat s zosortovanym polom

void nahr_interpunkcie(char *pole_char); //Ak si pouzivatel zvoli filter c2, interpunkcia bude pomocou tejto funkcie odfiltrovana

char *s_new_from_stdin(int beExact, size_t *len, size_t *size); //dynamicka alokacia vstupu, ktoreho velkost vopred nepozname

/*--------------------------------------------------------MAIN --------------------------------------------------------------------*/

int main()
{
    char pole_vyskyt_slov[MAX_SKEN_ZAPIS][MAX_SKEN], pole_char[MAX_SKEN_ZAPIS],data_citanie[MAX_SKEN_ZAPIS],nazsub1[261],nazsub2[261],*pole_abc_zoradene[MAX_SKEN_ZAPIS];
    int  i, pocitadlo = 0, space_count = 0, h = 0; // SPACE_COUNT : pocet medzier tzv pocitadlo vesmiru
    FILE *Subor_pointer ;

    int beExact = 0;  // NEPRAVDA
    size_t len=0, size=0;

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

        printf("\n\t Zadajte retazec, ktory sa zapise do suboru \n\n => ");
        fflush(stdin);
        fgets(pole_char,MAX_SKEN_ZAPIS+1,stdin);


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

        /* VKLADANIE VSTUPNEHO RETAZECA DO SUBORU */

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

        TICK(t_MSD_Radix_sort);

        for(i=0; i<TIMES; i++)
        {
            space_count = pole_org(pole_char,pole_abc_zoradene);
        }

        TOCK(t_MSD_Radix_sort);

        /* ASCII SORTING */

        TICK(t_RadixSort);

        for(i=0; i<TIMES; i++)
        {
            radixSort(space_count+1, pole_abc_zoradene,0);
        }

        TOCK(t_RadixSort);

        /* VYPISANIE UPRAVENEHO RETAZCA */

        printf("\t\n\n\n\n\t Po abecednom zoradeni kazdeho slova v subore string vyzera takto: \n\n\n");


        printf("-----------------------------------------------------------------------------------------------------------------\n\t  ");
        for (i = 0; i <= space_count; i++)
        {
            printf(" %s",pole_abc_zoradene[i]);
            pocitadlo++;
        }
        printf("\n-----------------------------------------------------------------------------------------------------------------\n  ");

        /* VYPISANIE VYSKYTU SLOV */

        vyskyt_vypis(space_count,pole_char,pocitadlo,pole_vyskyt_slov,pole_abc_zoradene);

        /* EXPORT UPRAVENEHO RETAZCA */

        export_novy_retazec(space_count, pole_char, nazsub1, nazsub1, Subor_pointer,pole_abc_zoradene);

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
        TICK(t_PROGRAM);
        for(h=0; h<TIMES_WHOLE; h++)
        {
            printf("\n\n\t V danom textovom subore je zapisane:\n\n\n") ;
            printf("-----------------------------------------------------------------------------------------------------------------\n\t");
            while( fgets ( pole_char, MAX_SKEN_ZAPIS, Subor_pointer) != NULL )
            {
                printf( " %s", pole_char );
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
                space_count = pole_org(pole_char,pole_abc_zoradene);
            }

            TOCK(t_pole_org);

            /* ASCII SORTING */

            TICK(t_MSD_Radix_sort);

            for(i=0; i<TIMES; i++)
            {
                radixSort(space_count+1, pole_abc_zoradene,0);
            }

            TOCK(t_MSD_Radix_sort);

            /* VYPISANIE UPRAVENEHO RETAZCA */

            printf("\n\n\t Po abecednom zoradeni kazdeho slova v subore string vyzera takto: \n\n\n");

            printf("-----------------------------------------------------------------------------------------------------------------\n\t");
            for (i = 0; i <= space_count; i++)
            {
                printf(" %s",pole_abc_zoradene[i]);
                pocitadlo++;
            }
            printf("\n-----------------------------------------------------------------------------------------------------------------\n");

            /* VYPISANIE VYSKYTU SLOV */

            vyskyt_vypis(space_count,pole_char, pocitadlo,pole_vyskyt_slov,pole_abc_zoradene);

        }
        TOCK(t_PROGRAM);

        /* EXPORT UPRAVENEHO RETAZCA */

        export_novy_retazec(space_count, pole_char,nazsub1,nazsub2,Subor_pointer,pole_abc_zoradene);

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
            space_count = pole_org(u,pole_abc_zoradene);
        }

        TOCK(t_pole_org);

        /* ASCII SORTING */

        TICK(t_MSD_Radix_sort);

        for(i=0; i<TIMES; i++)
        {
            radixSort(space_count+1, pole_abc_zoradene,0);
        }

        TOCK(t_MSD_Radix_sort);

        /* VYPISANIE UPRAVENEHO RETAZCA */

        printf("\t\n\t Po abecednom zoradeni kazdeho slova v subore string vyzera takto: \n\n\n");


        printf("-----------------------------------------------------------------------------------------------------------------\n\t  ");
        for (i = 0; i <= space_count ; i++)
        {
            printf("%s ",pole_abc_zoradene[i]);
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
                fputs(pole_abc_zoradene[i], Subor_pointer) ;
                fputs(" ", Subor_pointer) ;
            }
        }

        fclose(Subor_pointer) ;

        /* VYPISANIE VYSKYTU SLOV */

        vyskyt_vypis(space_count,u,pocitadlo,pole_vyskyt_slov,pole_abc_zoradene);

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
    free(pole_char);
}

/*------------------------------------VYMAZATEL EXTRA MEDZIER (WHITESPACES)----------------------------------------*/

void vymaz_extra_whitespaces(char* pole_char)
{
    int i, x;
    for(i=x=0; pole_char[i]; ++i)
        if(!isspace(pole_char[i]) || (i > 0 && !isspace(pole_char[i-1])))
            pole_char[x++] = pole_char[i];
    pole_char[x] = '\0';
}

/*------------------------------------------- ORGANIZACIA-POLE -----------------------------------------------------*/
int pole_org(char *pole_char,char **jednoroz)
{
    char pom_pole[MAX_SKEN_ZAPIS][MAX_SKEN]; // Pomocne 2D pole, s ktorym sa pracuje po riadkoch
    int i, j = 0, k, p = strlen(pole_char)-1, count_medzery =0;

    for (i = 0, j = 0, k = 0; j < p+1; j++) //PRECHADZA PO JEDNOTLIVYCH CHAR KYM NEPREJDE CELE POLE
    {
        if (isspace(pole_char[j]))
        {
            pom_pole[i][k] = '\0'; //RIADKY [i] PREDSTAVUJU SLOVA A STLPCE [k] PRESTAVUJU JEDNOTLIVE ZNAKY CHAR
            i++; //POSTUPUJE NA DRUHE SLOVO
            k = 0; //OBSAH PRE DALSIE SLOVO SA NASTAVI NA INDEX 0
            count_medzery ++;
        }
        else
        {
            pom_pole[i][k++] = pole_char[j]; //POSTUPUJE SA NA DALSI CHAR A ZAPISE SA DO SLOVA NA DANOM INDEXE

        }
    }

    if(FILTER == 1) //AK POUZIVATEL ZADA WHITESPACE ZA KONECNYM CHARAKTEROM V SUBORE ALEBO V SKENOVANOM RETAZCI, NACITA SA NADPOCETNA MEDZERA A VYPISE VYSKYT "NULL"
    {
        if (isspace(pole_char[p]))
        {
            count_medzery--;
        }

    }
    else if(FILTER == 2) //AK POUZIVATEL ZADA WHITESPACE/INTERPUNKCIU ZA KONECNYM CHARAKTEROM V SUBORE ALEBO V SKENOVANOM RETAZCI, NACITA SA NADPOCETNA MEDZERA A VYPISE VYSKYT "NULL"
    {
        if ((isspace(pole_char[p])) || (ispunct (pole_char[p])))
        {
            count_medzery--;
        }
    }
    for(i=0; i<count_medzery+1; i++)
    {
        jednoroz[i] = pom_pole[i];
    }
    return count_medzery;
}
/*-------------------------------------------SWAP STRINGS -----------------------------------------------------*/

void VymenStringy( char **a,  char **b)
{
    char *temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/*-------------------------------------------ASCII SORTOVANIE - RADIX SORT -----------------------------------------------------*/

void radixSort(int n,  char **pole_sort, int k)
{
    int i;
    int pocet[256];  /* počet retazcov s daným znakom na pozícii k*/
    int zaznam;                /* zaznam najviac vyskytujuceho charakteru */
    char **kyblicek[256]; /* bucket na znaky */
    char **vrchol[256];

    while(n > 1)
    {
        /* Pocitaj vyskyt kazdeho znaku */
        memset(pocet, 0, sizeof(int)*(256)); // Hodnota kazdeho elementu v poli pocet sa nastavi na 0

        for(i = 0; i < n; i++)
        {
            pocet[(unsigned char) pole_sort[i][k]]++;
        }

        /* Najdi najviac vyskytujuci-sa nenulovy znak */
        zaznam = 1;
        for(i = 2; i < 256; i++)
        {
            if(pocet[i] > pocet[zaznam])
            {
                zaznam = i;
            }
        }

        if(pocet[zaznam] < n)
        {

            /* Vytvor kyblicek a vrcholove pozicie */
            kyblicek[0] = vrchol[0] = pole_sort;
            for(i = 1; i < 256; i++)
            {
                vrchol[i] = kyblicek[i] = kyblicek[i-1] + pocet[i-1];
            }

            /* zorad podla k-th znaku */
            /* Je to podobne ako algoritmus Holandskej vlaky - problem s farbami */
            /* Zacneme od spodu az kym nebude vsetko zoradene */
            for(i = 0; i < 256; i++)
            {
                while(vrchol[i] < kyblicek[i] + pocet[i])
                {
                    if((unsigned char) vrchol[i][0][k] == i)
                    {
                        /* nechaj tak, pokracuj v kyblicku */
                        vrchol[i]++;
                    }
                    else
                    {
                        /* Vymen s vrcholom vrchol vhodneho bloku */
                        VymenStringy(vrchol[i], vrchol[(unsigned char) vrchol[i][0][k]]++);
                    }
                }
            }

            /* Zoradili sme vsetko */
            /* Rekurzia na ovsetko okrem 0 a zaznam */
            for(i = 1; i < 256; i++)
            {
                if(i != zaznam)
                {
                    radixSort(pocet[i], kyblicek[i], k+1);
                }
            }

            /* koncova rekurzia na zaznam */
            n = pocet[zaznam];
            pole_sort = kyblicek[zaznam];
            k = k+1;

        }
        else
        {
            printf("\n KONIEC");
            k = k+1;
        }
    }
}

/*-------------------------------------------------VYSKYT ----------------------------------------------------------*/
int vyskyt(int space_count, char **jednoroz,char vyskyt_slov[MAX_SKEN_ZAPIS][MAX_SKEN])
{

    int pocet = 0, i, j=0,k=0;

    for (i = 0; i <= space_count; i++) // I MUSI BYT VATCSIE AKO POCET MEDZIER, POTOM SA SKONCI - KED SA SKONCI BUDU POROVNANE VSETKY SLOVA
    {
        for (j = 0; j <= space_count; j++) // "j" MUSI BYT VATCSIE AKO POCET MEDZIER, POTOM SA SKONCI - KED SA SKONCI BUDE POROVNANE SLOVO NA INDEXE "i" ZO VSETKYMI SLOVAMI
        {
            if (i == j)  //AK SA ROVNAJU PREKOPIRUJKU SA NA "i" DVOJROZMERNEHO POLA DVROZVYS
            {
                strcpy(vyskyt_slov[k], jednoroz[i]);
                k++;
                pocet++; //POCET SA NAVYSI
                break;
            }
            else
            {
                if (strcmp(vyskyt_slov[j], jednoroz[i]) != 0) // AK SA NEROVNAJU TAK SA SLOVO ZAPISE A INDEX SA POSUNIE
                    continue;
                else
                    break;
            }
        }
    }
    return pocet; //POCET SA EXPORTUJE NA POUZITIE
}
/*------------------------------------------------VYSKYT TEXT -----------------------------------------------------*/
int vyskyt_vypis(int space_count, char *pole_char, int pocitadlo,char pole_vyskyt_slov[MAX_SKEN_ZAPIS][MAX_SKEN],char **jednoroz)
{
    int c = 0, pocet_slov, i, j = 0;
    pocet_slov = vyskyt(space_count,jednoroz,pole_vyskyt_slov);
    printf("\n\n\t Vyskyt jednotlivych slov v zadanom retazci: \n\n\n"); //VYPISUJE SA VYSKYT
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < pocet_slov; i++)
    {
        for (j = 0; j <= space_count; j++)
        {
            if (strcmp(pole_vyskyt_slov[i], jednoroz[j]) == 0)
                c++;
        }
        printf("\t  %s -> %d krat\n", pole_vyskyt_slov[i], c);
        c = 0;
    }
    printf("\n\t Celkovy pocet slov je %d \n",pocitadlo); //POCET SLOV
    if(pocitadlo>pocet_slov)
    {
        printf("\t Celkovy pocet slov bez duplicity je %d \n",pocet_slov); //POCET SLOV BEZ DUPLICITY
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

void export_novy_retazec(int space_pocet, char *pole_char, char *nazsub1, char *nazsub2,FILE *filePointer,char **jednoroz)
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
                fputs(jednoroz[i], filePointer) ;
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
                fputs(jednoroz[i], filePointer) ;
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

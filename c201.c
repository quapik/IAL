
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    L->First=NULL;
	L->Act=NULL;  
	
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
     tElemPtr elemPtr; //vytvoreni jednotlive polozky seznamu ktera buse slouzit jako pomocna elemptr
  while(L->First!=NULL) //dokud neni seznam prazdny
    {
        elemPtr=L->First;  //nacteni prvni polozky seznamu  
        L->First=elemPtr->ptr; //ukazatel na prvni polozku bude nyni ukazatel z predchoziho 
        free(elemPtr); //uvolneni polozky

    }  
    L->First=NULL;
	L->Act=NULL; 

}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
tElemPtr elemPtr=(tElemPtr*) malloc(sizeof(tElemPtr)); //vytvoreni noveho prvku

if(elemPtr==NULL)     //chyba pri alokace   
    {
    error();  
    }
elemPtr->data=val; //ulozeni hodnoty na misto v seznamu
elemPtr->ptr=L->First; //novy prvek ukazuje tam kam ukazoval prvni (ted je prnvi on)
L->First=elemPtr; //zacatek ukazuje na aktualne pridany prvek

}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L->Act=L->First; //aktivnim prvkem je proste prvni prvek

}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if(L->First==NULL) //pokud je seznam prazdny
    {
        error();
    }
    *val=L->First->data;  //ulozeni do pointeru hodnotu z prvniho prvku

}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/

tElemPtr elemPtr;
	if(L->First!=NULL) //pokud neni prazdny
    {
        if(L->First==L->First) //pokud je ruseny aktivni tak aktivnitu zrusime
        {
            L->=NULL; 
        }
    elemPtr=L->First;
    L->First=L->First->ptr;
    free(elemPtr);

    }

}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
tElemPtr elemPtr;
if(L->Act!=NULL) //pokud vubec mame aktivni prvek
{
    if(L->Act->ptr!=NULL) //ktery ma nejakeho nasledovnika
    {
            elemPtr=L->Act->ptr; //ukazatel na prvni prvek co bdueme rusit
            L->Act->ptr=elemPtr->ptr; //posunuti o jeden prvek za ruseny
            free(elemPtr); //uvolneni
    }

}


}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/

if(L->Act!=NULL) //pokud mame aktivni prvek
 {
    tElemPtr elemPtr=(tElemPtr*) malloc(sizeof(tElemPtr)); 
    if(elemPtr==NULL)     //chyba pri alokace   
        {
         error();  
        }
    elemPtr->data=val;
    elemPtr->ptr=L->Act->ptr; //nove vytvoreny ukrazuje tam, kde aktivni ukazuje dalsi prvek
    L->Act->ptr=elemPtr; //aktivni prvek ukazuje na nove vytvoreny
 } 

}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    if(L->Act==NULL) //pokud seznam neni aktivni
        {
            error();
        }
    else
        {
            *val=L->Acut->data;  //ulozeni do pointeru hodnotu aktivniho prvku
        }
        
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
     if(L->Act!=NULL)
     {
        L->Act->data=val; //zmena hodnoty aktivniho prvku
     }
	
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	
    if(L->Act!=NULL)
     {
        L->Act=L->Act->ptr; //Zmena aktivniho prvku na nasledujici
     }
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	if(L->Act!=NULL)
     {
        return 1; //aktivni
     }
     else
     {
         return 0; //neaktivni
     }
     
	
 
}

/* Konec c201.c */

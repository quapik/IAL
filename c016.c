
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	for(int i=0;i<HTSIZE;i++)
	{
		(*ptrht)[i]=NULL; //nastaveni vsech ukazatelu na NULL (defaultni hodnota)
	}

}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

 tHTItem* polozka;
 polozka=(*ptrht)[hashCode(key)]; //nalezeni indexu v poli (podle daneho klice)

 //pruchod seznamem synonym
	while(polozka!=NULL) //dokud mame co prohledavat
	{
		if(polozka->key==key)
		{
			return polozka; //pokud je nalezen dany prvek je vracen ten
		}
		else
		{
			polozka=polozka->ptrnext; //pokud nenalezen jdeme dal
		}

	}

	return NULL; //pokud while prosel cely a nenalezli jsme, return NULL

 
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

tHTItem* polozka=htSearch(ptrht, key); //nalezeni v HT podle klice

	if(polozka==NULL) //nebylo nic nalezeno, takze musime vytvorit
	{
		tHTItem* novapolozka=(tHTItem *) malloc(sizeof(tHTItem));
		if(novapolozka==NULL) 
		{
			return; //pokud selze malloc
		}
		else
		{
			novapolozka->data=data; //vlozeni dat a klice
			novapolozka->key=key;
			int hash = hashCode(key); //hasovaci klic

			novapolozka->ptrnext=(*ptrht)[hash]; //navazani polozky na prvni item
			(*ptrht)[hash]=novapolozka; //nazacatek

		}
	}
	else //bylo neco nalazeno, takze pouze aktualizace datove casti
	{
		polozka->data=data;
	}

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

tHTItem* polozka=htSearch(ptrht, key); //nalezeni v HT podle klice
	if(polozka!=NULL) //polozka je nalezena
	{
		return(&(polozka->data));
	}
	else //pokud ne returnujem NULL
	{
		return NULL;
	}
 
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	int hash=hashCode(key); //hasovaci klic
	tHTItem* polozka=(*ptrht)[hash]; //polozka na kterou odkazuje key
	if(polozka!=NULL) //je nalezeno co budeme rusit
	{
		tHTItem* pompolozka;
		if((polozka->key)==key) //je hned nalezeno
		{
			pompolozka=polozka;
			(*ptrht)[hash]=pompolozka->ptrnext;
			free(pompolozka);
		}
		else
		{
		 polozka=polozka->ptrnext; //dalsi polozka

		 while(polozka!=NULL) //dokud mame co prochazet
		 {
			 if(polozka->key==key) //nalezen
			 {
			pompolozka=polozka; //pomocne ulozeni, posun a vymazani
			polozka=polozka->ptrnext; 
			free(pompolozka);
			
			 }
			 else
			 {
				  polozka=polozka->ptrnext; //pokud nenalezen pouze posun dal
			 }
		 }
		}

	}

}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

 
 for(int i=0;i<HTSIZE;i++) //for pro prochazeni cele tabulky
 {
	tHTItem *polozka=(*ptrht)[i];
	
	while(polozka!=NULL) //dokud mame co mazat
	{
		tHTItem *pompolozka=polozka->ptrnext; //pomocna polozka pro zchoavnia pri vymazani
		free(polozka);
		polozka=pompolozka;

	}
	(*ptrht)[i]=NULL; //navrat do pocatescniho stavu


 }
}

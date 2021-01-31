
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/

	if (Ptr==NULL)
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
}

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK)
    printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {
		S->top++;
		S->a[S->top]=ptr;
	}
}

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;
}

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top==MAXSTACK)
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat.
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/
	

	 (*RootPtr)=NULL;
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/

	tBTNodePtr polozka; //pom pro vkladani listu
	tBTNodePtr pom; //pomocna pro prohazieni stromu
	if(*(RootPtr)==NULL) ///pokud je strom prazdny
	{
		polozka=(tBTNodePtr) malloc(sizeof(struct tBTNode));
		if(polozka==NULL) //malloc se nepovedl
		{
			return;
		}
		else
		{
			polozka->Cont=Content;
			polozka->RPtr=NULL; ///nikam jinam neukazuje (jediny prvek-list)
			polozka->LPtr=NULL;
			(*RootPtr)=polozka;
		}	
	}
	else
	{
		pom=(*RootPtr);
		bool prochazet=true;
		while(prochazet==true)
		{
			if((pom->Cont)>Content) //pokud je vkladany mensi nez aktualni - pracujeme s levym podstromem
				{
					if(pom->LPtr==NULL)//uz neni dalsi levy - muzeme vkladat
					{
					polozka=(tBTNodePtr) malloc(sizeof(struct tBTNode));	
					if(polozka==NULL) //malloc se nepovedl
						{
							return;
						}		
					polozka->Cont=Content;
					polozka->RPtr=NULL; ///nikam jinam neukazuje (posledni) prvek-list)
					polozka->LPtr=NULL;
					pom->LPtr=polozka;
					prochazet=false;
					}
					else //prochazime dal
					{
						pom=pom->LPtr;

					}

				}
				else
				{
					if((pom->Cont)<Content) //pokud je vkladany vetsi nez aktualni - pracujeme s pravym podstromem
					{


						if(pom->RPtr==NULL)//uz neni dalsi levy - muzeme vkladat
						{
						polozka=(tBTNodePtr) malloc(sizeof(struct tBTNode));	
						if(polozka==NULL) //malloc se nepovedl
							{
								return;
							}		
						polozka->Cont=Content;
						polozka->RPtr=NULL; ///nikam jinam neukazuje (posledni) prvek-list)
						polozka->LPtr=NULL;
						pom->RPtr=polozka;
						prochazet=false;
						}
						else //prochazime dal
						{
							pom=pom->RPtr;

					}

						
					}
					else 
					{return;}



				}

		
		
		}


	}
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/

	
	while(ptr!=NULL)
	{
		BTWorkOut(ptr); //zpracovani
		SPushP(Stack,ptr); //ulozeni ukazatele na zasobnik
		ptr=ptr->LPtr; //posun vice doleva
	}

}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/
	if(RootPtr==NULL)
	{
		return;
	}	
	else
	{
		tStackP s1;
		SInitP(&s1); //inicializace zasobniku
		Leftmost_Preorder(RootPtr,&s1); //ulozeni na zasbonik
		while(SEmptyP(&s1)==false) //dokud neni prazdny zasobnik
			{
				RootPtr=STopPopP(&s1); //pop vrcholu zasobbniku
				Leftmost_Preorder(RootPtr->RPtr, &s1); //prava cast stromu
			}


	}

}
/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku.
**/	
	 while(ptr!=NULL)
	 {
		//zde nic nezpracovavame
		 SPushP(Stack,ptr); 
		 ptr=ptr->LPtr;

	 }

}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/
		if(RootPtr==NULL)
		{
		return;
		}	
		else
		{
		tStackP s1;
		SInitP(&s1); //inicializace zasobniku
		Leftmost_Inorder(RootPtr,&s1); //leva vetev
			while(SEmptyP(&s1)==false)
			{
				RootPtr=STopPopP(&s1); 
				BTWorkOut(RootPtr); //zpracovani aktualniho lsitu (zmena od PreOrder)
				Leftmost_Inorder(RootPtr->RPtr,&s1); //pokracovani prava cast stromu
			}
		}	

}

/*                                 POSTORDER                                  */

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat.
**/

	while(ptr!=NULL) //hledame nejlevejsi (dokud neni nullpointer)
	{
		SPushP(StackP,ptr); //push na zasobnik ukazatelu
		SPushB(StackB,true); //push na zasobnik booleovskych hodnot
		ptr=ptr->LPtr;


	}
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut().
**/
	bool zleva;
	tStackP(s1); 
	SInitP(&s1);
	tStackB(sb1); 
	SInitB(&sb1);
	Leftmost_Postorder(RootPtr,&s1,&sb1);
	while(SEmptyP(&s1)==false) //dokud zasobniku pointeru neni prazdny
		{
		RootPtr=STopPopP(&s1);
		SPushP(&s1,RootPtr);
		zleva=STopPopB(&sb1);
		if(zleva) //prichazi zleva, pujde doprava
		{
		
			SPushB(&sb1, false);
			Leftmost_Postorder(RootPtr->RPtr,&s1,&sb1);
		}
		else //zprava, odsrtani se a zpracvuje uzel (jiz se vracime zpet)
		{
			STopPopP(&s1); //popnuti zasobniku
			BTWorkOut(RootPtr); //zpracovani

		}

		}

	
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujtS nerekurzivně s využitím zásobníku ukazatelů.
**/
		tStackP s1;
		SInitP(&s1);
	
	do{
		if((*RootPtr)==NULL) //vezme se uzel ze zasobniku
		{
			if(!SEmptyP(&s1))
			{
				(*RootPtr)=STopPopP(&s1);
			}
			
		}	
		else
		{
			if((*RootPtr)->RPtr!=NULL)
			{
				SPushP(&s1,(*RootPtr)->RPtr); //pravy jde do zasobniku
			}
		tBTNodePtr auxptr = (*RootPtr);
		(*RootPtr)=(*RootPtr)->LPtr; //jdeme do leva
		free(auxptr); //ruseni aktualniho

		}
	}	while(((*RootPtr)!=NULL)||(!SEmptyP(&s1)));
	
	(*RootPtr)=NULL;
		
}

/* konec c402.c */


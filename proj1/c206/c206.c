
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    /*nastaví všechny ukazatele na NULL*/
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    /*zkopíruje první prvek*/
    DLLElementPtr tmp = list->firstElement;
    DLLElementPtr pointer;
    /*zkontroluje, jestli první prvek není NULL*/
    while (tmp != NULL) {
        /*poté vždy uloží ukazatel na další prvek*/
        pointer = tmp->nextElement;
        /*první prvek nastaví na NULL a uvolní alokovanou paměť*/
        tmp = NULL;
        free(tmp);
        /*přesun na další prvek*/
        tmp = pointer;
    }
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    DLLElementPtr tmp = list->firstElement;
    /*alokujeme místo pro nový prvek*/
    DLLElementPtr pointer = malloc(sizeof(struct DLLElement));
    if (pointer == NULL) {
        DLL_Error();
        return;
    }
    /*přidáme prvku hodnotu*/
    pointer->data = data;
    /*vkládáme prvek na začátek seznamu => další prvek je původní první*/
    pointer->nextElement = tmp;
    /*jedná se o první prvek seznamu, předchozí je NULL*/
    pointer->previousElement = NULL;
    /*pokud byl seznam prázdný, bude nový prvek zároveň poslední prvek*/
    if (list->firstElement == NULL) {
        list->lastElement = pointer;
    }
    /*pokud seznam nebyl prázdný, bude nový prvek před původním prvním*/
    else {
        tmp->previousElement = pointer;
    }
    /*nastavíme nový prvek jako ukazatel na začátek seznamu*/
    list->firstElement = pointer;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    DLLElementPtr pointer = malloc(sizeof(struct DLLElement));
    if (pointer == NULL) {
        DLL_Error();
        return;
    }
    DLLElementPtr tmp = list->lastElement;
    pointer->data = data;
    pointer->nextElement = NULL;
    pointer->previousElement = tmp;
    /*pokud není seznam prázdný, bude prvek za původně posledním prvkem nově přidaný prvek*/
    if (tmp != NULL) {
        tmp->nextElement = pointer;
    }
    /*pokud se jeznam prázdný, bude nový prvek zároveň první*/
    else {
        list->firstElement = pointer;
    }
    /*nastavím ukazatel na poslední prvek na nový prvek*/
    list->lastElement = pointer;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    list->activeElement = list->lastElement;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    if (list->firstElement == NULL) {
        DLL_Error();
        return;
    }
    *dataPtr = list->firstElement->data;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    if (list->firstElement == NULL) {
        DLL_Error();
        return;
    }
    *dataPtr = list->lastElement->data;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    /*Pokud byl seznam list prázdný, nic se neděje.*/
    if (list->firstElement == NULL) {
        return;
    }
    /*Pokud byl první prvek aktivní, aktivita se ztrácí.*/
    if (list->firstElement == list->activeElement) {
        list->activeElement = NULL;
    }
    DLLElementPtr tmp = list->firstElement;
    /*pokud měl seznam jeden prvek, zrušíme ukazatel na poslední prvek*/
    if (tmp == list->lastElement) {
        list->lastElement = NULL;
    }
    /*pokud je v seznamu více prvků, bude ukazatel 2. prvku na předchozí prvek NULL*/
    else {
        tmp->nextElement->previousElement = NULL;
    }
    /*2. prvek bude nyní 1.*/
    list->firstElement = list->firstElement->nextElement;
    free(tmp);
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    /*Pokud byl seznam list prázdný, nic se neděje.*/
    if (list->firstElement == NULL) {
        return;
    }
    /*Pokud byl poslední prvek aktivní, aktivita se ztrácí.*/
    if (list->activeElement == list->lastElement) {
        list->activeElement = NULL;
    }
    DLLElementPtr tmp = list->lastElement;
    /*pokud měl seznam jeden prvek, zrušíme ukazatel na první prvek*/
    if (list->firstElement == list->lastElement) {
        list->firstElement = NULL;
    }
    /*pokud měl seznam více prvků, bude ukazatel předposledního prvku na další NULL*/
    else {
        tmp->previousElement->nextElement = NULL;
    }
    /*dříve předposlední prvek bude nyní poslední*/
    list->lastElement = list->lastElement->previousElement;
    free(tmp);
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    /*Pokud je seznam list neaktivní nebo pokud je aktivní prvek posledním prvkem seznamu, nic se neděje.*/
    if (list->activeElement == NULL || list->activeElement == list->lastElement) {
        return;
    }
    DLLElementPtr tmp = list->activeElement->nextElement;
    /*active prvek bude nyní přeskočí jeden prvek a bude ukazovat až na další v řadě (ACT -> prvek1 -> prvek2 bude nyní ACT -> prvek2) */
    list->activeElement->nextElement = tmp->nextElement;
    /*pokud byl aktivní prvek předposlední, bude nyní zároveň poslední*/
    if (list->lastElement == tmp) {
        list->lastElement = list->activeElement;
    }
    /*pokud aktivní prvek nebyl předposlední, nastavíme ukazatel na předchozí prvek prvku2 na aktivní prvek*/
    else {
        tmp->nextElement->previousElement = list->activeElement;
    }
    free(tmp);
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    /*Funguje stejně jako DeleteAfter, akorát místo dalšího prvku bere předchozí*/
    if (list->activeElement == NULL || list->activeElement == list->firstElement) {
        return;
    }
    DLLElementPtr tmp = list->activeElement->previousElement;
    list->activeElement->previousElement = tmp->previousElement;
    if (tmp == list->firstElement) {
        list->firstElement = list->activeElement;
    }
    else {
        tmp->previousElement->nextElement = list->activeElement;
    }
    free(tmp);
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    /*Pokud nebyl seznam list aktivní, nic se neděje.*/
    if (list->activeElement == NULL) {
        return;
    }
    /*alokace paměti pro nový prvek*/
    DLLElementPtr pointer = malloc(sizeof(struct DLLElement));
    if (pointer == NULL) {
        DLL_Error();
        return;
    }
    /*přidáme novému prvku hodnotu a ukazatele na patřičné prvky seznamu*/
    pointer->data = data;
    pointer->previousElement = list->activeElement;
    pointer->nextElement = list->activeElement->nextElement;
    list->activeElement->nextElement = pointer;
    /*pokud byl aktivní prvek zároveň poslední prvek, bude nyní poslední prvek ten, který jsme právě přidali*/
    if (list->activeElement == list->lastElement) {
        list->lastElement = pointer;
    }
    /*jestliže nový prvek není poslední v seznamu, nastavíme ukazatel na předchozí prvek dalšího prvku na nově přidaný prvek*/
    if (pointer->nextElement != NULL) {
        pointer->nextElement->previousElement = pointer;
    }
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    /*funguje opět stejně jako InsertAfter, akorát místo dalšího prvku bere vždy předchozí*/
    if (list->activeElement == NULL) {
        return;
    }
    DLLElementPtr pointer = malloc(sizeof(struct DLLElement));
    if (pointer == NULL) {
        DLL_Error();
        return;
    }
    pointer->data = data;
    pointer->previousElement = list->activeElement->previousElement;
    pointer->nextElement = list->activeElement;
    list->activeElement->previousElement = pointer;
    if (list->activeElement == list->firstElement) {
        list->firstElement = pointer;
    }
    if (pointer->previousElement != NULL) {
        pointer->previousElement->nextElement = pointer;
    }
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    if (list->activeElement == NULL) {
        DLL_Error();
        return;
    }
    *dataPtr = list->activeElement->data;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    if (list->activeElement == NULL) {
        return;
    }
    list->activeElement->data = data;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    if (list->activeElement == NULL) {
        return;
    }
    list->activeElement = list->activeElement->nextElement;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if (list->activeElement == NULL) {
        return;
    }
    list->activeElement = list->activeElement->previousElement;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return (list->activeElement == NULL ? 0 : 1);
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/* Konec c206.c */

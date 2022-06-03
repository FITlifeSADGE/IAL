
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char c;
    /*znak z vrcholu zásobníku uložíme do c, poté jej ze zásobníku odstraníme*/
    if (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &c);
        Stack_Pop(stack);
    }
    /*cyklus, který prochází prvky zásobníku, dokud nenarazí na levou závorku*/
    while (c != '(') {
        /*dokud není zásobník prázdný*/
        if (Stack_IsEmpty(stack)) {
            break;
        }
        /*do výsledného výrazu vložíme znak z vrcholu zásobníku*/
        postfixExpression[*(postfixExpressionLength)] = c;
        (*postfixExpressionLength)++;
        /*do c opět uložíme znak z vrcholu zásobníku a poté ho ze zásobníku odstaníme*/
        Stack_Top(stack, &c);
        Stack_Pop(stack);
    };
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char top;
    if (!(Stack_IsEmpty(stack))) {
        Stack_Top(stack, &top);
    }
    /*:zásobník je prázdný, nebo je na vrcholku zásobníku levá závorka, nebo je na vrcholku zásobníku operátor s nižší prioritou*/
    if ((Stack_IsEmpty(stack)) || (top == '(') || ((top == '+' || top == '-') && (c == '*' || c == '/'))) {
        Stack_Push(stack, c);
        return;
    }
    /*odstraníme prvek z vrcholku zásobníku*/
    Stack_Pop(stack);
    /*přidáváme znaky z vrcholu zásobníku na konec výsledného postfixového výrazu*/
    postfixExpression[*(postfixExpressionLength)] = top;
    (*postfixExpressionLength)++;
    //Stack_Push(stack, c); nefunguje na výraz bez závorek, zkusit fix
    /*jestliže znak nesplňuje ani jednu z podmínek, například výraz a+b*c-d, u Stack_Push nastane problém, když ve výrazu máme abc*, protože Stack_Push
    by přidal - na zásobník, který by nyní byl - +, a poté by přidal d do finálního výrazu. My potřebujeme do finálního výrazu přidat -, tudíž použijeme
    rekurzivně funkci doOperation, čímž se problém snad vyřešil*/
    doOperation(stack, c, postfixExpression, postfixExpressionLength);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    /*alokace pro strukturu*/
    Stack *stack = malloc(sizeof(Stack));
    /*kontrola alokace*/
    if (stack == NULL) {
        return NULL;
    }
    /*alokace pro výsledný výraz*/
    char *Expression = malloc(MAX_LEN * sizeof(char));
    /*kontrola alokace*/
    if (Expression == NULL) {
        free(stack);
        return NULL;
    }
    /*provedeme inicializaci*/
    Stack_Init(stack);
    char c = *infixExpression;
    char top;
    unsigned int pos = 0;
    while (c != '\0') {
        /*Je-li zpracovávanou položkou operand, přidáme ho na konec vznikajícího výstupního řetězce.
        *operand = Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
        *a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
        */
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            Expression[pos++] = c;
        }
        /*Je-li zpracovávanou položkou levá závorka, vložíme ji na vrchol zásobníku.*/
        if (c == '(') {
            Stack_Push(stack, c);
        }
        /*Je-li zpracovávanou položkou pravá závorka, odebíráme z vrcholu položky a 
        *dáváme je na konec výstupního řetězce až narazíš na levou závorku.
        */
        if (c == ')') {
            untilLeftPar(stack, Expression, &pos);
        }
        /*Je-li zpracovávanou položkou omezovač ‘=’, pak postupně odstraňujeme prvky z 
        *vrcholu zásobníku a přidáváme je na konec řetězce, až zásobník zcela vyprázdníme a 
        *na konec přidáme rovnítko.
        */
        if (c == '=') {
            while (!(Stack_IsEmpty(stack))) {
                Stack_Top(stack, &top);
                Stack_Pop(stack);
                Expression[pos++] = top;
            }
            Expression[pos++] = '=';
            break;
        }
        /*pro operátory +, -, *, / použijeme funkci doOperation*/
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            doOperation(stack, c, Expression, &pos);
        }
        /*přesun na další znak infixovaného výrazu*/
        infixExpression++;
        c = *(infixExpression);
    }
    /*ukončení řetězce*/
    Expression[pos] = '\0';
    /*uvolnění paměti alokované pro strukturu*/
    free(stack);
    return Expression;
    //solved = FALSE; /* V případě řešení smažte tento řádek! */
    //return NULL; /* V případě řešení můžete smazat tento řádek. */
}

/* Konec c204.c */

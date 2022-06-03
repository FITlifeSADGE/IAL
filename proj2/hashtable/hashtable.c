/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  int i = 0;
  while(i < HT_SIZE) { //nastaví všechny položky v tabulce na NULL
    (*table)[i] = NULL;
    i++;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int i = get_hash(key);
  ht_item_t *tmp = ((*table)[i]); //uložíme první položku z místa v tabulce
  while (tmp != NULL) { //projdeme všechyn prvky na tomto místě, dokud nenarazíme na konec, nebo nenajdeme hledaný prvek
    if (tmp->key == key) {
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  ht_item_t *tmp = ht_search(table, key); //pokud daný prvek existuje, skočí na konec funkce a změní hodnotu
  if (!tmp) {  //pokud hledaný prvek neexistuje
    int i = get_hash(key);  
    tmp = malloc(sizeof(ht_item_t));
    if (tmp == NULL) {
      return;
    }
    tmp->key = key; //uložíme novému prvku hodnoty
    tmp->value = value;
    tmp->next = (*table)[i]; //první prvek indexu se přesune na druhé místo
    (*table)[i] = tmp; //vložíme nový prvek na první místo indexu v tabulce
    return;
  }
  tmp->value = value;

}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *tmp;
  if ((tmp = ht_search(table, key))) { //pokud najdeme hledaný prvek
    return (&(tmp->value)); //vrátíme ukazatel na jeho hodnotu
  }
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int i = get_hash(key);
  ht_item_t *tmp = (*table)[i]; //uložíme první položku z indexu tabulky
  ht_item_t *prev;
  if (((*table)[i]) != NULL) { //pokud jsme položku našli
    while (tmp) { //procházíme všechny prvky na indexu tabulky
      if (strcmp(tmp->key, key) == 0) { //pokud jsme našli hledaný prvek
        if (!prev) { //prvek byl jediná položka na indexu
          (*table)[i] = NULL;
        }
        else { //na indexu bylo více prvků
          prev->next = tmp->next;
        }
        free(tmp);
        return;
      }
      prev = tmp;
      tmp = tmp->next;
    }
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  ht_item_t *tmp;
  ht_item_t *prev;
  int i = 0;
  for(; i < HT_SIZE; i++) { //pro všechyn indexy tabulky
    tmp = (*table)[i];
    while (tmp) { //projdeme všechny prvky na indexu a uvolníme alokovanou paměť
      prev = tmp;
      tmp = tmp->next;
      free(prev);
    }
    (*table)[i] = NULL; //nakonec nastavíme ukazatele na index tabulky na NULL
  }
}

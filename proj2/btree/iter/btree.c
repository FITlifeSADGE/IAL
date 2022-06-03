/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *tmp = tree; //začátek stromu
  while (tmp != NULL) { //posouváme se ve stromě, dokud nenarazíme na konec, nebo na prvek, který hledáme
    if (key < tmp->key) {
      tmp = tmp->left;
      continue;
    }
    if (key > tmp->key) {
      tmp = tmp->right;
      continue;
    }
    if (key == tmp->key) {
      *value = tmp->value;
      return true;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *tmp = *tree;
  bst_node_t *prev;
  while (tmp != NULL) { //najdeme hledaný prvek a změníme jeho hodnotu
    if (key < tmp->key) {
      tmp = tmp->left;
      continue;
    }
    if (key > tmp->key) {
      tmp = tmp->right;
      continue;
    }
    if (key == tmp->key) {
      tmp->value = value;
      return;
    }
  }
  tmp = *tree; //opět začátek stromu
  bst_node_t* new = malloc(sizeof(bst_node_t));
  if (!new) {
    return;
  }
  new->key = key; //vložíme novému prvky hodnoty
  new->value = value;
  new->left = NULL;
  new->right = NULL;
  if (tmp == NULL) { //pro prázdný strom
    *tree = new;
    return;
  }
  while (tmp != NULL) { //najdeme správné místo pro nový prvek, uložíme rodičovský uzel
    if (new->key < tmp->key) {
      prev = tmp;
      tmp = tmp->left;
      continue;
    }
    if (new->key > tmp->key) {
      prev = tmp;
      tmp = tmp->right;
      continue;
    }
  }
  if (new->key < prev->key) { //podle rodičovského uzlu přidáme nový uzel
    new->left = prev->left;
    prev->left = new;
  }
  if (new->key > prev->key) {
    new->right = prev->right;
    prev->right = new;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tmp = *tree;
  bst_node_t *prev;
  while (tmp->right != NULL) { //dostaneme se k nejvíce pravému prvku a uložíme rodičovský uzel
    prev = tmp;
    tmp = tmp->right;
  }
  target->key = tmp->key; //zkopírujeme hodnoty nejvíce pravého prvku
  target->value = tmp->value;
  if (tmp->left != NULL) { //pokud měl potomky, navážeme je na rodičkovský uzel
    prev->right = tmp->left;
  }
  else {
    prev->right = NULL;
  }
  //tmp = NULL;
  free(tmp);

}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *tmp = *tree;
  bst_node_t *prev = *tree;
  bst_node_t *replace = malloc(sizeof(bst_node_t));
  while (tmp != NULL) {
    if (key < tmp->key) {
      prev = tmp;
      tmp = tmp->left;
      continue;
    }
    if (key > tmp->key) {
      prev = tmp;
      tmp = tmp->right;
      continue;
    }
    if (key == tmp->key) {
      break;
    }
  }
  if (!tmp) {
    free(replace);
    return;
  }
  //pro 0 podstromů
  if ((!(tmp->left)) && (!(tmp->right))) {
    if (prev->left == tmp) {
      prev->left = NULL;
    }
    if (prev->right == tmp) {
      prev->right = NULL;
    }
    free(replace);
    free(tmp);
    return;
  }
  //
  //pro jeden podstrom
  if (tmp->left && !(tmp->right)) {
    prev->left = NULL;
    prev->left = tmp->left;
    //tmp = NULL;
    free(replace);
    free(tmp);
    return;
  }
  if (tmp->right && !(tmp->left)) {
    prev->right = NULL;
    prev->right = tmp->right;
    //tmp = NULL;
    free(replace);
    free(tmp);
    return;
  }
  //
  //pro 2 podstromy
  if (tmp->left && tmp->right) {
    bst_replace_by_rightmost(replace, &tmp->left);
    tmp->key = replace->key;
    tmp->value = replace->value;
    free(replace);
  }
  //
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t Stack;
    stack_bst_init(&Stack); //inicializace zásobníku
    bst_node_t *tmp;
    do {
      if ((*tree) == NULL) {
        if (!stack_bst_empty(&Stack)) {
          (*tree) = stack_bst_top(&Stack);
          stack_bst_pop(&Stack);
        }
      }
      else {
        if ((*tree)->right != NULL) {
          stack_bst_push(&Stack, (*tree)->right);
        }
        tmp = (*tree);
        (*tree) = (*tree)->left;
        free(tmp);
      }
    } while ((*tree != NULL) || !(stack_bst_empty(&Stack)));
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *tmp = tree;
  while (tmp != NULL) {
    stack_bst_push(to_visit, tmp);
    bst_print_node(tmp);
    tmp = tmp->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t Stack;
  stack_bst_init(&Stack); //inicializace zásobníku
  bst_node_t *tmp = tree;
  bst_leftmost_preorder(tmp, &Stack);
  while (!(stack_bst_empty(&Stack))) {
    tmp = stack_bst_top(&Stack);
    stack_bst_pop(&Stack);
    bst_leftmost_preorder(tmp->right, &Stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *tmp = tree;
  while (tmp != NULL) {
    stack_bst_push(to_visit, tmp);
    tmp = tmp->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t Stack;
  stack_bst_init(&Stack); //inicializace zásobníku
  bst_node_t *tmp = tree;
  bst_leftmost_inorder(tmp, &Stack);
  while (!(stack_bst_empty(&Stack))) {
    tmp = stack_bst_top(&Stack);
    stack_bst_pop(&Stack);
    bst_print_node(tmp);
    bst_leftmost_inorder(tmp->right, &Stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  bst_node_t *tmp = tree;
  while (tmp != NULL) {
    stack_bst_push(to_visit, tmp);
    stack_bool_push(first_visit, 1);
    tmp = tmp->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t Stack_node;
  stack_bst_init(&Stack_node); //inicializace zásobníku
  stack_bool_t Stack_visit;
  stack_bool_init(&Stack_visit);
  bst_node_t *tmp = tree;
  bool fromLeft;
  bst_leftmost_postorder(tmp, &Stack_node, &Stack_visit);
  while (!(stack_bst_empty(&Stack_node))) {
    tmp = stack_bst_top(&Stack_node);
    fromLeft = stack_bool_top(&Stack_visit);
    stack_bool_pop(&Stack_visit);
    if (fromLeft) {
      stack_bool_push(&Stack_visit, false);
      bst_leftmost_postorder(tmp->right, &Stack_node, &Stack_visit);
    }
    else {
      stack_bst_pop(&Stack_node);
      bst_print_node(tmp);
    }
  }
}

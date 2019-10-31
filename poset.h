/* Drugie zadanie z Języków i narzędzi programowania I 19/20
 * 2019-10-31
 * Adam Rozenek i Andrzej Małek
 */

#ifndef POSET_H
#define POSET_H


#ifdef NDEBUG
#ifdef __cplusplus
#include <iostream>
#endif
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
namespace jnp1 {
    extern "C" {
#endif

        /*      Tworzy nowy poset i zwraca jego identyfikator.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Identyfikator nowego posetu.
         */
        unsigned long poset_new(void);


        /*      Jeżeli istnieje poset o identyfikatorze id, usuwa go, a w przeciwnym
         *      przypadku nic nie robi.
         *
         *  Parametry :
         *      id - Identyfikator posetu do usunięcia.
         *  Return :
         *      void
         */
        void poset_delete(unsigned long id);


        /*      Jeżeli istnieje poset o identyfikatorze id, to wynikiem jest liczba jego
         *      elementów.
         *
         *  Parametry :
         *      id - Identyfikator poszukiwanego posetu.
         *  Return :
         *      Liczba elementów posetu o podanym id jeżeli istnieje, lub 0 wpp.
         */
        size_t poset_size(unsigned long id);


        /*      Jeżeli istnieje poset o identyfikatorze id i element value nie należy do
         *      tego zbioru, to dodaje element do zbioru, a w przeciwnym przypadku nic nie
         *      robi. Nowy element nie jest w relacji z żadnym elementem.
         *
         *  Parametry :
         *      id    - Identyfikator poszukiwanego posetu.
         *      value - Element do wstawienia do posetu.
         *  Return :
         *      Wartość true, jeżeli udało się wstawić element, lub false, w przeciwnym przypadku.
         */
        bool poset_insert(unsigned long id, char const *value);


        /*      Jeżeli istnieje poset o identyfikatorze id i element value należy do tego
         *      zbioru, to usuwa element ze zbioru oraz usuwa wszystkie relacje tego
         *      elementu, a w przeciwnym przypadku nic nie robi.
         *
         *  Parametry :
         *      id    - Identyfikator poszukiwanego posetu.
         *      value - Element do usunięcia z posetu.
         *  Return :
         *      Wartość true, jeżeli element udało się usunąć, lub false w przeciwnym przypadku.
         */
        bool poset_remove(unsigned long id, char const *value);


        /*      Jeżeli istnieje poset o identyfikatorze id oraz elementy value1 i value2
         *      należą do tego zbioru i nie są w relacji, to rozszerza relację w taki
         *      sposób, aby element value1 poprzedzał element value2 (domyka relację
         *      przechodnio), a w przeciwnym przypadku nic nie robi.
         *
         *  Parametry :
         *      id     - Identyfikator poszukiwanego posetu.
         *      value1 - Element poprzedzający element value2.
         *      value2 - Element za elementem value1.
         *  Return :
         *      Wartość true, jeżeli udało się poszerzyć relację, lub false w przeciwnym przypadku.
         */
        bool poset_add(unsigned long id, char const *value1, char const *value2);


        /*      Jeżeli istnieje poset o identyfikatorze id, elementy value1 i value2
         *      należą do tego zbioru, element value1 poprzedza element value2 oraz
         *      usunięcie relacji między elementami value1 i value2 nie zaburzy warunków
         *      bycia częściowym porządkiem, to usuwa relację między tymi elementami,
         *      a w przeciwnym przypadku nic nie robi.
         *
         *  Parametry :
         *      id     - Identyfikator poszukiwanego posetu.
         *      value1 - Pierwszy z elementów.
         *      value2 - Drugi z elementów
         *  Return :
         *      Wartość true, jeżeli zaszły zmiany w relacjach posetu, lub false w przeciwnym przypadku.
         */
        bool poset_del(unsigned long id, char const *value1, char const *value2);


        /*      Funkcja sprawdzająca, czy w danym posecie jeden element poprzedza drugi.
         *
         *  Parametry :
         *      id     - Identyfikator poszukiwanego posetu.
         *      value1 - Pierwszy z elementów.
         *      value2 - Drugi z elementów
         *  Return :
         *      Wartość true, jeżeli stwierdzono, że element value1 poprzedza element value2, lub false w przeciwnym
         *      przypadku.
         */
        bool poset_test(unsigned long id, char const *value1, char const *value2);


        /*      Jeżeli istnieje poset o identyfikatorze id, usuwa wszystkie jego elementy
         *      oraz relacje między nimi, a w przeciwnym przypadku nic nie robi.
         *
         *  Parametry :
         *      id - Identyfikator poszukiwanego posetu.
         *  Return :
         *      void
         */
        void poset_clear(unsigned long id);

#ifdef __cplusplus
    }
}
#endif


#endif /* POSET_H */

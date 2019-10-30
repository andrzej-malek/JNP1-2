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

        /*      Funkcja tworząca nowy poset.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Identyfikator nowego posetu.
         */
        unsigned long poset_new(void);

        /*      Funkcja usuwająca poset o podanym id jeżeli istnieje, lub nic nie robiąca w przeciwnym przypadku.
         *
         *  Parametry :
         *      id - Identyfikator posetu do usunięcia.
         *  Return :
         *      void
         */
        void poset_delete(unsigned long id);

        /*      Funkcja zwracająca liczbę elementów posetu o podanym identyfikatorze.
         *
         *  Parametry :
         *      id - Identyfikator poszukiwanego posetu.
         *  Return :
         *      Liczba elementów posetu o podanym id jeżeli istnieje, lub 0 wpp.
         */
        size_t poset_size(unsigned long id);

        /*      Funkcja wstawiająca nowy element do posetu.
         *
         *  Parametry :
         *      id    - Identyfikator poszukiwanego posetu.
         *      value - Element do wstawienia do posetu.
         *  Return :
         *      Wartość true, jeżeli udało się wstawić element, lub false, w przeciwnym przypadku.
         */
        bool poset_insert(unsigned long id, char const *value);

        /*      Funkcja usuwająca element z posetu wraz z jego relacjami.
         *
         *  Parametry :
         *      id    - Identyfikator poszukiwanego posetu.
         *      value - Element do usunięcia z posetu.
         *  Return :
         *      Wartość true, jeżeli element udało się usunąć, lub false w przeciwnym przypadku.
         */
        bool poset_remove(unsigned long id, char const *value);

        /*      Funkcja wstawiająca w poset relację między podanymi elementami.
         *
         *  Parametry :
         *      id     - Identyfikator poszukiwanego posetu.
         *      value1 - Element poprzedzający element value2.
         *      value2 - Element za elementem value1.
         *  Return :
         *      Wartość true, jeżeli udało się poszerzyć relację, lub false w przeciwnym przypadku.
         */
        bool poset_add(unsigned long id, char const *value1, char const *value2);

        /*      Funkcja usuwająca relację między podanymi elementami.
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

        /*      Funkcja usuwająca poset.
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

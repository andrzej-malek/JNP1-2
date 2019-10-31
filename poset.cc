/* Drugie zadanie z Języków i narzędzi programowania I 19/20
 * 2019-10-31
 * Adam Rozenek i Andrzej Małek
 */
#include "poset.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif



namespace jnp1 {
    // Typ identyfikatora elementów posetu.
    using id_graph = unsigned int;
    // Typ reprezentujący poset w postaci grafu skierowanego. Dla każdego elementu pod adresem jego identyfikatora
    // znajduje się set z elementami, które poprzedza bezpośrednio w grafie (lub które go poprzedzają dla
    // transgraph_map).
    using graph = std::unordered_map<id_graph, std::unordered_set<id_graph>>;
    // Typ słownika mapującego elementy posetu na ich identyfikatory.
    using dictionary = std::unordered_map<std::string, id_graph>;
    // Typ identyfikatora posetu.
    using id_poset = unsigned long;

    namespace {
        /*      Funkcja zwracajaca referencję do globalnej mapy id_poset -> dictionary, przypisującej każdemu
         *      id posetowemu słownik wierzchołków w grafie.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Mapa: id_poset -> (string -> id_graph)
         */
        std::unordered_map<id_poset, dictionary> &dictionary_map() {
            static std::unordered_map<id_poset, dictionary> *value = new std::unordered_map<id_poset, dictionary>;
            return *value;
        }


        /*      Funkcja zwracająca referencję do globalnej mapy id_poset -> id_graph, przypisującej każdemu
         *      posetowi nowy wolny identyfikator w grafie.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Mapa: id_poset -> (wolny) id_graf
         */
        std::unordered_map<id_poset, id_graph> &next_id_graph() {
            static std::unordered_map<id_poset, id_graph> *value = new std::unordered_map<id_poset, id_graph>;
            return *value;
        }


        /*      Funkcja zwracająca referencję do globalnej mapy id_poset -> graph, przypisujący każdemu
         *      posetowi graf reprezentujący go.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Mapa: id_poset -> graph
         */
        std::unordered_map<id_poset, graph> &graph_map() {
            static std::unordered_map<id_poset, graph> *value = new std::unordered_map<id_poset, graph>;
            return *value;
        }


        /*      Funkcja zwracająca referencję do globalnej mapy id_poset -> graph, przypisujący każdemu
         *      posetowi transponowany graf jego reprezentacji.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Mapa: id_poset -> graph (transponowany)
         */
        std::unordered_map<id_poset, graph> &transgraph_map() {
            static std::unordered_map<id_poset, graph> *value = new std::unordered_map<id_poset, graph>;
            return *value;
        }


        /*      Funkcja przydzielająca nowe numery id dla dodawanych posetów.
         *
         *  Parametry :
         *      void
         *  Return :
         *      Wolny id dla nowego posetu
         */
        id_poset new_poset_id() {
            static id_poset value = 0;
            return value++;
        }


        /*      Funkcja badająca czy element value1 poprzedza element value2 w posecie o identyfikatorze id. Zakłada,
         *      że taki poset istnieje, i że elementy value1 i value2 do niego należą. Nie zakłada, że value1 nie jest
         *      równe value2 (zwraca wartość true w takim przypadku).
         *
         *  Parametry :
         *      id     : Identyfikator badanego posetu.
         *      value1 : Element potencjalnie poprzedzający.
         *      value2 : Element potencjalnie poprzedzany.
         *  Return :
         *      Wartość true, jeżeli element value1 poprzedza element value2, lub false w przeciwnym przypadku.
         */
        bool poset_test_main(id_poset id, std::string const &value1, std::string const &value2) {
            if (!value1.compare(value2)) {
                return true;
            }
            id_graph node1 = dictionary_map()[id][value1];
            id_graph node2 = dictionary_map()[id][value2];
            if (graph_map()[id][node1].find(node2) == graph_map()[id][node1].end()) {
                return false;
            }
            return true;
        }


        /*      Funkcja weryfikująca poprawność argumentów funkcji o formacie func_name(id_poset id, char const *value1,
         *      char const *value2) i wypisująca stosowne informacje debuggowe.
         *  Parametry :
         *      func_name : string zawierający nazwę debugowanej funkcji.
         *      id        : Argument id debugowanej funkcji.
         *      value1    : Argument value1 debugowanej funkcji.
         *      value2    : Argument value2 debugowanej funkcji.
         *  Return :
         *      Wartość true, jeżeli argumenty są poprawne, lub false, gdy poset o danym id nie istnieje, któryś z
         *      argumentów value1, value2 jest NULL lub któryś z elementów value1, value2 nie należy do zadanego posetu.
         */
        bool verify_three_arg(std::string const &func_name, id_poset id, char const *value1, char const *value2) {
            if (value1 == NULL) {
                if (debug) {
                    if (value2 == NULL) {
                        std::cerr << func_name + "(" + std::to_string(id) + ", \"NULL\", \"NULL\")\n";
                    } else {
                        std::string str_value2(value2);
                        std::cerr << func_name + "(" + std::to_string(id) + ", \"NULL\", \"" + str_value2 + "\")\n";
                    }
                    if (dictionary_map().find(id) == dictionary_map().end()) {
                        std::cerr << func_name + ": poset " + std::to_string(id) + " does not exist\n";
                    }
                    std::cerr << func_name + ": invalid value1 (NULL)\n";
                    if (value2 == NULL) {
                        std::cerr << func_name + ": invalid value2 (NULL)\n";
                    }
                }
                return false;
            } else if (value2 == NULL) {
                if (debug) {
                    std::string str_value1(value1);
                    std::cerr << func_name + "(" + std::to_string(id) + ", \"" + str_value1 + "\", \"NULL\")\n";
                    if (dictionary_map().find(id) == dictionary_map().end()) {
                        std::cerr << func_name + ": poset " + std::to_string(id) + " does not exist\n";
                    }
                    std::cerr << func_name + ": invalid value2 (NULL)\n";
                }
                return false;
            } else if (dictionary_map().find(id) == dictionary_map().end()) {
                if (debug) {
                    std::string str_value1(value1);
                    std::string str_value2(value2);
                    std::cerr << func_name + "(" + std::to_string(id) + ", \"" + str_value1 + "\", \"" + str_value2 +
                                 "\")\n";
                    std::cerr << func_name + ": poset " + std::to_string(id) + " does not exist\n";
                }
                return false;
            } else {
                std::string str_value1(value1);
                std::string str_value2(value2);
                if (debug) {
                    std::cerr << func_name + "(" + std::to_string(id) + ", \"" + str_value1 + "\", \"" + str_value2 +
                                 "\")\n";
                }
                if (dictionary_map()[id].find(str_value1) == dictionary_map()[id].end() ||
                    dictionary_map()[id].find(str_value2) == dictionary_map()[id].end()) {
                        if (debug) {
                            std::cerr << func_name + ": poset " + std::to_string(id) + ", element \"" + str_value1 +
                                         "\" or \"" + str_value2 + "\" does not exist\n";
                        }
                        return false;
                } else {
                    return true;
                }
            }
        }

    }


    unsigned long poset_new(void) {
        id_poset added_poset = new_poset_id();
        dictionary empty_dictionary;
        graph empty_graph;

        dictionary_map()[added_poset] = empty_dictionary;
        graph_map()[added_poset] = empty_graph;
        transgraph_map()[added_poset] = empty_graph;
        next_id_graph()[added_poset] = 0;

        if (debug) {
            std::cerr << "poset_new: poset " + std::to_string(added_poset) + " created\n";
        }
        return added_poset;
    }


    void poset_delete(id_poset id) {
        if (debug) {
            std::cerr << "poset_delete(" + std::to_string(id) + ")\n";
        }
        if (dictionary_map().find(id) != dictionary_map().end()) {
            dictionary_map().erase(id);
            next_id_graph().erase(id);
            graph_map().erase(id);
            transgraph_map().erase(id);
            if (debug) {
                std::cerr << "poset_delete: poset " + std::to_string(id) + " deleted\n";
            }
        } else {
            if (debug) {
                std::cerr << "poset_delete: poset " + std::to_string(id) + " does not exist\n";
            }
        }
    }


    size_t poset_size(id_poset id) {
        if (dictionary_map().find(id) == dictionary_map().end()) {
            if (debug) {
                std::cerr << "poset_delete: poset " + std::to_string(id) + " does not exist\n";
            }
            return 0;
        }
        size_t result = dictionary_map()[id].size();
        if (debug) {
            std::cerr << "poset_size: poset " + std::to_string(id) + " contains " + std::to_string(result) +
                         " element(s)\n";
        }
        return result;
    }


    bool poset_insert(id_poset id, char const *value) {
        if (value == NULL) {
            if (debug) {
                if (dictionary_map().find(id) == dictionary_map().end()) {
                    std::cerr << "poset_insert(" + std::to_string(id) + ", \"NULL\")\n"
                              << "poset_insert: poset " + std::to_string(id) + " does not exist\n"
                              << "poset_insert: invalid value (NULL)\n";
                } else {
                    std::cerr << "poset_insert(" + std::to_string(id) + ", \"NULL\")\n"
                              << "poset_insert: invalid value (NULL)\n";
                }
            }
            return false;
        }
        std::string str_value(value);
        if (debug) {
            std::cerr << "poset_insert(" + std::to_string(id) + ", \"" + str_value + "\")\n";
        }
        if (dictionary_map().find(id) == dictionary_map().end()) {
            if (debug) {
                std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
            }
            return false;
        }
        if (dictionary_map().find(id) == dictionary_map().end()) {
            if (debug) {
                std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
            }
            return false;
        }
        if (dictionary_map()[id].find(str_value) != dictionary_map()[id].end()) {
            if (debug) {
                std::cerr << "poset_insert: poset " + std::to_string(id) + ", element \"" + str_value +
                             "\" already exists\n";
            }
            return false;
        }
        dictionary_map()[id].insert({str_value, next_id_graph()[id]});
        ++(next_id_graph()[id]);
        if (debug) {
            std::cerr << "poset_insert: poset " + std::to_string(id) + ", element \"" + str_value + "\" inserted\n";
        }
        return true;
    }


    bool poset_remove(id_poset id, char const *value) {
        bool result = true;
        std::string str_value;
        graph g = graph_map()[id];
        if (value == NULL) {
            if (debug) {
                std::cerr << "poset_remove(" + std::to_string(id) + ", \"NULL\")\n";
                if (dictionary_map().find(id) == dictionary_map().end()) {
                    std::cerr << "poset_remove: poset " + std::to_string(id) + " does not exist\n";
                }
                std::cerr << "poset_insert: invalid value (NULL)\n";
            }
            result &= false;
        } else {
            str_value = value;
        }
        if (dictionary_map().find(id) == dictionary_map().end()) {
            if (debug) {
                std::cerr << "poset_remove: poset " + std::to_string(id) + " does not exist\n";
            }
            result &= false;
        } else if (result && dictionary_map()[id].find(str_value) == dictionary_map()[id].end()) {
            if (debug) {
                std::cerr << "poset_remove: poset " + std::to_string(id) +
                    ", element \"" + str_value + "\" does not exist\n";
            }
            result &= false;
        }
        if (!result) return false;
        id_graph node = dictionary_map()[id][str_value];
        for (auto iter = graph_map()[id].begin(); iter != graph_map()[id].end(); ++iter) {
            (iter -> second).erase(node);
        }
        for (auto iter = transgraph_map()[id].begin(); iter != transgraph_map()[id].end(); ++iter) {
            (iter -> second).erase(node);
        }
        graph_map()[id].erase(node);
        transgraph_map()[id].erase(node);
        dictionary_map()[id].erase(str_value);
        if (debug) {
            std::cerr << "poset_remove: poset " + std::to_string(id) +
                         ", element \"" + str_value + "\" removed\n";
        }
        return true;
    }


    bool poset_add(id_poset id, char const *value1, char const *value2) {
        if (!verify_three_arg("poset_add", id, value1, value2)) {
            return false;
        }
        std::string str_value1(value1);
        std::string str_value2(value2);
        if (poset_test_main(id, str_value1, str_value2) || poset_test_main(id, str_value2, str_value1)) {
            if (debug) {
                std::cerr << "poset_add: poset " + std::to_string(id) + ", relation (\"" + str_value1 + "\", \"" +
                             str_value2 + "\") cannot be added\n";
            }
            return false;
        } else {
            id_graph value1_id = dictionary_map()[id][str_value1];
            id_graph value2_id = dictionary_map()[id][str_value2];
            // el1 poprzedza el2 i el2 jest poprzedzany przez el1
            graph_map()[id][value1_id].insert(value2_id);
            transgraph_map()[id][value2_id].insert(value1_id);
            // el1 poprzedza wszystkie elementy poprzedzane przez el2 i
            // el2 jest poprzedzany przez wszystkie elementy poprzedzające przez el1
            graph_map()[id][value1_id].insert(graph_map()[id][value2_id].begin(),
                                              graph_map()[id][value2_id].end());
            transgraph_map()[id][value2_id].insert(transgraph_map()[id][value1_id].begin(),
                                                   transgraph_map()[id][value1_id].end());
            // wszystkie elementy poprzedzające el1 teraz poprzedzają elementy poprzedzane przez el1
            for (auto iter = (transgraph_map()[id][value1_id]).begin();
                iter != (transgraph_map()[id][value1_id]).end(); ++iter) {
                graph_map()[id][*iter].insert(graph_map()[id][value1_id].begin(),
                                              graph_map()[id][value1_id].end());
            }
            // wszystkie elementy poprzedzane przez el2 są poprzedzane przez wszystkie elementy poprzedzające el2
            for (auto iter = (graph_map()[id][value2_id]).begin();
                iter != (graph_map()[id][value2_id]).end(); ++iter) {
                transgraph_map()[id][*iter].insert(transgraph_map()[id][value2_id].begin(),
                                                   transgraph_map()[id][value2_id].end());
            }
            if (debug) {
                std::cerr << "poset_add: poset " + std::to_string(id) + ", relation (\"" + str_value1 + "\", \"" +
                             str_value2 + "\") added\n";
            }
            return true;
        }
    }


    bool poset_del(id_poset id, char const *value1, char const *value2) {
        if (!verify_three_arg("poset_del", id, value1, value2)) {
            return false;
        }
        std::string str_value1(value1);
        std::string str_value2(value2);
        id_graph node_val1 = dictionary_map()[id][value1];
        id_graph node_val2 = dictionary_map()[id][value2];
        if (!poset_test_main(id, str_value1, str_value2)
            || graph_map()[id][node_val1].find(node_val2) == graph_map()[id][node_val1].end()) {
            if (debug) {
                std::cerr << "poset_del: poset " + std::to_string(id)
                             + ", relation (\"" + str_value1 + "\", \"" + str_value2 + "\") cannot be deleted\n";
            }
            return false;
        }
        for (auto iter = graph_map()[id][node_val1].begin(); iter != graph_map()[id][node_val1].end(); ++iter) {
            if (*iter == node_val2) {
                continue;
            } else if (graph_map()[id][*iter].find(node_val2) != graph_map()[id][*iter].end()) {
                if (debug) {
                    std::cerr << "poset_del: poset " + std::to_string(id)
                                 + ", relation (\"" + str_value1 + "\", \"" + str_value2 + "\") cannot be deleted\n";
                }
                return false;
            }
        }
        graph_map()[id][node_val1].erase(node_val2);
        transgraph_map()[id][node_val2].erase(node_val1);
        if (debug) {
            std::cerr << "poset_del: poset " + std::to_string(id)
                         + ", relation (\"" + str_value1 + "\", \"" + str_value2 + "\") deleted\n";
        }
        return true;
    }


    bool poset_test(id_poset id, char const *value1, char const *value2) {
        if (!verify_three_arg("poset_test", id, value1, value2)) {
            return false;
        }
        std::string str_value1(value1);
        std::string str_value2(value2);
        if (poset_test_main(id, str_value1, str_value2)) {
            if (debug) {
                std::cerr << "poset_test: poset " + std::to_string(id) + ", relation (\"" + str_value1 + "\", \"" +
                             str_value2 + "\") exists\n";
            }
            return true;
        } else {
            if (debug) {
                std::cerr << "poset_test: poset " + std::to_string(id) + ", relation (\"" + str_value1 + "\", \"" +
                             str_value2 + "\") does not exist\n";
            }
            return false;
        }
    }


    void poset_clear(id_poset id) {
        dictionary_map().erase(id);
        next_id_graph()[id] = 0;
        dictionary_map()[id].clear();
        graph_map()[id].clear();
        transgraph_map()[id].clear();
        if (debug) {
            std::cerr << "poset_clear: poset " + std::to_string(id) + " cleared\n";
        }
    }
}

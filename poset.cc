/* Drugie zadanie z Języków i narzędzi programowania I 19/20
 * 2019-10-31
 * Adam Rozenek i Andrzej Małek
 */

#ifdef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif

#include "poset.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>


namespace jnp1 {
    // Typ identyfikatora elementów posetu.
    using id_graph = unsigned int;
    // Typ reprezentujący poset w postaci grafu skierowanego. Dla każdego elementu pod adresem jego identyfikatora
    // znajduje się set z elementami, które poprzedza bezpośrednio w grafie.
    using graph = std::unordered_map<id_graph, std::unordered_set<id_graph>>;
    // Typ słownika mapującego elementy posetu na ich identyfikatory
    using dictionary = std::unordered_map<std::string, id_graph>;
    // Typ identyfikatora posetu.
    using id_poset = unsigned long;

    namespace {
        // Zmienna globalna przechowująca słowniki posetów.
        std::unordered_map<id_poset, dictionary>& dictionary_map() {
            static std::unordered_map<id_poset, dictionary> *value = new std::unordered_map<id_poset, dictionary>;
            return *value;
        };
        // Zmienna globalna przechowująca wartość identyfikatora dla następnego elementu danego posetu.
        std::unordered_map<id_poset, id_graph>& next_id_graph() {
            static std::unordered_map<id_poset, id_graph> *value = new std::unordered_map<id_poset, id_graph>;
            return *value;
        };
        // Zmienna globalna przechowująca reprezentację grafową posetów.
        std::unordered_map<id_poset, graph>& graph_map() {
            static std::unordered_map<id_poset, graph> *value = new std::unordered_map<id_poset, graph>;
            return *value;
        };
        // Zmienna globalna przechowująca wartość identyfikatora dla następnego posetu.
        id_poset new_poset_id = 0;

        /*      Funkcja pomocnicza funkcji poset_test_main. Oblicza rekursywnie i dynamicznie, czy element o
         *      identyfikatorze value1 poprzedza element o identyfikatorze value2.
         *
         *  Parametry :
         *      id                : Identyfikator badanego posetu.
         *      value1            : Identyfikator elementu potencjalnie poprzedzającego.
         *      value2            : Identyfikator elementu potencjalnie poprzedzanego.
         *      calculated_values : Mapa dynamicznie spamiętująca, czy wartość funkcji dla danego value1 została już
         *                          obliczona, czy też nie.
         *  Return :
         *      Wartość true, jeżeli element value1 poprzedza element value2, lub false w przeciwnym przypadku.
         */
        bool recursive_relation_test(id_poset id, id_graph value1, id_graph value2,
                                     std::unordered_map<id_graph, bool> &calculated_values) {
            bool result = false;
            if ((graph_map())[id][value1].find(value2) != (graph_map())[id][value1].end()) {
                return true;
            }
            // Funkcja rekursywnie sprawdza wszystkie gałęzie grafu.
            for (auto iter = graph_map()[id][value1].begin(); iter != graph_map()[id][value1].end() && !result; ++iter) {
                if (calculated_values[*iter] == false) {
                    result = result || recursive_relation_test(id, *iter, value2, calculated_values);
                }
            }

            calculated_values[value1] = true;
            return result;
        }

        /*      Funkcja badająca czy element value1 poprzedza element value2 w posecie o identyfikatorze id. Zakłada,
         *      że taki poset istnieje, i że elementy value1 i value2 do niego należą. Nie zakłada, że value1 nie jest
         *      równe value2 (zwraca wartość true w takim przypadku). Pesymistyczna złożoność czasowa liniowa względem
         *      liczby elementów posetu id.
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
            // Mapa przechowująca obliczone wartości. Jeżeli wartość została obliczona value wartość w mapie wynosi
            // true, w przeciwnym przypadku false.
            std::unordered_map<id_graph, bool> calculated_values;
            for (auto iter = dictionary_map()[id].begin(); iter != dictionary_map()[id].end(); ++iter) {
                calculated_values.emplace(iter->second, false);
            }
            return recursive_relation_test(id, dictionary_map()[id][value1], dictionary_map()[id][value2],
                                           calculated_values);
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
                        std::cerr << func_name + "(" + std::to_string(id) + ", \"NULL\", " + str_value2 + ")\n";
                    }
                    if (dictionary_map().count(id) == 0) {
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
                    std::cerr << func_name + "(" + std::to_string(id) + ", " + str_value1 + ", \"NULL\")\n";
                    if (dictionary_map().count(id) == 0) {
                        std::cerr << func_name + ": poset " + std::to_string(id) + " does not exist\n";
                    }
                    std::cerr << func_name + ": invalid value2 (NULL)\n";
                }
                return false;
            } else {
                std::string str_value1(value1);
                std::string str_value2(value2);
                if (debug) {
                    std::cerr << func_name + "(" + std::to_string(id) + ", " + str_value1 + ", " + str_value2 + ")\n";
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
        id_poset added_poset = new_poset_id;
        ++new_poset_id;
        dictionary empty_dictionary;
        graph empty_graph;
        dictionary_map()[added_poset] = empty_dictionary;
        graph_map()[added_poset] = empty_graph;
        next_id_graph()[added_poset] = 0;
        if (debug) {
            std::cerr << "poset_new: poset " + std::to_string(new_poset_id) + " created\n";
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
        size_t result = graph_map()[id].size();
        if (debug) {
            std::cerr << "poset_size: poset " + std::to_string(id) + " contains " + std::to_string(result) +
                         " element(s)\n";
        }
        return result;
    }

    bool poset_insert(id_poset id, char const *value) {
        if (value == NULL) {
            if (debug && dictionary_map().find(id) == dictionary_map().end()) {
                std::cerr << "poset_insert(" + std::to_string(id) + ", \"NULL\")\n"
                     << "poset_insert: poset " + std::to_string(id) + " does not exist\n"
                     << "poset_insert: invalid value (NULL)\n";
            } else if (debug) {
                std::cerr << "poset_insert(" + std::to_string(id) + ", \"NULL\")\n"
                     << "poset_insert: invalid value (NULL)\n";
            }

            return false;
        }

        std::string str_value(value);
        if (debug) {
            std::cerr << "poset_insert(" + std::to_string(id) + ", \"" + str_value + "\")\n";
        }

        if (dictionary_map().count(id) == 0) {
            if (debug) {
                std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
            }

            return false;
        }

        if (dictionary_map()[id].count(str_value) == 1) {
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
        graph g = graph_map()[id];
        if (value == NULL) {
            if (debug) {
                std::cerr << "poset remove: invalid value (NULL)\n";
            }
            result &= false;
        }
        std::string str_value(value);
        if (dictionary_map().find(id) == dictionary_map().end()) {
            if (debug) {
                std::cerr << "poset_remove: poset " + std::to_string(id) + " does not exist\n";
            }
            result &= false;
        } else if (dictionary_map()[id].find(str_value) == dictionary_map()[id].end()) {
            if (debug) {
                std::cerr << "poset_remove: poset " + std::to_string(id) +
                    ", element \"" + str_value + "\" does not exist\n";
            }
        }

        if (!result) return false;

        id_graph node = dictionary_map()[id][str_value];
        for (auto iter = graph_map()[id].begin(); iter != graph_map()[id].end(); ++iter) {
            (iter -> second).erase(node);
        }
        graph_map()[id].erase(node);
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
            graph_map()[id][value1_id].insert(value2_id);
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
                             + " relation (\"" + str_value1 + "\", \"" + str_value2 + "\") cannot be deleted\n";
            }
            return false;
        }

        graph_map()[id][node_val1].erase(node_val2);
        if (debug) {
            std::cerr << "poset_del: poset " + std::to_string(id)
                         + " relation (\"" + str_value1 + "\", \"" + str_value2 + "\") deleted\n";
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
        next_id_graph().erase(id);
        dictionary_map()[id].clear();
        for (auto iter = graph_map()[id].begin(); iter != graph_map()[id].end(); ++iter) {
            (iter -> second).clear();
        }
        graph_map()[id].clear();
        if (debug) {
            std::cerr << "poset_clear: poset " + std::to_string(id) + " cleared\n";
        }
    }
}

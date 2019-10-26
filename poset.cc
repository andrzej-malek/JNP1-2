/* Drugie zadanie z Języków i narzędzi programowania I 19/20
 * 2019-10-31
 * Adam Rozenek i Andrzej Małek
 */

#ifdef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;

#endif

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

#include "poset.h"

namespace jnp1
{
    using id_graph = unsigned int;
    using graph = std::unordered_map<id_graph, std::unordered_set<id_graph>>;
    using dictionary = std::unordered_map<std::string, id_graph>;
    using id_poset = unsigned long;

    std::unordered_map<id_poset, dictionary> dictionary_map;
    std::unordered_map<id_poset, id_graph> next_id_graph;
    std::unordered_map<id_poset, graph> graph_map;
    unsigned long new_poset_id = 0;

    namespace sup {
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
        bool poset_test_main(unsigned long id, std::string const &value1, std::string const &value2) {
            if (value1.compare(value2)) {
                return true;
            }

            return false;
        }

        /*      Funkcja wypisująca debugowe informacje dla funkcji o formacie func_name(unsigned long id, char const
         *      *value1, char const *value2) gdzie poset o podanym identyfikatorze nie istnieje, któryś z elementów ma
         *      wartość NULL lub któryś z elementów nie należy do posetu o podanym identyfikatorze.
         *
         *  Parametry :
         *      func_name : string zawierający nazwę debugowanej funkcji.
         *      id        : Argument id debugowanej funkcji.
         *      value1    : Argument value1 debugowanej funkcji.
         *      value2    : Argument value2 debugowanej funkcji.
         *  Return :
         *      void
         */
        void three_arg_debug(std::string const &func_name, unsigned long id, char const *value1, char const *value2) {
            if (value1 == NULL) {
                if (value2 == NULL) {
                    std::cerr << func_name + "(" + std::to_string(id) + ", \"NULL\", \"NULL\")\n";
                } else {
                    std::string str_value2(value2);
                    std::cerr << func_name + "(" + std::to_string(id) + ", \"NULL\", " + str_value2 + ")\n";
                }
                if (dictionary_map.count(id) == 0) {
                    std::cerr << func_name + ": poset " + std::to_string(id) + " does not exist\n";
                }
                std::cerr << func_name + ": invalid value1 (NULL)\n";
                if (value2 == NULL) {
                    std::cerr << func_name + ": invalid value2 (NULL)\n";
                }
            } else if (value2 == NULL) {
                std::string str_value1(value1);
                std::cerr << func_name + "(" + std::to_string(id) + ", " + str_value1 + ", \"NULL\")\n";
                if (dictionary_map.count(id) == 0) {
                    std::cerr << func_name + ": poset " + std::to_string(id) + " does not exist\n";
                }
                std::cerr << func_name + ": invalid value2 (NULL)\n";
            } else {
                std::string str_value1(value1);
                std::string str_value2(value2);
                std::cerr << func_name + "(" + std::to_string(id) + ", " + str_value1 + ", " + str_value2 + ")\n";
                std::cerr << func_name + ": poset " + std::to_string(id) + ", element \"" + str_value1 + "\" or \"" +
                             str_value2 + "\" does not exist\n";
            }

        }

        /* Funkcja weryfikująca podane parametry
         * Sprawdza, czy istnieje poset o danym id, czy zmienne value nie są nullami, czy istnieją w posecie
         * elementy o identyfikatorach równych zmiennym value1/value2 oraz wypisuje stosowne informacje na wyjście
         * diagnostyczne
         *
         * Parametry :
         *      func_name : string zawierający nazwę debugowanej funkcji.
         *      id        : Argument id debugowanej funkcji.
         *      value1    : Argument value1 debugowanej funkcji.
         *      value2    : Argument value2 debugowanej funkcji.
         *  Return :
         *      true - jeżeli dane są podane w poprawnym formacie i są dostępne w posecie
         *      false - w przeciwnym wypadku
         */
        bool verify_three_args(std::string const &func_name, unsigned long id, char const *value1, char const *value2) {
            bool result = true;
            if (dictionary_map.find(id) == dictionary_map.end()) {
                if (debug) {
                    std::cerr << func_name << ": poset " + std::to_string(id) + " does not exist\n";
                    result = false;
                } else {
                    return false;
                }
            }
            if (value1 == NULL) {
                if (debug) {
                    std::cerr << func_name + ": invalid value1 (NULL)\n";
                    result = false;
                } else {
                    return false;
                }
            }
            if (value2 == NULL) {
                if (debug) {
                    std::cerr << func_name + ": invalid value2 (NULL)\n";
                    result = false;
                } else {
                    return false;
                }
            }
            if (!result) {
                return false;
            }
            std::string str_value1(value1);
            std::string str_value2(value2);
            if (dictionary_map[id].find(str_value1) == dictionary_map[id].end()
                || dictionary_map[id].find(str_value2) == dictionary_map[id].end()) {
                std::cerr << func_name + ": poset " + std::to_string(id) + ", element \"" + str_value1 + "\" or \"" +
                             str_value2 + "\" does not exist\n";
            }
            return result;
        }
    }

    unsigned long poset_new(void) {
        id_poset added_poset = new_poset_id;
        new_poset_id++;
        dictionary empty_dictionary;
        graph empty_graph;
        dictionary_map[added_poset] = empty_dictionary;
        graph_map[added_poset] = empty_graph;
        next_id_graph[added_poset] = 0;
        if (debug) {
            std::cerr << "poset_new: poset " + std::to_string(new_poset_id) + " created\n";
        }
        return added_poset;
    }

    void poset_delete(unsigned long id) {
        if (debug) {
            std::cerr << "poset_delete(" + std::to_string(id) + ")\n";
        }
        if (dictionary_map.find(id) != dictionary_map.end()) {
            dictionary_map.erase(id);
            next_id_graph.erase(id);
            graph_map.erase(id);
            if (debug) {
                std::cerr << "poset_delete: poset " + std::to_string(id) + " deleted\n";
            }
        } else {
            if (debug) {
                std::cerr << "poset_delete: poset " + std::to_string(id) + " does not exist\n";
            }
        }
    }

    size_t poset_size(unsigned long id) {
        if (dictionary_map.find(id) == dictionary_map.end()) {
            if (debug) {
                std::cerr << "poset_delete: poset " + std::to_string(id) + " does not exist\n";
            }
            return 0;
        }
        size_t result = graph_map[id].size();
        if (debug) {
            std::cerr << "poset_size: poset " + std::to_string(id) + " contains " + std::to_string(result) +
                         " element(s)\n";
        }
        return result;
    }

    bool poset_insert(unsigned long id, char const *value) {
        if (value == NULL) {
            if (debug && dictionary_map.find(id) == dictionary_map.end()) {
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

        if (dictionary_map.count(id) == 0) {
            if (debug) {
                std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
            }

            return false;
        }

        if (dictionary_map[id].count(str_value) == 1) {
            if (debug) {
                std::cerr << "poset_insert: poset " + std::to_string(id) + ", element \"" + str_value +
                             "\" already exists\n";
            }

            return false;
        }

        dictionary_map[id].insert({str_value, next_id_graph[id]});
        (next_id_graph[id])++;
        if (debug) {
            std::cerr << "poset_insert: poset " + std::to_string(id) + ", element \"" + str_value + "\" inserted\n";
        }

        return true;
    }

    bool poset_remove(unsigned long id, char const *value) {
        bool result = true;
        graph g = graph_map[id];
        if (value == NULL) {
            if (debug) {
                std::cerr << "poset remove: invalid value (NULL)\n";
            }
            result &= false;
        }
        std::string str_value(value);
        if (dictionary_map.find(id) == dictionary_map.end()) {
            if (debug) {
                std::cerr << "poset_remove: poset " + std::to_string(id) + " does not exist\n";
            }
            result &= false;
        } else if (dictionary_map[id].find(str_value) == dictionary_map[id].end()) {
            if (debug) {
                std::cerr << "poset_remove: poset " + std::to_string(id) +
                    ", element \"" + str_value + "\" does not exist\n";
            }
        }
        if (!result) return false;

        id_graph node = dictionary_map[id][str_value];
        for (auto iter = graph_map[id].begin(); iter != graph_map[id].end(); iter++) {
            (iter -> second).erase(node);
        }
        graph_map[id].erase(node);
        if (debug) {
            std::cerr << "poset_remove: poset " + std::to_string(id) +
                         ", element \"" + str_value + "\" removed\n";
        }
        return true;
    }


    bool poset_test2(unsigned long id, char const *value1, char const *value2) {
        if (value1 == NULL) {
            if (debug) {
                sup::three_arg_debug("poset_add", id, value1, value2);
            }
            return false;
        }
        std::string str_value1(value1);
        std::string str_value2(value2);

        if (dictionary_map[id].find(str_value1) == dictionary_map[id].end()
            || dictionary_map[id].find(str_value2) == dictionary_map[id].end()) {
            if (debug) {
                sup::three_arg_debug("poset_add", id, value1, value2);
            }
            sup::three_arg_debug("poset_add", id, value1, value2);
            return false;
        }

        if (sup::poset_test_main(id, str_value1, str_value2) || sup::poset_test_main(id, str_value2, str_value1)) {
            if (debug) {
                std::cerr << "poset_add: poset " + std::to_string(id) + ", relation (\"" + str_value1 + "\", \"" +
                             str_value2 + "\") cannot be added\n";
            }
            return false;
        } else {
            id_graph value1_id = dictionary_map[id][str_value1];
            id_graph value2_id = dictionary_map[id][str_value2];
            graph_map[id][value1_id].insert(value2_id);
            if (debug) {
                std::cerr << "poset_add: poset " + std::to_string(id) + ", relation (\"" + str_value1 + "\", \"" +
                             str_value2 + "\") added\n";
            }
            return true;
        }
    }

    bool poset_del(unsigned long id, char const *value1, char const *value2) {

        if (!sup::verify_three_args("poset_del", id, value1, value2)) {
            return false;
        }
        std::string str_value1(value1);
        std::string str_value2(value2);
        id_graph node_val1 = dictionary_map[id][value1];
        id_graph node_val2 = dictionary_map[id][value2];
        if (sup::poset_test_main(id, str_value1, str_value2)) {
            graph_map[id][node_val2].erase(node_val2);
        } else {
            return false;
        }
        return true;
    }

    bool poset_test(unsigned long id, char const *value1, char const *value2) {
        if (value1 == NULL || value2 == NULL || dictionary_map.count(id) == 0) {
            if (debug) {
                sup::three_arg_debug("poset_insert", id, value1, value2);
            }
            return false;
        }
        std::string str_value1(value1);
        std::string str_value2(value2);

        if (dictionary_map[id].count(str_value1) == 0 || dictionary_map[id].count(str_value2) == 0) {
            if (debug) {
                sup::three_arg_debug("poset_insert", id, value1, value2);
            }
            sup::three_arg_debug("poset_insert", id, value1, value2);
            return false;
        }

        if (sup::poset_test_main(id, str_value1, str_value2)) {
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

    void poset_clear(unsigned long id) {
        dictionary_map.erase(id);
        next_id_graph.erase(id);
        dictionary_map[id].clear();
        for (auto iter = graph_map[id].begin(); iter != graph_map[id].end(); iter++) {
            (iter -> second).clear();
        }
        graph_map[id].clear();
        if (debug) {
            std::cerr << "poset_clear: poset " + std::to_string(id) + " cleared\n";
        }
    }
}

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

    namespace sup
    {
        /*      Funkcja badająca czy element value1 poprzedza element value2 w posecie o identyfikatorze id. Zakłada,
         *      że taki poset istnieje, i że elementy value1 i value2 do niego należą. Nie zakłada, że value1 nie jest
         *      równe value2.
         *
         *  Parametry :
         *      id     : Identyfikator badanego posetu.
         *      value1 : Element potencjalnie poprzedzający.
         *      value2 : Element potencjalnie poprzedzany.
         *  Return :
         *      Wartość true, jeżeli element value1 poprzedza element value2, lub false w przeciwnym przypadku.
         */
        bool poset_test_main(unsigned long id, std::string& value1, std::string& value2) {
            if (value1.compare(value2)) {
                return true;
            }
        }
    }

    void poset_delete(unsigned long id) {
        if (debug) {
            std::cerr << "poset_delete(" + std::to_string(id) + ")\n";
        }
        if (dictionary_map.count(id) == 1) {
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

    bool poset_insert(unsigned long id, char const *value) {
        if (value == NULL) {
            if (debug && dictionary_map.count(id) == 0) {
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

    bool poset_test(unsigned long id, char const *value1, char const *value2) {
        if (value1 == NULL) {
            if (debug) {
                if (value2 == NULL) {
                    std::cerr << "poset_test(" + std::to_string(id) + ", \"NULL\", \"NULL\")\n";
                } else {
                    std::string str_value2(value2);
                    std::cerr << "poset_test(" + std::to_string(id) + ", \"NULL\", " + str_value2 + "\")\n";
                }
                if (dictionary_map.count(id) == 0) {
                    std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
                }

                std::cerr << "poset test: invalid value1 (NULL)\n";

                if (value2 == NULL) {
                    std::cerr << "poset test: invalid value2 (NULL)\n";
                }
            }

            return false;
        } else if (value2 == NULL) {
            if (debug) {
                std::string str_value1(value1);

                std::cerr << "poset_test(" + std::to_string(id) + ", \"" + str_value1 + "\", \"NULL\")\n";

                if (dictionary_map.count(id) == 0) {
                    std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
                }

                std::cerr << "poset test: invalid value2 (NULL)\n";
            }

            return false;
        }

        if (debug) {
            std::string str_value1(value1);
            std::string str_value2(value2);
            std::cerr << "poset_test(" + std::to_string(id) + ", \"" + str_value1 + "\", \"" + str_value2 + "\")\n";
        }

        if (dictionary_map.count(id) == 0) {
            if (debug) {
                std::string str_value1(value1);
                std::string str_value2(value2);
                std::cerr << "poset_insert: poset " + std::to_string(id) + " does not exist\n";
            }
            return false;
        }

        std::string str_value1(value1);
        std::string str_value2(value2);

        if (dictionary_map[id].count(str_value1) == 0 || dictionary_map[id].count(str_value2) == 0) {
            if (debug) {
                std::cerr << "poset_test: poset " + std::to_string(id) + ", element \"" + str_value1 + "\" or \"" +
                             str_value2 + "\" does not exist\n";
            }

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
}

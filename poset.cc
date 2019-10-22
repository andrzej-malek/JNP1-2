/* Drugie zadanie z Języków i narzędzi programowania I 19/20
 * 2019-10-31
 * Adam Rozenek i Andrzej Małek
 */

#include <map>

namespace jnp1
{

    using id_graph = unsigned int;
    using graph = unordered_map<id_graph, unorder_set<id_graph>>;
    using dictionary = unordered_map<string, id_graph>;

    using id_poset = unsigned long;

    unorder_map<id_poset, dictionary> dictionary_map;
    unorder_map<id_poset, id_graph> next_id_graph;
    unorder_map<id_poset, graph> graph_map;

    namespace sup_func
    {

    }
}




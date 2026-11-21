

#include "node_graph.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

class save_manager {


    save_manager() {

    }

    static std::shared_ptr<node> create_node_by_name(std::string& name, node_graph& ng);
public:
    static void save(std::string& filepath,voice_descriptor& voice_description);


    /*
     * Load should return an array of the nodes and the links
     */

    static void load(const std::string& filepath, node_graph& ng);


};

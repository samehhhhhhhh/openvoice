

#include "saves.hpp"
#include "node.hpp"
#include "node_graph.hpp"

/*
 * A config file looks like this :
 * Openvoice Version
 * Voice name
 * Voice authour
 * Node list (Not actually written)
 * * Node name
 * * Node parameters
 * * "//"
 *
 * ///
 * Links (Not actually written)
 * (index of node a) (pin of node a) (index of node b) (pin of node b)
 */

void save_manager::save(std::string& filepath, voice_descriptor& voice_description) {

    std::ofstream file(filepath);
    if(file.is_open()) {

        file << OPENVOICE_VERSION << std::endl;
        file << voice_description.name << std::endl;
        file << voice_description.author << std::endl;
        for (auto& i : *voice_description.nodes) {

            file << i->config.title << std::endl;
            for(auto& a : i->config.parameters) {
                file << a << std::endl;
            }
            file << "//" << std::endl; // End of node
        }

        file << "///" << std::endl;

        for(auto& a : voice_description.links) {

            file << a.input_node_index << " " << a.output_node_index
                << " " << a.output_pin_index << " " << a.input_pin_index << std::endl;

            // Basically one line per link, using the index of the element in the active_nodes array to
            // As a way to reconstruct the links

        }
    }

}
std::shared_ptr<node> save_manager::create_node_by_name(std::string& name, node_graph& ng) {

    for(const auto &i : ng.get_node_types()) {
        if(i.title == name) {
            return ng.add_node(i);

        }

    }

    return nullptr;



}
void save_manager::load(const std::string& filepath, node_graph& ng) {

    std::ifstream file(filepath);

    std::vector<std::string> elements;

    std::string line;

    ng.get_active_nodes().clear();

    while(std::getline(file, line) >> std::ws) {
        elements.push_back(line);
    }

    // If its shorter than 3, then the file is incomplete

    if(elements.size() < 3) {
        std::cerr << "Error, Config file too small" << std::endl;


    }


    int a = 0;

    bool reading_links {false};
    std::vector<link> links;

    for(int i  = 3; i < elements.size(); i = a) {

        if(elements[i] == "///") {
            reading_links = true;
            a++;
            continue;

        }

        if(!reading_links) {

            std::shared_ptr<node> saved_node = create_node_by_name(elements[i], ng);

            std::vector<float> parameters;
            for(a = i+1; a < elements.size() && elements[a] != "//"   ;a++) {

                parameters.push_back(std::stof(elements[a]));

            }

            // Add parameters to the node's config
            saved_node->config.parameters = parameters;


        } else {

            // Splitting the line into 4 integers
            std::vector<int> link_info;
            std::stringstream ss(elements[i]);
            std::string token;

            while(getline(ss, token, ' ')) {
                link_info.push_back(std::stoi(token));
            }

            // Reapplying the link from the info

            // List of its input pins
            std::vector<pin> input_pin_IDs;

            std::copy_if(
                ng.get_active_nodes()[link_info[0]]->config.pins.begin(),
                ng.get_active_nodes()[link_info[0]]->config.pins.end(),
                std::back_inserter(input_pin_IDs),
                [](pin pin) {
                    return pin.type == pin_types::OUTPUT;
                }
            );

            std::vector<pin> output_pin_IDs;

            std::copy_if(
                ng.get_active_nodes()[link_info[1]]->config.pins.begin(),
                ng.get_active_nodes()[link_info[1]]->config.pins.end(),
                std::back_inserter(output_pin_IDs),
                [](pin pin) {
                    return pin.type == pin_types::INPUT;
                }
            );


            ng.add_link(input_pin_IDs[link_info[2]].ID,output_pin_IDs[link_info[3]].ID );


        }

        a++;

    }



}

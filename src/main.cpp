#include <iostream>
#include <vector>
#include <filesystem>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include "rapidxml/rapidxml.hpp"

const std::string kConfigFilePath = "config.xml";

struct Filter {
    std::string path;
    std::string target;
    std::vector<std::string> exts;
};

std::vector<std::filesystem::path> GetElementsInPath(const std::filesystem::path& path) {
    std::vector<std::filesystem::path> files;
    if (!std::filesystem::exists(path)) {
        return files;
    }

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        files.emplace_back(entry);
    }

    return files;
}

std::vector<std::filesystem::path> GetFilesByExt(const Filter& filter) {
    std::vector<std::filesystem::path> files;
    std::filesystem::path path(filter.path);
    auto elements = GetElementsInPath(path);
    for (const std::filesystem::path& entry : std::filesystem::directory_iterator(path)) {
        for (const auto& exts : filter.exts) {
            if (entry.extension() == exts) {
                files.emplace_back(entry);
            }
        }
    }
    return files;
}

void MoveFilesToTarget(const Filter& filter) {
    std::filesystem::path to(filter.target);
    auto files = GetFilesByExt(filter);
    for (const auto& entry : files) {
        try {
            std::filesystem::copy_file(entry, to / entry.filename());
        }
        catch (std::filesystem::filesystem_error& e) {
            std::cerr << "--ERROR: ";
            std::cerr << e.what() << std::endl;
        }
    }
}

void PerformFilter(const Filter& filter) {
    while (true) {
        MoveFilesToTarget(filter);
    }
}

std::string ParseXMLConfigToString() {
    std::ifstream configFile(kConfigFilePath);
    std::stringstream buffer;
    buffer << configFile.rdbuf();
    configFile.close();
    std::string content(buffer.str());
    return content;
}


std::vector<Filter*> LoadFilterList() {
    std::vector<Filter*> my_filters;

    auto xml_data = ParseXMLConfigToString();
    auto* doc = new rapidxml::xml_document<>;
    doc->parse<0>(&xml_data[0]);

    // Iterating over the root node, then
    // access to the filters list to start
    // the parsing.
    auto forga_node = doc->first_node();
    auto filters_node = forga_node->first_node("filters");
    for (auto filter_node = filters_node->first_node("filter"); filter_node; filter_node = filter_node->next_sibling()) {

        // Creating filter and adding from and to values.
        auto* filter = new Filter();
        filter->path = filter_node->first_node("from")->value();
        filter->target = filter_node->first_node("to")->value();

        // Iterate over extensions nodes
        // and get their values to the filter
        auto extensions_node = filter_node->first_node("extensions");
        for (auto ext_node = extensions_node->first_node("ext"); ext_node; ext_node = ext_node->next_sibling()) {
            filter->exts.emplace_back(ext_node->value());
        }

        // Add parsed node to the list
        my_filters.emplace_back(filter);
    }
    delete doc;
    return my_filters;
}

int main() {
    std::vector<Filter*> my_filters = LoadFilterList();
    for (const auto& i : my_filters) {
        std::cout << i->path << std::endl;
        std::cout << i->target << std::endl;
        // MoveFilesToTarget(*i);
    }

    for (const auto& i : my_filters) {
        delete i;
    }
    return EXIT_SUCCESS;
}
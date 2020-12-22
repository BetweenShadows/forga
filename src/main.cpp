#include <iostream>
#include <vector>
#include <filesystem>
#include <thread>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>
#include "filter.hpp"
#include "rapidxml/rapidxml.hpp"

const std::string kConfigFilePath = "config.xml";

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

std::vector<std::filesystem::path> GetFilesByExt(Filter& filter) {
    std::vector<std::filesystem::path> files;
    std::filesystem::path path(filter.GetWatchDir());
    auto elements = GetElementsInPath(path);
    for (const std::filesystem::path& entry : std::filesystem::directory_iterator(path)) {
        if(filter.HasExtension(entry.extension().u8string())) {
            files.emplace_back(entry);
        }
    }
    return files;
}

void MoveFilesToTarget(Filter& filter) {
    std::filesystem::path to(filter.GetTargetDir());
    auto files = GetFilesByExt(filter);
    for (const auto& entry : files) {
        try {
            std::filesystem::copy_file(entry, to / entry.filename());
            std::filesystem::remove(entry);
        }
        catch (std::filesystem::filesystem_error& e) {
            std::cerr << "--ERROR: ";
            std::cerr << e.what() << std::endl;
        }
    }
}

void PerformFilter(Filter& filter) {
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
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
        std::string from = filter_node->first_node("from")->value();
        std::string to = filter_node->first_node("to")->value();
        auto* filter = new Filter(from, to);

        // Iterate over extensions nodes
        // and get their values to the filter
        auto extensions_node = filter_node->first_node("extensions");
        for (auto ext_node = extensions_node->first_node("ext"); ext_node; ext_node = ext_node->next_sibling()) {
            filter->AddExtension(ext_node->value());
        }

        // Add parsed node to the list
        my_filters.emplace_back(filter);
    }
    delete doc;
    return my_filters;
}

int main() {

    std::vector<Filter*> my_filters = LoadFilterList();
    std::vector<std::thread*> f_threads;

    for(auto & my_filter : my_filters) {
        auto* thread = new std::thread(&PerformFilter, *my_filter);
        f_threads.emplace_back(thread);
    }

    for(auto& t : f_threads) {
        t->join();
    }

    for (auto& i : my_filters) {
        delete i;
    }

    for(auto& t : f_threads) {
        delete t;
    }
    return EXIT_SUCCESS;
}
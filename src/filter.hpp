#ifndef FORGA_FILTER_HPP
#define FORGA_FILTER_HPP

#include <vector>
#include <string>

class Filter {
public:
    Filter();
    Filter(const std::string& from, const std::string& to);
    void AddExtension(const std::string& ext);
    bool HasExtension(const std::string& ext);
    std::string GetWatchDir() const;
    std::string GetTargetDir() const;
private:
    std::string from_;
    std::string to_;
    std::vector<std::string> extensions_;
};

#endif //FORGA_FILTER_HPP

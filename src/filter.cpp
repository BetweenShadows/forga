#include <algorithm>
#include "filter.hpp"

Filter::Filter() {

}

Filter::Filter(const std::string &from, const std::string &to) {
    this->from_ = from;
    this->to_ = to;
}

void Filter::AddExtension(const std::string &ext) {
    this->extensions_.emplace_back(ext);
}

bool Filter::HasExtension(const std::string &ext) {
    return std::find(extensions_.begin(), extensions_.end(), ext) != extensions_.end();
}

std::string Filter::GetWatchDir() const {
    return from_;
}

std::string Filter::GetTargetDir() const {
    return to_;
}
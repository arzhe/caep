#ifndef CAEP_FILTERED_FILE_ADAPTER_CPP
#define CAEP_FILTERED_FILE_ADAPTER_CPP

#include <fstream>

#include "./filtered_file_adapter.h"
#include "../../exception/caep_exception.h"
#include "../../util/caep_util.h"

namespace caep {

bool FilteredFileAdapter::filterLine(std::string line, Filter* filter) {
    if(filter == nullptr)
        return false;

    std::vector<std::string> p = CaepUtil::Split(line, ",");
    if(p.size() == 0)
        return true;

    std::vector<std::string> filter_slice;
    std::string str = CaepUtil::Trim(p[0]);
    if(!str.compare("a"))
        filter_slice = filter->A;
    else if(!str.compare("r"))
        filter_slice = filter->R;
    else if(!str.compare("m"))
        filter_slice = filter->M;

    return filterWords(p, filter_slice);
}

bool FilteredFileAdapter::filterWords(std::vector<std::string> line, std::vector<std::string> filter) {
    if(line.size() < filter.size() + 1)
        return true;

    bool skip_line;
    for(size_t i = 0; i < filter.size(); ++i) {
        if(filter[i].length() > 0 && CaepUtil::Trim(filter[i]) != CaepUtil::Trim(line[i+1])) {
            skip_line = true;
            break;
        }
    }

    return skip_line;
}

void FilteredFileAdapter::loadFilteredPolicyFile(Model* model , Filter* filter, void (*handle)(std::string, Model*)) {
    std::ifstream out_file;
    try {
        out_file.open(this->file_path);
    } catch(const std::ifstream::failure e) {
        throw IOException("Cannot open file.");
    }

    std::string line;
    while(getline(out_file, line, '\n')) {
        line = CaepUtil::Trim(line);
        if(filterLine(line, filter))
            continue;

        handle(line, model);
    }

    out_file.close();
}

// FilteredFileAdapter is the constructor for FilteredFileAdapter.
FilteredFileAdapter::FilteredFileAdapter(std::string file_path) : FileAdapter(file_path) {
    this->filtered = true;
}

// LoadPolicy loads all policy rules from storage.
void FilteredFileAdapter::LoadPolicy(Model* model) {
    this->filtered = false;
    this->FileAdapter::LoadPolicy(model);
}

// LoadFilteredPolicy loads only policy rules that match the filter.
void FilteredFileAdapter::LoadFilteredPolicy(Model* model, Filter* filter) {
    if(filter == nullptr)
        this->LoadPolicy(model);

    if(!this->file_path.compare("")) 
        throw AdapterException("Invalid file path, file path cannot be empty.");

    this->loadFilteredPolicyFile(model, filter, LoadPolicyLine);
    this->filtered = true;
}

// IsFiltered returns true if the loaded policy has been filtered.
bool FilteredFileAdapter::IsFiltered() {
    return this->filtered;
}

// SavePolicy saves all policy rules to the storage.
void FilteredFileAdapter::SavePolicy(Model* model) {
    if(this->filtered)
        throw AdapterException("Cannot save a filtered policy");

    this->SavePolicy(model);
}

} // namespace caep 

#endif

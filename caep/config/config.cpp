#ifndef CAEP_CONFIG_CPP
#define CAEP_CONFIG_CPP

#include <fstream>
#include <sstream>

#include "./config.h"
#include "../exception/io_exception.h"
#include "../exception/illegal_argument_exception.h"
#include "../util/util.h"

namespace caep {

const std::string Config::DEFAULT_COMMENT = "#";
const std::string Config::DEFAULT_SECTION = "default";

std::mutex Config::mtx_lock;

bool Config::AddConfigData(std::string section, std::string key, std::string value) {
    if(!section.compare(""))
        section = DEFAULT_SECTION;

    bool found = data[section].find(key) != data[section].end();
    if(!found) {
        data[section][key] = value;
        return true;
    }
    
    return false;
}

void Config::ParseFile(std::string f_name) {
    mtx_lock.lock();
    std::ifstream ifile;
    try {
        ifile.open(f_name);
    } catch(const std::ifstream::failure e) {
        mtx_lock.unlock();
        throw IOException("Cannot open file!");
    }
    ParseBuffer(&ifile);
    mtx_lock.unlock();
    ifile.close();
}

void Config::ParseBuffer(std::istream* buffer) {
    std::string section = "";
    int line_num = 0;
    std::string line;
    while(true) {
        ++line_num;
        if(getline(*buffer, line, '\n')) {
            if(!line.compare(""))
                continue;
        }
        else 
            break;
        line = Tailor(line);
        if(line.find(DEFAULT_COMMENT) == 0)
            continue;
        else if(line.find("[") == 0 && EndWith(line, std::string("]")))
            section = line.substr(1, line.length() - 2);
        else {
            auto key_val = Split(line, std::string("="), 2);

            if(key_val.size() != 2) {
                char* error = new char;
                sprintf(error, "parse the content error: line %d, %s = ? ", line_num, key_val[0].c_str());
                throw IllegalArgumentException(std::string(error));
            }
            std::string key = Tailor(key_val[0]);
            std::string value = Tailor(key_val[1]);
            AddConfigData(section, key, value);
        }
    }
}

std::shared_ptr<Config> Config::NewConfigFromFile(std::string f_name) {
    std::shared_ptr<Config> c(new Config);
    c->ParseFile(f_name);
    return c;
}

std::shared_ptr<Config> Config::NewConfigFromText(std::string text) {
    std::shared_ptr<Config> c(new Config);
    std::stringstream textstream(text);
    c->ParseBuffer(&textstream);
    return c;
}

std::string Config::GetConfigValue(std::string section, std::string key) {
    if(!section.compare(""))
        section = DEFAULT_SECTION;
    bool found = data.find(section) != data.end() && data[section].find(key) != data[section].end();
    if(found)
        return data[section][key];
    return "";
}

} // namespace caep 

#endif


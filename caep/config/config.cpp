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

bool Config::AddConfig(std::string section, std::string key, std::string value) {
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
            AddConfig(section, key, value);
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

bool Config::GetBool(std::string sec_key) {
    return Get(sec_key).compare("true") == 0;
}

int Config::GetInt(std::string sec_key) {
    return atoi(Get(sec_key).c_str());
}

float Config::GetFloat(std::string sec_key) {
    return float(atof(Get(sec_key).c_str()));
}

std::string Config::GetString(std::string sec_key) {
    return Get(sec_key);
}

std::vector<std::string> Config:: GetStrings(std::string sec_key) {
    auto s = Get(sec_key);
    if(!s.compare("")) 
        return std::vector<std::string>();
    auto vs = Split(s, std::string(","));
    for(auto& avs : vs)
        avs = Tailor(avs);
    return vs;
}

void Config::Set(std::string sec_key, std::string value) {
    mtx_lock.lock();
    if(sec_key.length() == 0) {
        mtx_lock.unlock();
        throw IllegalArgumentException("sec_key is empty");
    }

    std::string section;
    std::string key;

    transform(sec_key.begin(), sec_key.end(), sec_key.begin(), ::tolower);
    auto sec_keys = Split(sec_key, std::string("::"));
    if(sec_keys.size() >= 2) {
        section = sec_keys[0];
        key = sec_keys[1];
    }
    else { 
        section = "";
        key = sec_keys[0];
    }
    AddConfig(section, key, value);
    mtx_lock.unlock();
}

std::string Config::Get(std::string sec_key) {
    std::string section;
    std::string key;

    transform(sec_key.begin(), sec_key.end(), sec_key.begin(), ::tolower);
    auto sec_keys = Split(sec_key, std::string("::"));
    if(sec_keys.size() >= 2) {
        section = sec_keys[0];
        key = sec_keys[1];
    }
    else {
        section = DEFAULT_SECTION;
        key = sec_keys[0];
    }

    bool found = data.find(section) != data.end() && data[section].find(key) != data[section].end();
    if(found)
        return data[section][key];
    return "";
}

} // namespace caep 

#endif


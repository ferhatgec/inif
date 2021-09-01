// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
// inif - fast .ini file parser in c++17
//
// github.com/ferhatgec/inif
//

#ifndef INIF_INIF_HPP
#define INIF_INIF_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

class inif_category {
public:
    std::string category_name;

    std::map<std::string, std::string> key_value;
public:
    inif_category() = default;
    ~inif_category()= default;
};

namespace __helpers__ {
    // from: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
    void ltrim(std::string& value) {
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    void rtrim(std::string& value) {
        value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), value.end());
    }
}

class inif {
    std::vector<inif_category> init;

    bool category_start = false, category_data = false, key_data = false;
    std::string category_name, key, data;

    enum InifTokens : const char {
        CategoryStart = '[',
        CategoryEnd = ']',
        Eq = '=',
        Comment = ';'
    };
public:
    inif() = default;
    ~inif()= default;

    void parse(const std::string file_data) noexcept {
        for(auto& ch : file_data) {
            if(this->key_data) {
                if(ch != '\n') {
                    this->data.push_back(ch);
                    continue;
                }

                __helpers__::ltrim(this->key);
                __helpers__::rtrim(this->key);

                if(this->init.size() > 0 && this->init.back().category_name == this->category_name) {
                    this->init.back().key_value.insert(std::make_pair(this->key, this->data));
                } else {
                    this->init.push_back(inif_category {
                        .category_name = this->category_name,
                        .key_value = std::map<std::string, std::string> {
                            {this->key, this->data}
                        }
                    });
                }

                this->key_data = false;
                this->key.clear(); this->data.clear();

                continue;
            }

            if(this->category_start) {
                if(ch != CategoryEnd) {
                    this->category_name.push_back(ch);
                    continue;
                }

                this->category_start = false;
                this->category_data = true;
                continue;
            }

            switch(ch) {
                case CategoryStart: {
                    if(!this->category_name.empty())
                        this->category_name.clear();

                    category_start = true;
                    break;
                }

                case Eq: {
                    if(this->category_data && !this->key.empty()) {
                        this->key_data = true;
                    }

                    break;
                }

                default: {
                    if(this->category_data && ch != ' ') {
                        this->key.push_back(ch);
                    }

                    break;
                }
            }
        }
    }

    std::string get(const std::string category, const std::string key) noexcept {
        for(auto& val : this->init) {
            if(val.category_name == category) {
                for(auto& store : val.key_value) {
                    if(store.first == key) {
                        return store.second;
                    }
                }
            }
        }

        return "\"\"";
    }
};

#endif // INIF_INIF_HPP

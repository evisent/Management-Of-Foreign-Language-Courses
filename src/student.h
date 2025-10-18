#pragma once
#include <string>
#include <memory>
#include <vector>
#include "languages.h"

class Student{
public:
    Student() = default;
    Student(const std::string& name, std::vector <std::unique_ptr<Language>> languages);
    Student(const std::string& name, std::vector <std::unique_ptr<Language>> languages, bool individual);
    std::string get_name() const;
    void get_info() const;
    const std::vector<std::unique_ptr<Language>>& get_languages() const;
    void delete_language(int index);
    bool is_individual() const;
    ~Student() = default;
    
private:
    std::string name_;
    std::vector <std::unique_ptr<Language>> languages_;
    bool individual_ = false;
};

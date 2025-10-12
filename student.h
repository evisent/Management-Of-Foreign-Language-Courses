#pragma once
#include <string>
#include <memory>
#include <vector>
#include "languages.h"

class Student{
public:
    Student () = default;
    Student(std::string name, std::vector <std::unique_ptr<Language>> languages);
    std::string get_name() const;
    void get_languages() const;

private:
    std::string name_;
    std::vector <std::unique_ptr<Language>> languages_;
    int price_;
};
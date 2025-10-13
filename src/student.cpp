#include<iostream>
#include"student.h"
#include"languages.h"

Student::Student(std::string name,  std::vector <std::unique_ptr<Language>> languages) : name_(name), languages_(std::move(languages)){};

std::string Student::get_name() const {
    return name_;
}

const std::vector<std::unique_ptr<Language>>& Student::get_languages() const {
    return languages_;
}


void Student::get_info() const {
    for (int i = 0; i < languages_.size(); ++i){
        std::cout << name_ << ": " << languages_[i]->get_name() << ", level is " << languages_[i]->get_level() << ", periods left " 
        << languages_[i]->get_intensity().get_period() << ", price is " << languages_[i]->get_price() << std::endl;
    }
}
#include<iostream>
#include"student.h"
#include"languages.h"

Student::Student(std::string name,  std::vector <std::unique_ptr<Language>> languages) : name_(name), languages_(std::move(languages)){};

std::string Student::get_name() const {
    return name_;
}

void Student::get_languages() const{
    for (int i = 0; i < languages_.size(); ++i){
        std::cout << languages_[i]->get_name() << " level is " << languages_[i]->get_level() << " intensity is " << languages_[i]->get_intensity() << std::endl;
    }
}
#include<iostream>
#include"student.h"
#include"languages.h"

Student::Student(const std::string& name,  std::vector <std::unique_ptr<Language>> languages) : name_(name), languages_(std::move(languages)){};

Student::Student(const std::string& name,  std::vector <std::unique_ptr<Language>> languages, bool individual) 
: name_(name), languages_(std::move(languages)), individual_(individual){
    for (int i = 0; i < languages_.size(); ++i){
        languages_[i]->set_individual_price();
    }
};

std::string Student::get_name() const {
    return name_;
}

const std::vector<std::unique_ptr<Language>>& Student::get_languages() const {
    return languages_;
}

void Student::delete_language(int index){
    languages_.erase(languages_.begin() + index);
}

void Student::get_info() const {
    
    for (int i = 0; i < languages_.size(); ++i){
        std::cout << name_ << ": " << languages_[i]->get_name() << ", level is " << languages_[i]->get_level() << ", periods left " 
        << languages_[i]->get_intensity().get_period() << ", price is " << languages_[i]->get_price() << std::endl;
    }
    std::cout << std::endl;
}

bool Student::is_individual() const {
    return individual_;
}
#include<iostream>
#include"student.h"
#include"languages.h"
#include"randoms.h"
#include"group_manager.h"

Student::Student(const std::string& name,  std::vector <std::unique_ptr<Language>> languages) 
    : name_(name), languages_(std::move(languages)) {}

Student::Student(const std::string& name,  std::vector <std::unique_ptr<Language>> languages, bool individual) 
    : name_(name), languages_(std::move(languages)), individual_(individual) {
    for (auto& language : languages_) {
        language->set_individual_price();
    }
}

std::string Student::get_name() const {
    return name_;
}

const std::vector<std::unique_ptr<Language>>& Student::get_languages() const {
    return languages_;
}

void Student::delete_language(int index) {
    if (index >= 0 && index < languages_.size()) {
        languages_.erase(languages_.begin() + index);
    }
}

void Student::get_info() const {
    for (const auto& language : languages_) {
        std::cout << name_ << ": " << language->get_name() 
                  << ", level is " << language->get_level() 
                  << ", periods left " << language->get_intensity().get_period_left() 
                  << ", price is " << language->get_price() << std::endl;
    }
    std::cout << std::endl;
}

bool Student::is_individual() const {
    return individual_;
}

void Student::is_graduate(GroupManager& manager) {
    std::vector<int> languages_to_remove;
    
    for (int i = languages_.size() - 1; i >= 0; --i) {
        if (languages_[i]->get_intensity().get_period_left() == 0 && 
            languages_[i]->get_level() == 3) {
            languages_to_remove.push_back(i);
        }
        else if (languages_[i]->get_intensity().get_period_left() == 0 && 
                 individual_) {
            languages_to_remove.push_back(i);
        }
    }
    
    for (int i : languages_to_remove) {
        std::string lang_name = languages_[i]->get_name();
        manager.delete_student(*this, lang_name);
        languages_.erase(languages_.begin() + i);
    }
}

void Student::change_intensity(GroupManager& manager) {
    for (int i = 0; i < languages_.size(); ++i) {
        languages_[i]->get_intensity().minus_period(); 
        
        if (languages_[i]->get_intensity().get_period_left() == 0 && 
            languages_[i]->get_level() < 3) {
            
            std::string lang_name = languages_[i]->get_name();
            int old_level = languages_[i]->get_level();
            Intensity old_intensity = languages_[i]->get_intensity();
            
            auto new_language = create_language(lang_name, old_level + 1, 
                                              Intensity(languages_[i]->get_intensity().get_type()));
            
            manager.delete_student(*this, lang_name);
            languages_[i] = std::move(new_language);
            manager.add_student(*this);
        }
    }
}

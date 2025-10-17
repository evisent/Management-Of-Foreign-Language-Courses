#include "group_manager.h"
#include "languages.h"
#include "student.h"
#include <map>
#include <vector>
#include <iostream>

void GroupManager::add_student(Student& student){
    for (const auto& language : student.get_languages()) {
        groups[language->get_name()][language->get_level()][language->get_intensity()].push_back(&student);
    }
}

void GroupManager::delete_student(Student& student, const std::string& language_name){
    for (const auto& language : student.get_languages()) {
        if (language->get_name() == language_name) {
            std::vector<Student*>& students = groups[language_name][language->get_level()][language->get_intensity()];               
            for (int i = 0; i < students.size(); ++i) {
                if (students[i] == &student) {
                    students.erase(students.begin() + i);
                    break;
                }
            }
            break;
        }
    }
}


std::vector<Student*> GroupManager::get_group(const std::string& language, int level, const Intensity& intensity) const {
    auto language_it = groups.find(language);  // find by language
    if (language_it == groups.end()) return {};
    
    auto level_it = language_it->second.find(level);        // find by level
    if (level_it == language_it->second.end()) return {};
    
    auto intensity_it = level_it->second.find(intensity);   // find by intensity
    if (intensity_it == level_it->second.end()) return {};
    
    return intensity_it->second;       
}

void GroupManager::print_all() const {
    for (const auto& [language, levels] : groups) {   
        for (const auto& [level, intensities] : levels) {
            for (const auto& [intensity, students] : intensities) {
                if (!students.empty()){
                    std::cout << language
                    << ", level " << level
                    << ", Intensity " << intensity.get_period() 
                    << " has " << students.size() 
                    << " students:" << std::endl;
                }
                
                
                for (const auto& student : students) {
                    std::cout << student->get_name() << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
}
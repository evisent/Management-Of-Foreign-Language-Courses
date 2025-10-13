#pragma once
#include "languages.h"
#include "student.h"
#include <map>
#include <vector>
#include <iostream>

class GroupManager {
public:
    void add_student(Student& student) {
        for (const auto& language : student.get_languages()) {
            groups[language->get_name()]
                  [language->get_level()]
                  [language->get_intensity()]
                  .push_back(&student);
        }
    }
    
    std::vector<Student*> get_group(const std::string& language, int level, const Intensity& intensity) const {
        auto language_it = groups.find(language);  // find by language
        if (language_it == groups.end()) return {};
        
        auto level_it = language_it->second.find(level);        // find by level
        if (level_it == language_it->second.end()) return {};
        
        auto intensity_it = level_it->second.find(intensity);   // find by intensity
        if (intensity_it == level_it->second.end()) return {};
        
        return intensity_it->second;       
    }
    
    void print_all() const {
        for (const auto& [language, levels] : groups) {   
            for (const auto& [level, intensities] : levels) {
                for (const auto& [intensity, students] : intensities) {
                    std::cout << language 
                              << ", level " << level 
                              << ", Intensity " << intensity.get_period() 
                              << " has " << students.size() << " students:" << std::endl;
                    
                    for (const auto& student : students) {
                        std::cout << student->get_name() << std::endl;
                    }
                    std::cout << std::endl;
                }
            }
        }
    }

private:
    std::map<std::string, std::map<int, std::map<Intensity, std::vector<Student*>>>> groups; // <language, <level, <intensity, students>>>
};


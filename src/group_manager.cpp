#include "group_manager.h"
#include "student.h"
#include "json_utils.h"

void GroupManager::add_student(Student& student){
    for (const auto& language : student.get_languages()) {
        if (groups[language->get_name()][language->get_level()][language->get_intensity()].empty()) ++amount_;
        groups[language->get_name()][language->get_level()][language->get_intensity()].push_back(&student);
    }
}

void GroupManager::delete_student(Student& student, const std::string& language_name){
    for (const auto& language : student.get_languages()) {
        if (language->get_name() == language_name) {
            auto& student_vec = groups[language_name][language->get_level()][language->get_intensity()];         
            if (student_vec.empty()) return;       
            for (int i = 0; i < student_vec.size(); ++i) {
                if (student_vec[i] == &student) {
                    student_vec.erase(student_vec.begin() + i);
                    if (student_vec.empty()) --amount_;
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

std::vector<Student*> GroupManager::get_unique_students() const{
    std::vector<Student*> unique;
    for (const auto& [language, levels] : groups) {   
        for (const auto& [level, intensities] : levels) {
            for (const auto& [intensity, students] : intensities) {
                if (!students.empty()) {  // ⚠️ ДОБАВЬ ПРОВЕРКУ!
                    unique.push_back(students[0]);
                }
            }
        }
    }
    return unique;
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

int GroupManager::get_amount() const {
    return amount_;
}

std::string GroupManager::get_groups_json() const {
    std::string json_body = "{\"groups\":[";
    
    bool first_group = true;
    int group_count = 0;
    
    for (const auto& [language, levels] : groups) {   
        for (const auto& [level, intensities] : levels) {
            for (const auto& [intensity, students] : intensities) {
                if (!students.empty()) {
                    group_count++;
                    if (!first_group) {
                        json_body += ",";
                    }
                    first_group = false;
                    
                    json_body += "{";
                    json_body += "\"language\":\"" + JsonEscape(language) + "\",";
                    json_body += "\"level\":" + std::to_string(level) + ",";
                    json_body += "\"periods_left\":" + std::to_string(intensity.get_period()) + ","; // общее количество периодов для группы
                    json_body += "\"intensity_name\":\"" + JsonEscape(intensity.get_name()) + "\",";
            
                    json_body += "\"students\":[";
                    
                    bool first_student = true;
                    for (const auto& student : students) {
                        if (!first_student) json_body += ",";
                        first_student = false;
                        
                        json_body += "{";
                        json_body += " \"name\":\"" + JsonEscape(student->get_name()) + "\"";
                        
                        for (const auto& studentLanguage : student->get_languages()){
                            if (studentLanguage->get_name() == language){
                                json_body += ",\"price\":" + std::to_string(studentLanguage->get_price());
                                json_body += ",\"student_periods_left\":" + std::to_string(studentLanguage->get_intensity().get_period_left()); // индивидуальный periods_left
                                break; 
                            }
                        }
                        
                        json_body += "}"; 
                    }
                    
                    json_body += "]}";
                }
            }
        }
    }
    
    json_body += "]}";
    std::cout << "DEBUG: Generated JSON for " << group_count << " groups" << std::endl;
    return json_body;
}

void GroupManager::reset() {
    groups.clear();  
    amount_ = 0;
}
#include "group_manager.h"
#include "student.h"
#include "json_utils.h"
#include "randoms.h"

void GroupManager::add_student(Student& student){
    for (const auto& language : student.get_languages()) {
        if (groups[language->get_name()][language->get_level()][language->get_intensity()].empty()) ++amount_;
        groups[language->get_name()][language->get_level()][language->get_intensity()].push_back(&student);
    }
}

void GroupManager::delete_student(Student& student, const std::string& language_name) {
    auto lang_it = groups.find(language_name);
    if (lang_it == groups.end()) return;
    
    auto& levels = lang_it->second;
    for (auto level_it = levels.begin(); level_it != levels.end(); ) {
        auto& intensities = level_it->second;
        for (auto intensity_it = intensities.begin(); intensity_it != intensities.end(); ) {
            auto& students_vec = intensity_it->second;
            auto student_it = std::find(students_vec.begin(), students_vec.end(), &student);
            
            if (student_it != students_vec.end()) {
                students_vec.erase(student_it);
                std::cout << "DEBUG: Deleted " << student.get_name() 
                          << " from " << language_name << " L" << level_it->first 
                          << " " << intensity_it->first.get_type() << std::endl;
            }
            
            if (students_vec.empty()) {
                intensity_it = intensities.erase(intensity_it);
            } else {
                ++intensity_it;
            }
        }
        
        if (intensities.empty()) {
            level_it = levels.erase(level_it);
        } else {
            ++level_it;
        }
    }
    
    if (levels.empty()) {
        groups.erase(lang_it);
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
                if (!students.empty()) { 
                    unique.push_back(students[0]);
                }
            }
        }
    }
    return unique;
}

bool GroupManager::is_small(const std::unique_ptr<Language>& language){
    if(groups[language->get_name()][language->get_level()][language->get_intensity()].size() < 5){
        return true;
    }
    return false;
}

void GroupManager::to_individual(std::vector<std::unique_ptr<Student>>& students_vec, std::vector<std::unique_ptr<Student>>& individual_students) {            
    std::vector<Student*> to_delete;
    std::vector<std::string> langs;

    for (auto& [language, levels] : groups) {   
        for (auto& [level, intensities] : levels) {
            for (auto& [intensity, students] : intensities) {
                if (students.size() < 5 && !students.empty()) {
                    for (Student* student : students) {
                        to_delete.push_back(student);
                        langs.push_back(language);
                        std::vector <std::unique_ptr<Language>> languages;
                        for (auto& lng: student->get_languages()){
                            languages.push_back(create_language(lng->get_name(), lng->get_level(), lng->get_intensity()));
                        }
                        individual_students.push_back(std::make_unique<Student>(student->get_name(), std::move(languages), true));
                    }
                }
            }
        }
    }

    for (int i = 0; i < to_delete.size(); ++i){
        delete_student(*to_delete[i], langs[i]);
    }

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
    return json_body;
}

void GroupManager::reset() {
    groups.clear();  
    amount_ = 0;
}
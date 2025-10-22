#include "group_manager.h"
#include "student.h"
#include "json_utils.h"
#include <iostream>
#include <algorithm>

void GroupManager::add_student(Student& student) {
    for (const auto& language : student.get_languages()) {
        std::string lang_name = language->get_name();
        int level = language->get_level();
        Intensity intensity = language->get_intensity();
        
        auto& groups_list = groups[lang_name][level][intensity];
        
        // Ищем группу с местом (<10 человек)
        bool added = false;
        for (auto& group : groups_list) {
            if (group.size() < 10) {
                group.push_back(&student);
                added = true;
                break;
            }
        }
        
        // Если не нашли подходящую группу, создаем новую
        if (!added) {
            groups_list.push_back({&student});
            ++amount_;
        }
        
        // Проверяем нужно ли разделить группы
        splitGroupIfNeeded(lang_name, level, intensity);
    }
}

void GroupManager::splitGroupIfNeeded(const std::string& language, int level, const Intensity& intensity) {
    auto& groups_list = groups[language][level][intensity];
    bool needs_recheck = true;
    
    while (needs_recheck) {
        needs_recheck = false;
        
        for (size_t i = 0; i < groups_list.size(); i++) {
            auto& group = groups_list[i];
            if (group.size() > 10) {
                std::cout << "Splitting group: " << language 
                          << " level " << level 
                          << " (size: " << group.size() << ")" << std::endl;
                
                // Создаем копию студентов для разделения
                std::vector<Student*> students_to_split = group;
                
                // Очищаем оригинальную группу
                group.clear();
                
                // Разделяем студентов на две группы
                size_t first_group_size = students_to_split.size() / 2;
                size_t second_group_size = students_to_split.size() - first_group_size;
                
                // Заполняем первую группу (оригинальную)
                for (size_t j = 0; j < first_group_size; j++) {
                    group.push_back(students_to_split[j]);
                }
                
                // Создаем вторую группу
                std::vector<Student*> second_group;
                for (size_t j = first_group_size; j < students_to_split.size(); j++) {
                    second_group.push_back(students_to_split[j]);
                }
                groups_list.push_back(second_group);
                
                amount_++; // Увеличиваем счетчик групп
                
                std::cout << "Split into groups of " << first_group_size 
                          << " and " << second_group_size << " students" << std::endl;
                
                needs_recheck = true; // Нужно проверить снова, так как структура изменилась
                break; // Выходим из цикла, чтобы начать проверку заново
            }
        }
    }
}

void GroupManager::optimizeAllGroups() {
    bool changes_made;
    
    do {
        changes_made = false;
        
        // Разделяем большие группы
        for (auto& language_entry : groups) {   
            auto& language = language_entry.first;
            auto& levels = language_entry.second;
            
            for (auto& level_entry : levels) {
                auto& level = level_entry.first;
                auto& intensities = level_entry.second;
                
                for (auto& intensity_entry : intensities) {
                    auto& intensity = intensity_entry.first;
                    auto& groups_list = intensity_entry.second;
                    
                    for (const auto& group : groups_list) {
                        if (group.size() > 10) {
                            int old_amount = amount_;
                            splitGroupIfNeeded(language, level, intensity);
                            if (amount_ != old_amount) {
                                changes_made = true;
                            }
                            break; // Переходим к следующей комбинации
                        }
                    }
                }
            }
        }
        
        // Объединяем маленькие группы (только если не было разделения)
        if (!changes_made) {
            int old_amount = amount_;
            mergeSmallGroups();
            if (amount_ != old_amount) {
                changes_made = true;
            }
        }
        
    } while (changes_made); // Повторяем пока есть изменения
    
    std::cout << "Optimization complete. Total groups: " << amount_ << std::endl;
}



void GroupManager::mergeSmallGroups() {
    int old_amount = amount_;
    
    // Сначала удаляем маленькие группы и получаем студентов для конвертации
    auto students_to_convert = find_small_groups();
    
    // Затем объединяем оставшиеся группы
    for (auto& language_entry : groups) {   
        auto& language = language_entry.first;
        auto& levels = language_entry.second;
        
        for (auto& level_entry : levels) {
            auto& level = level_entry.first;
            auto& intensities = level_entry.second;
            
            for (auto& intensity_entry : intensities) {
                auto& intensity = intensity_entry.first;
                auto& groups_list = intensity_entry.second;
                
                // Объединяем группы с одинаковыми параметрами
                if (groups_list.size() > 1) {
                    // Собираем всех студентов
                    std::vector<Student*> all_students;
                    for (auto& group : groups_list) {
                        all_students.insert(all_students.end(), group.begin(), group.end());
                    }
                    
                    int total_students = all_students.size();
                    int old_group_count = groups_list.size();
                    
                    std::cout << "Merging " << old_group_count << " groups for " 
                              << language << " level " << level << " (" 
                              << total_students << " students)" << std::endl;
                    
                    // Очищаем и пересоздаем группы оптимального размера
                    groups_list.clear();
                    
                    if (total_students <= 10) {
                        groups_list.push_back(all_students);
                    } else {
                        int first_group_size = total_students / 2;
                        int second_group_size = total_students - first_group_size;
                        
                        std::vector<Student*> group1, group2;
                        for (int i = 0; i < first_group_size; i++) {
                            group1.push_back(all_students[i]);
                        }
                        for (int i = first_group_size; i < total_students; i++) {
                            group2.push_back(all_students[i]);
                        }
                        
                        groups_list.push_back(group1);
                        groups_list.push_back(group2);
                    }
                    
                    amount_ -= (old_group_count - groups_list.size());
                }
            }
        }
    }
    
    if (amount_ != old_amount) {
        std::cout << "Groups count changed: " << old_amount << " -> " << amount_ << std::endl;
    }
    
    // Возвращаем студентов для конвертации (через отдельный метод)
    // В реальной реализации нужно сохранить их где-то
}

std::vector<Student*> GroupManager::find_small_groups() {
    std::vector<Student*> students_from_small_groups;
    
    for (auto& language_entry : groups) {   
        auto& language = language_entry.first;
        auto& levels = language_entry.second;
        
        for (auto& level_entry : levels) {
            auto& level = level_entry.first;
            auto& intensities = level_entry.second;
            
            for (auto& intensity_entry : intensities) {
                auto& intensity = intensity_entry.first;
                auto& groups_list = intensity_entry.second;
                
                // Находим маленькие группы (<5 студентов)
                for (const auto& group : groups_list) {
                    if (group.size() < 5 && group.size() > 0) {
                        std::cout << "Small group found: " << language 
                                  << " level " << level << " with " 
                                  << group.size() << " students" << std::endl;
                        
                        // Добавляем студентов в список для конвертации (без удаления из группы)
                        students_from_small_groups.insert(students_from_small_groups.end(), 
                                                         group.begin(), group.end());
                    }
                }
            }
        }
    }
    
    return students_from_small_groups;
}

void GroupManager::delete_student(Student& student, const std::string& language_name) {
    for (const auto& language : student.get_languages()) {
        if (language->get_name() == language_name) {
            int level = language->get_level();
            Intensity intensity = language->get_intensity();
            
            auto& groups_list = groups[language_name][level][intensity];
            
            // Ищем студента во всех группах этой комбинации
            bool student_removed = false;
            for (auto& group : groups_list) {
                auto it = std::find(group.begin(), group.end(), &student);
                if (it != group.end()) {
                    group.erase(it);
                    student_removed = true;
                    
                    // Если группа стала пустой, удаляем ее
                    if (group.empty()) {
                        groups_list.erase(
                            std::remove_if(groups_list.begin(), groups_list.end(),
                                [](const std::vector<Student*>& g) { return g.empty(); }),
                            groups_list.end()
                        );
                        amount_--;
                    }
                    break;
                }
            }
            
            // После удаления проверяем оптимизацию
            if (student_removed) {
                mergeSmallGroups();
            }
            break;
        }
    }
}

int GroupManager::get_amount() const {
    // Пересчитываем количество групп на всякий случай
    int actual_count = 0;
    
    for (const auto& language_entry : groups) {   
        auto& levels = language_entry.second;
        
        for (const auto& level_entry : levels) {
            auto& intensities = level_entry.second;
            
            for (const auto& intensity_entry : intensities) {
                auto& groups_list = intensity_entry.second;
                
                for (const auto& group : groups_list) {
                    if (!group.empty()) {
                        actual_count++;
                    }
                }
            }
        }
    }
    
    // Если расхождение, выводим предупреждение
    if (actual_count != amount_) {
        std::cout << "WARNING: Group count mismatch! amount_=" << amount_ 
                  << ", actual=" << actual_count << std::endl;
    }
    
    return actual_count;
}

// Остальные методы без изменений...
std::vector<Student*> GroupManager::get_group(const std::string& language, int level, const Intensity& intensity) const {
    auto language_it = groups.find(language);
    if (language_it == groups.end()) return {};
    
    auto level_it = language_it->second.find(level);
    if (level_it == language_it->second.end()) return {};
    
    auto intensity_it = level_it->second.find(intensity);
    if (intensity_it == level_it->second.end()) return {};
    
    // Возвращаем первую непустую группу (для обратной совместимости)
    for (const auto& group : intensity_it->second) {
        if (!group.empty()) {
            return group;
        }
    }
    return {};
}

std::vector<Student*> GroupManager::get_unique_students() const {
    std::vector<Student*> unique;
    std::unordered_set<Student*> seen_students;
    
    for (const auto& [language, levels] : groups) {   
        for (const auto& [level, intensities] : levels) {
            for (const auto& [intensity, groups_list] : intensities) {
                // Берем только первую непустую группу для каждой комбинации
                // и только первого студента из этой группы
                for (const auto& group : groups_list) {
                    if (!group.empty()) {
                        // Берем первого студента из группы как представителя
                        Student* representative = group[0];
                        if (seen_students.insert(representative).second) {
                            unique.push_back(representative);
                        }
                        // Прерываем после первого непустого студента в этой комбинации
                        break;
                    }
                }
            }
        }
    }
    return unique;
}

void GroupManager::print_all() const {
    int total_groups = 0;
    int total_students = 0;
    
    for (const auto& language_entry : groups) {   
        auto& language = language_entry.first;
        auto& levels = language_entry.second;
        
        for (const auto& level_entry : levels) {
            auto& level = level_entry.first;
            auto& intensities = level_entry.second;
            
            for (const auto& intensity_entry : intensities) {
                auto& intensity = intensity_entry.first;
                auto& groups_list = intensity_entry.second;
                
                int group_num = 1;
                for (const auto& group : groups_list) {
                    if (!group.empty()) {
                        std::cout << "Group " << group_num << ": " << language
                                  << ", level " << level
                                  << ", " << intensity.get_name()
                                  << " (" << intensity.get_period() << " periods)"
                                  << " has " << group.size() 
                                  << " students:" << std::endl;
                        
                        for (const auto& student : group) {
                            std::cout << "  - " << student->get_name() << std::endl;
                        }
                        std::cout << std::endl;
                        
                        total_groups++;
                        total_students += group.size();
                        group_num++;
                    }
                }
            }
        }
    }
    
    std::cout << "Total: " << total_groups << " groups, " 
              << total_students << " students" << std::endl;
    std::cout << "Amount counter: " << amount_ << std::endl;
}

std::string GroupManager::get_groups_json() const {
    std::string json_body = "{\"groups\":[";
    
    bool first_group = true;
    int group_count = 0;
    
    for (const auto& language_entry : groups) {   
        auto& language = language_entry.first;
        auto& levels = language_entry.second;
        
        for (const auto& level_entry : levels) {
            auto& level = level_entry.first;
            auto& intensities = level_entry.second;
            
            for (const auto& intensity_entry : intensities) {
                auto& intensity = intensity_entry.first;
                auto& groups_list = intensity_entry.second;
                
                int group_index = 1;
                for (const auto& students : groups_list) {
                    if (!students.empty()) {
                        group_count++;
                        if (!first_group) {
                            json_body += ",";
                        }
                        first_group = false;
                        
                        json_body += "{";
                        json_body += "\"language\":\"" + JsonEscape(language) + "\",";
                        json_body += "\"level\":" + std::to_string(level) + ",";
                        json_body += "\"periods_left\":" + std::to_string(intensity.get_period()) + ",";
                        json_body += "\"intensity_name\":\"" + JsonEscape(intensity.get_name()) + "\",";
                        json_body += "\"group_number\":" + std::to_string(group_index) + ",";
                
                        json_body += "\"students\":[";
                        
                        bool first_student = true;
                        for (const auto& student : students) {
                            if (!first_student) json_body += ",";
                            first_student = false;
                            
                            json_body += "{";
                            json_body += "\"name\":\"" + JsonEscape(student->get_name()) + "\"";
                            
                            for (const auto& studentLanguage : student->get_languages()){
                                if (studentLanguage->get_name() == language){
                                    json_body += ",\"price\":" + std::to_string(studentLanguage->get_price());
                                    json_body += ",\"student_periods_left\":" + std::to_string(studentLanguage->get_intensity().get_period_left());
                                    break; 
                                }
                            }
                            
                            json_body += "}"; 
                        }
                        
                        json_body += "]}";
                        group_index++;
                    }
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
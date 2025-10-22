#pragma once
#include <chrono> 
#include <random>
#include <string>
#include "names.h"
#include "student.h"
#include "languages.h"
#include "intensity.h"
#include "group_manager.h"
#include "randoms.h"

void step(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students,
          std::vector<std::unique_ptr<Student>>& individual_students) {
    std::cout << "=== SIMULATION STEP ===" << std::endl;
    
    // Обновляем интенсивность групповых студентов
    for (int i = 0; i < students.size(); ++i){
        students[i]->change_intensity(manager);
        students[i]->is_graduate(manager, students, i);
    }
    
    // Обновляем интенсивность индивидуальных студентов
    for (int i = 0; i < individual_students.size(); ++i){
        // Для индивидуальных студентов просто уменьшаем периоды
        for (auto& language : individual_students[i]->get_languages()) {
            language->get_intensity().minus_period();
        }
        
        // Проверяем завершение курса для индивидуальных студентов
        bool should_remove = false;
        for (int j = individual_students[i]->get_languages().size() - 1; j >= 0; --j) {
            const auto& language = individual_students[i]->get_languages()[j];
            if (language->get_intensity().get_period_left() == 0 && 
                language->get_level() == 3) {
                
                std::cout << individual_students[i]->get_name() 
                          << " completed individual course: " << language->get_name() << std::endl;
                
                if (individual_students[i]->get_languages().size() == 1) {
                    // Если это последний язык - удаляем студента
                    should_remove = true;
                    break;
                } else {
                    // Удаляем только завершенный язык
                    individual_students[i]->delete_language(j);
                }
            }
        }
        
        if (should_remove) {
            std::cout << individual_students[i]->get_name() << " graduated from all individual lessons" << std::endl;
            individual_students.erase(individual_students.begin() + i);
            i--;
        }
    }

    // Случайное удаление студентов
    rand_leave(manager, students, individual_students);

    // Добавление новых студентов
    rand_add(manager, students);

    // Находим студентов из маленьких групп
    auto students_from_small_groups = manager.find_small_groups();
    
    if (!students_from_small_groups.empty()) {
        std::cout << "Found " << students_from_small_groups.size() 
                  << " students from small groups to convert to individual lessons" << std::endl;
    }
    
    // Переводим студентов из маленьких групп на индивидуальное обучение
    for (auto* student : students_from_small_groups) {
        // Находим студента в основном векторе
        auto it = std::find_if(students.begin(), students.end(),
            [student](const std::unique_ptr<Student>& s) {
                return s.get() == student;
            });
        
        if (it != students.end() && !(*it)->is_individual()) {
            std::cout << "Converting " << (*it)->get_name() << " to individual lessons" << std::endl;
            
            // Переводим на индивидуальное обучение
            (*it)->convert_to_individual();
            
            // Удаляем из всех групп
            for (const auto& language : (*it)->get_languages()) {
                manager.delete_student(**it, language->get_name());
            }
            
            // Перемещаем в вектор индивидуальных студентов
            individual_students.push_back(std::move(*it));
            students.erase(it);
            
            std::cout << "Transferred " << individual_students.back()->get_name() 
                      << " to individual lessons" << std::endl;
        }
    }

    // Оптимизация групп после всех изменений
    manager.optimizeAllGroups();
    
    std::cout << "Final students: " << students.size() << " group, " 
              << individual_students.size() << " individual" << std::endl;
    std::cout << "=====================" << std::endl;
}
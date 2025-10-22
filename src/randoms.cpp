#include <iostream>
#include <chrono> 
#include <random>
#include <string>
#include "names.h"
#include "student.h"
#include "languages.h"
#include "intensity.h"
#include "group_manager.h"

int my_rand() {
    std::random_device rd;
    std::mt19937 rng(rd());  
    std::uniform_int_distribution<int> dist(1, INT_MAX);
    return dist(rng);
}

std::string rand_name(){
    int index = my_rand() % NAMES.size();
    std::string name = NAMES[index];
    NAMES.erase(NAMES.begin() + index);
    return name;
}

int rand_level(){
    int level = my_rand() % 3;
    switch (level){
        case 0: return 1;  
        case 1: return 2;    
        case 2: return 3;
        default: return 1;
    }
}

Intensity rand_intensity(){
    int intensity = my_rand() % 3;
    switch (intensity){
        case 0: return Intensity(1);  
        case 1: return Intensity(2);    
        case 2: return Intensity(3);
        default: return Intensity(1);
    }
}

std::unique_ptr<Language> rand_language(){
    int language = my_rand() % 4;  
    switch(language) {
        case 0: return std::make_unique<English>(rand_level(), rand_intensity());
        case 1: return std::make_unique<Spanish>(rand_level(), rand_intensity());
        case 2: return std::make_unique<Chinese>(rand_level(), rand_intensity());
        case 3: return std::make_unique<German>(rand_level(), rand_intensity());
        default: return std::make_unique<English>(rand_level(), rand_intensity());
    }
}

std::vector<std::unique_ptr<Language>> rand_languages(){
    int chance = my_rand() % 100; 
    std::vector<std::unique_ptr<Language>> languages;
    languages.push_back(rand_language());
    
    if (chance >= 80 && chance < 95) {  // 2 языка (15%)
        std::unique_ptr<Language> language2 = rand_language();
        while (language2->get_name() == languages[0]->get_name()) {
            language2 = rand_language();
        }
        languages.push_back(std::move(language2));
    } 
    else if (chance >= 95) {  // 3 языка (5%)
        std::unique_ptr<Language> language2 = rand_language();
        while (language2->get_name() == languages[0]->get_name()) {
            language2 = rand_language();
        }
        languages.push_back(std::move(language2));
        
        std::unique_ptr<Language> language3 = rand_language();
        while (language3->get_name() == languages[0]->get_name() || 
               language3->get_name() == languages[1]->get_name()) {
            language3 = rand_language();
        }
        languages.push_back(std::move(language3));
    }
    
    return languages;
}                                                            

std::unique_ptr<Student> rand_student(){
    return std::make_unique<Student>(rand_name(), rand_languages());
}


void rand_leave(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students,
                std::vector<std::unique_ptr<Student>>& individual_students) {
    int chance = my_rand() % 100;

    // Удаление групповых студентов
    if (chance < 5 && !students.empty()){
        int rand_student = my_rand() % students.size();
        std::cout << "DELETED: " << students[rand_student]->get_name() << '\n';

        Student& student = *students[rand_student];
        
        const auto& languages = student.get_languages();
        
        // Удаляем студента из всех групп
        for (const auto& language : languages) {
            manager.delete_student(student, language->get_name());
        }
        
        // Полностью удаляем студента
        students.erase(students.begin() + rand_student);
    }
    
    // Удаление индивидуальных студентов
    if (chance < 2 && !individual_students.empty()) {
        int rand_student = my_rand() % individual_students.size();
        std::cout << "DELETED INDIVIDUAL: " << individual_students[rand_student]->get_name() << '\n';
        individual_students.erase(individual_students.begin() + rand_student);
    }
}

std::unique_ptr<Language> create_language(const std::string& name, int level, const Intensity& intensity) {
    if (name == "English") return std::make_unique<English>(level, intensity);
    else if (name == "Spanish") return std::make_unique<Spanish>(level, intensity);
    else if (name == "French") return std::make_unique<French>(level, intensity);
    else if (name == "German") return std::make_unique<German>(level, intensity);
    else if (name == "Chinese") return std::make_unique<Chinese>(level, intensity);
    else if (name == "Arabian") return std::make_unique<Arabian>(level, intensity);
    else return std::make_unique<English>(level, intensity);
}


void rand_add(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students){
    if (students.size() >= 30) return;
    
    std::vector<std::unique_ptr<Student>> students_to_add;
    auto unique_students = manager.get_unique_students();
    
    for(auto cur_student : unique_students){
        int amount = my_rand() % 100;
        int cycles = 0;
        if (amount < 60 && students.size() + students_to_add.size() < 30) cycles = 1;
        else if (amount < 80 && students.size() + students_to_add.size() < 29) cycles = 2; 
        else cycles = 0;
        
        for (int i = 0; i < cycles; ++i){
            std::vector<std::unique_ptr<Language>> languages_copy;
            for (const auto& language : cur_student->get_languages()) {
                auto new_lang = create_language(language->get_name(), language->get_level(), 
                                              Intensity(language->get_intensity().get_type()));
                languages_copy.push_back(std::move(new_lang));
            }
            
            auto new_student = std::make_unique<Student>(rand_name(), std::move(languages_copy));
            students_to_add.push_back(std::move(new_student));
        }    
    }
    
    for (auto& student : students_to_add) {
        manager.add_student(*student);
        students.push_back(std::move(student));
    }

    if (!students_to_add.empty()) {
        std::cout << "Added " << students_to_add.size() << " new student(s) across " 
                  << unique_students.size() << " groups" << std::endl;
    }
}   

std::vector<std::unique_ptr<Student>> fifteen_students(){
    std::vector<std::unique_ptr<Student>> students;
    
    std::string first_lang_name, second_lang_name, third_lang_name;
    
    auto lang1 = rand_language();
    first_lang_name = lang1->get_name();
    
    auto lang2 = rand_language();
    while(lang2->get_name() == first_lang_name){
        lang2 = rand_language();
    }
    second_lang_name = lang2->get_name();
    
    auto lang3 = rand_language();
    while(lang3->get_name() == first_lang_name || lang3->get_name() == second_lang_name){
        lang3 = rand_language();
    }
    third_lang_name = lang3->get_name();
    
    for (int i = 0; i < 5; ++i){
        auto lang1_copy = std::move(create_language(first_lang_name, lang1->get_level(), lang1->get_intensity()));
        std::vector<std::unique_ptr<Language>> langs1;
        langs1.push_back(std::move(lang1_copy));
        students.push_back(std::make_unique<Student>(rand_name(), std::move(langs1)));
        
        auto lang2_copy = std::move(create_language(second_lang_name, lang2->get_level(), lang2->get_intensity()));
        std::vector<std::unique_ptr<Language>> langs2;
        langs2.push_back(std::move(lang2_copy));
        students.push_back(std::make_unique<Student>(rand_name(), std::move(langs2)));
        
        auto lang3_copy = std::move(create_language(third_lang_name, lang3->get_level(), lang3->get_intensity()));
        std::vector<std::unique_ptr<Language>> langs3;
        langs3.push_back(std::move(lang3_copy));
        students.push_back(std::make_unique<Student>(rand_name(), std::move(langs3)));
    }

    return students;
}

#include <chrono> 
#include <random>
#include <string>
#include <climits>
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
    
    if (chance >= 100) {  // 2 языка (15%)
        std::unique_ptr<Language> language2 = rand_language();
        while (language2->get_name() == languages[0]->get_name()) {
            language2 = rand_language();
        }
        languages.push_back(std::move(language2));
    } 
    else if (chance >= 100) {  // 3 языка (5%)
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

std::unique_ptr<Language> create_language(const std::string& name, int level, const Intensity& intensity) {
    if (name == "English") return std::make_unique<English>(level, intensity);
    else if (name == "Spanish") return std::make_unique<Spanish>(level, intensity);
    else if (name == "French") return std::make_unique<French>(level, intensity);
    else if (name == "German") return std::make_unique<German>(level, intensity);
    else if (name == "Chinese") return std::make_unique<Chinese>(level, intensity);
    else if (name == "Arabian") return std::make_unique<Arabian>(level, intensity);
    else return std::make_unique<English>(level, intensity);
}

void rand_add(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students, std::vector<std::unique_ptr<Student>>& individual_students){
    if (students.size() + individual_students.size() >= 30) {
        return;
    }
    int available_slots = 30 - students.size() - individual_students.size();
    
    int amount = my_rand() % 100;
    int cycles = 0;
    
    if (amount < 20){
        cycles = 1;
    } else if(amount < 40){
        cycles = 2;
    } else if(amount < 60){
        cycles = 3;
    } else if(amount < 80){
        cycles = 4;
    } else {
        cycles = 5;
    }
    
    cycles = std::min(cycles, available_slots);
    
    if (cycles == 0) {
        return;
    }
        
    for (int i = 0; i < cycles; ++i){
        try {
            auto new_student = std::make_unique<Student>(rand_name(), rand_languages());
            
            for (const auto& language: new_student->get_languages()){
                if(manager.is_small(language)){
                    individual_students.push_back(std::move(new_student));
                    //std::cout << "Individual added\n";
                } else{
                    if (new_student) {
                        manager.add_student(*new_student);
                        students.push_back(std::move(new_student));
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Error creating student: " << e.what() << std::endl;
        }
    }
    
}  

void rand_leave(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students, std::vector<std::unique_ptr<Student>>& individual_students){
    int chance = my_rand() % 100;

    if (chance < 5 && !students.empty()){
        std::cout << "STUDENT LEAVED\n";

        int rand_student = my_rand() % students.size();

        Student& student = *students[rand_student];
        
        const auto& languages = student.get_languages();
        
        int rand_language = my_rand() % languages.size();
        std::string language_name = languages[rand_language]->get_name();
        
        manager.delete_student(student, language_name);
        
        if (languages.size() == 1){
            students.erase(students.begin() + rand_student);
        } else {
            students[rand_student]->delete_language(rand_language);
        }
    }
    if (chance >= 5 && chance < 15 && !individual_students.empty()){
        std::cout << "INDIVIDUAL STUDENT LEAVED\n";
        int rand_student = my_rand() % individual_students.size();

        Student& student = *individual_students[rand_student];
        
        const auto& languages = student.get_languages();
        
        int rand_language = my_rand() % languages.size();
        std::string language_name = languages[rand_language]->get_name();
                
        if (languages.size() == 1){
            individual_students.erase(individual_students.begin() + rand_student);
        } else {
            individual_students[rand_student]->delete_language(rand_language);
        }
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

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
    std::mt19937 rng(rd());  // локальный для функции
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




void rand_leave(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students){
    int chance = my_rand() % 100;

    if (chance <= 100 && !students.empty()){
        int rand_student = my_rand() % students.size();
        std::cout << "DELETED: " << students[rand_student]->get_name() << '\n';

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
}



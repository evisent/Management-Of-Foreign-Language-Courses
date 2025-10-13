#include <iostream>
#include <vector>
#include <memory>
#include "languages.h"
#include "student.h"
#include "group_manager.h"

int main() {

    std::vector<std::unique_ptr<Language>> student1_langs; // vector of languages for one student
    student1_langs.push_back(std::make_unique<English>(1, Intensity(3)));  // english, level 1, intensity 3
    student1_langs.push_back(std::make_unique<French>(2, Intensity(2)));   // french, level 2, intensity 2
    
    std::vector<std::unique_ptr<Language>> student2_langs;
    student2_langs.push_back(std::make_unique<English>(1, Intensity(3)));  // english, level 1, intensity 3
    student2_langs.push_back(std::make_unique<German>(1, Intensity(1)));   // german, level 1, intensity 1
    
    std::vector<std::unique_ptr<Language>> student3_langs;
    student3_langs.push_back(std::make_unique<French>(2, Intensity(1)));   // french, level 2, intensity 1
    student3_langs.push_back(std::make_unique<Chinese>(3, Intensity(3)));  // chinese, level 3, intensity 3

    std::vector<std::unique_ptr<Language>> student4_langs;
    student4_langs.push_back(std::make_unique<English>(1, Intensity(3)));  // english, level 1, intensity 3
    student4_langs.push_back(std::make_unique<Spanish>(2, Intensity(2)));  // spanish, level 2, intensity 2

    Student student1("Alice", std::move(student1_langs));
    Student student2("Bob", std::move(student2_langs));
    Student student3("Charlie", std::move(student3_langs));
    Student student4("Diana", std::move(student4_langs));

    GroupManager manager;               // group manager, sorts by groups
    manager.add_student(student1);
    manager.add_student(student2);
    manager.add_student(student3);
    manager.add_student(student4);

    manager.print_all();
    
    std::vector<Student*> english_group = manager.get_group("English", 1, Intensity(3)); // find group (english, level 1, intensity 3)
    std::cout << "English, level 1, intensity 3 group has " << english_group.size() << " students:" << std::endl;
    for (const auto& student : english_group) {
        std::cout << student->get_name() << std::endl;
    }
    std::cout << std::endl;

    std::vector<Student*> french_group = manager.get_group("French", 2, Intensity(2));
    std::cout << "French, level 2, intensity 2 group has " << french_group.size() << " students:" << std::endl;
    for (const auto& student : french_group) {
        std::cout << student->get_name() << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
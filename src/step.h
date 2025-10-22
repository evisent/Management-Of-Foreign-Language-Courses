#include <chrono> 
#include <random>
#include <string>
#include "names.h"
#include "student.h"
#include "languages.h"
#include "intensity.h"
#include "group_manager.h"
#include "randoms.h"

void step(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students, std::vector<std::unique_ptr<Student>>& individual_students) {
    for (int i = 0; i < students.size(); ++i) {
        students[i]->change_intensity(manager);
    }

    for (int i = 0; i < individual_students.size(); ++i) {
        individual_students[i]->change_intensity(manager);
    }
    
    for (int i = 0; i < students.size(); ++i) {
        students[i]->is_graduate(manager);
    }

    for (int i = 0; i < individual_students.size(); ++i) {
        individual_students[i]->is_graduate(manager);
    }
    
    std::vector<int> empty_students;
    for (int i = 0; i < students.size(); ++i) {
        if (students[i]->get_languages().empty()) {
            empty_students.push_back(i);
        }
    }
    
    for (int i = empty_students.size() - 1; i >= 0; --i) {
        int index = empty_students[i];
        students.erase(students.begin() + index);
    }

    std::vector<int> empty_individual_students;
    for (int i = 0; i < individual_students.size(); ++i) {
        if (individual_students[i]->get_languages().empty()) {
            empty_individual_students.push_back(i);
        }
    }
    
    for (int i = empty_individual_students.size() - 1; i >= 0; --i) {
        int index = empty_individual_students[i];
        individual_students.erase(individual_students.begin() + index);
    }

    
    rand_leave(manager, students);
    rand_add(manager, students, individual_students);
    manager.to_individual(students, individual_students);
    manager.print_all();
}
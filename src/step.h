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

void step(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students){
    for (int i = 0; i < students.size(); ++i){
        students[i]->change_intensity(manager);
        students[i]->is_graduate(manager, students, i);
    }

    for (int i = 0; i < students.size(); ++i){
        //students[i]->get_info();
    }

    rand_leave(manager, students);

    rand_add(manager, students);
}

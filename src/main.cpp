#include "httplib.h"
#include "group_manager.h"
#include "randoms.h"
#include "json_utils.h"
#include <vector>
#include <memory>



GroupManager manager;
std::vector<std::unique_ptr<Student>> students;

int main() {
    httplib::Server server;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Headers", "Content-Type"},
        {"Access-Control-Allow-Methods", "GET,POST,OPTIONS"},
    });

    server.Options(R"(.*)", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
    });

    // Static files
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("<meta http-equiv='refresh' content='0; url=/index.html'>",
                        "text/html");
    });

    // School state endpoint
    server.Get("/state", [](const httplib::Request&, httplib::Response& res) {
        int total_students = students.size();
        
        int total_groups = manager.get_amount();
    
        std::string json_body = "{";
        json_body += "\"students_count\":" + std::to_string(total_students) + ",";
        json_body += "\"groups_count\":" + std::to_string(total_groups);
        json_body += "}";

        res.set_content(json_body, "application/json; charset=utf-8");
    });

    server.Post("/create_student", [](const httplib::Request& req, httplib::Response& res) {
        auto student = rand_student();
        manager.add_student(*student);
        students.push_back(std::move(student));
        
        res.set_content("{\"created\":true}", "application/json");
    });

    server.Get("/groups", [](const httplib::Request&, httplib::Response& res) {
        std::string json_body = manager.get_groups_json();
        res.set_content(json_body, "application/json; charset=utf-8");
    });

    server.Post("/reset", [](const httplib::Request&, httplib::Response& res) {
        students.clear();
        manager.reset();  //
        res.set_content("{\"reset\":true}", "application/json");
    });

    server.Post("/delete_student", [](const httplib::Request&, httplib::Response& res) {
        rand_leave(manager, students);
        res.set_content("{\"deleted\":true}", "application/json");
    });


    server.set_mount_point("/", "./www");

    std::cout << "Language School Server running on http://0.0.0.0:8080\n";
    server.listen("0.0.0.0", 8080);
    return 0;
}

// http://localhost:8080
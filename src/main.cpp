#include "httplib.h"
#include "group_manager.h"
#include "randoms.h"
#include <vector>
#include <memory>

static std::string JsonEscape(const std::string& s) {
  std::string result;
  result.reserve(s.size() + 8);
  for (char c : s) {
    switch (c) {
      case '\"': result += "\\\""; break;
      case '\\': result += "\\\\"; break;
      case '\n': result += "\\n"; break;
      case '\r': result += "\\r"; break;
      case '\t': result += "\\t"; break;
      default: result += c;
    }
  }
  return result;
}

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
        
        // TODO: Добавить получение реальных данных о группах
        int total_groups = 0;
        std::string last_action = "System initialized";
        
        std::string json_body = std::string("{") +
            "\"students_count\":" + std::to_string(total_students) + "," +
            "\"groups_count\":" + std::to_string(total_groups) + "," +
            "\"last_action\":\"" + JsonEscape(last_action) + "\"," +
            "\"status\":\"running\"" +
            "}";

        res.set_content(json_body, "application/json; charset=utf-8");
    });

    // Create student endpoint
    server.Post("/create_student", [](const httplib::Request& req, httplib::Response& res) {
        auto student = std::make_unique<Student>(rand_name(), rand_languages());
        manager.add_student(*student);
        students.push_back(std::move(student));
        
        res.set_content("{\"created\":true}", "application/json");
    });

    // Get groups endpoint
    server.Get("/groups", [](const httplib::Request&, httplib::Response& res) {
        // TODO: Реализовать получение групп в JSON формате
        std::string json_body = "{\"groups\":[]}";
        res.set_content(json_body, "application/json; charset=utf-8");
    });

    // Reset school endpoint
    server.Post("/reset", [](const httplib::Request&, httplib::Response& res) {
        students.clear();
        // TODO: Reset manager
        res.set_content("{\"reset\":true}", "application/json");
    });

    // Serve static files
    server.set_mount_point("/", "./www");

    std::cout << "Language School Server running on http://0.0.0.0:8080\n";
    server.listen("0.0.0.0", 8080);
    return 0;
}

// http://localhost:8080
#pragma once
#include "person.hpp"
#include "csv.hpp"
#include <vector>

class Instructor : public Person {
  std::string office_;
  std::vector<std::string> teaches_;
public:
  Instructor(int id, std::string name, std::string email,
             std::string office, std::vector<std::string> teaches = {})
    : Person(id, std::move(name), std::move(email)),
      office_(std::move(office)), teaches_(std::move(teaches)) {
    if (office_.empty()) throw ValidationError("office is required");
  }

  std::string role() const override { return "Instructor"; }

  nlohmann::json to_json() const override {
    return {
      {"role", role()},
      {"id", id_},
      {"name", name_},
      {"email", email_},
      {"office", office_},
      {"teaches", teaches_}
    };
  }

  YAML::Node to_yaml() const override {
    YAML::Node n;
    n["role"] = role();
    n["id"] = id_;
    n["name"] = name_;
    n["email"] = email_;
    n["office"] = office_;
    for (const auto& c : teaches_) n["teaches"].push_back(c);
    return n;
  }

  std::string csv_header() const override {
    return "role,id,name,email,office,teaches"; // teaches joined by ';'
  }

  std::string csv_row() const override {
    std::string joined;
    for (size_t i=0;i<teaches_.size();++i) {
      if (i) joined+=';';
      joined += teaches_[i];
    }
    return csv_escape(role()) + "," + std::to_string(id_) + "," +
           csv_escape(name_) + "," + csv_escape(email_) + "," +
           csv_escape(office_) + "," + csv_escape(joined);
  }

  // Challenge extension 2: round-trip parsing (JSON -> Instructor).
  // Same idea as Student::from_json, just with office/teaches instead of
  // grad_year/courses.
  static Instructor from_json(const nlohmann::json& j) {
    if (!j.is_object()) throw ValidationError("Instructor JSON must be an object");
    if (!j.contains("role") || j["role"] != "Instructor")
      throw ValidationError("role must be Instructor");
    if (!j.contains("id") || !j["id"].is_number_integer())
      throw ValidationError("id is missing or not an integer");
    if (!j.contains("name") || !j["name"].is_string())
      throw ValidationError("name is missing or not a string");
    if (!j.contains("email") || !j["email"].is_string())
      throw ValidationError("email is missing or not a string");
    if (!j.contains("office") || !j["office"].is_string())
      throw ValidationError("office is missing or not a string");

    std::vector<std::string> teaches;
    if (j.contains("teaches")) {
      if (!j["teaches"].is_array()) throw ValidationError("teaches must be an array");
      for (const auto& c : j["teaches"]) {
        if (!c.is_string()) throw ValidationError("each course must be a string");
        teaches.push_back(c.get<std::string>());
      }
    }

    return Instructor(j["id"].get<int>(), j["name"].get<std::string>(),
                      j["email"].get<std::string>(), j["office"].get<std::string>(),
                      std::move(teaches));
  }
};

#include "student.hpp"
#include "instructor.hpp"
#include "csv_writer.hpp"
#include <iostream>
#include <memory>
#include <vector>

int main() {
  try {
    std::vector<std::unique_ptr<Person>> people;
    people.emplace_back(std::make_unique<Student>(1,"Ada","ada@uni.edu",2026, std::vector<std::string>{"CS101","MATH200"}));
    people.emplace_back(std::make_unique<Instructor>(2,"Grace","grace@uni.edu","Room 314", std::vector<std::string>{"CS101"}));

    // Serialize to JSON array
    nlohmann::json j = nlohmann::json::array();
    for (const auto& p : people) j.push_back(p->to_json());
    std::cout << "JSON:\n" << j.dump(2) << "\n\n";

    // Serialize to YAML sequence
    YAML::Node y;
    for (const auto& p : people) y.push_back(p->to_yaml());
    std::cout << "YAML:\n" << YAML::Dump(y) << "\n";

    // CSV (per concrete type; headers differ)
    std::cout << "\nCSV (Student):\n";
    Student tmpS(99,"Tmp","tmp@uni.edu",2030);
    std::cout << tmpS.csv_header() << "\n";
    std::cout << static_cast<Student&>(*people[0]).csv_row() << "\n";

    std::cout << "\nCSV (Instructor):\n";
    Instructor tmpI(98,"TmpI","tmpI@uni.edu","Room 1");
    std::cout << tmpI.csv_header() << "\n";
    std::cout << static_cast<Instructor&>(*people[1]).csv_row() << "\n";

    // ---- Challenge extension 1: one CSV for everybody ----
    // Only uses the Person interface, so I just hand it raw pointers.
    std::vector<const Person*> ptrs;
    for (const auto& p : people) ptrs.push_back(p.get());
    std::cout << "\nCSV (everyone, superset header):\n";
    std::cout << write_people_csv(ptrs);

    // ---- Challenge extension 2: JSON round trip ----
    Student ada2 = Student::from_json(j[0]);
    Instructor grace2 = Instructor::from_json(j[1]);
    std::cout << "\nRound trip:\n";
    std::cout << "Student same JSON after from_json?    "
              << (ada2.to_json() == j[0] ? "yes" : "no") << "\n";
    std::cout << "Instructor same JSON after from_json? "
              << (grace2.to_json() == j[1] ? "yes" : "no") << "\n";

    // and show what happens with bad JSON (missing grad_year)
    nlohmann::json bad = {{"role","Student"},{"id",5},{"name","Bob"},{"email","bob@uni.edu"}};
    try {
      Student::from_json(bad);
    } catch (const ValidationError& e) {
      std::cout << "Bad JSON was rejected: " << e.what() << "\n";
    }
  } catch (const ValidationError& e) {
    std::cerr << "ValidationError: " << e.what() << "\n";
    return 2;
  } catch (const SerializationError& e) {
    std::cerr << "SerializationError: " << e.what() << "\n";
    return 3;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled std::exception: " << e.what() << "\n";
    return 1;
  }
  return 0;
}

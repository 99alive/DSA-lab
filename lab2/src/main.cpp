#include "instructor.hpp"
#include "student.hpp"

#include <iostream>
#include <memory>
#include <vector>

int main()
{
    try {
        std::vector<std::unique_ptr<Person>> people;
        people.emplace_back(std::make_unique<Student>(
            1, "Ada", "ada@uni.edu", 2026, std::vector<std::string>{"CS101", "MATH200"}));
        people.emplace_back(std::make_unique<Instructor>(
            2, "Grace", "grace@uni.edu", "Room 314", std::vector<std::string>{"CS101"}));

        nlohmann::json json_people = nlohmann::json::array();
        for (const auto& person : people) {
            json_people.push_back(person->to_json());
        }
        std::cout << "JSON:\n" << json_people.dump(2) << "\n\n";

        YAML::Node yaml_people;
        for (const auto& person : people) {
            yaml_people.push_back(person->to_yaml());
        }
        std::cout << "YAML:\n" << YAML::Dump(yaml_people) << "\n";

        Student student = Student::from_json(people[0]->to_json());
        Instructor instructor = Instructor::from_json(people[1]->to_json());

        std::cout << "\nCSV (Student):\n" << student.csv_header() << '\n'
                  << student.csv_row() << "\n\n";
        std::cout << "CSV (Instructor):\n" << instructor.csv_header() << '\n'
                  << instructor.csv_row() << '\n';
    } catch (const ValidationError& error) {
        std::cerr << "ValidationError: " << error.what() << '\n';
        return 2;
    } catch (const SerializationError& error) {
        std::cerr << "SerializationError: " << error.what() << '\n';
        return 3;
    } catch (const std::exception& error) {
        std::cerr << "Unhandled std::exception: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

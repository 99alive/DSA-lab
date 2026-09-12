#pragma once
// Challenge extension 1: polymorphic CSV writer.
//
// Student and Instructor have different CSV headers, so you cannot just
// print them into the same file. This function writes one CSV for a mix
// of people using a "superset" header that has every column from both
// classes. A column that does not apply to a person is left empty.
//
// I did not want this function to know about Student or Instructor at
// all (that is the LSP idea from the lab), so it only uses the Person
// interface: it asks each person for its to_json() and then picks the
// columns out of the JSON. If a column is missing from the JSON it just
// prints "".

#include "person.hpp"
#include "csv.hpp"
#include <span>
#include <string>
#include <vector>

// the column order is fixed so the output is always the same
inline const std::vector<std::string>& people_csv_columns() {
  static const std::vector<std::string> cols = {
    "role", "id", "name", "email", "grad_year", "office", "courses", "teaches"
  };
  return cols;
}

// turn one JSON value into the text that goes in a CSV cell
inline std::string csv_cell_from_json(const nlohmann::json& value) {
  if (value.is_null()) return "";
  if (value.is_string()) return csv_escape(value.get<std::string>());
  if (value.is_number_integer()) return std::to_string(value.get<long long>());
  if (value.is_array()) {
    // lists (courses / teaches) are joined with ';' like in csv_row()
    std::string joined;
    for (size_t i = 0; i < value.size(); ++i) {
      if (i) joined += ';';
      joined += value[i].get<std::string>();
    }
    return csv_escape(joined);
  }
  // anything else (bool, float, ...) - just dump it as text
  return csv_escape(value.dump());
}

inline std::string people_csv_header() {
  std::string header;
  const auto& cols = people_csv_columns();
  for (size_t i = 0; i < cols.size(); ++i) {
    if (i) header += ',';
    header += cols[i];
  }
  return header;
}

inline std::string people_csv_row(const Person& p) {
  nlohmann::json j = p.to_json();
  std::string row;
  const auto& cols = people_csv_columns();
  for (size_t i = 0; i < cols.size(); ++i) {
    if (i) row += ',';
    if (j.contains(cols[i])) row += csv_cell_from_json(j[cols[i]]);
    // else: leave the cell empty
  }
  return row;
}

// header line + one line per person
inline std::string write_people_csv(std::span<const Person*> people) {
  std::string out = people_csv_header() + "\n";
  for (const Person* p : people) {
    out += people_csv_row(*p) + "\n";
  }
  return out;
}

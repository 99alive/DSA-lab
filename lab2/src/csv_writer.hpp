#pragma once
// Challenge extension 1: polymorphic CSV writer.
//
// Student and Instructor have different CSV headers, so you cannot just
// print them into the same file. This writes one CSV for a mix of people
// using a "superset" header that has every column from both classes.
// A column that does not apply to a person is just left empty.
//
// I did not want this to know about Student or Instructor at all (that is
// the LSP idea from the lab), so it only uses the Person interface: it asks
// each person for its to_json() and then looks up the columns in the JSON.

#include "person.hpp"
#include "csv.hpp"
#include <span>
#include <string>
#include <vector>

// the columns, always in this order
inline std::vector<std::string> people_csv_columns() {
  return {"role", "id", "name", "email", "grad_year", "office", "courses", "teaches"};
}

inline std::string people_csv_header() {
  std::string header;
  std::vector<std::string> cols = people_csv_columns();
  for (size_t i = 0; i < cols.size(); ++i) {
    if (i) header += ',';
    header += cols[i];
  }
  return header;
}

// turns one JSON value into the text for a CSV cell
inline std::string csv_cell(const nlohmann::json& value) {
  if (value.is_string()) {
    return csv_escape(value.get<std::string>());
  }
  if (value.is_number_integer()) {
    return std::to_string(value.get<int>());
  }
  if (value.is_array()) {
    // lists (courses / teaches) get joined with ';' like csv_row() does
    std::string joined;
    for (size_t i = 0; i < value.size(); ++i) {
      if (i) joined += ';';
      joined += value[i].get<std::string>();
    }
    return csv_escape(joined);
  }
  // anything else just gets written as text
  return csv_escape(value.dump());
}

inline std::string people_csv_row(const Person& p) {
  nlohmann::json j = p.to_json();
  std::string row;
  std::vector<std::string> cols = people_csv_columns();
  for (size_t i = 0; i < cols.size(); ++i) {
    if (i) row += ',';
    if (j.contains(cols[i])) {
      row += csv_cell(j[cols[i]]);
    }
    // if the person does not have this column the cell stays empty
  }
  return row;
}

// header line followed by one line per person
inline std::string write_people_csv(std::span<const Person*> people) {
  std::string out = people_csv_header() + "\n";
  for (const Person* p : people) {
    out += people_csv_row(*p) + "\n";
  }
  return out;
}

#include "csv_lexer.hpp"

#include <string>
#include <fstream>
#include <iterator>
#include <iostream>

using namespace std;
using namespace laser;

int main(int argc, char** argv) {
  if ( argc != 3 ) {
    cout << "This program takes exactly two arguments the name of the csv file to read in and the column header name to print out" << endl;
    return 1;
  }

  ifstream ifs(argv[1]);

  if ( ! ifs.is_open() ) {
    cout << "could not open file " << argv[1] << endl;
  }

  string csv_data((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

  csv_lexer lexer;
  csv_table_t csv_table;

  if ( ! lexer.lex(csv_data, csv_table) ) {
    cout << "failed to lex csv data" << endl;
    return 1;
  }

  for (int r = 0; r < csv_table.size(); r++) {
    for (int c = 0; c < csv_table[r].size(); c++){
      cout << csv_table[r][c] << " ";
    }
    cout << endl;
  }

  csv_column_map_t column_map;

  lexer.lex( csv_data, column_map);

  csv_column_map_t::iterator it, end;

  it = column_map.find( argv[2] );
  if ( it == end ) {
    cout << "column name " << argv[2] << " not in column map" << endl;
    return 1;
  }

  csv_column_t& column = it->second;

  csv_column_t::iterator cit = column.begin();
  for(; cit != column.end(); ++cit) {
    cout << *cit << endl;
  }
  
  return 0;
}

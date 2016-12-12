#ifndef CSV_LEXER_HPP
#define CSV_LEXER_HPP

#include <map>
#include <vector>
#include <string>

namespace laser {

  typedef std::vector<std::string> csv_row_t;
  typedef csv_row_t csv_column_t;
  typedef std::vector<csv_row_t > csv_table_t;
  typedef std::map<std::string, csv_column_t > csv_column_map_t;

  class csv_lexer_impl;

  class csv_lexer {
  public:
    csv_lexer();
    virtual ~csv_lexer();
    bool lex(std::string const& csv_data_in, csv_table_t& csv_table_out);
    bool lex(std::string const& csv_data_in, 
             csv_column_map_t& csv_column_map_out);

  private:
    // disable copying this class
    csv_lexer(csv_lexer const&);
    const csv_lexer& operator=(csv_lexer const&);
    // pointer to private implementation pimpl idiom
    csv_lexer_impl* pimpl;
  };
}
#endif

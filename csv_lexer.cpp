#include "csv_lexer.hpp"
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace laser {
  class csv_lexer_impl {
  public:
    bool lex(string const& csv_data_in, csv_table_t& csv_table_out) {
      if ( csv_data_in.empty() ) return false;

      buffer_ = csv_data_in;
      pos_ = 0;
      field_.clear();
      row_.clear();
      current_ = buffer_[pos_];

      return csv(csv_table_out);
    }

    bool lex(string const& csv_data_in, 
             csv_column_map_t& csv_column_map_out) {
      csv_table_t table;
      bool lexed = lex(csv_data_in, table);
      if ( ! lexed ) return false;
      for( int c = 0; c < table[0].size(); c++) {
        csv_column_t column;
        for( int r = 1; r < table.size(); r++) {
          column.push_back( table[r][c] );
        }
        csv_column_map_out[ table[0][c] ] = column;
      }
      return true;
    }


    bool csv(csv_table_t& table) {
      while( row(table) );
      return !buffer_left();
    }

    bool row(csv_table_t& table) {
      if ( ! buffer_left() ) return false;

      row_.clear();
      while( field() );
      table.push_back( row_ );
      return newline() && consume();
    }

    bool field() {
      if ( ! buffer_left() || newline() ) return false;
      field_.clear();
      bool lexed_field = empty_field() || unquoted_field() || quoted_field();
      if ( lexed_field ) {
        row_.push_back( field_ );
      }
      return lexed_field;
    }

    bool empty_field() {
      while ( whitespace() ) {
        consume();
      }

      return field_end();
    }

    bool field_end() {
      return (comma() && consume()) || newline(); 
    }

    bool unquoted_field() {
      if ( !buffer_left() || quote() ) return false;

      while ( ! field_terminator() ) {
        field_ += consume();
      }
      return field_end();
    }

    bool quoted_field() {
      if ( ! buffer_left() || ! quote() ) return false;

      consume(); // drop first quote;

      while( ! singular_quote() ) {
        
        if ( escaped_quote() ) {
          field_ += consume();
          consume(); // drop extra quote
        } else {
          field_ += consume();
        }
      }
      consume(); // consume the singular quote

      // drop trailing whitespace between singular quote and field terminator
      while ( whitespace() ) {
        consume();
      }
      return field_end();
    }

    bool singular_quote() {
      return quote() && ! next_quote();
    }

    bool escaped_quote() {
      return quote() && next_quote();
    }

    bool next_quote() {
      return next() == '"';
    }

    char next() {
      return pos_ + 1 < buffer_.size() ? buffer_[pos_ + 1] : '\0';
    }

    char consume() {
      char previous = current_;
      current_ = buffer_left() ? buffer_[++pos_] : '\0';
      return previous;
    }

    bool quote() { 
      return current_ == '"';
    }

    bool field_terminator() {
      return comma() || newline();
    }

    bool comma() {
      return current_ == ',';
    }

    bool whitespace() {
      return current_ == ' ' || current_ == '\t';
    }

    bool newline() {
      return buffer_left() && current_ == '\n';
    }
    
    bool buffer_left() {
      return pos_ < buffer_.size();
    }
    
    string buffer_;
    csv_row_t row_;
    string field_;
    int pos_;
    char current_;

  };


  csv_lexer::csv_lexer() : pimpl( new csv_lexer_impl() ) {
  }
  
  csv_lexer::~csv_lexer() {
    delete pimpl;
  }


  bool csv_lexer::lex(std::string const& csv_data_in, 
                      csv_table_t& csv_table_out) {
    return pimpl->lex(csv_data_in, csv_table_out);
  }


  bool csv_lexer::lex(std::string const& csv_data_in, 
           csv_column_map_t& csv_column_map_out) {
    return pimpl->lex(csv_data_in, csv_column_map_out);
  }


  

}

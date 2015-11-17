// $Id: basic_parser.h,v 1.2 2014/02/25 21:43:13 david Exp $ -*- c++ -*-
#ifndef __CDK9_BASE_PARSER_H__
#define __CDK9_BASE_PARSER_H__

#include <memory>
#include <string>
#include <cdk/basic_scanner.h>

namespace cdk {

  class compiler;

  class basic_parser {

  protected:
    //! @var the language to be scanned
    const std::string _language = "";

    //! @var the compiler this parser belongs to
    std::shared_ptr<compiler> _compiler = nullptr;

    //! @var the scanner providing the input tokens
    std::shared_ptr<basic_scanner> _scanner = nullptr;

  protected:
    inline basic_parser(const std::string &language,
                        std::shared_ptr<basic_scanner> scanner) :
        _language(language), _compiler(nullptr), _scanner(scanner) {
    }

  public:
    virtual ~basic_parser() {
      _compiler = nullptr;
    }

  public:
    inline std::shared_ptr<compiler> owner() {
      return _compiler;
    }

  private:
    friend class compiler;
    inline void set_owner(std::shared_ptr<compiler> compiler) {
      _compiler = compiler;
      if (_scanner != nullptr)
        _scanner->set_owner(compiler);
    }

  public:
    inline std::shared_ptr<basic_scanner> scanner() {
      return _scanner;
    }

    inline void scanner(std::shared_ptr<basic_scanner> scanner) {
      _scanner = scanner;
      _scanner->switch_streams();
    }

  public:
    /**
     * Parse algorithm: the compiler object stores the result.
     * @param compiler the compiler object this parser belongs to.
     * @return true if the operation is successful
     */
    virtual int parse() = 0;

  };

} // cdk

#endif

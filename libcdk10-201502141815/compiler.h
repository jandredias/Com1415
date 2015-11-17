// $Id: compiler.h,v 1.5 2014/02/25 21:43:13 david Exp $ -*- c++ -*-
#ifndef __CDK9_COMPILER_H__
#define __CDK9_COMPILER_H__

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <cdk/null_deleter.h>
#include <cdk/basic_scanner.h>
#include <cdk/basic_parser.h>
#include <cdk/basic_target.h>

namespace cdk {

  class basic_node;

  class compiler: public std::enable_shared_from_this<compiler> {
    /** @var _name is the compiler's name */
    std::string _name;

    /** @var _extension is extension of the output file */
    std::string _extension;

    /** @var _ifile is the input file name */
    std::string _ifile = "";

    /** @var _ofile is the output file name */
    std::string _ofile = "";

  protected:

    /** @var _scanner is a reference to the scanner */
    std::shared_ptr<basic_scanner> _scanner = nullptr;

    /** @var _parser is a reference to the parser */
    std::shared_ptr<basic_parser> _parser = nullptr;

    /** @var _evaluators knows about all evaluators */
    std::vector<std::shared_ptr<basic_target>> _evaluators;

    /** @var _ast is the root of the syntax tree */
    basic_node *_ast;

  private:

    /** @var _optimize is a flag: optimization (default behaviour unavailable) */
    bool _optimize = false;

    /** @var _debug is a flag: debug (default behaviour: false) */
    bool _debug = true;

    /** Compilation errors */
    int _errors = 0;

  public:
    static inline std::shared_ptr<compiler> create(const std::string &language,
                                                   std::shared_ptr<basic_scanner> scanner,
                                                   std::shared_ptr<basic_parser> parser) {
      std::shared_ptr<compiler> c = std::make_shared<compiler>(language, scanner, parser);
      scanner->set_owner(c);
      parser->set_owner(c);
      return c;
    }

  public:
    // HORRENDOUS DESIGN: FIXME: SHOULD NOT BE DIRECTLY CALLED
    inline compiler(const std::string &language, std::shared_ptr<basic_scanner> scanner,
                    std::shared_ptr<basic_parser> parser) :
        _name(language), _extension("asm"), _ifile(""), _ofile(""), _scanner(scanner), _parser(
            parser), _ast(nullptr), _optimize(false), _debug(false), _errors(0) {
    }

  public:
    virtual ~compiler() {
      // make the forget about us....
      _scanner->set_owner(nullptr);
      _parser->set_owner(nullptr);
      // ... and forget about them...
      _scanner = nullptr;
      _parser = nullptr;
      _evaluators.clear();
    }

  public:
    inline const std::string &name() const {
      return _name;
    }
    inline void name(const std::string &name) {
      _name = name;
    }

    inline const std::string &extension() const {
      return _extension;
    }
    inline void extension(const std::string &extension) {
      _extension = extension;
    }

    inline const std::string &ifile() const {
      return _ifile;
    }
    inline void ifile(const std::string &ifile) {
      _ifile = ifile;
      if (_ifile != "")
        _scanner->input_stream(std::make_shared<std::ifstream>(_ifile.c_str()));
      else
        _scanner->input_stream(std::shared_ptr<std::istream>(&std::cin, null_deleter()));
    }

    inline const std::string &ofile() const {
      return _ofile;
    }
    inline void ofile(const std::string &ofile) {
      _ofile = ofile;
      if (_ofile != "")
        _scanner->output_stream(std::make_shared<std::ofstream>(_ofile.c_str()));
      else
        _scanner->output_stream(std::shared_ptr<std::ostream>(&std::cout, null_deleter()));
    }

    inline std::shared_ptr<std::istream> istream() {
      return _scanner->input_stream();
    }

    inline std::shared_ptr<std::ostream> ostream() {
      return _scanner->output_stream();
    }

  public:
    inline basic_node *ast() {
      return _ast;
    }
    inline void ast(basic_node *ast) {
      _ast = ast;
    }

    inline std::shared_ptr<basic_scanner> scanner() {
      return _scanner;
    }
    inline void scanner(std::shared_ptr<basic_scanner> scanner) {
      _scanner = scanner;
    }

    inline std::shared_ptr<basic_parser> parser() {
      return _parser;
    }
    inline void parser(std::shared_ptr<basic_parser> parser) {
      _parser = parser;
    }

  public:
    inline bool optimize() const {
      return _optimize;
    }
    inline void optimize(bool optimize) {
      _optimize = optimize;
    }

    inline bool debug() const {
      return _debug;
    }
    inline void debug(bool debug) {
      _debug = debug;
    }

    inline int errors() const {
      return _errors;
    }

  public:

    inline int parse() {
      if (_parser)
        return _parser->parse();
      else {
        std::cerr << "FATAL: No parser available. Exiting..." << std::endl;
        exit(1);
      }
    }

    /**
     * Processes the AST and produces the output file.
     * The specific processing strategy is provided independently by each
     * back-end implementation (evaluator subclasses).
     */
    inline bool evaluate() {
      basic_target *evaluator = basic_target::get_target_for(_extension);
      if (evaluator)
        return evaluator->evaluate(shared_from_this());
      else {
        std::cerr << "FATAL: No evaluator defined for target '" << _extension << "'. Exiting..."
            << std::endl;
        exit(1);
      }
    }

  };

} // cdk

#endif

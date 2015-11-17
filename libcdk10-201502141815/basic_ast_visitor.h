// $Id: basic_ast_visitor.h,v 1.2 2014/02/25 21:43:13 david Exp $ -*- c++ -*-
#ifndef __CDK9_BASIC_AST_VISITOR_H__
#define __CDK9_BASIC_AST_VISITOR_H__

#include <iostream>

namespace cdk {
  class basic_node;
  class nil_node;
  class data_node;
  class composite_node;
  class sequence_node;
  class expression_node;
  template<typename StoredType> class simple_value_node;
  class double_node;
  class integer_node;
  class string_node;
  class identifier_node;
  class unary_expression_node;
  class neg_node;
  class binary_expression_node;
  class add_node;
  class sub_node;
  class mul_node;
  class div_node;
  class mod_node;
  class lt_node;
  class le_node;
  class ge_node;
  class gt_node;
  class eq_node;
  class ne_node;
} // cdk

//!
//! This class is only for compiling the package: it will not be
//! installed. Specific compilers are supposed to define their own,
//! with the SAME NAME, but defining ALL processing functions
//! corresponding to their specific problem.
//!
class basic_ast_visitor {
  //! The output stream
  std::ostream &_os;

protected:
  //! Debug flag
  bool _debug;

protected:
  /**
   * Initialization of a semantic processor.
   *
   * @param os is the output stream to be used by the
   *        semantic processor.
   */
  basic_ast_visitor(std::ostream &os = std::cout, bool debug = false);

  /**
   * Return the current output stream.
   * @return an output stream.
   */
  inline std::ostream &os() {
    return _os;
  }

public:
  /**
   * How to destroy a semantic processor.
   */
  virtual ~basic_ast_visitor();

public:
  virtual void do_nil_node(cdk::nil_node * const node, int lvl) = 0;
  virtual void do_data_node(cdk::data_node * const node, int lvl) = 0;
  virtual void do_composite_node(cdk::composite_node * const node, int lvl) = 0;
  virtual void do_sequence_node(cdk::sequence_node * const node, int lvl) = 0;

public:
  virtual void do_integer_node(cdk::integer_node * const node, int lvl) = 0;
  virtual void do_double_node(cdk::double_node * const node, int lvl) = 0;
  virtual void do_string_node(cdk::string_node * const node, int lvl) = 0;
  virtual void do_identifier_node(cdk::identifier_node * const node, int lvl) = 0;

public:
  virtual void do_neg_node(cdk::neg_node * const node, int lvl) = 0;

public:
  virtual void do_add_node(cdk::add_node * const node, int lvl) = 0;
  virtual void do_sub_node(cdk::sub_node * const node, int lvl) = 0;
  virtual void do_mul_node(cdk::mul_node * const node, int lvl) = 0;
  virtual void do_div_node(cdk::div_node * const node, int lvl) = 0;
  virtual void do_mod_node(cdk::mod_node * const node, int lvl) = 0;

public:
  virtual void do_lt_node(cdk::lt_node * const node, int lvl) = 0;
  virtual void do_le_node(cdk::le_node * const node, int lvl) = 0;
  virtual void do_ge_node(cdk::ge_node * const node, int lvl) = 0;
  virtual void do_gt_node(cdk::gt_node * const node, int lvl) = 0;
  virtual void do_ne_node(cdk::ne_node * const node, int lvl) = 0;
  virtual void do_eq_node(cdk::eq_node * const node, int lvl) = 0;
};

#endif

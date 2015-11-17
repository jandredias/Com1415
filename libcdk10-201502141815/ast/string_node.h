// $Id: string_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_STRING_H__
#define __CDK9_AST_EXPRESSION_STRING_H__

#include <string>
#include <cdk/ast/simple_value_node.h>

namespace cdk {

  /**
   * Class for describing syntactic tree leaves for holding string
   * values.
   */
  class string_node: public virtual simple_value_node<std::string> {
  public:
    inline string_node(int lineno, const char *s) :
        simple_value_node<std::string>(lineno, s) {
    }
    inline string_node(int lineno, const std::string &s) :
        simple_value_node<std::string>(lineno, s) {
    }
    inline string_node(int lineno, const std::string *s) :
        simple_value_node<std::string>(lineno, *s) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_string_node(this, level);
    }

  };

} // cdk

#endif

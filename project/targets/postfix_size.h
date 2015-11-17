#ifndef __PWN_SEMANTICS_PF_SIZE_H__
#define __PWN_SEMANTICS_PF_SIZE_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace pwn {

    class postfix_size : public basic_ast_visitor {
        int _value;
        public:
          postfix_size(std::shared_ptr<cdk::compiler> compiler):
				basic_ast_visitor(compiler), _value(0) {}

          ~postfix_size() {}

          int value(){ return _value; }
          void do_sequence_node(cdk::sequence_node * const node, int lvl);
          void do_double_node(cdk::double_node * const node, int lvl);
          void do_integer_node(cdk::integer_node * const node, int lvl);
          void do_string_node(cdk::string_node * const node, int lvl);
          void do_neg_node(cdk::neg_node * const node, int lvl);
          void do_add_node(cdk::add_node * const node, int lvl);
          void do_sub_node(cdk::sub_node * const node, int lvl);
          void do_mul_node(cdk::mul_node * const node, int lvl);
          void do_div_node(cdk::div_node * const node, int lvl);
          void do_mod_node(cdk::mod_node * const node, int lvl);
          void do_lt_node(cdk::lt_node * const node, int lvl);
          void do_le_node(cdk::le_node * const node, int lvl);
          void do_ge_node(cdk::ge_node * const node, int lvl);
          void do_gt_node(cdk::gt_node * const node, int lvl);
          void do_ne_node(cdk::ne_node * const node, int lvl);
          void do_eq_node(cdk::eq_node * const node, int lvl);
          void do_while_node(cdk::while_node * const node, int lvl);
          void do_if_node(cdk::if_node * const node, int lvl);
          void do_if_else_node(cdk::if_else_node * const node, int lvl);
          void do_lvalue_node(pwn::lvalue_node * const node, int lvl);
          void do_rvalue_node(pwn::rvalue_node * const node, int lvl);
          void do_evaluation_node(pwn::evaluation_node * const node, int lvl);
          void do_print_node(pwn::print_node * const node, int lvl);
          void do_read_node(pwn::read_node * const node, int lvl);
          void do_assignment_node(pwn::assignment_node * const node, int lvl);
          void do_println_node(pwn::println_node * const node, int lvl);
          void do_stop_node(pwn::stop_node * const node, int lvl);
          void do_next_node(pwn::next_node * const node, int lvl);
          void do_or_node(pwn::or_node * const node, int lvl);
          void do_and_node(pwn::and_node * const node, int lvl);
          void do_repeat_node(pwn::repeat_node * const node, int lvl);
          void do_return_node(pwn::return_node * const node, int lvl);
          void do_not_node(pwn::not_node * const node, int lvl);
          void do_address_node(pwn::address_node * const node, int lvl);
          void do_var_node(pwn::var_node * const node, int lvl);
          void do_var_dec_node(pwn::var_dec_node * const node, int lvl);
          void do_noob_node(pwn::noob_node * const node, int lvl);
          void do_block_node(pwn::block_node * const node, int lvl);
          void do_function_call_node(pwn::function_call_node * const node, int lvl);
          void do_function_def_node(pwn::function_def_node * const node, int lvl);
          void do_function_dec_node(pwn::function_dec_node * const node, int lvl);
          void do_identity_node(pwn::identity_node * const node, int lvl);
          void do_index_node(pwn::index_node * const node, int lvl);
          void do_mem_alloc_node(pwn::mem_alloc_node * const node, int lvl);
  };

} // pwn

#endif

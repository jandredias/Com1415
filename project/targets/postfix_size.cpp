#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/postfix_size.h"
#include "ast/all.h"  // all.h is automatically generated


void pwn::postfix_size::do_mem_alloc_node(pwn::mem_alloc_node * const node, int lvl){
  _value += 4;
  _value += node->size() * 8;
}
void pwn::postfix_size::do_var_dec_node(pwn::var_dec_node * const node, int lvl){
  _value += node->type()->size();
}
void pwn::postfix_size::do_block_node(pwn::block_node * const node, int lvl){
  if(node->dec() != nullptr)
    node->dec()->accept(this, lvl);
}
void pwn::postfix_size::do_sequence_node(cdk::sequence_node * const node, int lvl){
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}
void pwn::postfix_size::do_double_node(cdk::double_node * const node, int lvl){}
void pwn::postfix_size::do_integer_node(cdk::integer_node * const node, int lvl){}
void pwn::postfix_size::do_string_node(cdk::string_node * const node, int lvl){}
void pwn::postfix_size::do_neg_node(cdk::neg_node * const node, int lvl){}
void pwn::postfix_size::do_add_node(cdk::add_node * const node, int lvl){}
void pwn::postfix_size::do_sub_node(cdk::sub_node * const node, int lvl){}
void pwn::postfix_size::do_mul_node(cdk::mul_node * const node, int lvl){}
void pwn::postfix_size::do_div_node(cdk::div_node * const node, int lvl){}
void pwn::postfix_size::do_mod_node(cdk::mod_node * const node, int lvl){}
void pwn::postfix_size::do_lt_node(cdk::lt_node * const node, int lvl){}
void pwn::postfix_size::do_le_node(cdk::le_node * const node, int lvl){}
void pwn::postfix_size::do_ge_node(cdk::ge_node * const node, int lvl){}
void pwn::postfix_size::do_gt_node(cdk::gt_node * const node, int lvl){}
void pwn::postfix_size::do_ne_node(cdk::ne_node * const node, int lvl){}
void pwn::postfix_size::do_eq_node(cdk::eq_node * const node, int lvl){}
void pwn::postfix_size::do_while_node(cdk::while_node * const node, int lvl){}
void pwn::postfix_size::do_if_node(cdk::if_node * const node, int lvl){}
void pwn::postfix_size::do_if_else_node(cdk::if_else_node * const node, int lvl){}
void pwn::postfix_size::do_lvalue_node(pwn::lvalue_node * const node, int lvl){}
void pwn::postfix_size::do_rvalue_node(pwn::rvalue_node * const node, int lvl){}
void pwn::postfix_size::do_evaluation_node(pwn::evaluation_node * const node, int lvl){}
void pwn::postfix_size::do_print_node(pwn::print_node * const node, int lvl){}
void pwn::postfix_size::do_read_node(pwn::read_node * const node, int lvl){}
void pwn::postfix_size::do_assignment_node(pwn::assignment_node * const node, int lvl){}
void pwn::postfix_size::do_println_node(pwn::println_node * const node, int lvl){}
void pwn::postfix_size::do_stop_node(pwn::stop_node * const node, int lvl){}
void pwn::postfix_size::do_next_node(pwn::next_node * const node, int lvl){}
void pwn::postfix_size::do_or_node(pwn::or_node * const node, int lvl){}
void pwn::postfix_size::do_and_node(pwn::and_node * const node, int lvl){}
void pwn::postfix_size::do_repeat_node(pwn::repeat_node * const node, int lvl){}
void pwn::postfix_size::do_return_node(pwn::return_node * const node, int lvl){}
void pwn::postfix_size::do_not_node(pwn::not_node * const node, int lvl){}
void pwn::postfix_size::do_address_node(pwn::address_node * const node, int lvl){}
void pwn::postfix_size::do_var_node(pwn::var_node * const node, int lvl){}
void pwn::postfix_size::do_noob_node(pwn::noob_node * const node, int lvl){}
void pwn::postfix_size::do_function_call_node(pwn::function_call_node * const node, int lvl){}
void pwn::postfix_size::do_function_def_node(pwn::function_def_node * const node, int lvl){
  _value += node->ret()->size();
  node->block()->accept(this, lvl);
}
void pwn::postfix_size::do_function_dec_node(pwn::function_dec_node * const node, int lvl){}
void pwn::postfix_size::do_identity_node(pwn::identity_node * const node, int lvl){}
void pwn::postfix_size::do_index_node(pwn::index_node * const node, int lvl){}

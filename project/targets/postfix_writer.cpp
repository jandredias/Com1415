// $Id: postfix_writer.cpp,v 1.47 2015/05/20 09:41:56 ist175741 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/postfix_size.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(mklbl(++_lbl));
  _pf.DOUBLE(node->value());
  _pf.TEXT();
  _pf.ALIGN();
  _pf.ADDR(mklbl(_lbl));
  _pf.DLOAD();
}
void pwn::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}
void pwn::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;
  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.NEG(); // 2-complement
  else if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DNEG();
}

//---------------------------------------------------------------------------
void pwn::postfix_writer::do_while_node(cdk::while_node * const node, int lvl) {}

void pwn::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->left()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_INT ||
     node->type()->name() == basic_type::TYPE_POINTER)
    _pf.ADD();
  else if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DADD();
  else{
    std::cerr << "ERROR: incompatible add arguments types" << std::endl;
    exit(-1);
  }
}
void pwn::postfix_writer::do_mem_alloc_node(pwn::mem_alloc_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if(_level == 0){
    std::cerr << "mem alloc node global not defined";
    return;
  }
  //LOCAL
  std::shared_ptr<pwn::symbol> s = _symtab.find_local(*node->name());
  if(s.get() != NULL){
    std::cerr << "error: redefinition of '" << *node->name() << "'" << std::endl;
    exit(-1);
  }
  _head += 4;
  symbol *pointer = new symbol(new basic_type(4,basic_type::TYPE_POINTER), *node->name(), 0-_head);
  std::shared_ptr<pwn::symbol> s1(pointer);
  _symtab.insert(*node->name(), s1);
  _head += node->size() * 8;
}

void pwn::postfix_writer::do_var_dec_node(pwn::var_dec_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  //Variables are global

  if(_level == 0){
    std::shared_ptr<pwn::symbol> s = _symtab.find(*node->name());
    if(s.get() != NULL){
      std::cerr << "error: redefinition of '" << *node->name() << "'" << std::endl;
      exit(-1);
    }
    symbol *pointerG = new symbol(node->type(), *node->name(), 0);
    std::shared_ptr<pwn::symbol> s3(pointerG);
    _symtab.insert(*node->name(), s3);
    if(node->init() != NULL){
      _pf.DATA();
      _pf.ALIGN();
      _pf.GLOBAL(*node->name(), _pf.OBJ());
      _pf.LABEL(*node->name());
      if((node->type()->name() == basic_type::TYPE_INT) &&
         (dynamic_cast<cdk::integer_node*>(node->init()) != NULL)){
          _pf.CONST((dynamic_cast<cdk::integer_node*>(node->init()))->value());
      }
      else if((node->type()->name() == basic_type::TYPE_DOUBLE) &&
         (dynamic_cast<cdk::double_node*>(node->init()) != NULL)){
        _pf.DOUBLE((dynamic_cast<cdk::double_node*>(node->init()))->value());
      }else if((node->type()->name() == basic_type::TYPE_STRING) &&
         (dynamic_cast<cdk::string_node*>(node->init()) != NULL)){
           _pf.RODATA(); // strings are DATA readonly
           _pf.ALIGN(); // make sure we are aligned
           _pf.LABEL(mklbl(++_lbl)); // give the string a name
           _pf.STR((dynamic_cast<cdk::string_node*>(node->init()))->value()); // output string characters

           /* leave the address on the stack */
           _pf.DATA(); // return to the TEXT segment
           _pf.ALIGN();
           _pf.ID(mklbl(_lbl)); // the string to be printed
           _pf.TEXT();


      }else if(node->type()->name() == basic_type::TYPE_POINTER){
        _pf.INT(0);
      }
      node->init()->accept(this, lvl);
    }else{
      _pf.BSS();
      _pf.ALIGN();
      _pf.GLOBAL(*node->name(), _pf.OBJ());
      _pf.LABEL(*node->name());
      _pf.BYTE(node->type()->size());
    }

    _pf.TEXT();
    _pf.ALIGN();
    return;
  }

  //LOCAL VARIABLES

  //Else variables are local to a function
  //Search on symbol table, and if it exists shows error: REDECLARED VARIABLE OR FUNCTION
  //It is not possible to have a function and a variable with the same name

  std::shared_ptr<pwn::symbol> s = _symtab.find_local(*node->name());
  if(s.get() != NULL){
    std::cerr << "error: redefinition of '" << *node->name() << "'" << std::endl;
    exit(-1);
  }
  _head += node->type()->size();
  symbol *pointer = new symbol(node->type(), *node->name(), 0-_head);
  std::shared_ptr<pwn::symbol> s2(pointer);
  _symtab.insert(*node->name(), s2);

  if(node->init() != NULL)
    node->init()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.LOCAL(pointer->offset());
    _pf.DSTORE();
  }
  else
    _pf.LOCA(pointer->offset());
}



void pwn::postfix_writer::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  _level++;
  _head = 0;

  std::shared_ptr<pwn::symbol> s = _symtab.find(*node->name());
  if(s.get() != NULL && s.get()->defined()){
    std::cerr << "error: redefinition of '" << *node->name() << "'" << std::endl;
    exit(-1);
  }else if(s.get() == NULL){
    std::shared_ptr<pwn::symbol> s2(new symbol(new basic_type(node->ret()->size(),node->ret()->name()), *node->name(), 0, true));
    _symtab.insert(*node->name(), s2);
  }else{
    s.get()->defined(true);
  }
  //std::cout << size_args << std::endl;
  if(node->main()){
	  _pf.EXTERN("readi");
	  _pf.EXTERN("printi");
	  _pf.EXTERN("prints");
	  _pf.EXTERN("printd");
	  _pf.EXTERN("println");
    _pf.EXTERN("printsp");
    _pf.EXTERN("readln");
    _pf.EXTERN("argc");
    _pf.EXTERN("argv");
    _pf.EXTERN("readb");
    _pf.EXTERN("readd");
    _pf.EXTERN("strlen");
    _pf.EXTERN("atoi");
    _pf.EXTERN("atod");
    _pf.EXTERN("itoa");
    _pf.EXTERN("dtoa");
	  _pf.TEXT();
	  _pf.ALIGN();
	  _pf.GLOBAL("_main",_pf.FUNC());
	  _pf.LABEL("_main");
  }else{
	  _pf.TEXT();
	  _pf.ALIGN();
	  _pf.GLOBAL(*node->name(),_pf.FUNC());
	  _pf.LABEL(*node->name());
  }
  _symtab.push();
  int size_args = 8;

  if(node->arg() != NULL){
    for(int arg_i = node->arg()->size() - 2; arg_i >= 0; arg_i--){
      size_args += ((var_node *) node->arg()->node(arg_i))->type()->size();
    }
    std::shared_ptr<pwn::symbol> function = _symtab.find(*node->name());
    function.get()->offset(size_args - 8);
    for(int arg_i = node->arg()->size() - 1; arg_i >= 0; arg_i--){
      var_node *var = ((var_node *)node->arg()->node(arg_i));
      pwn::symbol *arg = new symbol(new basic_type(var->type()->size(), var->type()->name()), *var->name(), size_args);
      size_args -= var->type()->size();
      std::shared_ptr<pwn::symbol> s(arg);
      _symtab.insert(*var->name(), s);
    }
  }
  /* Default return
   * INT:     -4
   * DOUBLE:  -8
   * STRING:  -4
   * POINTER: -4
   */
  if(node->ret()->name() != basic_type::TYPE_VOID){
    _head += node->ret()->size();
    symbol *p = new symbol(new basic_type(node->ret()->size(), node->ret()->name()), *node->name(), 0-node->ret()->size());
    std::shared_ptr<pwn::symbol> s1(p);
    _symtab.insert(*node->name(), s1);
  }
  if(node->ret()->name() == basic_type::TYPE_INT || node->ret()->name() == basic_type::TYPE_STRING || node->ret()->name() == basic_type::TYPE_POINTER){
    _sizeAtual = 4;
    _hasReturn = true;
  }else if(node->ret()->name() == basic_type::TYPE_DOUBLE){
    _sizeAtual = 8;
    _hasReturn = true;
  }else if(node->ret()->name() == basic_type::TYPE_VOID) _hasReturn = false;

  pwn::postfix_size sizeVisitor(_compiler);
  (node)->accept(&sizeVisitor, 0);

  _pf.ENTER(sizeVisitor.value());

  if(node->returnDefault() != NULL){
    node->returnDefault()->accept(this, lvl);
    _pf.LOCAL(0-node->ret()->size());
    if(node->ret()->name() == basic_type::TYPE_DOUBLE)
      _pf.DSTORE();
    else
      _pf.STORE();
  }else{ //There is no default return... it'll be zero or ""
    if(node->ret()->name() == basic_type::TYPE_INT){
      _pf.INT(0);
      _pf.LOCV(-4);
    }else if(node->ret()->name() == basic_type::TYPE_DOUBLE){
      _pf.RODATA();
      _pf.ALIGN();
      _pf.LABEL(mklbl(++_lbl));
      _pf.DOUBLE(0);
      _pf.TEXT();
      _pf.ALIGN();
      _pf.ADDR(mklbl(_lbl));
      _pf.DLOAD();
      _pf.LOCAL(-8);
      _pf.DSTORE();
    }else if(node->ret()->name() == basic_type::TYPE_POINTER){
      _pf.INT(0);
      _pf.LOCV(-4);
    }else if(node->ret()->name() == basic_type::TYPE_STRING){
      _pf.RODATA(); // strings are DATA readonly
      _pf.ALIGN(); // make sure we are aligned
      _pf.LABEL(mklbl(++_lbl)); // give the string a name
      _pf.STR(""); // output string characters
      /* leave the address on the stack */
      _pf.TEXT(); // return to the TEXT segment
      _pf.ADDR(mklbl(_lbl)); // the string to be printed
      _pf.LOCV(-4);
    }
  }
  node->block()->accept(this, lvl);
  if(node->ret()->name() == basic_type::TYPE_INT ||
     node->ret()->name() == basic_type::TYPE_POINTER ||
     node->ret()->name() == basic_type::TYPE_STRING){

    _pf.LOCV(-4);
    _pf.POP();
  }else if(node->ret()->name() == basic_type::TYPE_DOUBLE){
    _pf.LOCV(-8);
    _pf.DPOP();
  }else if(node->ret()->name() == basic_type::TYPE_VOID){

  }else{
    std::cerr << "return type not supported yet" << std::endl;
    exit(-1);
  }

  _symtab.pop();
  _pf.LEAVE();
  _pf.RET();
  _level--;
}

void pwn::postfix_writer::do_block_node(pwn::block_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  if(node->dec() != NULL) node->dec()->accept(this,lvl+2);
  if(node->inst() != NULL) node->inst()->accept(this,lvl+2);
}
void pwn::postfix_writer::do_repeat_node(pwn::repeat_node * const node, int lvl){
	CHECK_TYPES(_compiler, _symtab, node);
	int lbl_condition, lbl_endRepeat, lbl_increment;
    _pf.TEXT();
    if(node->init() != NULL) node->init()->accept(this, lvl + 2);

    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl_condition = ++_lbl));
    node->condition()->accept(this, lvl + 2);

    _pf.JZ(mklbl(lbl_endRepeat = ++_lbl));
    node->block()->accept(this, lvl + 2);

    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl_increment = ++_lbl));
    node->step()->accept(this, lvl + 2);
    _pf.JMP(mklbl(lbl_condition));
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl_endRepeat));
}


void pwn::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->left()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->right()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  _pf.SUB();
}
void pwn::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->left()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->right()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  _pf.MUL();
}
void pwn::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->left()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->right()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  _pf.DIV();
}
void pwn::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->left()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE &&
     node->right()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  _pf.MOD();
}
void pwn::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void pwn::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void pwn::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void pwn::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void pwn::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void pwn::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}
void pwn::postfix_writer::do_not_node(pwn::not_node * const node, int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl);
  _pf.NOT();
}



void pwn::postfix_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.TRASH(8);
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_VOID){

  }else {
    std::cerr << "ERROR: CANNOT HAPPEN! evaluation node... return: " << node->argument()->type()->name() <<  std::endl;
    exit(1);
  }
}

void pwn::postfix_writer::do_print_node(pwn::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.CALL("printd"); //FIXME
    _pf.TRASH(8);
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: print CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void pwn::postfix_writer::do_println_node(pwn::println_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: println CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_read_node(pwn::read_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  //FIXME
  _pf.CALL("readi");
  _pf.PUSH();
  node->argument()->accept(this, lvl);
  _pf.STORE();
}

void pwn::postfix_writer::do_if_node(cdk::if_node * const node, int lvl) {
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void pwn::postfix_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
}

//---------------------------------------------------------------------------
void pwn::postfix_writer::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  std::shared_ptr<pwn::symbol> s = _symtab.find(*node->name());
  if(s.get() == NULL){
    std::cerr << "error: variable " << *node->name() << " not declared" << std::endl;
    exit(-1);
  }
  s = _symtab.find(*node->name());
  if(s.get() != NULL){
    if(s.get()->offset() != 0){
      //LOCAL
      _pf.LOCAL(s.get()->offset());
    }else{
      //GLOBAL
      _pf.ADDR(*node->name());
      //Variable is global
    }
    //Variable is local

  }else{
    std::cerr << "var is not defined lvalue" << std::endl;
  }
}
void pwn::postfix_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DLOAD();
  }else{
    _pf.LOAD();
  }
}

void pwn::postfix_writer::do_and_node(pwn::and_node * const node, int lvl) {
	int lbl1;
  CHECK_TYPES(_compiler, _symtab, node);
	node->left()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}
void pwn::postfix_writer::do_or_node(pwn::or_node * const node, int lvl){
  int lbl1;
	CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  _pf.JNZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}




void pwn::postfix_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if(node->lvalue()->type()->name() == node->rvalue()->type()->name() &&
     node->lvalue()->type()->name() == basic_type::TYPE_INT){
    node->rvalue()->accept(this, lvl);
    _pf.DUP();
    node->lvalue()->accept(this, lvl);
    _pf.STORE();
  }else if(node->lvalue()->type()->name() == node->rvalue()->type()->name() &&
     node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE){
    //assignment is double
  }else if(node->lvalue()->type()->name() == node->rvalue()->type()->name() &&
     node->lvalue()->type()->name() == basic_type::TYPE_STRING){
    //assignment is string
  }else if(node->lvalue()->type()->name() == node->rvalue()->type()->name() &&
     node->lvalue()->type()->name() == basic_type::TYPE_POINTER){
    //assignment is pointer
  }
}

void pwn::postfix_writer::do_return_node(pwn::return_node * const node, int lvl) {
  if(_hasReturn){
    if(_sizeAtual == 4){
      _pf.LOCV(-4);
      _pf.POP();
    }else{
      _pf.LOCV(-8);
    _pf.DPOP();
    }
  }
	_pf.RET();
}


void pwn::postfix_writer::do_index_node(pwn::index_node * const node, int lvl) {
//CHECK_TYPES(_compiler, _symtab, node);
    //FIX ME
}
void pwn::postfix_writer::do_function_dec_node(pwn::function_dec_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  std::shared_ptr<pwn::symbol> s = _symtab.find(*node->name());
  if(s.get() != NULL){
    std::cerr << "error: redeclaration of '" << *node->name() << "'" << std::endl;
    exit(-1);
  }

  std::shared_ptr<pwn::symbol> s2(new symbol(new basic_type(node->ret()->size(),node->ret()->name()), *node->name(), 0, true, false));
  _symtab.insert(*node->name(), s2);
}
void pwn::postfix_writer::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int trash = 0;
  if(node->argument() != NULL){
    for(size_t i = 0; i < node->argument()->size(); i++){
      node->argument()->node(i)->accept(this, lvl);
      trash += ((cdk::expression_node *) node->argument()->node(i))->type()->size();
    }
  }
  _pf.CALL(*node->name());
  std::shared_ptr<pwn::symbol> s = _symtab.find(*node->name());
  _pf.TRASH(trash);
  if(s.get()->type()->name() == basic_type::TYPE_INT ||
     s.get()->type()->name() == basic_type::TYPE_STRING ||
     s.get()->type()->name() == basic_type::TYPE_POINTER)
    _pf.PUSH();
  else if(s.get()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DPUSH();
}
void pwn::postfix_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
  _pf.INT(0);
}
void pwn::postfix_writer::do_var_node(pwn::var_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  //FIX ME
}

void pwn::postfix_writer::do_address_node(pwn::address_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  //FIX ME
}
void pwn::postfix_writer::do_next_node(pwn::next_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	//FIX ME
}
void pwn::postfix_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	//FIX ME
}

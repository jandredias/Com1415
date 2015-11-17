// $Id: symbol_table.h,v 1.3 2014/02/27 20:32:41 david Exp $ -*- c++ -*-
#ifndef __CDK9_SYMBOL_TABLE_H__
#define __CDK9_SYMBOL_TABLE_H__

#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <vector>

namespace cdk {

  template<typename Symbol>
  class symbol_table {

  private:
    typedef typename std::map<std::string, std::shared_ptr<Symbol>> context_type;
    //typedef typename context_type::const_iterator context_iterator;

    /** the context level */
    int _level;

    /** this is the current context */
    context_type *_current;

    /** stores a context path from the root to the current (last) context */
    std::vector<context_type*> _contexts;

  public:
    inline symbol_table() :
        _level(0) {
      _current = new context_type;
      _contexts.push_back(_current);
    }

    /**
     * Destroy the symbol table and all symbols it contains.
     * Note that SYMBOLS ARE ALSO DELETED!!
     * Again: the death of a table also means the death of its symbols.
     * Hint: do not symbols anywhere other than on their native table.
     * Hint: do not share symbols with other tables or other data structures.
     * Remember: symbols are tightly connected to tables: they die with them.
     * ****   Y O U   H A V E   B E E N   W A R N E D   ****
     */
    inline virtual ~symbol_table() {
      while (_level > 0)
        pop();
      destroyCurrent();
      _contexts.clear();
    }

  private:
    /**
     * First, destroy all symbols in the current context.
     * Next, remove all entries from the current context.
     * Finally, delete the current context and set the pointer to null.
     */
    void destroyCurrent() {
      //for (typename context_type::iterator it = _current->begin(); it != _current->end(); it++) {
      //  delete it->second;
      //}
      _current->clear();
      delete _current;
      _current = nullptr;
    }

  public:

    /**
     * Create a new context and make it current.
     */
    inline void push() {
      _level++;
      _current = new context_type;
      _contexts.push_back(_current);
    }

    /**
     * Destroy the current context: the previous context becomes
     * the current one. If the first context is reached no operation
     * is performed.
     */
    inline void pop() {
      if (_level == 0)
        return;
      destroyCurrent();
      _contexts.pop_back();
      _current = _contexts.back();
      _level--;
    }

    /**
     * Define a new identifier in the local (current) context.
     *
     * @param name the symbol's name.
     * @param symbol the symbol.
     * @return
     *   <tt>true</tt> if new identifier (may be defined in an upper
     *   context); <tt>false</tt> if identifier already exists in the
     *   current context.
     */
    inline bool insert(const std::string &name, std::shared_ptr<Symbol> symbol) {
      auto it = _current->find(name);
      if (it == _current->end()) {
        (*_current)[name] = symbol;
        return true;
      }
      return false;
    }

    /**
     * Replace the data corresponding to a symbol in the current context.
     *
     * @param name the symbol's name.
     * @param symbol the symbol.
     * @return
     *   <tt>true</tt> if the symbol exists; <tt>false</tt> if the
     *   symbol does not exist in any of the contexts.
     */
    inline bool replace_local(const std::string &name, std::shared_ptr<Symbol> symbol) {
      auto it = _current->find(name);
      if (it != _current->end()) {
        delete it->second;
        (*_current)[name] = symbol;
        return true;
      }
      return false;
    }

    /**
     * Replace the data corresponding to a symbol (look for the symbol in all
     * available contexts, starting with the innermost one).
     *
     * @param name the symbol's name.
     * @param symbol the symbol.
     * @return
     *   <tt>true</tt> if the symbol exists; <tt>false</tt> if the
     *   symbol does not exist in any of the contexts.
     */
    inline bool replace(const std::string &name, std::shared_ptr<Symbol> symbol) {
      for (size_t ix = _contexts.size(); ix > 0; ix--) {
        context_type &ctx = *_contexts[ix - 1];
        auto it = ctx.find(name);
        if (it != ctx.end()) {
          //FIXME: BUG: should free previous symbol
          ctx[name] = symbol;
          return true;
        }
      }
      return false;
    }

    /**
     * Search for a symbol in the local (current) context.
     *
     * @param name the symbol's name.
     * @param symbol the symbol.
     * @return
     *   <tt>true</tt> if the symbol exists; <tt>false</tt> if the
     *   symbol does not exist in the current context.
     */
    inline std::shared_ptr<Symbol> find_local(const std::string &name) {
      auto it = _current->find(name);
      if (it != _current->end())
        return it->second; // symbol data
      return nullptr;
    }

    /**
     * Search for a symbol in the avaible contexts, starting with the first
     * one and proceeding until reaching the outermost context.
     *
     * @param name the symbol's name.
     * @param from how many contexts up from the current one (zero).
     * @return
     *    <tt>nullptr</tt> if the symbol cannot be found in any of the
     *    contexts; or the symbol and corresponding attributes.
     */
    inline std::shared_ptr<Symbol> find(const std::string &name, size_t from = 0) const {
      if (from >= _contexts.size())
        return nullptr;
      for (size_t ix = _contexts.size() - from; ix > 0; ix--) {
        context_type &ctx = *_contexts[ix - 1];
        auto it = ctx.find(name);
        if (it != ctx.end())
          return it->second; // symbol data
      }
      return nullptr;
    }

  };

} // cdk

#endif

/*
 * MRustC - Rust Compiler
 * - By John Hodge (Mutabah/thePowersGang)
 *
 * include/ident.hpp
 * - Identifiers with hygiene
 */
#pragma once
#include <vector>
#include <string>
#include <memory>
#include <rc_string.hpp>

struct Ident
{
    struct ModPath
    {
        ::std::vector<RcString> ents;
    };
    class Hygiene
    {
        static unsigned g_next_scope;

        ::std::vector<unsigned int> contexts;
        ::std::shared_ptr<ModPath> search_module;

        Hygiene(unsigned int index):
            contexts({index})
        {}
    public:
        Hygiene():
            contexts({})
        {}

        static Hygiene new_scope()
        {
            return Hygiene(++g_next_scope);
        }
        static Hygiene new_scope_chained(const Hygiene& parent)
        {
            Hygiene rv;
            rv.search_module = parent.search_module;
            rv.contexts.reserve( parent.contexts.size() + 1 );
            rv.contexts.insert( rv.contexts.begin(),  parent.contexts.begin(), parent.contexts.end() );
            rv.contexts.push_back( ++g_next_scope );
            return rv;
        }
        Hygiene get_parent() const
        {
            //assert(this->contexts.size() > 1);
            Hygiene rv;
            rv.contexts.insert(rv.contexts.begin(),  this->contexts.begin(), this->contexts.end()-1);
            return rv;
        }

        bool has_mod_path() const {
            return this->search_module != 0;
        }
        const ModPath& mod_path() const {
            return *this->search_module;
        }
        void set_mod_path(ModPath p) {
            this->search_module.reset( new ModPath(::std::move(p)) );
        }

        Hygiene(Hygiene&& x) = default;
        Hygiene(const Hygiene& x) = default;
        Hygiene& operator=(Hygiene&& x) = default;
        Hygiene& operator=(const Hygiene& x) = default;

        // Returns true if an ident with hygine `source` can see an ident with this hygine
        bool is_visible(const Hygiene& source) const;
        //bool operator==(const Hygiene& x) const { return scope_index == x.scope_index; }
        //bool operator!=(const Hygiene& x) const { return scope_index != x.scope_index; }

        friend ::std::ostream& operator<<(::std::ostream& os, const Hygiene& v);
    };

    Hygiene hygiene;
    RcString   name;

    Ident(const char* name):
        hygiene(),
        name(name)
    { }
    Ident(RcString name):
        hygiene(),
        name(::std::move(name))
    { }
    Ident(Hygiene hygiene, RcString name):
        hygiene(::std::move(hygiene)), name(::std::move(name))
    { }

    Ident(Ident&& x) = default;
    Ident(const Ident& x) = default;
    Ident& operator=(Ident&& x) = default;
    Ident& operator=(const Ident& x) = default;

    RcString into_string() {
        return ::std::move(name);
    }

    bool operator==(const char* s) const {
        return this->name == s;
    }

    bool operator==(const Ident& x) const {
        if( this->name != x.name )
            return false;
        //if( this->hygine.indexes != x.hygine.indexes )
        //    return false;
        return true;
    }
    bool operator!=(const Ident& x) const {
        return !(*this == x);
    }
    bool operator<(const Ident& x) const;

    friend ::std::ostream& operator<<(::std::ostream& os, const Ident& x);
};

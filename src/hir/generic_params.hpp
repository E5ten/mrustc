/*
 * MRustC - Mutabah's Rust Compiler
 * - By John Hodge (Mutabah/thePowersGang)
 *
 * hir/generic_params.hpp
 * - HIR version of generic definition blocks
 */
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "type.hpp"

namespace HIR {

struct TypeParamDef
{
    RcString    m_name;
    ::HIR::TypeRef  m_default;
    bool    m_is_sized;
};
struct LifetimeDef
{
    RcString    m_name;
};

TAGGED_UNION(GenericBound, Lifetime,
    (Lifetime, struct {
        LifetimeRef test;
        LifetimeRef valid_for;
        }),
    (TypeLifetime, struct {
        ::HIR::TypeRef  type;
        LifetimeRef valid_for;
        }),
    (TraitBound, struct {
        ::HIR::TypeRef  type;
        ::HIR::TraitPath    trait;
        })/*,
    (NotTrait, struct {
        ::HIR::TypeRef  type;
        ::HIR::GenricPath    trait;
        })*/,
    (TypeEquality, struct {
        ::HIR::TypeRef  type;
        ::HIR::TypeRef  other_type;
        })
    );
extern ::std::ostream& operator<<(::std::ostream& os, const GenericBound& x);

class GenericParams
{
public:
    ::std::vector<TypeParamDef> m_types;
    ::std::vector<LifetimeDef>  m_lifetimes;

    ::std::vector<GenericBound>    m_bounds;

    //GenericParams() {}

    GenericParams clone() const;

    struct PrintArgs {
        const GenericParams& gp;
        PrintArgs(const GenericParams& gp): gp(gp) {}
        friend ::std::ostream& operator<<(::std::ostream& os, const PrintArgs& x);
    };
    PrintArgs fmt_args() const { return PrintArgs(*this); }
    struct PrintBounds {
        const GenericParams& gp;
        PrintBounds(const GenericParams& gp): gp(gp) {}
        friend ::std::ostream& operator<<(::std::ostream& os, const PrintBounds& x);
    };
    PrintBounds fmt_bounds() const { return PrintBounds(*this); }
};

}   // namespace HIR

extern Ordering ord(const HIR::GenericBound& a, const HIR::GenericBound& b);


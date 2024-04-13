#pragma once

#include <concepts>

#include "Traits.hpp"
#include "RuleRPNType.hpp"

namespace sb {
// Concepts
  // CreatorRPN
    template<class Creator, class Letter, class Type>
    concept C_CreatorRPN = 
    requires(
        Creator creator,
        const Letter letter,
        const Type type)
    {
        creator.put(letter, type);
        {creator.empty()} -> std::same_as<bool>;
        {creator.get()} -> std::same_as<CreatorRPNTraitsT<Creator>>;
    };
  // TableRPN
    template<class Table, class Letter, class T>
    concept C_TableRPN = 
        std::same_as<RuleRPNType, TableRPNTraitsT<Table, Letter>> &&
    requires(
        const Table table,
        const Letter letter,
        const TableRPNTraitsR<Table, Letter> rule,
        T value) 
    {
        {table.get(letter)} -> std::same_as<const TableRPNTraitsR<Table, Letter>&>;
        rule.type();
        rule.priority();
        rule(value, std::move(value));
    };
} // namespace sb
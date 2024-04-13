#pragma once

#include <type_traits>

#include <TypeTraits/DeclVal.hpp>

namespace sb {
	template<class Table, class Letter>
    struct TableRPNTraits;
	// have
		// Rule
		// Type
		// Priority
	//

	template<class Table, class Letter>
	using TableRPNTraitsR = typename TableRPNTraits<Table, Letter>::Rule;
	template<class Table, class Letter>
	using TableRPNTraitsT = typename TableRPNTraits<Table, Letter>::Type;
	template<class Table, class Letter>
	using TableRPNTraitsP = typename TableRPNTraits<Table, Letter>::Priority;
	

	template<class Rule>
    struct TableRPNRuleTraits;
	// have
		// Type
		// Priority
	// 

	template<class Creator>
	struct CreatorRPNTraits;
	// have
		// T
	//

	template<class Creator>
	using CreatorRPNTraitsT = typename CreatorRPNTraits<Creator>::T;
// realization
  // TableRPNRuleTraits
   // Base
	template<class Rule>
	concept _C_TableRPNRule = 
	requires(const Rule rule) {
		rule.type();
		rule.priority();
	};

	template<class Rule, bool isRule>
	struct _T_TableRPNRule {
		using Type = void;
		using Priority = void;
	};

	template<class Rule>
	struct _T_TableRPNRule<Rule, 1> {
		using Type = std::remove_cvref_t<decltype(DeclLVal<const Rule>().type())>;
		using Priority = std::remove_cvref_t<decltype(DeclLVal<const Rule>().priority())>;
	};
   // Traits
	template<class Rule>
    struct TableRPNRuleTraits {
		using Type = typename _T_TableRPNRule<Rule, _C_TableRPNRule<Rule>>::Type;
		using Priority = typename _T_TableRPNRule<Rule, _C_TableRPNRule<Rule>>::Priority;
	};
  // TableRPNTraits
   // Base
	template<class Table, class Letter>
	concept _C_TableRPN = 
	requires(const Table table, const Letter letter) {
		table.get(letter);
	};

	template<class Table, class Letter, bool isTable>
	struct _T_TableRPN {
		using Rule = void;
	};

	template<class Table, class Letter>
	struct _T_TableRPN<Table, Letter, 1> {
		using Rule = std::remove_cvref_t<decltype(DeclLVal<const Table>().get(DeclLVal<const Letter>()))>;
	};
   // Traits
	template<class Table, class Letter>
    struct TableRPNTraits {
		using Rule = typename _T_TableRPN<Table, Letter, _C_TableRPN<Table, Letter>>::Rule;
		using Type = typename TableRPNRuleTraits<Rule>::Type;
		using Priority = typename TableRPNRuleTraits<Rule>::Priority;
	};
  // CreatorRPNTraits
   // Base
	template<class Creator>
	concept _C_CreatorRPN = 
	requires(Creator creator) {
		creator.get();
	};

	template<class Creator, bool isCreator>
	struct _T_CreatorRPN {
		using T = void;
	};

	template<class Creator>
	struct _T_CreatorRPN<Creator, 1> {
		using T = std::remove_cvref_t<decltype(DeclLVal<Creator>().get())>;
	};
   // Traits
	template<class Creator>
    struct CreatorRPNTraits {
		using T = typename _T_CreatorRPN<Creator, _C_CreatorRPN<Creator>>::T;
	};
} // namespace sb
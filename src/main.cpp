#include <bits/stdc++.h>
//#include "FA/PrintFA.hpp"
//#include "FA/RE2DFA.hpp"

#include <FA/Class/FA.hpp>
#include <FA/Class/Traits.hpp>

#include <FA/Class/Concept.hpp>

template<sb::C_NFA FA>
void check() {}

int main() {
   using _FA = sb::FA<1>;
   using _ItSt = typename sb::FATraits<_FA>::IteratorBySt;
   using _CItSt = typename sb::FATraits<_FA>::ConstIteratorBySt;
   using _ItTr = typename sb::FATraits<_FA>::IteratorByTr;
   using _CItTr = typename sb::FATraits<_FA>::ConstIteratorByTr;
   using _State = typename sb::FATraits<_FA>::State;
   using _Transition = typename sb::FATraits<_FA>::Transition;
   using _Letter = typename sb::FATraits<_FA>::Letter;
   using _Type = typename sb::FATraits<_FA>::Type;

   std::cout << typeid(_ItSt).name() << '\n';
   std::cout << typeid(_CItSt).name() << '\n';
   std::cout << typeid(_ItTr).name() << '\n';
   std::cout << typeid(_CItTr).name() << '\n';
   std::cout << typeid(_State).name() << '\n';
   std::cout << typeid(_Transition).name() << '\n';
   std::cout << typeid(_Letter).name() << '\n';
   std::cout << typeid(_Type).name() << '\n';
   std::cout << '\n';

   check<sb::FA<1>>();
   //std::cout << sb::C_FA<sb::FA<0>> << '\n';
   sb::FA<0> dfa;
   auto& s = dfa.emplace();
   s.emplace('a', s);
   auto& tr = *s.begin();
   auto& s1 = tr.next();
   s1.emplace('b', s);
}
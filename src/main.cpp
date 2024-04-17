#include <iostream>
#include <Base/Tree/Print.hpp>

int main() {
    sb::Tree tree;
    auto& vertex = sb::Tree::makeVertex<std::string>(tree, "ok");
    sb::Tree::makeVertex<std::string>(vertex, "one");
    auto& robot = sb::Tree::makeVertex<std::string>(vertex, "robot");
    auto& hhh = sb::Tree::makeVertex<std::string>(robot, "detail1");
    sb::Tree::makeVertex<std::string>(hhh, "detail01");
    sb::Tree::makeVertex<std::string>(hhh, "detail02");

    auto& h = sb::Tree::makeVertex<std::string>(robot, "detail2");
    sb::Tree::makeVertex<std::string>(h, "lol");
    
    auto& det = sb::Tree::makeVertex<std::string>(robot, "detail3");
    sb::Tree::makeVertex<std::string>(det, "det");
    sb::Tree::makeVertex<std::string>(det, "det");
    

    sb::Tree::makeVertex<std::string>(vertex, "two");
    sb::Tree::makeVertex<std::string>(vertex, "three");
    auto& four = sb::Tree::makeVertex<std::string>(vertex, "four");
    sb::Tree::makeVertex<std::string>(four, "0");
    sb::Tree::makeVertex<std::string>(four, "2");
    sb::Tree::makeVertex<std::string>(four, "3");
    
    
    sb::printTree(std::cout, tree);
}
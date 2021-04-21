#include <iostream>
#include "binarytree.h"
#include <cassert>

//strategie di comparazione

struct equal_int
{
    bool operator()(int a, int b) const
    {
        return a == b;
    }
};
struct compare_int
{
    bool operator()(int a, int b) const
    {
        return a < b;
    }
};

struct equal_double
{
    bool operator()(double a, double b) const
    {
        return a == b;
    }
};

struct compare_double
{
    bool operator()(double a, double b) const
    {
        return a < b;
    }
};

struct even
{
    bool operator()(int a) const
    {
        return a % 2 == 0;
    }
};

struct odd
{
    bool operator()(int a) const
    {
        return a % 2 != 0;
    }
};
struct point2d
{
    int x;
    int y;
    point2d(int xx, int yy) : x(xx), y(yy) {}
};
std::ostream &operator<<(std::ostream &os, const point2d &p)
{
    std::cout << "(" << p.x << "," << p.y << ")";
    return os;
}
struct equal_point2d
{
    bool operator()(const point2d &a, const point2d &b) const
    {
        return (a.x == b.x and a.y == b.y);
    }
};

struct compare_point2d
{
    bool operator()(const point2d &a, const point2d &b) const
    {
        bool ret = (a.x < b.x);
        return ret;
    }
};

struct compare_string
{
    bool operator()(const std::string &b, const std::string &a) const
    {
        return a.size() < b.size();
    }
};

struct equal_string
{
    bool operator()(const std::string &b, const std::string &a) const
    {
        return a.size() == b.size();
    }
};

typedef binarytree<int, compare_int, equal_int> btint;
typedef binarytree<double, compare_double, equal_double> btdouble;
typedef binarytree<std::string, compare_string, equal_string> btstr;
typedef binarytree<point2d, compare_point2d, equal_point2d> btpoint;

void test_fondamentali()
{


}
void test_bst_int()
{
    std::cout << "----inizio test BST.int-----" << std::endl;

    btint mytree(5);
    mytree.add(4);
    mytree.add(2);
    mytree.add(1);
    mytree.add(3);

    std::cout << "stampa dell'albero" << std::endl;
    mytree.ptree();

    std::cout << "stampa verbosa dell'albero" << std::endl;
    mytree.ptree_verbose();

    std::cout << "stampa tramite operator<< " << mytree << std::endl;

    std::cout << "stampa di mytree2, in copia" << std::endl;
    btint mytree2(mytree);
    mytree2.ptree();

    std::cout << "stampa di mytree3, in assegnamento" << std::endl;
    btint mytree3;
    mytree3 = mytree;
    mytree3.ptree();

    std::cout << "stampa di un subtree di mytree" << std::endl;
    mytree.subtree(2).ptree();

    std::cout << "----test PRINTIF-----" << std::endl;

    odd o1;
    even e1;
    std::cout << "test dispari" << std::endl;

    printif(mytree, o1);
    std::cout << "test pari" << std::endl;

    printif(mytree, e1);


        std::cout << "stampa con iteratore" << std::endl;

    btint altro = mytree;
    btint::const_iterator i, ie;
    i = altro.begin();
    ie = altro.end();
    while(i!=ie) {
        std::cout << *i << " ";
        ++i;
    }
    std::cout<<std::endl;

    std::cout << "-------fine test int BST --------" << std::endl << std::endl;

}

void test_bst_str()
{   
    std::cout << "----inizio test BST.str-----" << std::endl;

    btstr mytree("super");
    mytree.add("califragilisti");
    mytree.add("chespiralitoso!");
    mytree.ptree();
    assert(mytree.exist("quarantadue!!!!"));
    
    btstr mytree2 = mytree.subtree("chespiralitoso!");
    btstr mytree3 = mytree;
    mytree.ptree_verbose();
    mytree3.ptree();
    mytree2.ptree();

    std::cout << "-------fine test str BST --------" << std::endl << std::endl;

}

void test_bst_double()
{
    std::cout << "----inizio test BST.double-----" << std::endl;
    btdouble mytreed;
    mytreed.add(4.5);
    mytreed.add(3.2);
    mytreed.add(5.4);
    
    std::cout << " prima stampa " << std::endl;
    mytreed.ptree();
    mytreed.add(5.5);
    mytreed.add(5.55);
    std::cout << "seconda stampa" << std::endl;
    mytreed.ptree();
    assert(mytreed.exist(5.55));

    std::cout << "-------fine test double BST --------" << std::endl << std::endl;

}
void test_bst_Point2d()
{
    std::cout << "----inizio test BST.point2d-----" << std::endl;

    btpoint mytreep(point2d(5, 6));
    mytreep.add(point2d(2, 2));
    mytreep.add(point2d(7, 9));
    
    std::cout << " stampa verbosa" << std::endl;
    mytreep.ptree_verbose();
    std::cout << " stampa" << std::endl;
    mytreep.ptree();
    mytreep.add(point2d(4,5));
    mytreep.ptree();
    btpoint mytreep2 = mytreep.subtree(point2d(2, 2));
    
    std::cout << "stampa subtree" << std::endl;
    mytreep2.ptree_verbose();
    std::cout << mytreep2 << std::endl;
    std::cout << "-------fine test point2d BST --------" << std::endl << std::endl;


}




int main()
{

   test_bst_int();

    test_bst_double();

    test_bst_str();

    test_bst_Point2d();

    return 0;
}
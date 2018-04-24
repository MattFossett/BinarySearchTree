#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

/************************************************************/
// System includes

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/************************************************************/
// Local includes

#include "SearchTree.hpp"

/************************************************************/
// Using declarations

using std::abs;
using std::advance;
using std::cerr;
using std::cout;
using std::equal;
using std::fixed;
using std::mt19937;
using std::ostringstream;
using std::pair;
using std::remove;
using std::set;
using std::setprecision;
using std::string;
using std::to_string;
using std::vector;
using std::endl;

/************************************************************/
// Function prototypes

template<typename T>
void
REQUIRE_TREE (const SearchTree<T>& t, string treeString);

template<typename T>
void
REQUIRE_TREE (const SearchTree<T>& t, string treeString, int depth);

template<typename T>
void
REQUIRE_HEAD (const SearchTree<T>& tree, T left, T right, T parent);

template<typename T>
void
REQUIRE_HEAD_NULL (const SearchTree<T>& tree);

template<typename T>
void
REQUIRE_LEVEL_ORDER (const SearchTree<T>& tree, string answer);

template<typename T>
void
tree_insert (SearchTree<T>& tree, const vector<T>& values);

template<typename T>
void
tree_erase (SearchTree<T>& tree, const vector<T>& values);

template<typename T>
void
REQUIRE_ALL_EQUAL (const vector<T>& values);

void
CHECK_AND_WARN (bool condition, string message);

ostringstream
CHECK_AND_WARN (bool condition, string message, ostringstream stream);


/************************************************************/

/************************************************************/
void 
printTestResult(const string& test,
		 const string& expected,
		 const ostringstream& actual);

int
main (int argc, char* argv[])
{
    

  ostringstream output;
  
  SearchTree<int> A;
  cout << (A.end().m_nodePtr == nullptr);
  output.str ("");
  output << A.size ();
  printTestResult ("size", "0", output);  

  
  output.str ("");
  output << A.empty ();
  printTestResult ("empty", "1", output);  

  output.str ("");
  output << (A.end().m_nodePtr->left == nullptr);  
  printTestResult ("checkNull", "1", output);  

    //cout << A.begin().m_nodePtr->data;
    output.str ("");
    A.insert(4);
    A.insert(2);
    A.insert(7);
    A.insert(6);
    A.insert(1);
    A.insert(3);
    A.insert(3);
    A.insert(3);

    output.str ("");
    output << A;
    printTestResult ("Insert big", "[ 1 2 3 4 6 7 ]", output);

    SearchTree<int> B (A);
    output.str ("");
    output << B;
    printTestResult ("copy", "[ 1 2 3 4 6 7 ]", output);

    auto i = B.find(7);
    i--;
    output.str ("");
    output << *i;
    printTestResult ("Begin", "6", output);

    SearchTree<string> C;
    C.insert("my");
    C.insert("name");
    C.insert("Jeff");

    auto j = C.begin();
    j--;
    j++;
    output.str("");
    output << *j;
    ++j;
    output << " " << *j;
    j++;
    output << " " << *j;
    printTestResult("begin test", "Jeff my name" , output);
    
   /* SearchTree<int> D;
    D.insert(5);
    D.erase(5);

    cout << D;
    output.str("");
    output << D.empty();
    printTestResult("EMpty", "1" , output);*/
    SearchTree<int> D;
    //{ 5, 2, 8, 1, 4, 7, 3, 6 });
  //  D.clear();
    D.insert(5);
    D.insert(2);
    D.insert(8);
    D.insert(1);
    D.insert(4);
    D.insert(7);
    D.insert(3);
    D.insert(6);
    D.insert(1);
    D.insert(1);

    cout << D;
    D.erase(4);
    D.erase(6);
    D.erase(7);

    cout << D;

    return EXIT_SUCCESS;
}

/************************************************************/

/************************************************************/
void
printTestResult (const string& test,
		 const string& expected,
		 const ostringstream& actual)
{
  cout << "Test: " << test << endl;
  cout << "==========================" << endl;
  cout << "Expected: " << expected << endl;
  cout << "Actual  : " << actual.str () << endl;
  cout << "==========================" << endl << endl;

  // Ensure the two results are the same
  assert (expected == actual.str ());
}
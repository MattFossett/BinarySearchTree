/*
  Filename   : SearchTree.hpp
  Author     : Matt Fossett, Bryce Eaby
  Course     : CSCI 362
  Description: Binary search tree class, 
                 a basis for implementing associative ADTs
		 like set and map. 
*/

/************************************************************/
// Macro guard

#ifndef SEARCHTREE_HPP
#define SEARCHTREE_HPP

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::ostream;
using std::queue;

/************************************************************/

template<typename T> 
struct Node
{
  using NodePtr = Node*;

  // TODO
  Node (const T& d = T ())
    : data (d),
      left (nullptr),
      right (nullptr),
      parent (nullptr)

  {

  }

  // TODO
  Node (const T& d, NodePtr l, NodePtr r, NodePtr p)
    : data(d),
      left (l),
      right (r),
      parent (p)
  {

  }

  //**

  T        data;
  NodePtr  left;
  NodePtr  right;
  NodePtr  parent;
};

/************************************************************/

// Forward declaration
template <typename T>
class SearchTree;

/************************************************************/

template <typename T>
struct TreeIterator
{
  friend class SearchTree<T>;

  using Self = TreeIterator<T>;
  using NodePtr = Node<T>*;
  using ConstNodePtr = const Node<T>*;

  using difference_type = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  using value_type = T;
  using pointer = const T*;
  using reference = const T&;

  TreeIterator ()
    : m_nodePtr ()
  { }

  explicit
  TreeIterator (ConstNodePtr n)
    : m_nodePtr (n)
  { }

  reference
  operator* () const
  {
    return this->m_nodePtr->data;
  }

  // Return address of node's data member.
  pointer
  operator-> () const
  {
    return &*this;
  }

  // Pre-increment
  Self&
  operator++ ()
  {
    m_nodePtr = increment (m_nodePtr);

    return *this;
  }

  // Post-increment
  Self
  operator++ (int)
  {
    Self copy(*this);
    m_nodePtr = increment (m_nodePtr);
    return copy;
  }

  // Pre-decrement
  Self&
  operator-- ()
  {
    m_nodePtr = decrement (m_nodePtr);
    return *this;
  }

  // Post-decrement
  Self
  operator-- (int)
  {
    Self copy(*this);
    m_nodePtr = decrement (m_nodePtr);
    return copy;
  }

  bool
  operator== (const Self& i) const
  {
    return m_nodePtr == i.m_nodePtr;
  }

  bool
  operator!= (const Self& i) const
  {
    return m_nodePtr != i.m_nodePtr;
  }

private:
  static ConstNodePtr
  increment (ConstNodePtr n)
  {
//cases:
//1: n->right exists and has no left, return n->right (1 and 2 could be on while(n->left !=nullptr))
//2. n->right exists and has a left, return the left most of that subtree
//3. n->right does not exist and parent->left == n, return parent
//4. n->right does not exist and parent->left != n(n is in rightsubtree),return first parent > n->data
//    or first parent whos parent == searchNode head, which means it is header 

    if (n->right != nullptr)
    {
      n = n->right;
      while (n->left != nullptr)
        n = n->left;
      return n;
    } 
    else 
    {
      if (n->parent->left == n)
        return n->parent;
      ConstNodePtr temp = n; 
      while (temp->data <= n->data)
      {
        //If temp is header, we have reached end. 
        if (temp == temp->parent->parent)
          return temp->parent;
        temp = temp->parent;
      }
      return temp;
    }
  }

  static ConstNodePtr
  decrement (ConstNodePtr n)
  {
    if (n->left != nullptr)
    {
      n = n->left;
      while (n->right != nullptr)
        n = n->right;
      return n;
    } 
    else 
    {
      if (n->parent->right == n)
        return n->parent;
      ConstNodePtr temp = n; 
      while (temp->data >= n->data)
      {
        //If temp is header, we have reached end. 
        if (temp == temp->parent->parent)
          return temp->parent;
        temp = temp->parent;
      }
    return temp;
    }
  }

  // FOR TESTING, otherwise would be PRIVATE
public:

  ConstNodePtr m_nodePtr;
};

/************************************************************/

template <typename T>
class SearchTree
{
  friend class TreeIterator<T>;

public:

  // DO NOT MODIFY type aliases!
  using value_type = T;
  using pointer =  T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  using iterator = TreeIterator<T>;
  using const_iterator = TreeIterator<T>;

  // Header parent points to root of tree or is nullptr
  //   if the tree is empty.
  // Header left points to LARGEST node or is nullptr
  //   if the tree is empty.  
  // Header right points to SMALLEST node or is nullptr
  //   if the tree is empty.
  // size represents the number of elements in the tree.
  SearchTree ()
    : m_header (Node()),
      m_size (0)
  {
  }

  // Copy constructor
  SearchTree (const SearchTree& t)
    : m_header(Node()), 
      m_size (0)
  {
    queue<NodePtr> nodeQ;
    NodePtr r = t.m_header.parent;
    if(r == nullptr)
      return;
    nodeQ.push(r);
    while (!nodeQ.empty())
    {
      NodePtr temp = nodeQ.front();
      nodeQ.pop();
      if (temp->left != nullptr)
        nodeQ.push(temp->left);
      if(temp->right != nullptr)
        nodeQ.push(temp->right);
      insert (temp->data);
    }

    ConstNodePtr newNode = t.begin().m_nodePtr;
    while (m_size != t.m_size)
    {
      insert (newNode->data);
      ++newNode;
    }
  }

  ~SearchTree ()
  {
    clear ();
  }

  // Return whether the tree is empty.
  bool
  empty () const
  {
    return (m_size==0);
  }

  // Return the size.
  size_t
  size () const
  {
    return m_size;
  }

  int
  depth () const
  {
    return depth (m_header.parent);
  }
  
  // Return an iterator pointing to the smallest element. 
  iterator
  begin ()
  {
    return iterator (m_header.right);
  }

  const_iterator
  begin () const
  {
    return const_iterator (m_header.right);
  }

  // Return an iterator pointing one beyond the last element,
  //   i.e. the header node. 
  iterator
  end ()
  {
    return iterator (&m_header);
  }

  const_iterator
  end () const
  {
    return const_iterator (&m_header);
  }

  iterator
  find (const T& v) const
  {
    // Call private helper method
    ConstNodePtr n = findHelper (v);
    if (n == nullptr)
    {
      // Wasn't found so return end ()
      n = &m_header;
    }
    return iterator (n);
  }

  std::pair<iterator, bool>
  insert (const T& v)
  {
    NodePtr insertedNode = insert (v, m_header.parent, &m_header);
    bool inserted = insertedNode != nullptr;
    if (inserted)
    {
      // Update header right to point to smallest node
      if (m_header.right == nullptr || v < m_header.right->data)
        m_header.right = insertedNode;
      // Update header left to point to largest node
      if (m_header.left == nullptr || v > m_header.left->data)
        m_header.left = insertedNode;
    }

    return { iterator (insertedNode), inserted };
  }

  size_t
  erase (const T& v)
  {
    // Update header right to point to smallest node
    if (m_header.right != nullptr && v == m_header.right->data)
      m_header.right =
        const_cast<NodePtr> (iterator::increment (m_header.right));
    // Update header left to point to largest node
    else if (m_header.left != nullptr && v == m_header.left->data)
      m_header.left =
        const_cast<NodePtr> (iterator::decrement (m_header.left));
    
    bool erased = erase (v, m_header.parent, &m_header);
    // If we erased the last value set header left and right to nullptr
    if (erased && empty ())
      m_header.left = m_header.right = nullptr;
    
    return erased ? 1 : 0;
  }

  // Delete all nodes, set header's parent, left, and right links to nullptr,
  //   and set size to 0. Utilizes a private, recursive "clear"
  //   declared below. 
  void
  clear ()
  {
    clear (m_header.parent);
    m_header.parent = nullptr;
    m_header.left = nullptr;
    m_header.right = nullptr;
    m_size = 0;
  }

  void
  printInOrder (ostream& out) const
  {
    printInOrder (out, m_header.parent);
  }

  void
  printLevelOrder (ostream& out) const
  {
    printLevelOrder (out, m_header.parent);
  }

private:

  using Node = struct Node<T>;
  using NodePtr = Node*;
  using ConstNodePtr = const Node*;

  int
  depth (ConstNodePtr r) const
  {
    if (r == nullptr)
      return -1;
    return std::max (depth (r->left), depth (r->right)) + 1;
  }

  NodePtr
  minimum (NodePtr r) const
  {
    // Return minimum node in tree rooted at "r".
    if (r != nullptr)
    {
      while (r ->left !=  nullptr)
        r = r->left;
    }  
    return r;  
  }

  NodePtr
  maximum (NodePtr r) const
  {
    // Return maximum node in tree rooted at "r".
    if (r != nullptr)
    {
      while (r ->right !=  nullptr)
        r = r->right;
    }  
    return r;
  }

  ConstNodePtr
  findHelper (const T& v) const
  {
    // Return a pointer to the node that contains "v".
    NodePtr pn = m_header.parent;
    while (pn != nullptr)
    {
      if (v < pn->data)
        pn = pn->left;
      else if (v > pn->data)
        pn = pn->right;
      else return pn;
    }
    
    return nullptr;
  }

  NodePtr
  insert (const T& v, NodePtr& r, NodePtr parent)
  {
    if (r != nullptr)
    {
      if (v < r->data)
        return insert (v, r->left, r); 
      if (v > r->data)
        return insert (v, r->right,r);
      return nullptr;
    }
    r = new Node (v, nullptr, nullptr, parent);
    ++m_size;
    return r;
  }

  bool
  erase (const T& v, NodePtr& r, NodePtr parent)
  {
    if (r == nullptr)
      return false;
    if (v < r->data)
      return erase (v, r->left, r);  
    if (v > r->data)
      return erase (v, r->right, r);
    if (r->left != nullptr && r->right != nullptr) 
    {
      NodePtr min = minimum (r->right);
      r->data = min->data;
      return erase (min->data,r->right, r);
    }
    NodePtr child = r->left;
    if (child == nullptr)
      child = r->right;
    delete r;
    r = child;
    if (r != nullptr)
      child->parent = parent;
    --m_size;
    return true;
  }

  void
  clear (NodePtr r)
  {
    if (r != nullptr) 
    {
      clear (r->left);
      clear (r->right);
    }
    delete r;

    m_header.left = nullptr;
    m_header.right = nullptr;
    m_header.parent = nullptr;
  }

  void
  printInOrder (ostream& out, NodePtr r) const
  {
    if (r != nullptr)
    {
      printInOrder (out, r->left);
      out << r->data << " ";
      printInOrder (out, r->right);
    }
  }

  void
  printLevelOrder (ostream& out, NodePtr r) const
  {
    unsigned treeDepth = depth(r);
    unsigned d = 1;
    unsigned depthCount = 0;
    unsigned currDepth = -1;
    queue<NodePtr> q;
    q.push (r);
    while (!q.empty ())
    {
      r = q.front ();
      q.pop ();
      if (depthCount == 0)
      {
          ++currDepth;
      }
      if (r == nullptr)
      {
        out << "- ";
      } else {
        out << r->data << " ";
        if (depthCount == 0)
        {
            out << "| ";
            d *= 2;
            depthCount = d;
        }
        q.push (r->left);
        q.push (r->right);
      }
      if (currDepth == treeDepth)
      {
          break;
      }
      --depthCount;
    }
  }
  
private:

  Node   m_header;
  size_t m_size;
};

/************************************************************/

// Output tree as [ e1 e2 e3 ... en ]
//   with a '|' to indicate the end of a level,
//   and a '-' to indicate a missing node.
// E.g., the tree
//       4
//    2     7
//         6
// should be printed EXACTLY like so: [ 4 | 2 7 | - - 6 - ]
// ONLY print the levels that exist, and ensure each level contains
//   2^k entries (a T object or "-"), where "k" is the level number. 
template<typename T>
ostream&
operator<< (ostream& out, const SearchTree<T>& tree)
{
  out << "[ ";
  // For the version you submit, ensure you are using "printLevelOrder"!
  // You may find "printInOrder" useful for early testing, since it's
  //   been written for you.
   tree.printInOrder (out);
  //tree.printLevelOrder (out);
  out << "]";

  return out;
}

/************************************************************/

#endif

/************************************************************/
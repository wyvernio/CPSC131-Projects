/*******************************************************************************
** CPSC 131: Part 4 - Associative Containers
**           Binary Search Tree Quiz
**
** Implement the Binary Search Tree code asked for in each question (TO-DO) below.
** The tree shall have no duplicate keys
**
** While the solutions to the questions below are highly inspired by the implementation
** examples presented in class, they will not be identical.  Read the questions carefully.
*******************************************************************************/


#include <utility>
#include <cstddef>
#include <string>
#include <iostream>
#include <iomanip>




template<typename Key, typename Value>
struct BST
{
  using KeyValue_Pair = std::pair<Key const, Value>;

  struct Node
  {
    Node() = default;
    Node( KeyValue_Pair const & pair ) : _pair{ pair } {}

    KeyValue_Pair _pair = { Key{}, Value{} };

    Node * _left   = nullptr;
    Node * _right  = nullptr;
    Node * _parent = nullptr;
  };

  Node *      _root = nullptr;
  std::size_t _size = 0;









  /*****************************************************************************
  ** Implement the BST's find function
  **   1)  find takes a key as its only argument and returns a pointer to the node containing that key
  **   2)  Write a recursive solution (not an iterative solution  Hint:  no while or for loops)
  **       o)  Write both the "public" and "private helper" functions
  **       o)  Clearly identify the Base Case, Visit, and Recurse steps  Hint:  write comments for each
  **           step before writing code
  **       o)  You may only compare the keys once
  *****************************************************************************/
  Node * find( Key const & key )
  {
    ///////////////////////// TO-DO (1) //////////////////////////////
    return find(key, _root);
    /////////////////////// END-TO-DO (1) ////////////////////////////
  }


  Node * find( Key const & key, Node * current )
  {
    ///////////////////////// TO-DO (2) //////////////////////////////

if (current == nullptr || current->_pair.first == key)
{
  // Base Case: If the current node is null or its key matches the given key, return the current node
  return current;
}
else if (key < current->_pair.first)
{
  // Visit: Compare the key with the current node's key
  // Recurse: If the key is less than the current node's key, recursively call find on the left subtree
  return find(key, current->_left);
}
else 
{
  // Visit: Compare the key with the current node's key
  // Recurse: If the key is greater than the current node's key, recursively call find on the right subtree
  return find(key, current->_right);
}
    /////////////////////// END-TO-DO (2) ////////////////////////////
  }



  /*****************************************************************************
  ** Implement the BST's insert function
  **   1)  Insert takes a key-value pair as its only argument and returns a pointer to the
  **       node containing that key
  **   2)  Write an iterative solution (not a recursive solution)
  **   3)  You may only compare the keys once
  *****************************************************************************/
  Node * insert( KeyValue_Pair const & pair )
  {
    ///////////////////////// TO-DO (3) //////////////////////////////

  Node * current = _root;

  Node * parent = nullptr;

  
  while (current != nullptr) {
    parent = current;
    if (pair.first == current->_pair.first)
      return current;
    else if (pair.first < current->_pair.first)
      current = current->_left;
    else
      current = current->_right;
  }



  Node * newNode = new Node(pair);
  if (parent == nullptr) {
    _root = newNode;
  } else if (pair.first < parent->_pair.first) {
    parent->_left = newNode;
  } else {
    parent->_right = newNode;
  }

  ++_size;

  return newNode;
    /////////////////////// END-TO-DO (3) ////////////////////////////
  }



  /*****************************************************************************
  ** Implement the BST's erase function
  **   1)  Erase takes a key as its only argument and returns nothing
  **   2)  Write an iterative solution (not a recursive solution)
  **   3)  Simplification:: Assume the node you are erasing does NOT have two children.  Do not write code
  **       to erase a node with two children.
  *****************************************************************************/
  void erase( Key const & key )
  {
    ///////////////////////// TO-DO (4) //////////////////////////////
  Node * current = _root;

  Node * parent = nullptr;

 
  while (current != nullptr && current->_pair.first != key) {
    parent = current;
    if (key < current->_pair.first)
      current = current->_left;
    else
      current = current->_right;
  }


  
  if (current != nullptr) {
    if (parent == nullptr) {
      _root = nullptr;
    } else if (current == parent->_left) {
      parent->_left = nullptr;
    } else {
      parent->_right = nullptr;
    }
    delete current;
    --_size;
  }

    /////////////////////// END-TO-DO (4) ////////////////////////////
  }



  /*****************************************************************************
  ** Implement the BST's index operator function
  **   1)  operator[] takes a key as its only argument and returns a reference to the
  **       value associated with that key
  **   2)  If the key does not already exist, insert it
  *****************************************************************************/
  Value & operator[]( Key const & key )
  {
    ///////////////////////// TO-DO (5) //////////////////////////////
  Node * node = find(key);

  if (node != nullptr)
  {
    return node->_pair.second;
  }
  Node* newNode = insert({key, Value{}});
  return newNode->_pair.second;

    /////////////////////// END-TO-DO (5) ////////////////////////////
  }



  /*****************************************************************************
  ** Implement the BST's clear function
  **   1)  Clear takes a pointer-to-Node as its only argument and returns nothing
  **   2)  Write a recursive solution (not an iterative solution  Hint:  no while or for loops)
  **       o)  Write the "private helper" function only (the "public" function is provided)
  **       o)  Clearly identify the Base Case, Visit, and Recurse steps  Hint:  write comments for each
  **           step before writing code
  *****************************************************************************/
  void clear() noexcept
  {
    clear( _root );
    _root = nullptr;
    _size = 0;
  }


  void clear( Node * current ) noexcept
  {
    ///////////////////////// TO-DO (6) //////////////////////////////
  if (current == nullptr)
  {
    return;
  }

  clear(current->_left);
  clear(current->_right);

  delete current;
    /////////////////////// END-TO-DO (6) ////////////////////////////
  }









  void print()
  {
    auto count = 0uz;
    print( _root, count );
  }

  void print( Node * current, std::size_t & count )  // in-order traversal
  {
    if( current == nullptr ) return;
    print( current->_left, count );

    auto && [key, value] = current->_pair;
    std::cout << std::setw(3) << ++count << ":  {" << key << ", " << value << "}\n";

    print( current->_right, count );
  }


  ~BST() noexcept
  { clear(); }
};






int main()
{
  BST<unsigned int, std::string> myTree;

  std::cout << "Test Case 1:\n";                     //          50         //
  myTree[50] = "indeed";                             //        /   \        //
  myTree.insert( { 40, "Structures" } );             //       40    60      //
  myTree.insert( { 60, "very" } );                   //      / \   / \      //
  myTree.insert( { 30, "Lego" } );                   //     30 45 55  70    //
  myTree.insert( { 45, "are" } );
  myTree[55] = "truly";
  myTree[70] = "entertaining";
  myTree.print();
  std::cout << "------------------------------\n";



  std::cout << "\n\nTest Case 2:\n";                 //          50         //
  myTree.erase( 70 );                                //        /   \        //
  myTree[30] = "Data";                               //       40    55      //
  myTree.erase( 60 );                                //      / \     \      //
  myTree.insert( { 75, "awesome" } );                //     30 45     75    //
  myTree.insert( { 50, "never" } );
  myTree.print();
  std::cout << "------------------------------\n";



  std::cout << "\n\nTest Case 3:\n";
  BST<char, int> anotherTree;
  anotherTree.find ('A');
  anotherTree.erase('B');
  std::cout << "myTree._size:       " << myTree     ._size << '\n'
            << "anotherTree._size:  " << anotherTree._size << '\n';

  auto n1 = anotherTree.insert({ 'C',0 });
  auto n2 = anotherTree.insert({ 'D',0 });
  auto n3 = anotherTree.insert({ 'B',0 });
  auto n4 = anotherTree.insert({ 'C',0 });
  auto n5 = anotherTree.insert({ 'D',0 });
  auto n6 = anotherTree.insert({ 'B',0 });
  std::cout << std::boolalpha
            << "Node 'B' found:  " << (n3 == n6 && n6 == anotherTree.find('B')) << '\n'
            << "Node 'C' found:  " << (n1 == n4 && n4 == anotherTree.find('C')) << '\n'
            << "Node 'D' found:  " << (n2 == n5 && n5 == anotherTree.find('D')) << '\n';
    std::cout << "------------------------------\n";
}

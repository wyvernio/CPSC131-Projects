#include <algorithm>                                                                      // max()
#include <array>                                                                          // array
#include <cmath>                                                                          // abs()
#include <cstddef>                                                                        // size_t
#include <exception>                                                                      // exception
#include <format>                                                                         // format_to()
#include <iostream>                                                                       // cerr, ,clog, cin, fixed(), showpoint(), left(), right(), ostream
#include <iterator>                                                                       // ostreambuf_iterator
#include <locale>                                                                         // locale, use_facet, moneypunct
#include <map>                                                                            // map
#include <queue>                                                                          // queue
#include <stack>                                                                          // stack
#include <stdexcept>                                                                      // invalid_argument, out_of_range
#include <string>                                                                         // stod(). string
#include <string_view>                                                                    // string_view
#include <utility>                                                                        // move()

#include "GroceryItem.hpp"
#include "GroceryItemDatabase.hpp"



namespace
{
  // Output some observed behavior.
  // Call this function from within the carefully_move_grocery_items functions, just before kicking off the recursion and then just after each move.

  // trace()
  void trace( std::stack<GroceryItem> const & sourceCart, std::stack<GroceryItem> const & destinationCart, std::stack<GroceryItem> const & spareCart, std::ostream & s = std::clog )
  {
    // Count and label the number of moves
    static std::size_t move_number = 0;

    // First time called will bind parameters to copies.
    //
    // Interrogating the stacks is a destructive process, so local copies of the parameters are made to work with.  The
    // carefully_move_grocery_items algorithm will swap the order of the arguments passed to this functions, but they will always be
    // the same objects - just in different orders. When outputting the stack contents, keep the original order so we humans can
    // trace the movements easier.  A container (std::map) indexed by the object's identity (address) is created to map address to a
    // predictable index and then the index is used so the canonical order remains the same from one invocation to the next.
    auto createMapping = [&]() -> std::map<std::stack<GroceryItem> const *, const unsigned>      // Let's accommodate mixing up the parameters
    {
      if( destinationCart.size() == 0 && spareCart.size() == 0 )
      {
        return { {&sourceCart, 0}, {&destinationCart, 1}, {&spareCart, 2} };
      }
      else if( sourceCart.size() == 0 && destinationCart.size() != 0 && spareCart.size() == 0 )
      {
        return { {&sourceCart, 1}, {&destinationCart, 0}, {&spareCart, 2} };
      }
      throw std::invalid_argument( "Error - Invalid argument:  Order of passed parameters passed to function trace(...) is incorrect" );

    };
    static std::map<std::stack<GroceryItem> const *, const unsigned> indexMapping = createMapping();
    struct LabeledCart
    {
      std::string             label;
      std::stack<GroceryItem> cart;
    };
    static std::array<LabeledCart, 3> groceryCarts = { LabeledCart{ "Broken Cart",  {} },
                                                       LabeledCart{ "Working Cart", {} },
                                                       LabeledCart{ "Spare Cart",   {} } };

    groceryCarts[indexMapping[&sourceCart]     ].cart = sourceCart;
    groceryCarts[indexMapping[&destinationCart]].cart = destinationCart;
    groceryCarts[indexMapping[&spareCart]      ].cart = spareCart;


    // Determine the height of the tallest stack
    std::size_t tallestStackSize = std::max( { groceryCarts[0].cart.size(),
                                               groceryCarts[1].cart.size(),
                                               groceryCarts[2].cart.size() } );


    // Print the header and underline it
    auto obuf_itr = std::ostreambuf_iterator<std::ostream::char_type>( s );

    std::format_to( obuf_itr, "After {:>3} moves:     ", move_number++ );                                   // print the move number
    for( auto && currentCart : groceryCarts ) std::format_to( obuf_itr, "{:<25.25}", currentCart.label );   // print the column labels
    std::format_to( obuf_itr, "\n{0:21}{0:->{1}}\n", "", 25 * groceryCarts.size() );                        // underline the labels




    // Print the stack's contents
    for( ; tallestStackSize > 0;  --tallestStackSize )                                                      // for each grocery item in a cart
    {
      std::format_to( obuf_itr, "{:21}", "" );                                                              // output a left margin to keep things lined up

      for( auto && currentCart : groceryCarts )                                                             // for each grocery item cart
      {
        if( currentCart.cart.size() == tallestStackSize )                                                   // if the current cart is this tall, print the top grocery item
        {
          std::string_view name = currentCart.cart.top().productName();

          if( name.size() > 24 ) std::format_to( obuf_itr, "{}... ", name.substr( 0, 21 ) );                // replace last few characters of long names with "..."
          else                   std::format_to( obuf_itr, "{:<25}", name) ;                                // 24 characters plus a space to separate columns

          currentCart.cart.pop();
        }
        else std::format_to( obuf_itr, "{:25}", "");                                                        // otherwise, nothing to print in this cart so print whitespace instead
      }
      s << '\n';
    }
    std::format_to( obuf_itr, "{0:21}{0:=>{1}}\n\n\n\n\n\n\n", "", 25 * groceryCarts.size() );              // display a distinct marker between moves
  }  // trace()







  // carefully_move_grocery_items() - recursive
  /*********************************************************************************************************************************
  ** A recursive algorithm to carefully move grocery items from a broken cart to a working cart is given as follows:
  ** START
  ** Procedure carefully_move_grocery_items (number_of_items_to_be_moved, broken_cart, working_cart, spare_cart)
  **
  **    IF number_of_items_to_be_moved == 1, THEN
  **       move top item from broken_cart to working_cart
  **       trace the move
  **
  **    ELSE
  **       carefully_move_grocery_items (number_of_items_to_be_moved-1, broken_cart, spare_cart, working_cart)
  **       move top item from broken_cart to working_cart
  **       trace the move
  **       carefully_move_grocery_items (number_of_items_to_be_moved-1, spare_cart, working_cart, broken_cart)
  **
  **    END IF
  **
  ** END Procedure
  ** STOP
  **
  ** As a side note, the efficiency class of this algorithm is exponential.  That is, the Big-O is O(2^n).
  *********************************************************************************************************************************/
  void carefully_move_grocery_items( std::size_t quantity, std::stack<GroceryItem> & broken_cart, std::stack<GroceryItem> & working_cart, std::stack<GroceryItem> & spare_cart )
  {
    ///////////////////////// TO-DO (1) //////////////////////////////
      /// Implement the algorithm above.
    if (quantity == 1) {
        // Base case: Move the top item from the broken cart to the working cart
        working_cart.push(std::move(broken_cart.top()));
        broken_cart.pop();
        // Trace the move
        trace(broken_cart, working_cart, spare_cart);
    } else {

        // Recursive:
        // Move (quantity - 1) items from broken_cart to spare_cart using working_cart as spare
        carefully_move_grocery_items(quantity - 1, broken_cart, spare_cart, working_cart);

        // Move the top item from broken_cart to working_cart
        working_cart.push(broken_cart.top());
        broken_cart.pop();

        // Trace the move
        trace(broken_cart, working_cart, spare_cart);

        // Move (quantity - 1) items from spare_cart to working_cart using broken_cart as spare
        carefully_move_grocery_items(quantity - 1, spare_cart, working_cart, broken_cart);
    }
    /////////////////////// END-TO-DO (1) ////////////////////////////
  }



  // carefully_move_grocery_items() - starter
  void carefully_move_grocery_items( std::stack<GroceryItem> & from, std::stack<GroceryItem> & to )
  {
    ///////////////////////// TO-DO (2) //////////////////////////////
      /// Implement the starter function for the above algorithm.  If the "from" cart contains grocery items, move those grocery
      /// items to the "to" cart while ensuring the breakable grocery items are always on top of the nonbreakable grocery items,
      /// just like they already are in the "from" cart.  That is, call the above carefully_move_grocery_items function to start
      /// moving grocery items recursively.  Call the above trace function just before calling carefully_move_grocery_items to get a
      /// starting point reference in the movement report.


    // Create an empty spare cart
    std::stack<GroceryItem> spareCart;

    // Trace before moving items
    trace(from, to, spareCart);

    // Start moving grocery items recursively
    if (!from.empty()) {
        carefully_move_grocery_items(from.size(), from, to, spareCart);
    }

    // Trace after moving items
    trace(from, to, spareCart);

    /////////////////////// END-TO-DO (2) ////////////////////////////
  }
}    // namespace




// main()
int main( int argc, char * argv[] )
{
  try
  {
    // Snag an empty cart as I enter the grocery store
    ///////////////////////// TO-DO (3) //////////////////////////////
      /// Create an empty grocery item cart as a stack of grocery items and call it myCart.

    std::stack<GroceryItem> myCart;
    /////////////////////// END-TO-DO (3) ////////////////////////////




    // Shop for a while placing grocery items into my grocery item cart
    //
    // Put the following grocery items into your cart with the heaviest grocery item on the bottom and the lightest grocery item on
    // the top according to the ordering given in the table below
    //
    //      UPC Code         Name             Brand
    //      --------------   -------------    ---------------
    //      00688267039317   eggs             any                     <=== lightest item, put this on the top so heavy items wont break them
    //      00835841005255   bread            any
    //      09073649000493   apple pie        any
    //      00025317533003   hotdogs          Applegate Farms
    //      00038000291210   rice krispies    Kellogg's
    //      00075457129000   milk             any                     <===  heaviest item, put this on the bottom

    ///////////////////////// TO-DO (4) //////////////////////////////
    
    myCart.push(GroceryItem("milk",           "",                     "00075457129000"));
    myCart.push(GroceryItem("rice krispies",  "Kellogg's",            "00038000291210"));
    myCart.push(GroceryItem("hotdogs",        "Applegate Farms",      "00025317533003"));
    myCart.push(GroceryItem("apple pie",      "",                     "09073649000493"));
    myCart.push(GroceryItem("bread",          "",                     "00835841005255"));
    myCart.push(GroceryItem("eggs",           "",                     "00688267039317"));
    /////////////////////// END-TO-DO (4) ////////////////////////////




    // A wheel on my cart has just broken and I need to move grocery items to a new cart that works
    ///////////////////////// TO-DO (5) //////////////////////////////
      /// Create an empty grocery item cart as a stack of grocery items and call it workingCart.  Then carefully move the grocery
      /// items in your now broken cart to this working cart by calling the above carefully_move_grocery_items function with two
      /// arguments.

    std::stack<GroceryItem> workingCart;
    carefully_move_grocery_items(myCart, workingCart);
    /////////////////////// END-TO-DO (5) ////////////////////////////




    // Time to checkout and pay for all this stuff.  Find a checkout line and start placing grocery items on the counter's conveyor belt
    ///////////////////////// TO-DO (6) //////////////////////////////
      /// Create an empty checkout counter as a queue of grocery items and call it checkoutCounter.  Then remove the grocery items
      /// from your working cart and place them on the checkout counter, i.e., put them in this checkoutCounter queue.

    std::queue<GroceryItem> checkoutCounter;
    while (!workingCart.empty()) 
    {
      checkoutCounter.push(std::move(workingCart.top()));
      workingCart.pop();
    }
    /////////////////////// END-TO-DO (6) ////////////////////////////




    // Now add it all up and print a receipt
    double amountDue = 0.0;
    GroceryItemDatabase & worldWideDatabase = GroceryItemDatabase::instance();              // Get a reference to the world wide database of grocery items. The database
                                                                                            // contains the full description and price of the grocery item.

    ///////////////////////// TO-DO (7) //////////////////////////////
      /// For each grocery item in the checkout counter queue, find the grocery item by UPC in the world wide grocery item database.
      /// If the grocery item on the counter is found in the database then accumulate the amount due and print the grocery item's full
      /// description and price on the receipt (i.e. write the grocery item's full description and price to standard output).
      /// Otherwise, print a message on the receipt that a description and price for the grocery item wasn't found and there will be
      /// no charge.


while (!checkoutCounter.empty()) {
    
    auto item = worldWideDatabase.find(checkoutCounter.front().upcCode());
    if (item != nullptr) {
        
        amountDue += item->price();
        
        std::cout << *item << '\n';
    } else {
        
        std::cout << '"' << checkoutCounter.front().upcCode() << "\" (" << checkoutCounter.front().productName() << ") not found, so today is your lucky day - You get it for free! Hooray!\n";
    }
    checkoutCounter.pop();
}
    /////////////////////// END-TO-DO (7) ////////////////////////////



    // Now check the receipt - are you getting charged the correct amount?
    // You can either pass the expected total when you run the program by supplying a parameter, like this:
    //    program 35.89
    // or if no expected results is provided at the command line, then prompt for and obtain expected result from standard input
    double expectedAmountDue = 0.0;
    if( argc >= 2 )
    {
      try
      {
        expectedAmountDue = std::stod( argv[1] );
      }
      catch( std::invalid_argument & ) {}                                                   // ignore anticipated bad command line argument
      catch( std::range_error &      ) {}                                                   // ignore anticipated bad command line argument
    }
    else
    {
      std::cout << "What is your expected amount due?  ";
      std::cin  >> expectedAmountDue;
    }

    auto locale          = std::locale( "en_US.UTF-8" );
    auto currency_symbol = std::use_facet<std::moneypunct<char>>( locale ).curr_symbol();
    std::cout << std::format( locale, "{:->25}\nTotal  {}{:.2Lf}\n\n\n", "", currency_symbol, amountDue );

    if( std::abs(amountDue - expectedAmountDue) < 1E-4 ) std::clog << "PASS - Amount due matches expected\n";
    else                                                 std::clog << "FAIL - You're not paying the amount you should be paying\n";
  }

  catch( std::exception & ex )
  {
    std::cerr << "ERROR:  Unhandled exception:  " << typeid( ex ).name() << '\n'
              << ex.what() << '\n';
  }
  return 0;
}

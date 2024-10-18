#include <exception>
#include <iostream>
#include <sstream>                                                                    // istringstream
#include <typeinfo>

#include "GroceryItem.hpp"
#include "GroceryList.hpp"




namespace
{
  void basicScenario()
  {
    // Let's start a grocery list
    GroceryList thingsToBuy = { { "milk"     },
                                { "hot dogs" },
                                { "eggs"     },
                                { "bread"    } };

    // Changed my mind, I want to make sure I can get eggs before running out of money so I'm going to move that to the top of my list
    thingsToBuy.moveToTop( { "eggs" } );

    // Let's see what's on the list so far
    std::cout << "My Grocery List" << thingsToBuy << "\n\n";



    // Hmm ..., no.  Need to add a few more things
    thingsToBuy += { { "bananas" },
                     { "apples"  } };




    // My roommate also has a grocery list
    GroceryList roommatesList = { { "potato chips", "Ruffles"    },
                                  { "potato chips", "Frito Lays" },
                                  { "beer",         "Bud Lite"   },
                                  { "eggs"                       },
                                  { "pretzels"                   } };

    std::cout << "My roommate's Grocery List" << roommatesList << "\n\n";

    // Let's combine the lists (if different) and go shopping
    if( thingsToBuy != roommatesList ) thingsToBuy += roommatesList;


    // Oops, forgot butter.  Let's add that right before bread.
    thingsToBuy.insert( { "butter", "Lakes 'Ole" }, thingsToBuy.find( { "bread" } ) );

    // And beer, really?  You should be studying!
    thingsToBuy.remove( { "beer", "Bud Lite" } );


    // Now let's see what's on the list
    std::cout << "Combined Grocery Lists" << thingsToBuy << "\n\n";



    // Did you get it right?
    //
    // I could construct "expectedResults" with the list of grocery items as above, but I want to exercise GroceryList's extraction
    // operator.  To do that, I could:
    //   1) read the list of grocery items from standard input and type the data at the keyboard, but that is time consuming and
    //      error prone.  That's easily handled by creating a text file with grocery item data then redirecting that text file to
    //      standard input at the command line when launching the program.  But then I would have to separately maintain that file
    //      and include it in the set of student files.  More importantly, that would prevent you from doing that in the TO-DO
    //      section below.
    //   2) open a text file directly using either command line arguments that supply the file's path and name, or hard coding the
    //      file's path and name.  But that would require using (and understanding) the standard's ifstream interface, which is easy
    //      enough given sufficient time, but I don't want to go down that rabbit hole in this course.
    //   3) create an in-memory text file (well, more accurately, an in-memory input stream) and read from there.  This allows me to
    //      exercise the extraction operator while avoiding the perils described in options 1) and 2) above.
    // So, as a design decision, let's chose option 3), create an in-memory input stream and read from there.
    //
    //                                            UPC | Brand Name   | Product Name   | Price
    //                                            ----+--------------+----------------+--------
    std::istringstream expectedResultsStream( R"( "",   "",            "eggs",          0.0
                                                  "",   "",            "milk",          0.0
                                                  "",   "",            "hot dogs",      0.0
                                                  "",   "Lakes 'Ole",  "butter",        0.0
                                                  "",   "",            "bread",         0.0
                                                  "",   "",            "bananas",       0.0
                                                  "",   "",            "apples",        0.0
                                                  "",   "Ruffles",     "potato chips",  0.0
                                                  "",   "Frito Lays",  "potato chips",  0.0
                                                  "",   "",            "pretzels",      0.0 )"   // multi-line raw string literal of grocery items
                                               );

    GroceryList expectedResults;
    expectedResultsStream >> expectedResults;

    std::cout << "\nExpected results:" << expectedResults << "\n\n"
              << "\nActual results:  " << thingsToBuy     << "\n\n"
              << "\nTest results:    " << ( thingsToBuy == expectedResults ? "PASS" : "FAIL" ) << '\n';
  }
}




int main()
{
  try
  {
    basicScenario();


    ///////////////////////// TO-DO (1) //////////////////////////////
     /// Create, manipulate, and display your own GroceryList object here.  Not looking for anything specific but don't just repeat
     /// what I've already done above.  Be imaginative and create your own story.  Maybe you're restocking a depleted food bank, or
     /// preparing for a holiday meal, or catering a banquette, or planning an all night study session, or ...  You *must* use all
     /// the functions of the GroceryList interface, including the insertion, extraction, and relational operators.  Try inserting
     /// grocery items from both the top and the bottom of the lists using the enumerated position values TOP and BOTTOM as well as
     /// indexed offsets.  Remove grocery items from the top, middle, and bottom.  Create, concatenate, rearrange, and compare
     /// several (more than two) lists. Have some fun with it!  The purpose is to show me you, as a GroceryList class consumer
     /// (i.e., the client) understand how to *use* the GroceryList.


  // Start with a basic list of ingredients for making a cake
  GroceryList cakeIngredients = { { "flour" },
                                  { "sugar" },
                                  { "eggs" },
                                  { "butter" },
                                  { "vanilla extract" } };

  // Display the initial shopping list
  std::cout << "Initial Cake Baking Shopping List" << cakeIngredients << "\n\n";

  // You realize you need more items, add them to the list
  cakeIngredients += { { "baking powder" },
                       { "cocoa powder" },
                       { "milk" } };

  std::cout << "Added Ingredients Cake Baking Shopping List" << cakeIngredients << "\n\n";

  // Oops, forgot to get vanilla extract, move it to the top of the list
  cakeIngredients.moveToTop({ "vanilla extract" });

  // Remove sugar from the list, you decide to use a sugar substitute
  cakeIngredients.remove({ "sugar" });

  // Let's create another list for frosting
  GroceryList frostingIngredients = { { "powdered sugar" },
                                     { "butter" },
                                     { "cream cheese" },
                                     { "almond extract" } };

  std::cout << "Frosting Ingredients List To Add" << frostingIngredients << "\n\n";

  // Combine the cake and frosting ingredients
  cakeIngredients += frostingIngredients;

  // Insert chocolate chips just before cream cheese in the combined list
  cakeIngredients.insert({ "chocolate chips" }, cakeIngredients.find({ "cream cheese" }));

  // Display the final combined list
  std::cout << "Non-adjusted Cake Baking Shopping List" << cakeIngredients << "\n\n";

  // Now, let's compare it with an expected result
  std::istringstream expectedCakeListStream   (R"( "", "",               "vanilla extract",            0.0
                                                   "", "",               "flour",    0.0
                                                   "", "",               "eggs",     0.0
                                                   "", "",               "butter",             0.0
                                                   "", "",               "baking powder",           0.0
                                                   "", "",               "cocoa powder",  0.0
                                                   "", "",               "milk",   0.0
                                                   "", "",               "powdered sugar",     0.0
                                                   "", "",               "chocolate chips",  0.0  
                                                   "", "",               "cream cheese",     0.0
                                                   "", "",               "almond extract",  0.0 )" // multi-line raw string literal of grocery items
                                                   );

  GroceryList expectedCakeList;
  expectedCakeListStream >> expectedCakeList;

  std::cout << "\nExpected Cake Baking Shopping List:" << expectedCakeList << "\n\n"
            << "\nActual Cake Baking Shopping List:  " << cakeIngredients     << "\n\n"
            << "\nTest results:                      " << (cakeIngredients == expectedCakeList ? "PASS" : "FAIL") << '\n';



    /////////////////////// END-TO-DO (1) ////////////////////////////


    
  }

  catch( const std::exception & ex )
  {
    std::cerr << "Fatal Error:  unhanded exception \"" << typeid( ex ).name() << "\"\n"
              << ex.what() << '\n';
  }
}

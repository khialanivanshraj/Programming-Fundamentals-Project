#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

struct Product
{
  int categoryID;
  int productID;
  string company;
  string model;
  double price;
  int stock;
};

// --- FUNCTION PROTOTYPES ---

// Auth Functions
bool authSystem();
void registerUser();
bool loginUser();

// Core Functions
int loadProducts(Product p[100]);
void saveProducts(Product p[100], int count);
void showCategories(Product p[100], int count);
void showProductsInCategory(Product p[100], int count, int catID);
void searchProducts(Product p[100], int count);

// Cart Functions
int addToCart(Product p[100], int pCount, Product cart[100], int cartCount);
int removeFromCart(Product p[100], int pCount, Product cart[100], int cartCount);
void viewCart(Product cart[100], int cCount);

bool checkout(Product p[100], int pCount, Product cart[100], int cCount);
void generateReceipt(Product cart[100], int cCount, double total, double discount, double finalAmount);

// HELPER FUNCTION
string toLowerCase(string str);

int main()
{

  // AUTHENTICATION
  if (!authSystem())
  {
    cout << "Exiting program. Goodbye!\n";
    return 0;
  }
  // LOAD PRODUCTS & INITIALIZE CART
  Product products[100], cart[100];
  int productCount = loadProducts(products); // LOAD PRODUCTS FROM FILE INTO products ARRAY AND GET COUNT HOW MANY PRODUCTS LOADED
  int cartCount = 0; // INITIALIZE CART COUNT TO ZERO

  if (productCount == 0) //IF NO PRODUCTS LOADED
  {
    cout << "Unable to load products. Make sure 'products.txt' exists.\n";
    return 0;
  }

  cout << "\n============================================\n";
  cout << "          WELCOME TO ELECTRO STORE\n";
  cout << "============================================\n";

  int choice;

  do
  {
    cout << "\n------------- MAIN MENU -------------\n";
    cout << "1. View Categories\n";
    cout << "2. Search Products\n";
    cout << "3. Add to Cart\n";
    cout << "4. Remove From Cart\n";
    cout << "5. View Cart\n";
    cout << "6. Checkout\n";
    cout << "7. Exit\n";
    cout << "-------------------------------------\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1)
    {
      showCategories(products, productCount); //productCount is the total number of products loaded from the file. save value to count variable used in showCategories function!
    }
    else if (choice == 2)
    {
      searchProducts(products, productCount);
    }
    else if (choice == 3)
    {
      cartCount = addToCart(products, productCount, cart, cartCount); // Update cartCount after adding to cart will save the new cart count returned by addToCart function. thats why we assign it back to cartCount variable.
    }
    else if (choice == 4)
    {
      cartCount = removeFromCart(products, productCount, cart, cartCount);
    }
    else if (choice == 5)
    {
      viewCart(cart, cartCount);
    }
    else if (choice == 6)
    {
      if (checkout(products, productCount, cart, cartCount))
      {
        cout << "\nThank you!\n";
        break;
      }
    }
    else if (choice != 7)
    {
      cout << "Invalid choice.\n";
    }

  } while (choice != 7);

  return 0;
}

bool authSystem()
{
  int choice;
  while (true)
  {
    cout << "\n========= AUTHENTICATION =========\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Exit\n";
    cout << "==================================\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1)
    {
      if (loginUser())
        return true;
    }
    else if (choice == 2)
    {
      registerUser();
    }
    else if (choice == 3)
    {
      return false;
    }
    else
    {
      cout << "Invalid choice.\n";
    }
  }
}

// REGISTER FUNCTION
void registerUser()
{
  string username, password;
  cout << "\n--- REGISTER ---\n";
  cout << "Enter new username: ";
  cin.ignore();
  getline(cin, username);
  cout << "Enter new password: ";
  getline(cin, password);

  ofstream file("users.txt", ios::app); //  ios::app to add the new user at the end of the file
  if (file.is_open())
  {
    file << username << endl;
    file << password << endl;
    file.close();
    cout << "Registration Successful! Please Login.\n";
  }
  else
  {
    cout << "Error opening users file.\n";
  }
}

// LOGIN FUNCTION
bool loginUser()
{
  string username, password, u, p;
  cout << "\n--- LOGIN ---\n";
  cout << "Username: ";
  cin.ignore();
  getline(cin, username);
  cout << "Password: ";
  getline(cin, password);

  ifstream file("users.txt"); // Reading from users file
  bool found = false;

  if (!file)
  { // File doesn't exist or can't be opened (no users registered)
    cout << "No users registered yet.\n";
    return false;
  }

  while (getline(file, u) && getline(file, p))
  { // READ USERNAME AND PASSWORD PAIR (STRUCTURED SQUENTIAL ACCESS)
    if (u == username && p == password)
    {
      found = true; // HERE WE FOUND THE USER
      break;
    }
  }
  file.close();

  if (found)
  {
    cout << "Login Successful! Welcome " << username << ".\n";
    return true;
  }
  else
  {
    cout << "Invalid credentials. Try again.\n";
    return false;
  }
}

// HELPER FUNCTION TO CONVERT STRING TO LOWERCASE
string toLowerCase(string str)
{
  string lowerStr = str; // START WITH ORIGINAL STRING
  for (char &c : lowerStr)
  {                 // ITERATE THROUGH EACH CHARACTER (BY REFERENCE MEANS WE CAN MODIFY IT DIRECTLY)
    c = tolower(c); // CONVERT TO LOWERCASE BY USING tolower FUNCTION FROM <cctype>
  }
  return lowerStr;
}

// SEARCH FUNCTION
void searchProducts(Product p[100], int count)
{
  string query;
  cout << "\nEnter product name to search: ";
  cin.ignore();
  getline(cin, query);

  string lowerQuery = toLowerCase(query); // CONVERT SEARCH QUERY TO LOWERCASE FOR CASE-INSENSITIVE COMPARISON
  bool foundAny = false;

  cout << "\n---------------- SEARCH RESULTS ----------------\n";
  cout << left << setw(10) << "ID"
       << setw(15) << "Company"
       << setw(20) << "Model"
       << setw(12) << "Price" << endl;
  cout << "------------------------------------------------\n";

  for (int i = 0; i < count; i++)
  {

    string fullName = p[i].company + " " + p[i].model; // COMBINE COMPANY AND MODEL NAME FOR EASY SEARCHING
    string lowerName = toLowerCase(fullName);          // CONVERT PRODUCT NAME TO LOWERCASE

    if (lowerName.find(lowerQuery) != string::npos)
    { // CHECK IF SEARCH QUERY IS A SUBSTRING OF PRODUCT NAME (STRING::npos MEANS NOT FOUND) (NPOS IS A STRING CONSTANT REPRESENTING AN INVALID POSITION)
      cout << left << setw(10) << p[i].productID
           << setw(15) << p[i].company
           << setw(20) << p[i].model
           << setw(12) << p[i].price << endl;
      foundAny = true; // WE FOUND AT LEAST ONE MATCH
    }
  }

  if (!foundAny)
  { // NO PRODUCTS MATCHED THE SEARCH QUERY
    cout << "No products found matching '" << query << "'.\n";
  }
  cout << "------------------------------------------------\n";
}

// LOAD PRODUCTS FROM FILE
int loadProducts(Product p[100])
{
  ifstream file("products.txt");
  if (!file)
    return 0;
  int i = 0;
  while (file >> p[i].categoryID >> p[i].productID >> p[i].company >> p[i].model >> p[i].price >> p[i].stock) // WILL READ UNTIL EOF AT FIRST i = 0 first product will be stored at index 0.
  {
    i++; // INCREMENT INDEX FOR NEXT PRODUCT
  }
  return i; // RETURN TOTAL NUMBER OF PRODUCTS LOADED
}


// SAVE PRODUCTS TO FILE AFTER CHECKOUT
void saveProducts(Product p[100], int count)
{
  ofstream file("products.txt"); // OPEN FILE IN WRITE MODE TO OVERWRITE EXISTING DATA
  for (int i = 0; i < count; i++) // WRITE EACH PRODUCT BACK TO FILE
  {
    file << p[i].categoryID << " " << p[i].productID << " " << p[i].company << " " << p[i].model << " " << p[i].price << " " << p[i].stock << endl;
  }
}

// SHOW CATEGORIES
void showCategories(Product p[100], int count)
{
  cout << "\n-------------- CATEGORIES --------------\n";
  cout << "1. Laptop\n2. Mouse\n3. Keyboard\n4. Charger\n";
  cout << "----------------------------------------\n";
  cout << "Choose category: ";
  int choice;
  cin >> choice;
  if (choice >= 1 && choice <= 4)
    showProductsInCategory(p, count, choice); // CALL FUNCTION TO SHOW PRODUCTS IN SELECTED CATEGORY
  else
    cout << "Invalid category choice.\n";
}

// SHOW PRODUCTS IN A CATEGORY
void showProductsInCategory(Product p[100], int count, int catID)
{
  cout << "\n================ Category " << catID << " ================\n";
  cout << left << setw(10) << "ID" << setw(15) << "Company" << setw(20) << "Model Name" << setw(12) << "Price" << setw(10) << "Stock" << endl;
  cout << "----------------------------------------------------------------\n";
  for (int i = 0; i < count; i++) // ITERATE THROUGH ALL PRODUCTS
  {
    if (p[i].categoryID == catID) // CHECK IF PRODUCT BELONGS TO SELECTED CATEGORY
    {
      cout << left << setw(10) << p[i].productID << setw(15) << p[i].company << setw(20) << p[i].model << setw(12) << fixed << setprecision(2) << p[i].price << setw(10) << p[i].stock << endl;
    }
  }
}

// ADD TO CART FUNCTION
int addToCart(Product p[100], int pCount, Product cart[100], int cartCount)
{
  int id, qty;
  cout << "\nEnter Product ID: ";
  cin >> id;
  int index = -1; //intialize index to -1 to indicate not found
  for (int i = 0; i < pCount; i++) // iterate through products to find product by ID
  {
    if (p[i].productID == id) // if found
    {
      index = i; // store index
      break;
    }
  }

  if (index == -1) // if not found
  {
    cout << "Product not found.\n";
    return cartCount; // return current cart count without changes
  }

  cout << "Enter quantity: ";
  cin >> qty;
  if (qty <= 0 || qty > p[index].stock) // check for valid quantity
  {
    cout << "Invalid or Not Enough quantity (Available: " << p[index].stock << ").\n";
    return cartCount;
  }

  cart[cartCount] = p[index]; // add product to cart
  cart[cartCount].stock = qty; // set quantity in cart
  cartCount++; // increment cart count
  p[index].stock -= qty; // reduce stock in main product list
  cout << "\n✔ Added to cart successfully.\n";
  return cartCount; // return updated cart count
}

// REMOVE FROM CART FUNCTION
int removeFromCart(Product p[100], int pCount, Product cart[100], int cartCount)
{
  if (cartCount == 0) // check if cart is empty
  {
    cout << "Cart empty.\n";
    return cartCount; // return current cart count
  }
  int id;
  cout << "Enter Product ID to remove: ";
  cin >> id;
  int index = -1; // initialize index to -1 to indicate not found
  for (int i = 0; i < cartCount; i++) // iterate through cart to find product by ID
  {
    if (cart[i].productID == id) // if found
    {
      index = i; // store index
      break;
    }
  }
  if (index == -1)
  {
    cout << "Not in cart.\n";
    return cartCount; // return current cart count without changes
  }

  for (int i = 0; i < pCount; i++) // restore stock to main product list
  {
    if (p[i].productID == id) // find product by ID
    {
      p[i].stock += cart[index].stock; // restore stock to main product list
      break;
    }
  }
  for (int i = index; i < cartCount - 1; i++) // shift remaining items in cart to fill the gap
    cart[i] = cart[i + 1]; // shift left to remove the item
  cartCount--; // decrement cart count
  cout << "\n Removed from cart.\n";
  return cartCount; // return updated cart count
}

// VIEW CART FUNCTION
void viewCart(Product cart[100], int cCount)
{
  if (cCount == 0) // check if cart is empty
  {
    cout << "\nCart is empty.\n";
    return;
  }
  double total = 0; // initialize total amount
  cout << "\n============== YOUR CART ==============\n";
  cout << left << setw(10) << "ID" << setw(15) << "Company" << setw(20) << "Model Name" << setw(10) << "Qty" << setw(12) << "Subtotal" << endl;
  cout << "----------------------------------------------------------------\n";
  for (int i = 0; i < cCount; i++) // iterate through cart items
  {
    double sub = cart[i].price * cart[i].stock; // calculate subtotal for each item
    total += sub; // add to total amount
    cout << left << setw(10) << cart[i].productID << setw(15) << cart[i].company << setw(20) << cart[i].model << setw(10) << cart[i].stock << setw(12) << fixed << setprecision(2) << sub << endl;
  }
  cout << "----------------------------------------------------------------\n";
  cout << "Total = " << fixed << setprecision(2) << total << endl; // display total amount (used fixed and setprecision to format the output to 2 decimal places)
}


// CHECKOUT FUNCTION
bool checkout(Product p[100], int pCount, Product cart[100], int cCount)
{
  if (cCount == 0) // check if cart is empty
  {
    cout << "Cart empty.\n";
    return false; // return false indicating checkout failed
  }
  viewCart(cart, cCount); // show cart before checkout (function reuse)
  char x;
  cout << "\nConfirm purchase (y/n): ";
  cin >> x;
  if (x != 'y' && x != 'Y') // if user cancels checkout
  {
    cout << "\nCancelled. Restoring stock.\n";
    for (int i = 0; i < cCount; i++) // restore stock to main product list
    {
      for (int j = 0; j < pCount; j++) // find product by ID
      {
        if (cart[i].productID == p[j].productID) // if found
        {
          p[j].stock += cart[i].stock; // restore stock to main product list
          break;
        }
      }
    }
    return false; // return false indicating checkout cancelled
  }
  double total = 0;
  for (int i = 0; i < cCount; i++) // calculate total amount
    total += cart[i].price * cart[i].stock;   // sum up subtotals for each item
  double discount = (total > 500000) ? total * 0.20 : 0; // 20% discount for purchases over 500,000 PKR
  double finalAmount = total - discount; // calculate final amount after discount
  cout << fixed << setprecision(2);
  saveProducts(p, pCount); // save updated product stock to file(function calling saveProducts to persist the updated stock levels)
  generateReceipt(cart, cCount, total, discount, finalAmount); // generate receipt (function calling generateReceipt to create a receipt file for the purchase)
  cout << "\n Purchase Successful!\n Receipt Generated!\n";
  return true;
}

void generateReceipt(Product cart[100], int cCount, double total, double discount, double finalAmount)
{
  string name, phone;
  cin.ignore();
  cout << "\nEnter your Name: ";
  getline(cin, name);
  cout << "Enter your Phone Number: ";
  getline(cin, phone);
  ofstream receipt("receipt.txt"); // open receipt file for writing
  receipt << fixed << setprecision(2); // set decimal precision for monetary values
  receipt << "============================================\n             PURCHASE RECEIPT\n============================================\n";
  receipt << "Customer Name : " << name << "\nPhone Number  : " << phone << "\n";
  receipt << "--------------------------------------------\n"
          << left << setw(10) << "ID" << setw(20) << "Product" << setw(10) << "Qty" << setw(12) << "Subtotal" << endl
          << "--------------------------------------------\n";
  for (int i = 0; i < cCount; i++) // iterate through cart items
  {
    double sub = cart[i].price * cart[i].stock;
    string fullName = cart[i].company + " " + cart[i].model;
    receipt << left << setw(10) << cart[i].productID << setw(20) << fullName << setw(10) << cart[i].stock << setw(12) << sub << endl;
  }
  receipt << "--------------------------------------------\nTotal Amount : " << total << " PKR\nDiscount     : " << discount << " PKR\nFinal Payable: " << finalAmount << " PKR\n============================================\n         THANK YOU FOR SHOPPING!\n============================================\n";
  receipt.close();
  cout << "\n Receipt saved as 'receipt.txt'.\n";
}

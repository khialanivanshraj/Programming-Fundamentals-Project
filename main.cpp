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

bool authSystem();
void registerUser();
bool loginUser();
void forgotPassword();

int loadProducts(Product p[100]);
void saveProducts(Product p[100], int count);
void showCategories(Product p[100], int count);
void showProductsInCategory(Product p[100], int count, int catID);
void searchProducts(Product p[100], int count);

int addToCart(Product p[100], int pCount, Product cart[100], int cartCount);
int removeFromCart(Product p[100], int pCount, Product cart[100], int cartCount);
void viewCart(Product cart[100], int cCount);

bool checkout(Product p[100], int pCount, Product cart[100], int cCount);
void generateReceipt(Product cart[100], int cCount, double total, double discount, double finalAmount);

string toLowerCase(string str); //helper for search

int main()
{
    if (!authSystem())
    {
        cout << "Exiting program. Goodbye!\n";
        return 0;
    }

    Product products[100], cart[100];
    int productCount = loadProducts(products);
    int cartCount = 0;

    if (productCount == 0)
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
            showCategories(products, productCount);
        }
        else if (choice == 2)
        {
            searchProducts(products, productCount);
        }
        else if (choice == 3)
        {
            cartCount = addToCart(products, productCount, cart, cartCount);
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
                cout << "\nExiting Program. Have a nice day!\n";
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
        cout << "3. Forgot Password\n";
        cout << "4. Exit\n";

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
            forgotPassword();
        }
        else if(choice == 4){
          return false;
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }
}

void registerUser()
{
  string username, email, password;
  cout << "\n--- REGISTER ---\n";
  cout << "Enter new username: ";
  cin.ignore();
  getline(cin, username);

  cout << "Enter your email: ";
  getline(cin, email);

  cout << "Enter new password: ";
  getline(cin, password);

  ofstream file("users.txt", ios::app);
  if (file.is_open())
  {
    file << username << endl;
    file << email << endl;
    file << password << endl;
    file.close();
    cout << "Registration Successful! Please Login.\n";
  }
  else
  {
    cout << "Error opening users file.\n";
  }
}

bool loginUser()
{
  string username, password, u, e, p;
  cout << "\n--- LOGIN ---\n";
  cout << "Username: ";
  cin.ignore();
  getline(cin, username);
  cout << "Password: ";
  getline(cin, password);

  ifstream file("users.txt");
  bool found = false;

  if (!file)
  {
    cout << "No users registered yet.\n";
    return false;
  }

  while (getline(file, u) && getline(file, e) && getline(file, p))
  {
    if (u == username && p == password)
    {
      found = true;
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

void forgotPassword()
{
  string targetUser, targetEmail, newPass;
  cout << "\n--- FORGOT PASSWORD ---\n";
  cout << "Enter your username: ";
  cin.ignore();
  getline(cin, targetUser);
  cout << "Enter your email: ";
  getline(cin, targetEmail);

  string storedUsers[100];
  string storedEmails[100];
  string storedPass[100];
  int count = 0;

  ifstream inFile("users.txt");
  if (!inFile)
  {
    cout << "No users registered yet.\n";
    return;
  }

  string user, email, password;

  while (count < 100 && getline(inFile, user) && getline(inFile, email) && getline(inFile, password))
  {
    storedUsers[count] = user;
    storedEmails[count] = email;
    storedPass[count] = password;
    count++;
  }
  inFile.close();

  bool found = false;
  for (int i = 0; i < count; i++)
  {
    if (storedUsers[i] == targetUser && storedEmails[i] == targetEmail)
    {
      cout << "Identity Verified.\n";
      cout << "Enter new password: ";
      getline(cin, newPass);
      storedPass[i] = newPass;
      found = true;
      break;
    }
  }

  if (found)
  {
    ofstream outFile("users.txt");
    if (outFile.is_open())
    {
      for (int i = 0; i < count; i++)
      {
        outFile << storedUsers[i] << endl;
        outFile << storedEmails[i] << endl;
        outFile << storedPass[i] << endl;
      }
      outFile.close();
      cout << "Password updated successfully! Please Login.\n";
    }
    else
    {
      cout << "Error saving changes.\n";
    }
  }
  else
  {
    cout << "Invalid Username or Email. Cannot reset password.\n";
  }
}

string toLowerCase(string str)
{
    string lowerStr = str;
    for (char &c : lowerStr)
    {
        c = tolower(c);
    }
    return lowerStr;
}

void searchProducts(Product p[100], int count)
{
    string query;
    cout << "\nEnter product name to search: ";
    cin.ignore();
    getline(cin, query);

    string lowerQuery = toLowerCase(query);
    bool foundAny = false;

    cout << "\n---------------- SEARCH RESULTS ----------------\n";
    cout << left << setw(10) << "ID"
         << setw(15) << "Company"
         << setw(20) << "Model"
         << setw(12) << "Price" << endl;
    cout << "------------------------------------------------\n";

    for (int i = 0; i < count; i++)
    {
        string fullName = p[i].company + " " + p[i].model;
        string lowerName = toLowerCase(fullName);

        if (lowerName.find(lowerQuery) != string::npos)
        {
            cout << left << setw(10) << p[i].productID
                 << setw(15) << p[i].company
                 << setw(20) << p[i].model
                 << setw(12) << p[i].price << endl;
            foundAny = true;
        }
    }

    if (!foundAny)
    {
        cout << "No products found matching '" << query << "'.\n";
    }
    cout << "------------------------------------------------\n";
}

int loadProducts(Product p[100])
{
    ifstream file("products.txt");
    if (!file)
        return 0;
    int i = 0;
    while (file >> p[i].categoryID >> p[i].productID >> p[i].company >> p[i].model >> p[i].price >> p[i].stock)
    {
        i++;
    }
    return i;
}

void saveProducts(Product p[100], int count)
{
    ofstream file("products.txt");
    for (int i = 0; i < count; i++)
    {
        file << p[i].categoryID << " " << p[i].productID << " " << p[i].company << " " << p[i].model << " " << p[i].price << " " << p[i].stock << endl;
    }
}

void showCategories(Product p[100], int count)
{
    cout << "\n-------------- CATEGORIES --------------\n";
    cout << "1. Laptop\n2. Mouse\n3. Keyboard\n4. Charger\n";
    cout << "----------------------------------------\n";
    cout << "Choose category: ";
    int choice;
    cin >> choice;
    if (choice >= 1 && choice <= 4)
        showProductsInCategory(p, count, choice);
    else
        cout << "Invalid category choice.\n";
}

void showProductsInCategory(Product p[100], int count, int catID)
{
    cout << "\n================ Category " << catID << " ================\n";
    cout << left << setw(10) << "ID" << setw(15) << "Company" << setw(20) << "Model Name" << setw(12) << "Price" << setw(10) << "Stock" << endl;
    cout << "----------------------------------------------------------------\n";
    for (int i = 0; i < count; i++)
    {
        if (p[i].categoryID == catID)
        {
            cout << left << setw(10) << p[i].productID << setw(15) << p[i].company << setw(20) << p[i].model << setw(12) << fixed << setprecision(2) << p[i].price << setw(10) << p[i].stock << endl;
        }
    }
}

int addToCart(Product p[100], int pCount, Product cart[100], int cartCount)
{
    int id, qty;
    cout << "\nEnter Product ID: ";
    cin >> id;
    int index = -1;
    for (int i = 0; i < pCount; i++)
    {
        if (p[i].productID == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Product not found.\n";
        return cartCount;
    }

    cout << "Enter quantity: ";
    cin >> qty;
    if (qty <= 0 || qty > p[index].stock)
    {
        cout << "Invalid or Not Enough quantity (Available: " << p[index].stock << ").\n";
        return cartCount;
    }

    cart[cartCount] = p[index];
    cart[cartCount].stock = qty;

    cartCount++;
    p[index].stock -= qty;
    cout << "\n Added to cart successfully.\n";
    return cartCount;
}

int removeFromCart(Product p[100], int pCount, Product cart[100], int cartCount)
{
    if (cartCount == 0)
    {
        cout << "Cart empty.\n";
        return cartCount;
    }
    int id;
    cout << "Enter Product ID to remove: ";
    cin >> id;
    int index = -1;
    for (int i = 0; i < cartCount; i++)
    {
        if (cart[i].productID == id)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        cout << "Not in cart.\n";
        return cartCount;
    }

    for (int i = 0; i < pCount; i++)
    {
        if (p[i].productID == id)
        {
            p[i].stock += cart[index].stock;
            break;
        }
    }
    for (int i = index; i < cartCount - 1; i++)
    {
        cart[i] = cart[i + 1];
    }
    cartCount--;
    cout << "\n Removed from cart.\n";
    return cartCount;
}

void viewCart(Product cart[100], int cCount)
{
    if (cCount == 0)
    {
        cout << "\nCart is empty.\n";
        return;
    }
    double total = 0;
    cout << "\n============== YOUR CART ==============\n";
    cout << left << setw(10) << "ID" << setw(15) << "Company" << setw(20) << "Model Name" << setw(10) << "Qty" << setw(12) << "Subtotal" << endl;
    cout << "----------------------------------------------------------------\n";
    for (int i = 0; i < cCount; i++)
    {
        double sub = cart[i].price * cart[i].stock;
        total += sub;
        cout << left << setw(10) << cart[i].productID << setw(15) << cart[i].company << setw(20) << cart[i].model << setw(10) << cart[i].stock << setw(12) << fixed << setprecision(2) << sub << endl;
    }
    cout << "----------------------------------------------------------------\n";
    cout << "Total = " << fixed << setprecision(2) << total << endl;
}

bool checkout(Product p[100], int pCount, Product cart[100], int cCount)
{
    if (cCount == 0)
    {
        cout << "Cart empty.\n";
        return false;
    }
    viewCart(cart, cCount);
    char x;
    cout << "\nConfirm purchase (y/n): ";
    cin >> x;
    if (x != 'y' && x != 'Y')
    {
        cout << "\nCancelled. Restoring stock.\n";
        for (int i = 0; i < cCount; i++)
        {
            for (int j = 0; j < pCount; j++)
            {
                if (cart[i].productID == p[j].productID)
                {
                    p[j].stock += cart[i].stock;
                    break;
                }
            }
        }
        return false;
    }
    double total = 0;
    for (int i = 0; i < cCount; i++)
        total += cart[i].price * cart[i].stock;
    double discount = (total > 500000) ? total * 0.20 : 0;
    double finalAmount = total - discount;

    cout << fixed << setprecision(2);
    cout << "\n\n";
    cout << "\t*******************************************\n";
    cout << "\t* PAYMENT SUMMARY             *\n";
    cout << "\t*******************************************\n";
    cout << "\t" << left << setw(25) << "Total Amount:" << right << setw(12) << total << " PKR\n";
    cout << "\t" << left << setw(25) << "Discount:" << right << setw(12) << discount << " PKR\n";
    cout << "\t-------------------------------------------\n";
    cout << "\t" << left << setw(25) << "FINAL PAYABLE:" << right << setw(12) << finalAmount << " PKR\n";
    cout << "\t*******************************************\n";

    saveProducts(p, pCount);
    generateReceipt(cart, cCount, total, discount, finalAmount);
    cout << "\n\t>>> Purchase Successful! Receipt Generated. <<<\n";
    return true;
}

void generateReceipt(Product cart[100], int cCount, double total, double discount, double finalAmount)
{
    string name;
    string phone;
    cin.ignore();
    cout << "\nEnter your Name: ";
    getline(cin, name);
    do {
        cout << "Enter your Phone Number (11 digits): ";
        cin >> phone;
        if(phone.length() != 11) {
            cout << "Invalid number. Must be exactly 11 digits.\n";
        }
    } while(phone.length() != 11);

    ofstream receipt("receipt.txt");
    receipt << fixed << setprecision(2);
    receipt << "****************************************************************\n";
    receipt << "                      ELECTRO STORE RECEIPT                     \n";
    receipt << "****************************************************************\n";
    receipt << " Customer Name : " << name << "\n";
    receipt << " Phone Number  : " << phone << "\n";
    receipt << "****************************************************************\n";
    receipt << left << setw(10) << "ID" << setw(25) << "Product" << setw(10) << "Qty" << setw(15) << "Subtotal" << endl;
    receipt << "----------------------------------------------------------------\n";

    for (int i = 0; i < cCount; i++)
    {
        double sub = cart[i].price * cart[i].stock;
        string fullName = cart[i].company + " " + cart[i].model;
        receipt << left << setw(10) << cart[i].productID << setw(25) << fullName << setw(10) << cart[i].stock << setw(15) << sub << endl;
    }

    receipt << "----------------------------------------------------------------\n";
    receipt << right << setw(45) << "Total Amount : " << setw(10) << total << " PKR\n";
    receipt << right << setw(45) << "Discount     : " << setw(10) << discount << " PKR\n";
    receipt << "----------------------------------------------------------------\n";
    receipt << right << setw(45) << "FINAL PAYABLE: " << setw(10) << finalAmount << " PKR\n";
    receipt << "****************************************************************\n";
    receipt << "                    THANK YOU FOR SHOPPING!                     \n";
    receipt << "****************************************************************\n";

    receipt.close();
}


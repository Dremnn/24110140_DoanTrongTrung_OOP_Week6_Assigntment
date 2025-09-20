#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>

using namespace std;

// Abstract class 
class Discountable {
public:
    virtual double applyDiscount(double discountRate) = 0;
    virtual ~Discountable() = default;
};

// 
template<typename T>
class InventoryList {
private:
    vector<T> items;

public:
    // Add item to inventory
    void addItem(const T& item) {
        items.push_back(item);
        cout << "Item added to inventory list.\n";
    }

    // Remove item from inventory
    bool removeItem(const T& item) {
        auto it = find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
            cout << "Item removed from inventory list.\n";
            return true;
        }
        cout << "Item not found in inventory list.\n";
        return false;
    }

    // Search for item in inventory
    bool searchItem(const T& item) const {
        return find(items.begin(), items.end(), item) != items.end();
    }
    
    // Search by ID for products 
    T searchById(int id) {
        for (const auto& item : items) {
            if (item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    // Get all items
    vector<T> getAllItems() const {
        return items;
    }

    // Get size of inventory
    size_t size() const {
        return items.size();
    }

    // Check if inventory is empty
    bool empty() const {
        return items.empty();
    }

    // Display all items 
    void displayAll() const {
        cout << "Inventory List contains " << items.size() << " items:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << "Item " << (i + 1) << ": " << items[i] << "\n";
        }
    }
};

// Base Product class
class Product : public Discountable {
protected:
    int id;
    string name;
    double price;
    int stock;

public:
    Product(int id = 0, const string& name = "", double price = 0, int stock = 0) 
        : id(id), name(name), price(price), stock(stock) {}

    // Virtual destructor for proper inheritance cleanup
    virtual ~Product() = default;

    // Getter methods
    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Setter methods
    void setPrice(double newPrice) { 
        if (newPrice >= 0) {
            price = newPrice; 
            cout << "Price updated to $" << newPrice << "\n";
        } else {
            cout << "Error: Price cannot be negative.\n";
        }
    }
    
    void setStock(int newStock) { 
        if (newStock >= 0) {
            stock = newStock; 
            cout << "Stock updated to " << newStock << "\n";
        } else {
            cout << "Error: Stock cannot be negative.\n";
        }
    }

    // Virtual method for stock updates (can be overridden by derived classes)
    virtual void updateStock(int quantity) {
        if (stock + quantity >= 0) {
            stock += quantity;
            cout << "Stock updated: " << (quantity > 0 ? "+" : "") << quantity 
                 << " (New stock: " << stock << ")\n";
        } else {
            cout << "Error: Cannot reduce stock below 0. Current stock: " << stock << "\n";
        }
    }

    // Virtual method to display product information
    virtual void displayInfo() const {
        cout << "Product ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Price: $" << fixed << setprecision(2) << price << "\n";
        cout << "Stock: " << stock << " units\n";
    }

    // Implement Discountable interface
    virtual double applyDiscount(double discountRate) override {
        if (discountRate >= 0.0 && discountRate <= 1.0) {
            double discountedPrice = price * (1.0 - discountRate);
            cout << "Product Discount Applied: " << (discountRate * 100) << "%\n";
            cout << "Original price: $" << fixed << setprecision(2) << price 
                 << " -> Discounted price: $" << discountedPrice << "\n";
            return discountedPrice;
        } else {
            cout << "Error: Invalid discount rate. Must be between 0.0 and 1.0\n";
            return price;
        }
    }

    // ===== OPERATOR OVERLOADING =====
    
    // Equality operator for product comparison (by ID)
    bool operator==(const Product& other) const {
        return this->id == other.id;
    }

    // Inequality operator
    bool operator!=(const Product& other) const {
        return !(*this == other);
    }

    // Less than operator for sorting (by price)
    bool operator<(const Product& other) const {
        return this->price < other.price;
    }

    // Greater than operator for sorting (by price)
    bool operator>(const Product& other) const {
        return this->price > other.price;
    }

    // Assignment operator
    Product& operator=(const Product& other) {
        if (this != &other) {
            id = other.id;
            name = other.name;
            price = other.price;
            stock = other.stock;
            cout << "Product assigned successfully.\n";
        }
        return *this;
    }

    // Stream insertion operator for easy output
    friend ostream& operator<<(ostream& os, const Product& product) {
        os << "Product[ID:" << product.id << ", Name:'" << product.name 
           << "', Price:$" << fixed << setprecision(2) << product.price 
           << ", Stock:" << product.stock << "]";
        return os;
    }
};

// Electronics class
class Electronics : public Product {
private:
    int warrantyPeriod; // in months
    string brand;

public:
    Electronics(int id = 0, const string& name = "", double price = 0.0, int stock = 0, 
                int warranty = 0, const string& brand = "")
        : Product(id, name, price, stock), warrantyPeriod(warranty), brand(brand) {}

    // Getter methods 
    int getWarrantyPeriod() const { return warrantyPeriod; }
    string getBrand() const { return brand; }

    // Override updateStock 
    void updateStock(int quantity) override {
        if (stock + quantity >= 0) {
            stock += quantity;
            cout << "Electronics stock updated: " << (quantity > 0 ? "+" : "") << quantity 
                 << " (New stock: " << stock << ")\n";
            // Electronics-specific handling fee for stock reduction
            if (quantity < 0) {
                cout << "*** Electronics handling fee of $5 applied for stock reduction ***\n";
            }
        } else {
            cout << "Error: Cannot reduce electronics stock below 0. Current stock: " << stock << "\n";
        }
    }

    // Override displayInfo 
    void displayInfo() const override {
        cout << "========== ELECTRONICS PRODUCT ==========\n";
        Product::displayInfo(); // Call base class method
        cout << "Brand: " << brand << "\n";
        cout << "Warranty: " << warrantyPeriod << " months\n";
        cout << "========================================\n";
    }

    // Override applyDiscount 
    double applyDiscount(double discountRate) override {
        if (discountRate >= 0.0 && discountRate <= 1.0) {
            // Electronics get an additional 5% discount (bonus feature)
            double enhancedRate = min(discountRate + 0.05, 1.0);
            double discountedPrice = price * (1.0 - enhancedRate);
            cout << "*** ELECTRONICS SPECIAL DISCOUNT ***\n";
            cout << "Base discount: " << (discountRate * 100) << "% + Electronics bonus: 5%\n";
            cout << "Total discount applied: " << (enhancedRate * 100) << "%\n";
            cout << "Original price: $" << fixed << setprecision(2) << price 
                 << " -> Final price: $" << discountedPrice << "\n";
            return discountedPrice;
        } else {
            cout << "Error: Invalid discount rate. Must be between 0.0 and 1.0\n";
            return price;
        }
    }
};

// 
class CartItem {
private:
    shared_ptr<Product> product;
    int quantity;

public:
    CartItem(shared_ptr<Product> prod, int qty) : product(prod), quantity(qty) {}

    // Getter methods
    shared_ptr<Product> getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    
    // Setter method
    void setQuantity(int qty) { 
        if (qty > 0) {
            quantity = qty; 
        } else {
            cout << "Error: Quantity must be positive.\n";
        }
    }

    // Calculate total price for this cart item
    double getTotalPrice() const {
        return product->getPrice() * quantity;
    }

    // Display cart item information
    void displayItem() const {
        cout << "- " << product->getName();
        // If product is Electronics, display brand
        shared_ptr<Electronics> electronicsProduct = dynamic_pointer_cast<Electronics>(product);

        if (electronicsProduct) {
            cout << " (" << electronicsProduct->getBrand() << ") ";
        }
        
        cout << "(Qty: " << quantity << ") - Unit: $" << fixed << setprecision(2) 
            << product->getPrice() << " | Total: $" << getTotalPrice() << "\n";
    }

    // Equality operator for cart item comparison
    bool operator==(const CartItem& other) const {
        return *(this->product) == *(other.product);
    }
};

// ShoppingCart class 
class ShoppingCart : public Discountable {
private:
    InventoryList<CartItem> cartItems;
    double totalAmount;

    // Private method to recalculate total amount
    void calculateTotal() {
        totalAmount = 0.0;
        for (const auto& item : cartItems.getAllItems()) {
            totalAmount += item.getTotalPrice();
        }
    }

public:
    // Constructor
    ShoppingCart() : totalAmount(0.0) {}

    // OPERATOR OVERLOADING
    
    // += operator to add product 
    ShoppingCart& operator+=(const pair<shared_ptr<Product>, int>& productQuantityPair) {
        shared_ptr<Product> product = productQuantityPair.first;
        int quantity = productQuantityPair.second;

        // Validation checks
        if (!product) {
            cout << "Error: Cannot add null product to cart.\n";
            return *this;
        }

        if (quantity <= 0) {
            cout << "Error: Quantity must be positive. Received: " << quantity << "\n";
            return *this;
        }

        if (product->getStock() < quantity) {
            cout << "Error: Insufficient stock for " << product->getName() 
                 << ". Available: " << product->getStock() << ", Requested: " << quantity << "\n";
            return *this;
        }

        // Check if product already exists in cart
        bool found = false;
        vector<CartItem> items = cartItems.getAllItems();
        
        cartItems = InventoryList<CartItem>(); // Clear current items
        
        for (auto& item : items) {
            if (*(item.getProduct()) == *product) {
                // Update existing item quantity
                item.setQuantity(item.getQuantity() + quantity);
                found = true;
            }
            cartItems.addItem(item); // Re-add item
        }

        if (!found) {
            // Add new item to cart
            CartItem newItem(product, quantity);
            cartItems.addItem(newItem);
        }

        // Update product stock and recalculate total
        product->updateStock(-quantity);
        calculateTotal();
        
        cout << " Successfully added " << quantity << " x " << product->getName() 
             << " to cart (Total: $" << fixed << setprecision(2) << totalAmount << ")\n";
        return *this;
    }

    // -= operator to remove product from cart
    ShoppingCart& operator-=(shared_ptr<Product> product) {
        if (!product) {
            cout << "Error: Cannot remove null product from cart.\n";
            return *this;
        }

        vector<CartItem> items = cartItems.getAllItems();
        cartItems = InventoryList<CartItem>(); // Clear current items
        
        bool found = false;
        for (const auto& item : items) {
            if (*(item.getProduct()) == *product) {
                // Return stock to product and skip re-adding this item
                product->updateStock(item.getQuantity());
                found = true;
                cout << " Removed " << product->getName() << " from cart.\n";
            } else {
                cartItems.addItem(item); // Re-add other items
            }
        }

        if (!found) {
            cout << "Error: Product " << product->getName() << " not found in cart.\n";
        }

        calculateTotal();
        return *this;
    }

    // Display cart contents
    void displayCart() const {
        cout << "\n============= SHOPPING CART =============\n";
        if (cartItems.empty()) {
            cout << "Cart is empty.\n";
            cout << "========================================\n";
            return;
        }

        cout << "Items in your cart:\n";
        for (const auto& item : cartItems.getAllItems()) {
            item.displayItem();
        }
        cout << "----------------------------------------\n";
        cout << "Cart Total: $" << fixed << setprecision(2) << totalAmount << "\n";
        cout << "Total Items: " << getItemCount() << " different products\n";
        cout << "========================================\n";
    }

    // Implement Discountable interface for cart-wide discounts
    double applyDiscount(double discountRate) override {
        if (discountRate >= 0.0 && discountRate <= 1.0) {
            double discountAmount = totalAmount * discountRate;
            double discountedTotal = totalAmount - discountAmount;
            cout << "CART DISCOUNT APPLIED\n";
            cout << "Discount Rate: " << (discountRate * 100) << "%\n";
            cout << "Discount Amount: $" << fixed << setprecision(2) << discountAmount << "\n";
            cout << "Original Total: $" << totalAmount << " -> New Total: $" << discountedTotal << "\n";
            return discountedTotal;
        } else {
            cout << "Error: Invalid discount rate. Must be between 0.0 and 1.0\n";
            return totalAmount;
        }
    }

    // Remove specific product from cart (non-operator method)
    bool removeProduct(shared_ptr<Product> product) {
        *this -= product; // Use -= operator
        return true;
    }

    // Getter methods
    double getTotalAmount() const { return totalAmount; }
    size_t getItemCount() const { return cartItems.size(); }
    bool isEmpty() const { return cartItems.empty(); }
    vector<CartItem> getCartItems() const { return cartItems.getAllItems(); }

    // Clear entire cart
    void clearCart() {
        cout << "Clearing shopping cart...\n";
        // Return all items to stock
        for (const auto& item : cartItems.getAllItems()) {
            item.getProduct()->updateStock(item.getQuantity());
        }
        cartItems = InventoryList<CartItem>();
        totalAmount = 0.0;
        cout << "Cart cleared successfully.\n";
    }
};

// Order class 
class Order {
private:
    static int nextOrderId;
    int orderId;
    vector<CartItem> orderItems;
    double totalAmount;
    string status;
    string orderDate;

public:
    Order(const ShoppingCart& cart) {
        orderId = nextOrderId++;
        orderItems = cart.getCartItems();
        totalAmount = cart.getTotalAmount();
        status = "Confirmed";
        orderDate = "2024-01-15"; // Simplified date
    }

    void displayOrder() const {
        cout << "\n========== ORDER CONFIRMATION ==========\n";
        cout << "Order ID: #" << orderId << "\n";
        cout << "Date: " << orderDate << "\n";
        cout << "Status: " << status << "\n";
        cout << "----------------------------------------\n";
        cout << "Ordered Items:\n";
        for (const auto& item : orderItems) {
            item.displayItem();
        }
        cout << "----------------------------------------\n";
        cout << "Total Amount: $" << fixed << setprecision(2) << totalAmount << "\n";
        cout << "Thank you for your purchase!\n";
        cout << "========================================\n";
    }

    // Getter methods
    int getOrderId() const { return orderId; }
    double getTotalAmount() const { return totalAmount; }
    string getStatus() const { return status; }
};

// Initialize static member
int Order::nextOrderId = 1;

// Main system manager class 
class ECommerceManager {
private:
    InventoryList<shared_ptr<Product>> inventory;
    ShoppingCart cart;
    vector<Order> orderHistory;

public:
    // Add product to main inventory
    void addProductToInventory(shared_ptr<Product> product) {
        if (product) {
            inventory.addItem(product);
            cout << "Added '" << product->getName() << "' to main inventory.\n";
        } else {
            cout << "Error: Cannot add null product to inventory.\n";
        }
    }

    // Display complete inventory
    void displayInventory() const {
        cout << "\n========== CURRENT INVENTORY ==========\n";
        if (inventory.empty()) {
            cout << "Inventory is empty.\n";
            cout << "==========================================\n";
            return;
        }
        
        int index = 1;
        for (const auto& product : inventory.getAllItems()) {
            cout << "Product #" << index++ << ":\n";
            product->displayInfo();
            cout << "----------------------------------------\n";
        }
        cout << "Total Products: " << inventory.size() << "\n";
        cout << "==========================================\n";
    }

    // Add product to cart by ID
    void addToCart(int productId, int quantity) {
        cout << "\nAdding product ID " << productId << " (Qty: " << quantity << ") to cart...\n";
        shared_ptr<Product> product = inventory.searchById(productId);
        if (product) {
            cart += make_pair(product, quantity); // Uses += operator
        } else {
            cout << "Error: Product with ID " << productId << " not found in inventory.\n";
        }
    }

    // Remove product from cart by ID
    void removeFromCart(int productId) {
        cout << "\nRemoving product ID " << productId << " from cart...\n";
        shared_ptr<Product> product = inventory.searchById(productId);
        if (product) {
            cart -= product; // Uses -= operator
        } else {
            cout << "Error: Product with ID " << productId << " not found in inventory.\n";
        }
    }

    // Display current cart
    void displayCart() const {
        cart.displayCart();
    }

    // Apply discount to entire cart
    void applyCartDiscount(double rate) {
        cout << "\nApplying discount to cart...\n";
        cart.applyDiscount(rate);
    }
    
    // Process checkout and create order
    void checkout() {
        cout << "\nProcessing checkout...\n";
        if (cart.isEmpty()) {
            cout << "Error: Cannot checkout. Shopping cart is empty.\n";
            return;
        }
        Order newOrder(cart);
        orderHistory.push_back(newOrder);
        newOrder.displayOrder();
        cart.clearCart();
        cout << "Order processed successfully!\n";
    }

    // Display order history
    void displayOrderHistory() const {
        cout << "\n========== ORDER HISTORY ==========\n";
        if (orderHistory.empty()) {
            cout << "No orders found.\n";
            cout << "=====================================\n";
            return;
        }
        
        for (const auto& order : orderHistory) {
            cout << "Order #" << order.getOrderId() << " - Total: $" 
                 << fixed << setprecision(2) << order.getTotalAmount() 
                 << " - Status: " << order.getStatus() << "\n";
        }
        cout << "Total Orders: " << orderHistory.size() << "\n";
        cout << "=====================================\n";
    }

    // Get inventory size
    size_t getInventorySize() const { return inventory.size(); }
    
    // Get cart item count
    size_t getCartItemCount() const { return cart.getItemCount(); }

    // Get reference to cart
    const ShoppingCart& getCart() const {
        return cart;
    }
};

int main() {
    cout << "=========== E-COMMERCE PRODUCT MANAGEMENT SYSTEM ===========\n";
    cout << "Demonstrating ALL Object-Oriented Programming Concepts\n";
    cout << "============================================================\n\n";

    // Create manager and products
    ECommerceManager manager;
    auto laptop = make_shared<Electronics>(101, "Gaming Laptop", 1299.99, 10, 24, "ASUS");
    auto phone = make_shared<Electronics>(102, "Smartphone", 799.99, 15, 12, "Samsung");
    auto book = make_shared<Product>(201, "C++ Programming Book", 49.99, 20);

    // 1. TESTING CLASSES AND OBJECTS
    cout << "1. TESTING CLASSES AND OBJECTS\n";
    cout << "Creating products and adding to inventory...\n";
    manager.addProductToInventory(laptop);
    manager.addProductToInventory(phone);
    manager.addProductToInventory(book);
    manager.displayInventory();

    // 2. TESTING OPERATOR OVERLOADING
    cout << "\n2. TESTING OPERATOR OVERLOADING\n";
    
    // Test << operator (stream insertion)
    cout << "Stream insertion operator test:\n";
    cout << "Laptop: " << *laptop << "\n";
    cout << "Phone: " << *phone << "\n";
    cout << "Book: " << *book << "\n\n";
    
    // Test comparison operators
    cout << "Comparison operators test:\n";
    auto anotherLaptop = make_shared<Electronics>(101, "Gaming Laptop", 1299.99, 5, 24, "ASUS");
    cout << "laptop == anotherLaptop: " << (*laptop == *anotherLaptop ? "TRUE" : "FALSE") << "\n";
    cout << "laptop == phone: " << (*laptop == *phone ? "TRUE" : "FALSE") << "\n";
    cout << "laptop != phone: " << (*laptop != *phone ? "TRUE" : "FALSE") << "\n";
    cout << "laptop < phone (by price): " << (*laptop < *phone ? "TRUE" : "FALSE") << "\n";
    cout << "laptop > book (by price): " << (*laptop > *book ? "TRUE" : "FALSE") << "\n\n";

    // Test += and -= operators for cart
    cout << "Cart operators test (+=, -=):\n";
    manager.addToCart(101, 2); // Uses += internally
    manager.addToCart(201, 3);
    manager.addToCart(102, 1);
    manager.displayCart();

    // 3. TESTING TEMPLATE CLASS WITH DIFFERENT TYPES
    cout << "\n3. TESTING TEMPLATE CLASS WITH DIFFERENT TYPES\n";
    
    // Template with Product pointers (already demonstrated above)
    cout << "InventoryList<shared_ptr<Product>> already demonstrated in inventory management.\n";
    
    // Template with strings
    cout << "Testing InventoryList<string> for categories:\n";
    InventoryList<string> categories;
    categories.addItem("Electronics");
    categories.addItem("Books");
    categories.addItem("Clothing");
    categories.addItem("Sports");
    cout << "Categories inventory size: " << categories.size() << "\n";
    cout << "Search for 'Electronics': " << (categories.searchItem("Electronics") ? "FOUND" : "NOT FOUND") << "\n";
    cout << "Search for 'Toys': " << (categories.searchItem("Toys") ? "FOUND" : "NOT FOUND") << "\n\n";
    
    // Template with integers
    cout << "Testing InventoryList<int> for order IDs:\n";
    InventoryList<int> orderIds;
    orderIds.addItem(1001);
    orderIds.addItem(1002);
    orderIds.addItem(1003);
    cout << "Order IDs inventory size: " << orderIds.size() << "\n";
    cout << "Search for order 1002: " << (orderIds.searchItem(1002) ? "FOUND" : "NOT FOUND") << "\n\n";

    // 4. TESTING INHERITANCE AND METHOD OVERRIDING
    cout << "\n4. TESTING INHERITANCE AND METHOD OVERRIDING\n";
    cout << "Testing virtual method overriding:\n";
    
    cout << "Electronics updateStock() override:\n";
    laptop->updateStock(-1); // Will show Electronics-specific message
    
    cout << "\nProduct base updateStock() method:\n";
    book->updateStock(-2); // Will show Product base message
    
    cout << "\nElectronics displayInfo() override:\n";
    laptop->displayInfo(); // Will show Electronics-specific format
    
    cout << "\nProduct displayInfo() base method:\n";
    book->displayInfo(); // Will show Product base format

    // 5. TESTING INTERFACE (ABSTRACT CLASS) - POLYMORPHISM
    cout << "\n5. TESTING INTERFACE (DISCOUNTABLE) - POLYMORPHISM\n";
    cout << "Demonstrating polymorphic behavior with Discountable interface:\n";
    
    // Create vector of Discountable pointers
    vector<shared_ptr<Discountable>> discountableItems;
    discountableItems.push_back(laptop);    // Electronics object
    discountableItems.push_back(phone);     // Electronics object  
    discountableItems.push_back(book);      // Product object
    discountableItems.push_back(make_shared<ShoppingCart>(manager.getCart())); // ShoppingCart object
    
    cout << "\nApplying 15% discount to all discountable items polymorphically:\n";
    for (size_t i = 0; i < discountableItems.size(); ++i) {
        cout << "\n--- Item " << (i+1) << " ---\n";
        discountableItems[i]->applyDiscount(0.15); // Polymorphic call
    }

    // 6. TESTING ERROR HANDLING
    cout << "\n6. TESTING ERROR HANDLING\n";
    cout << "Testing various error conditions:\n";
    
    // Test insufficient stock
    manager.addToCart(101, 50); // Should fail
    
    // Test invalid product ID
    manager.addToCart(999, 1); // Should fail
    
    // Test invalid discount rates
    cout << "\nTesting invalid discount rates:\n";
    laptop->applyDiscount(-0.1); // Negative rate
    phone->applyDiscount(1.5);   // Rate > 1.0
    
    // Test negative quantities and prices
    cout << "\nTesting validation in setters:\n";
    book->setPrice(-10.0);   // Should fail
    book->setStock(-5);      // Should fail
    
    // Test removing non-existent product from cart
    manager.removeFromCart(999); // Should fail

    // 7. ADVANCED OPERATIONS AND CHECKOUT
    cout << "\n7. ADVANCED OPERATIONS AND CHECKOUT\n";
    
    // Apply cart discount
    manager.applyCartDiscount(0.20);
    
    // Process checkout
    manager.checkout();
    
    // Display order history
    manager.displayOrderHistory();
    
    // Try to checkout empty cart (should fail)
    cout << "\nTrying to checkout empty cart:\n";
    manager.checkout(); // Should fail

    // 8. DEMONSTRATING ASSIGNMENT OPERATOR
    cout << "\n8. TESTING ASSIGNMENT OPERATOR\n";
    Product newBook;
    newBook = *book; // Uses assignment operator
    cout << "Assigned product: " << newBook << "\n";

    return 0;
}
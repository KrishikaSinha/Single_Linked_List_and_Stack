#include <iostream>
#include <stack>
#include <string>
using namespace std;

// -------------------------------
// Book Node (Linked List Node)
// -------------------------------
struct Book {
    int bookID;
    string title;
    string author;
    string status; // "Available" or "Issued"
    Book* next;

    Book(int id, string t, string a, string s = "Available") {
        bookID = id;
        title = t;
        author = a;
        status = s;
        next = nullptr;
    }
};

// -------------------------------
// Linked List for Book Management
// -------------------------------
class BookList {
private:
    Book* head;

public:
    BookList() : head(nullptr) {}

    // Insert new book
    void insertBook(int id, string title, string author) {
        Book* newBook = new Book(id, title, author);
        newBook->next = head;
        head = newBook;
        cout << "✅ Book inserted successfully.\n";
    }

    // Delete book by ID
    void deleteBook(int id) {
        if (!head) {
            cout << "Library is empty.\n";
            return;
        }
        Book *temp = head, *prev = nullptr;
        while (temp && temp->bookID != id) {
            prev = temp;
            temp = temp->next;
        }
        if (!temp) {
            cout << "Book not found.\n";
            return;
        }
        if (!prev)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;
        cout << "🗑️ Book deleted successfully.\n";
    }

    // Search for a book
    Book* searchBook(int id) {
        Book* temp = head;
        while (temp) {
            if (temp->bookID == id)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Display all books
    void displayBooks() {
        if (!head) {
            cout << "No books in the library.\n";
            return;
        }
        Book* temp = head;
        cout << "\n📚 Current Library Books:\n";
        while (temp) {
            cout << "-----------------------------------\n";
            cout << "Book ID: " << temp->bookID << "\n";
            cout << "Title: " << temp->title << "\n";
            cout << "Author: " << temp->author << "\n";
            cout << "Status: " << temp->status << "\n";
            temp = temp->next;
        }
        cout << "-----------------------------------\n";
    }
};

// -------------------------------
// Transaction Record for Stack
// -------------------------------
struct Transaction {
    string action; // "issue" or "return"
    int bookID;
};

// -------------------------------
// Stack for Undo Functionality
// -------------------------------
class TransactionStack {
private:
    stack<Transaction> transStack;

public:
    void pushTransaction(string action, int id) {
        transStack.push({action, id});
    }

    bool isEmpty() {
        return transStack.empty();
    }

    Transaction popTransaction() {
        Transaction t = transStack.top();
        transStack.pop();
        return t;
    }

    void viewTransactions() {
        if (transStack.empty()) {
            cout << "No transactions yet.\n";
            return;
        }
        stack<Transaction> temp = transStack;
        cout << "\n🧾 Transaction History:\n";
        while (!temp.empty()) {
            Transaction t = temp.top();
            temp.pop();
            cout << t.action << " - Book ID: " << t.bookID << "\n";
        }
    }
};

// -------------------------------
// Library Management System
// -------------------------------
class LibrarySystem {
private:
    BookList bookList;
    TransactionStack transStack;

public:
    void issueBook(int id) {
        Book* b = bookList.searchBook(id);
        if (!b) {
            cout << "Book not found.\n";
            return;
        }
        if (b->status == "Issued") {
            cout << "Book already issued.\n";
            return;
        }
        b->status = "Issued";
        transStack.pushTransaction("issue", id);
        cout << "📘 Book issued successfully.\n";
    }

    void returnBook(int id) {
        Book* b = bookList.searchBook(id);
        if (!b) {
            cout << "Book not found.\n";
            return;
        }
        if (b->status == "Available") {
            cout << "Book already available.\n";
            return;
        }
        b->status = "Available";
        transStack.pushTransaction("return", id);
        cout << "📗 Book returned successfully.\n";
    }

    void undoTransaction() {
        if (transStack.isEmpty()) {
            cout << "No transactions to undo.\n";
            return;
        }

        Transaction last = transStack.popTransaction();
        Book* b = bookList.searchBook(last.bookID);
        if (!b) {
            cout << "Book not found.\n";
            return;
        }

        if (last.action == "issue") {
            b->status = "Available";
            cout << "⏪ Undo successful: Issue reverted.\n";
        } else if (last.action == "return") {
            b->status = "Issued";
            cout << "⏪ Undo successful: Return reverted.\n";
        }
    }

    void menu() {
        int choice, id;
        string title, author;
        do {
            cout << "\n========== Library Menu ==========\n";
            cout << "1. Insert Book\n2. Delete Book\n3. Search Book\n4. Display Books\n5. Issue Book\n6. Return Book\n7. Undo Transaction\n8. View Transactions\n9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter Book ID, Title, Author: ";
                    cin >> id;
                    cin.ignore();
                    getline(cin, title);
                    getline(cin, author);
                    bookList.insertBook(id, title, author);
                    break;
                case 2:
                    cout << "Enter Book ID to delete: ";
                    cin >> id;
                    bookList.deleteBook(id);
                    break;
                case 3:
                    cout << "Enter Book ID to search: ";
                    cin >> id;
                    if (Book* b = bookList.searchBook(id))
                        cout << "Found -> " << b->title << " by " << b->author << " (" << b->status << ")\n";
                    else
                        cout << "Book not found.\n";
                    break;
                case 4:
                    bookList.displayBooks();
                    break;
                case 5:
                    cout << "Enter Book ID to issue: ";
                    cin >> id;
                    issueBook(id);
                    break;
                case 6:
                    cout << "Enter Book ID to return: ";
                    cin >> id;
                    returnBook(id);
                    break;
                case 7:
                    undoTransaction();
                    break;
                case 8:
                    transStack.viewTransactions();
                    break;
                case 9:
                    cout << "Exiting program...\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 9);
    }
};

// -------------------------------
// Main Function
// -------------------------------
int main() {
    LibrarySystem system;
    system.menu();
    return 0;
}

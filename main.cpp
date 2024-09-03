#include <sstream>
#include <iostream>
//#include <time.h>
#include <vector>
#include <string>
#include <exception>
//#include <assert.h>

// description of the type of data types
enum type_data {
    TYPE_INT,     // the whole type
    TYPE_DOUBLE,  // real type
    TYPE_STRING,  // string type
    TYPE_SIZE  // contains the number of elements in the union
};

// key and value data
struct Data {
    // data type
    type_data type;

    // the data itself
    int dataInt;
    double dataDouble;
    std::string dataString;
};

// base class
class Field
{
public:
    // constructor
    virtual ~Field() {}
    
    // methods

    // getting the key and value in the string representation
    virtual std::string toString() const = 0;
    // comparing data with a key
    virtual bool equal_to_the_key(const Data & data) const = 0;
};

// template class
template <typename T_key, typename T_value>
class Registry: public Field
{
private:
    T_key key;  // ключ
    T_value value;  // значение

public:
    // constructors
    Registry() {}
    Registry(T_key inKey, T_value inValue): key(inKey), value(inValue) {}

    // destructor
    ~Registry() {}

    // methods
    // getting the key and value in the string representation
    [[nodiscard]] std::string toString() const
    {
        std::stringstream ss;
        ss << key << ": " << value;
        return ss.str();
    }
    // comparing data with a key
    [[nodiscard]] bool equal_to_the_key(const Data & data) const
    {
        std::stringstream ss;
        ss << key;
        return ss.str() == data.dataString;
    }
};

// data registry
std::vector<Field *> dictionary;

/////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 @brief A data entry routine with a data type definition.
 @param [in]inInvitationStr an invitation message to enter.
 @param [out]outData the data structure for the entered data.
*/
void dictionary_data_entry(const std::string & inInvitationStr, struct Data & outData)
{
    // entering the data
    std::cout << inInvitationStr;
    std::cin >> outData.dataString;

    // analyzing the data type
    try {
        // if it is possible to convert the data here
        outData.dataInt = std::stoi(outData.dataString);
        outData.dataDouble = std::stof(outData.dataString);
        
        // then determine its type
        outData.type = (outData.dataDouble > outData.dataInt) ? TYPE_DOUBLE : TYPE_INT;
    }
    catch(...) {
        // otherwise, leave it as a string type
        outData.type = TYPE_STRING;
    }
}

/*!
 @brief A routine for adding data to the registry.
*/
void dictionary_add()
{
    struct Data key, value;

    // Data entry
    dictionary_data_entry("Enter the key: ", key);
    dictionary_data_entry("Enter the value: ", value);

    // adding data depending on the types of data entered
    switch((int)(key.type * TYPE_SIZE + value.type)) {
        case 0: dictionary.push_back(new Registry<int, int>(key.dataInt, value.dataInt)); break;
        case 1: dictionary.push_back(new Registry<int, double>(key.dataInt, value.dataDouble)); break;
        case 2: dictionary.push_back(new Registry<int, std::string>(key.dataInt, value.dataString)); break;
        case 3: dictionary.push_back(new Registry<double, int>(key.dataDouble, value.dataInt)); break;
        case 4: dictionary.push_back(new Registry<double, double>(key.dataDouble, value.dataDouble)); break;
        case 5: dictionary.push_back(new Registry<double, std::string>(key.dataDouble, value.dataString)); break;
        case 6: dictionary.push_back(new Registry<std::string, int>(key.dataString, value.dataInt)); break;
        case 7: dictionary.push_back(new Registry<std::string, double>(key.dataString, value.dataDouble)); break;
        case 8: dictionary.push_back(new Registry<std::string, std::string>(key.dataString, value.dataString)); break;
        default: {
            throw std::runtime_error("An error occurred during the execution of the program.");
        }
    }

    // displaying a message about the successful addition of data
    std::cout << "The key element has been added.\n";
}

/*!
 @brief The subroutine removes elements by key.
*/
void dictionary_remove()
{
    struct Data key;
    int count = 0;

    // entering the key
    dictionary_data_entry("Enter the key: ", key);

    // deleting items with the 'key' key
    for(std::vector<Field *>::iterator it = dictionary.begin(); it != dictionary.end(); it++) {
        if((*it)->equal_to_the_key(key)) {
            delete *it;
            dictionary.erase(it);
            it--;
            count++;
        }
    }

    // displaying a message about successful deletion
    std::cout << "Successfully deleted " << count << " items.\n";
}

/*!
 @brief A routine for displaying a list of elements in a convenient way.
 @param inList a list of items to output.
 @param inCaption the title of the list.
*/
void dictionary_print(std::vector<Field *> & inList, std::string inCaption)
{
    std::cout << "\n--- " << inCaption << " list: ---\n";
    if(inList.empty())
        std::cout << "No elements.\n";
    else
        for(std::vector<Field *>::iterator it = inList.begin(); it != inList.end(); it++) {
            std::cout << (*it)->toString() << std::endl;
        }
    std::cout << "--- End of " << inCaption << " list. ---\n";
}

/*!
 @brief A routine for searching for items by key and adding it to the resulting list.
 @param [in]inKey the search key.
 @param [in]inList the list in which we are looking for items.
 @param [out]outList the list to which we add the found items.
*/
void dictionary_find(const struct Data & inKey, std::vector<Field *> & inList, std::vector<Field *> & outList)
{
    outList.clear();
    for(std::vector<Field *>::iterator it = inList.begin(); it != inList.end(); it++) {
        if((*it)->equal_to_the_key(inKey)) {
            outList.push_back(*it);
        }
    }
}

/*!
 @brief A routine for deleting data from the 'dictionary' list.
*/
void dictionary_destroy()
{
    for(std::vector<Field *>::iterator it = dictionary.begin(); it != dictionary.end(); it++) {
        delete *it;
    }
    dictionary.clear();
}

int main()
{
    while(true) {
        std::string command, article;
        int count;

        // entering a command
        std::cout << "\nEnter the command (add, remove, print, find, end): ";
        std::cin >> command;

        // team analysis
        if(command == "add") {
            dictionary_add();
        }
        else if(command == "remove") {
            dictionary_remove();
        }
        else if(command == "print") {
            dictionary_print(dictionary, "Dictionary");
        }
        else if(command == "find") {
            struct Data key;
            std::vector<Field *> dictionary_result;

            // entering the key
            dictionary_data_entry("Enter the key: ", key);
            // creating a search list
            dictionary_find(key, dictionary, dictionary_result);
            // output of the search result
            dictionary_print(dictionary_result, "Find");
        }
        else if(command == "end") {
            dictionary_destroy();
            break;
        }
        else
        {
            std::cout << "The command is not defined.\n";
        }
    }

    // output of the program shutdown message
    std::cout
        << "----------------------------------------------------------\n"
        << "End of program.\n";
    return 0;
}

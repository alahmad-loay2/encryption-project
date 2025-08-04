#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream> // for files
#include <chrono> // for time

using namespace std;
using namespace chrono;

// (ceaser cipher) encrypt by adding 2 to ascii value only for characters
string encrypt(string line) {
    string encrypted = "";
    for (int i = 0; i < line.length(); i++){
        char c = line[i];
        if (isalpha(c)) { 
            char base = islower(c) ? 'a' : 'A'; 
            c = (c - base + 2) % 26 + base; 
        }
        encrypted += c; 
    }
    return encrypted;
}

// Caesar cipher decryption
string decrypt(string line) {
    string decrypted = "";
    for (int i = 0; i < line.length(); i++) {
        char c = line[i];
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base - 2 + 26) % 26 + base;  
        }
        decrypted += c;
    }
    return decrypted;
}

// function to encrypt a file and copy it to output file
void encryptFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);  
    ofstream outputFile(outputFilePath); 
  
    if (!inputFile) {
        cout << "Error opening input file" << endl;
        return; 
    }

    if (!outputFile) {
        cout << "Error opening output file" << endl;
        return; 
    }

    string line;
    while (getline(inputFile, line)) {
        string encryptedLine = encrypt(line);
        outputFile << encryptedLine << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "File has been encrypted successfully" << endl;
}

// function to decrypt a file and copy it to decrypted file
void decryptFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);  
    ofstream decryptedFile(outputFilePath); 
  
    if (!inputFile) {
        cout << "Error opening encrypted file" << endl;
        return; 
    }

    if (!decryptedFile) {
        cout << "Error opening decrypted file" << endl;
        return; 
    }

    string line;
    while (getline(inputFile, line)) {
        string decryptedLine = decrypt(line);
        decryptedFile << decryptedLine << endl;
    }

    inputFile.close();
    decryptedFile.close();

    cout << "File has been decrypted successfully" << endl;
}

int main() {

    // ofstream inputFile("../input-output/input.txt");
    // for (int i = 0; i < 100000; i++) { // Repeating the text 100000 times
    //     inputFile << "This is a sample text meant to test encryption. \n " 
    //             << "The words are randomly chosen, but they will serve their purpose in our test. \n "
    //             << "Encryption works on the characters, shifting each letter as specified by the algorithm.\n\n";
    // }
    // inputFile.close();

    int choice;
    cout << "Do you want to encrypt or decrypt?" << endl;
    cout << "1 - encrypt" << endl;
    cout << "2 - decrypt" << endl;
    cin >> choice;

    // start timer
    auto start = high_resolution_clock::now();

    switch(choice){
        case 1:
            encryptFile("../input-output/input.txt", "../input-output/output.txt");
            break;
        case 2:
            decryptFile("../input-output/output.txt", "../input-output/decrypted.txt");
            break;
        default:
            cout << "invalid choice" << endl;
            break;
    }

    auto end = high_resolution_clock::now();

    duration<double> duration = end - start;
    cout << "operation took in normal way " << duration.count() << " seconds" << endl;

    return 0;
}

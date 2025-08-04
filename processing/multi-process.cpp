#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream> // for files
#include <chrono> // for time
#include <sys/wait.h>  // For wait()

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

void processText(const string& text, string& result, bool encryptMode) {
    result = "";
    for (int i = 0; i < text.length(); i++) {
        if (encryptMode) {
            result += encrypt(string(1, text[i]));
        } else {
            result += decrypt(string(1, text[i]));
        }
    }
}

// function to encrypt a file and copy it to output file
void encryptFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);  
    ofstream outputFile(outputFilePath); 
    string fileContent, firstHalf, secondHalf, encryptedFirstHalf, encryptedSecondHalf;
  
    if (!inputFile) {
        cout << "error opening input file" << endl;
        return; 
    }
    if (!outputFile) {
        cout << "error opening output file" << endl;
        return; 
    }
    string line;
    while (getline(inputFile, line)) {
         fileContent += line + "\n"; 
    }
    inputFile.close();
    int mid = fileContent.size() / 2;
    firstHalf = fileContent.substr(0, mid);
    secondHalf = fileContent.substr(mid);

    pid_t pid = fork();

     if (pid == -1) {
        cout << "fork failed" << endl;
        return;
    }
    if (pid == 0) {
        processText(secondHalf, encryptedSecondHalf, true);
        exit(0);
    } else {
        processText(firstHalf, encryptedFirstHalf, true);
        wait(NULL);
        string finalResult = encryptedFirstHalf + encryptedSecondHalf;
        outputFile << finalResult;
    }
    outputFile.close();

    cout << "file has been encrypted successfully" << endl;
}

// function to decrypt a file and copy it to decrypted file
void decryptFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);  
    ofstream decryptedFile(outputFilePath); 
    string fileContent, firstHalf, secondHalf, decryptedFirstHalf, decryptedSecondHalf;
  
    if (!inputFile) {
        cout << "error opening encrypted file" << endl;
        return; 
    }

    if (!decryptedFile) {
        cout << "error opening decrypted file" << endl;
        return; 
    }

    string line;
    while (getline(inputFile, line)) {
        fileContent += line + "\n"; 
    }

    inputFile.close();
    int mid = fileContent.size() / 2;
    firstHalf = fileContent.substr(0, mid);
    secondHalf = fileContent.substr(mid);

    pid_t pid = fork();

    if (pid == -1) {
        cout << "fork failed" << endl;
        return;
    }
    if (pid == 0) {  
        processText(secondHalf, decryptedSecondHalf, false);
        exit(0);
    } else {  
        processText(firstHalf, decryptedFirstHalf, false);
        wait(NULL);  
        string finalResult = decryptedFirstHalf + decryptedSecondHalf;
        decryptedFile << finalResult;
    }
    decryptedFile.close();

    cout << "file has been decrypted successfully" << endl;
}

int main() {
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
    cout << "operation took " << duration.count() << " seconds" << endl;

    return 0;
}

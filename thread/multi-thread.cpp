#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream> // for files
#include <thread>   // For threads
#include <semaphore.h>  // For semaphores
#include <chrono> // for time

using namespace std;
using namespace chrono;

// Declare semaphores
sem_t first, second;

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

void encryptAndWrite(const string& inputPart, string& result) {
    string encryptedPart = encrypt(inputPart);

    sem_wait(&first);  
    result += encryptedPart;  

    sem_post(&second);
}

void encryptAndWriteSecond(const string& inputPart, string& result) {
    string encryptedPart = encrypt(inputPart);
    sem_wait(&second);
    result += encryptedPart;
    sem_post(&first);
}

// function to encrypt a file and copy it to output file
void encryptFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);  
    ofstream outputFile(outputFilePath); 
    string fileContent, firstHalf, secondHalf, finalResult;
  
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

    sem_init(&first, 0, 1);  // first available
    sem_init(&second, 0, 0);  

    // Create threads
    thread t1(encryptAndWrite, ref(firstHalf), ref(finalResult));
    thread t2(encryptAndWriteSecond, ref(secondHalf), ref(finalResult));

    // Wait for threads to finish
    t1.join();
    t2.join();

    outputFile << finalResult;

    outputFile.close();

    cout << "file has been encrypted successfully" << endl;
}

void decryptAndWrite(const string& inputPart, string& result) {
    string decryptedPart = decrypt(inputPart);

    sem_wait(&first); 
    result += decryptedPart; 

    sem_post(&second); 
}

void decryptAndWriteSecond(const string& inputPart, string& result) {
    string decryptedPart = decrypt(inputPart);

    sem_wait(&second); 
    result += decryptedPart; 

    sem_post(&first); 
}

// function to decrypt a file and copy it to decrypted file
void decryptFile(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);  
    ofstream decryptedFile(outputFilePath); 
    string fileContent, firstHalf, secondHalf, finalResult;
  
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

    sem_init(&first, 0, 1);  
    sem_init(&second, 0, 0); 

    thread t1(decryptAndWrite, ref(firstHalf), ref(finalResult));
    thread t2(decryptAndWriteSecond, ref(secondHalf), ref(finalResult));

    t1.join();
    t2.join();

    decryptedFile << finalResult;

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

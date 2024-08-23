# SmbForce
SmbForce is a simple tool designed to attempt SMB (Server Message Block) authentication against a target host by using a username and a wordlist of potential passwords. This tool can be used to test the strength of SMB passwords for a specific user on a Windows network.

## Features
1.Attempts authentication against the default administrative share (C$).
2.Provides detailed error messages for failed authentication attempts.
3.Logs successful authentication attempts, including the password used.

## Prerequisites
Windows operating system
A valid username in the format DOMAIN\username or HOSTNAME\username
A wordlist file containing possible passwords

## Installation
Clone or download the source code.
Ensure you have the Mpr.lib library linked during compilation.

### Compilation
To compile the code, you can use any C++ compiler that supports Windows libraries. Here's an example using g++:
```powershell
g++ -o SmbForce SmbForce.cpp -lMpr
```

### Usage
Prepare your wordlist file with potential passwords, one per line.
Modify the username, wordlistPath, and hostname variables in the main() function to match your target environment.
Run the compiled executable.

### Example
```cpp
const char username[] = "Test"; // Ensure this is the correct format
const char wordlistPath[] = "C:\\passwords.txt"; // Path to the password wordlist
const char hostname[] = "192.168.1.70"; // The target hostname or IP address
```

### Output
Output
The tool will display the following outputs:

[+] Authentication Successful! if a correct password is found.
[-] Connection failed with error: <error_code> along with specific error messages for each failed attempt.

## Disclaimer
SmbForce is intended for educational purposes and authorized testing only. Unauthorized use of this tool against systems without permission is illegal and unethical. Use responsibly and with proper authorization.

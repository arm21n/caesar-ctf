#include <iostream>
#include <string>
#include <limits>
using namespace std;

char shift(char c, int k) {
  if (c >= 'A' && c <= 'Z') return (c - 'A' - k + 26) % 26 + 'A';
  if (c >= 'a' && c <= 'z') return (c - 'a' - k + 26) % 26 + 'a';
  return c; }

int main () {
  string cipher; getline(cin, cipher);
  for (int k = 1; k <= 25; k++) {
    string copyCipher = cipher;
    for (char &c : copyCipher) c = shift(c, k); 

    cout << "k = " << k << ": " << copyCipher << endl; } return 0; }


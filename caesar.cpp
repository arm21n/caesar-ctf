#include <iostream>
#include <string>
#include <limits>
using namespace std;

char shift(char c, int k) {
  if (c >= 'A' && c <= 'Z') return (c - 'A' - k + 26) % 26 + 'A';
  if (c >= 'a' && c <= 'z') return (c - 'a' - k + 26) % 26 + 'a';
  return c; }

int main () {
  char mode; string s; int k; cin >> mode;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, s);
  cin >> k; k%=26;
  if (mode == 'e') k = 26 - k;
  for (char &c : s) c = shift(c, k);
  cout << s << endl; return 0; }


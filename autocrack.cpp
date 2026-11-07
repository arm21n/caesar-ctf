#include <iostream>
#include <string>
#include <cctype>
using namespace std;
double freq[26] = { 8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15,
                    0.77, 4.0, 2.4, 6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1,
                    2.8, 0.98, 2.4, 0.15, 2.0, 0.074 };

char shift(char c, int k) {
  if (c >= 'A' && c <= 'Z') return (c - 'A' - k + 26) % 26 + 'A';
  if (c >= 'a' && c <= 'z') return (c - 'a' - k + 26) % 26 + 'a';
  return c; }

double score(string text) {
  double total = 0;
  for (char c : text) {
    if (c >= 'A' && c <= 'Z') c = tolower(c);
    if (c >= 'a' && c <= 'z') total += freq[c - 'a']; }
  return total; }

int main () {
  string cipher;
  getline(cin, cipher);
  int bestk = 1;
  double bestScore = -1;
  for (int k = 1; k <= 25; k++) {
    string copyCipher = cipher;
    for (char &c : copyCipher) c = shift(c, k);
    if (score(copyCipher) > bestScore) {
      bestScore = score(copyCipher);
      bestk = k; } }
  for (char &c : cipher) c = shift(c, bestk);
  cout << "Key: " << bestk << " -> " << cipher << endl;
  return 0; }

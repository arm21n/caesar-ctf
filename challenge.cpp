#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

const string GREEN  = "\033[1;32m";
const string RED    = "\033[1;31m";
const string YELLOW = "\033[1;33m";
const string CYAN   = "\033[1;36m";
const string DIM    = "\033[2m";
const string RESET  = "\033[0m";

// prints text letter by letter, like a terminal in a hacker movie
void slowPrint(string text, int ms) {
  for (char c : text) {
    cout << c << flush;
    this_thread::sleep_for(chrono::milliseconds(ms));
  }
  cout << endl;
}

void pause(int ms) {
  this_thread::sleep_for(chrono::milliseconds(ms));
}

int main() {
  string cipher = "czt, fzzk wzdib dixmzydwgz! d gjqz do rczi ht kvgn yj xjjg ncd cvcv!";
  string answer = "hey, keep being incredible! i love it when my pals do cool shi haha!";
  string flag   = "ARMANCRYPT{kn0wl3dg3_1s_th3_r34l_k3y}";

  // banner
  cout << "\033[2J\033[H";
  cout << CYAN;
  cout << "╔══════════════════════════════════════════════╗" << endl;
  cout << "║       ARMANCRYPT  //  CAESAR CHALLENGE       ║" << endl;
  cout << "║       [ intercepted transmission #021 ]      ║" << endl;
  cout << "╚══════════════════════════════════════════════╝" << endl;
  cout << RESET << endl;

  // fake connection sequence
  slowPrint("[*] Connecting to enemy relay...", 25);
  pause(400);
  slowPrint("[*] Capturing traffic...", 25);
  pause(400);
  cout << GREEN;
  slowPrint("[+] 1 encrypted message captured", 25);
  cout << RESET << endl;

  // the challenge
  cout << DIM << "----------------------------------------------" << RESET << endl;
  cout << YELLOW;
  slowPrint(cipher, 15);
  cout << RESET;
  cout << DIM << "----------------------------------------------" << RESET << endl << endl;

  cout << "Intel says it's an old Roman cipher. Decrypt it." << endl;
  cout << CYAN << "> " << RESET << flush;

  // read the answer
  string guess;
  getline(cin, guess);
  while (!guess.empty() && (guess.back() == '\r' || guess.back() == ' '))
    guess.pop_back();   // remove invisible extras some terminals send

  cout << endl;
  slowPrint("[*] Verifying...", 40);
  pause(600);

  if (guess == answer) {
    cout << GREEN << "[+] ACCESS GRANTED" << RESET << endl << endl;
    cout << "Your flag: " << GREEN;
    slowPrint(flag, 35);
    cout << RESET;
  } else {
    cout << RED << "[-] ACCESS DENIED" << RESET << endl;
    cout << DIM << "Hint: there are only 25 possible keys..." << RESET << endl;
  }

  cout << endl;
  return 0;
}

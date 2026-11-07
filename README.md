# caesar-ctf

**From one exam problem to a working attack, in one night.**

A Caesar cipher toolkit in C++: an encrypter, a brute-force cracker, an automatic cracker that uses frequency analysis, and a CTF-style challenge server you connect to over the network. All of it started with problem 812 on my programming end-term prep list.

```
caesar.cpp      encrypt / decrypt any sentence
crack.cpp       brute force: try all 25 keys
autocrack.cpp   frequency analysis: find the key automatically
challenge.cpp   CTF challenge served over the network with ncat
```

---

## How it started

I'm a first-year cybersecurity student at AITU. Before the Intro to Programming end-term, our teacher put the scope on the screen: structs, strings, and functions/pointers from Assignments 4 and 5.

![Exam scope](screenshots/00-exam-scope.jpg)

While practicing, I hit **problem 812: Caesar cipher (decrypt)**. My solution is a few lines long, and its core is one line:

```cpp
*p = (*p - 'A' - k + 26) % 26 + 'A';
```

![Problem 812 in my exam trainer](screenshots/01-trainer-problem-812.jpg)

Then I wondered: is this a real cipher? Could I *attack* it the way a pentester would? It was past midnight, I was in the zone, and the exam prep turned into this project.

---

## Phase 1: A real Caesar tool

The exam version only handles one uppercase word. I wanted full sentences with lowercase, spaces and punctuation, so I turned the formula into a function:

![shift function](screenshots/02-shift-function.png)

`shift()` has three paths: uppercase wraps within `A–Z`, lowercase within `a–z`, and everything else passes through unchanged.

My first attempt didn't compile because the function had no closing brace, so the compiler thought `main` was inside `shift`:

![Missing brace](screenshots/03-missing-brace.png)

The first run worked, even though I typed `Zrog` instead of `Zruog`. The program decrypted exactly what I gave it:

![First decrypt](screenshots/04-first-decrypt.png)

Then I added an encrypt mode. Moving forward by `k` is the same as moving back by `26 - k`, so one function does both jobs. I also had to use `cin.ignore()` so `getline` doesn't read the leftover Enter, which is the same trick as Assignment 4, problem 618.

![Encrypt mode code](screenshots/05-encrypt-mode-code.png)
![Encrypt and decrypt test](screenshots/06-encrypt-decrypt-test.png)

---

## Phase 2: Brute force, trying all 25 keys

Now the attacker's side: I have the ciphertext but **not** the key. A Caesar cipher only has 25 possible keys, so I just try them all.

It took me four attempts. I passed a whole string to a function that takes one `char`, put `return 0` inside the loop, and printed inside the inner loop:

![Attempt 1](screenshots/07-crack-attempt-1.png)
![Attempt 2](screenshots/08-crack-attempt-2.png)
![Attempt 3](screenshots/09-crack-attempt-3.png)
![Attempt 4](screenshots/10-crack-attempt-4.png)

The pattern I finally understood: a **loop inside a loop**. The inner loop decrypts every character of one try, and the outer loop goes through every key. Each key also works on a fresh copy of the ciphertext.

![Brute force success](screenshots/11-bruteforce-success.png)

`k = 3: Hello, World!` is the only line that's real English.

---

## Phase 3: Frequency analysis, so the computer finds the answer

Reading 25 lines by eye is slow. In English, **E** makes up about 12.7% of letters, **T** 9.1%, and **A** 8.2%, while **Z** is only 0.07%. So I score every attempt by adding up how common its letters are, and the highest score wins.

Two bugs on the way: `freq[0]` gave every letter the score of `a`, and I stored a string in a `double`:

![Score bug](screenshots/12-score-bug.png)
![bestScore bug](screenshots/13-bestscore-bug.png)
![Fixed](screenshots/14-bestscore-fixed.png)

Then it cracked a message without me reading anything:

![Autocrack success](screenshots/15-autocrack-success.png)

### Where it fails

I tried to break my own tool. I encrypted `SOS. HELP.` with key 5, and autocrack confidently answered **`HDH. WTAE.`**:

![SOS HELP failure](screenshots/16-sos-help-failure.png)

| Candidate | Letter scores | Total |
|---|---|---|
| `SOS HELP` (correct) | 6.3 + 7.5 + 6.3 + 6.1 + 12.7 + 4.0 + 1.9 | **44.8** |
| `HDH WTAE` (garbage) | 6.1 + 4.3 + 6.1 + 2.4 + 9.1 + 8.2 + 12.7 | **48.9** |

With only 7 letters, statistics don't have enough data. The garbage line happens to contain T, A and E, so it wins. On long sentences, real English always comes out on top. My tool only knows which letters are common, not what words look like. Ideas to improve it: show the top 3 candidates, check against a dictionary, or use a chi-squared test.

---

## Phase 4: A CTF challenge

If I can break it, so can my friends. I wrote a challenge: it shows an intercepted message, asks for the plaintext, and gives a flag if the answer is right.

![Challenge code](screenshots/17-challenge-code.png)

To test it end to end, I used my own `autocrack` on the challenge's ciphertext. It found key 21, and the flag came out:

![Challenge works](screenshots/18-challenge-works.png)

Then it got a terminal makeover: ANSI colors, a banner, a typewriter effect, and ACCESS GRANTED / DENIED:

![Styled challenge](screenshots/19-styled-challenge.png)

---

## Phase 5: Serving it on the network

```bash
ncat -l -k 4444 -e ./challenge
```

`ncat` listens on port 4444 and runs `./challenge` for every player who connects. `cin` and `cout` now talk to the network connection instead of my keyboard. That's why every line needs `endl` / `flush`.

### A finding I didn't expect

When I tried to open the port, Fedora said it was **already open**:

![Firewall check](screenshots/20-firewall-check.png)
![Open ports](screenshots/21-open-ports.png)

The default Fedora Workstation firewall zone opens **every port from 1025 to 65535**, TCP and UDP. Any program I run on a high port is reachable by everyone on the same Wi-Fi. That's convenient at home, but risky on university or café networks. The fix for untrusted networks is to put the connection in the stricter `public` zone:

```bash
sudo nmcli connection modify "<wifi-name>" connection.zone public
```

### First connection attempt: failed

My phone couldn't connect on the first Wi-Fi network. I switched to my phone's hotspot, which gave my laptop a new IP:

![Network switch](screenshots/22-network-switch.png)

*(IP addresses are redacted in all screenshots.)*

---

## Phase 6: The attack

From my phone, in Termux:

```bash
nc <laptop-ip> 4444
```

The challenge came through over the network, colors and animation included:

![Phone connected](screenshots/23-phone-connected.jpg)

I copied the intercepted message to my laptop, and `autocrack` found key 21:

![Autocrack on the intercepted message](screenshots/24-autocrack-intercepted.png)

Then I typed the plaintext back on my phone:

![Flag captured](screenshots/25-phone-flag.jpg)

**ACCESS GRANTED.** That's an intercepted message, cracked without the key, and the flag captured remotely.

---

## Run it yourself

```bash
g++ caesar.cpp    -o caesar
g++ crack.cpp     -o crack
g++ autocrack.cpp -o autocrack
g++ challenge.cpp -o challenge
```

| Tool | Input |
|---|---|
| `./caesar` | mode (`e` or `d`), a sentence, a key, each on its own line |
| `./crack` | a ciphertext line, prints all 25 attempts |
| `./autocrack` | a ciphertext line, prints the best key and plaintext |
| `./challenge` | play locally, or serve it with `ncat -l -k 4444 -e ./challenge` |

Only run the server on a network you trust, and stop it with `Ctrl+C` when you're done.

> ⚠️ Spoiler: `challenge.cpp` contains the answer and the flag. Try to crack the server before reading the source.

---

## What I learned

- **The Caesar cipher has no real security.** A 25-key space falls to brute force instantly, and frequency analysis breaks it without even guessing.
- **Statistics need enough data.** My cracker fails on short messages, and knowing where your tool fails is part of the skill.
- **Loops inside loops**, the `best so far` pattern, `char` vs `string`, and `cin` vs `getline`: exam material that clicked once I used it for something real.
- **`stdin`/`stdout` can be a network socket.** With `ncat`, any console program becomes a network service.
- **Check the defaults.** Fedora leaves ports 1025–65535 open on its default zone, the kind of thing a pentester looks for.

## Next ideas

- [ ] Show the top 3 candidates instead of one
- [ ] Dictionary-based scoring
- [ ] A random key for every connection
- [ ] Vigenère cipher, and a cracker for it

---

Built by Arman ([@arm21n](https://github.com/arm21n)), first-year cybersecurity student at AITU, aspiring penetration tester. This project is part of my learning in public; I built it step by step with an AI tutor giving hints along the way.

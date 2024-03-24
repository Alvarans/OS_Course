#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> // нужен для вызова функции rand(), srand()
#include <time.h> // нужен для вызова функции time()
#include <stdio.h>



#define MAX_LEN 10000
using namespace std;
bool ShipinTer1 = false, ShipinTer2 = false; //Flags for ships in terminals


int main() {
    srand(time(0));
    signed int fdParentChild1[2], fdParentChild2[2], fdChildParent1[2], fdChildParent2[2];
//Channels
    bool StableState1 = false, UpDownState1 = false, DownUpState1 = false, //Terminal 1 states
    StableState2 = false, UpDownState2 = false, DownUpState2 = false; //Terminal 2 states
//Process creation
    pid_t pid;
    //Buffer initialize
    char buf[MAX_LEN];
    //Channels creation
    pipe(fdParentChild1);
    pipe(fdParentChild2);
    pipe(fdChildParent1);
    pipe(fdChildParent2);
    pid = fork();
    while (true) {
        if (-1 == pid) {
            perror("fork");
            exit(1);
        } else if (pid == 0) { // Server
            int state = rand() % 3;
            sleep(3);
            if (state == 0)
                cout << "\033[35m\nWhere are no ships. Waiting...\n\033[0m\n";
            else if (state == 1 && !ShipinTer1) {
                ShipinTer1 = true;
                close(fdParentChild1[0]);
                write(fdParentChild1[1], &ShipinTer1, sizeof(ShipinTer1));
                ShipinTer1 = false;
            } else if (state == 2 && !ShipinTer2) {
                ShipinTer2 = true;
                close(fdParentChild2[0]);
                write(fdParentChild2[1], &ShipinTer2, sizeof(ShipinTer2));
                ShipinTer2 = false;
            }
            sleep(3);
        } else { //Terminals
            pid_t npid = fork();
            if (-1 == npid) {
                perror("fork");
                exit(1);
            } else if (npid == 0) { //Terminal 1
                bool state1;
                close(fdParentChild1[1]);
                read(fdParentChild1[0], &state1, sizeof(state1));
                if (state1) {
                    ShipinTer1 = true;
                    srand(time(0));
                    bool direction = rand() % 2;
                    if (direction) {
                        cout << "\033[32m\nShip incoming in \033[34m terminal 1\033[32m from"
                        << "above.\n\033[34m terminal1\033[32m upper gate is open.\033[0m\n";
                        sleep(3);
                        cout << "\033[32m\nShip in \033[34m terminal 1\033[32m. Close upper gate. "
                             << "The water level is decreasing.\033[0m\n";
                        StableState1 = false;
                        UpDownState1 = true;
                        sleep(5);
                        cout << "\033[32m\nThe ship leaves from \033[34m terminal 1\033[32m through the lower gate.\033[0m\n";
                        UpDownState1 = false;
                        StableState1 = true;
                        ShipinTer1 = false;
                        close(fdChildParent1[0]);
                        write(fdChildParent1[1], &ShipinTer1, sizeof(ShipinTer1));
                    } else {
                        cout << "\033[32m\nShip incoming in \033[34m terminal 1\033[32m from below."
                             << "\n\033[34m terminal1\033[32m lowergate is open.\033[0m\n";
                        sleep(3);
                        cout << "\033[32m\nShip in \033[34m terminal 1\033[32m. Close lower gate." 
                             << "The water level is increasing.\033[0m\n";
                        StableState1 = false;
                        DownUpState1 = true;
                        sleep(5);
                        cout << "\033[32m\nThe ship leaves from \033[34m terminal 1\033[32m through the upper gate.\033[0m\n";
                        DownUpState1 = false;
                        StableState1 = true;
                        ShipinTer1 = false;
                        close(fdChildParent1[0]);
                        write(fdChildParent1[1], &ShipinTer1, sizeof(ShipinTer1));
                    }
                } else {
                    cout << "\033[31m\nShip incoming in terminal 1, but terminal is already full!\033[0m\n";
                }
            } else { //Terminal 2
                bool state2;
                close(fdParentChild2[1]);
                read(fdParentChild2[0], &state2, sizeof(state2));
                if (state2) {
                    ShipinTer2 = true;
                    srand(time(0));
                    bool direction = rand() % 2;
                    if (direction) {
                        cout << "\033[33m\nShip incoming in \033[34mterminal 2\033[33m from above."
                             << "\n\033[34mterminal 2\033[33m upper gate is open.\033[0m\n";
                        sleep(3);
                        cout << "\033[33m\nShip in \033[34mterminal 2\033[33m. Close upper gate."
                             << " The water level is decreasing.\033[0m\n ";
                        StableState2 = false;
                        UpDownState2 = true;
                        sleep(5);
                        cout << "\033[33m\nThe ship leaves from \033[34mterminal 2\033[33m through the lower gate.\033[0m\n";
                        UpDownState2 = false;
                        StableState2 = true;
                        ShipinTer2 = false;
                        close(fdChildParent2[0]);
                        write(fdChildParent2[1], &ShipinTer2, sizeof(ShipinTer2));
                    } else {
                        cout << "\033[33m\nShip incoming in \033[34mterminal 2\033[33m from below."
                             << "\n\033[34mterminal 2\033[33m lower gate is open.\033[0m\n ";
                        sleep(3);
                        cout << "\033[33m\nShip in \033[34mterminal 2\033[33m. Close lower gate. "
                             << "The water level is increasing.\033[0m\n";
                        StableState2 = false;
                        DownUpState2 = true;
                        sleep(5);
                        cout << "\033[33m\nThe ship leaves from \033[34mterminal 2\033[33m through the upper gate.\033[0m\n";
                        DownUpState2 = false;
                        StableState2 = true;
                        ShipinTer2 = false;
                        close(fdChildParent2[0]);
                        write(fdChildParent2[1], &ShipinTer2, sizeof(ShipinTer2));
                    }
                } else {
                    cout << "\033[31m\nShip incoming in terminal 2, but terminal is already full!\033[0m\n";
                }
            }
        }
    }
    return 0;
}

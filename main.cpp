#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class armour {
public:
    string name;
    double stat[14]{};
};

int main() {
    ifstream helm, chest, gauntlet, leg;
    helm.open(R"(../res/Helms.csv)");
    if (!helm.is_open()) {
        cout << "Error opening file" << '\n';
        return 0;
    }
    chest.open(R"(../res/ChestArmour.csv)");
    if (!chest.is_open()) {
        cout << "Error opening file" << '\n';
        return 0;
    }
    gauntlet.open(R"(../res/Gauntlets.csv)");
    if (!gauntlet.is_open()) {
        cout << "Error opening file" << '\n';
        return 0;
    }
    leg.open(R"(../res/LegArmour.csv)");
    if (!leg.is_open()) {
        cout << "Error opening file" << '\n';
        return 0;
    }
    vector<armour> helms;
    vector<armour> chests;
    vector<armour> gauntlets;
    vector<armour> legs;
    while (!helm.eof()) {
        armour a;
        getline(helm, a.name, ';');
        for (int i = 0; i < 13; i++) {
            string s;
            getline(helm, s, ';');
            a.stat[i] = atof(s.c_str());
        }
        string s;
        getline(helm, s);
        a.stat[13] = atof(s.c_str());
        helms.push_back(a);
    }
    while (!chest.eof()) {
        armour a;
        getline(chest, a.name, ';');
        for (int i = 0; i < 13; i++) {
            string s;
            getline(chest, s, ';');
            a.stat[i] = atof(s.c_str());
        }
        string s;
        getline(chest, s);
        a.stat[13] = atof(s.c_str());
        chests.push_back(a);
    }
    while (!gauntlet.eof()) {
        armour a;
        getline(gauntlet, a.name, ';');
        for (int i = 0; i < 13; i++) {
            string s;
            getline(gauntlet, s, ';');
            a.stat[i] = atof(s.c_str());
        }
        string s;
        getline(gauntlet, s);
        a.stat[13] = atof(s.c_str());
        gauntlets.push_back(a);
    }
    while (!leg.eof()) {
        armour a;
        getline(leg, a.name, ';');
        for (int i = 0; i < 13; i++) {
            string s;
            getline(leg, s, ';');
            a.stat[i] = atof(s.c_str());
        }
        string s;
        getline(leg, s);
        a.stat[13] = atof(s.c_str());
        legs.push_back(a);
    }
    helm.close();
    chest.close();
    gauntlet.close();
    leg.close();
    attributeSelect:
    cout << "Which attribute do you want to prioritize?" << '\n'
         << "Physical - 1" << '\n'
         << "Vs Strike - 2" << '\n'
         << "Vs Slash - 3" << '\n'
         << "Vs Pierce - 4" << '\n'
         << "Magic - 5" << '\n'
         << "Fire - 6" << '\n'
         << "Lightning - 7" << '\n'
         << "Holy - 8" << '\n'
         << "Immunity - 9" << '\n'
         << "Robustness - 10" << '\n'
         << "Focus - 11" << '\n'
         << "Vitality - 12" << '\n'
         << "Poise - 13" << '\n';
    int priority;
    cin >> priority;
    priority--;
    switch (priority) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        default:
            cout << "Wrong input" << '\n';
            goto attributeSelect;
    }
    double max_load, current_weight;
    Weight:
    cout << "What is your maximum equip load?" << '\n';
    cin >> max_load;
    cout << "What is your current equipment weight (weight of equipped "
            "non-armour items)?"
         << '\n';
    cin >> current_weight;
    cout << "What is the weight category you want to belong to?" << '\n'
         << "1-Light(30%)" << '\n'
         << "2-Medium(70%)" << '\n'
         << "3-Heavy(100%)" << '\n';
    double ratio;
    int input;
    cin >> input;
    switch (input) {
        case 1:
            ratio = 0.3;
            break;
        case 2:
            ratio = 0.7;
            break;
        case 3:
            ratio = 1;
            break;
        default:
            goto Weight;
    }
    if (current_weight >= ratio * max_load) {
        cout << "Error - you are already overloaded" << '\n';
        goto Weight;
    }
    Calculations:
    double maxStat = 0;
    int maxSet[4] = {0};
    for (int i = 0; i < chests.size(); i++) {
        double x = current_weight, a = 0;
        x += chests[i].stat[13];
        if (x >= ratio * max_load) {
            continue;
        }
        a += chests[i].stat[priority];
        for (int j = 0; j < legs.size(); j++) {
            double y = x, b = a;
            y += legs[j].stat[13];
            if (y >= ratio * max_load) {
                continue;
            }
            b += legs[j].stat[priority];
            for (int k = 0; k < helms.size(); k++) {
                double z = y, c = b;
                z += helms[k].stat[13];
                if (z >= ratio * max_load) {
                    continue;
                }
                c += helms[k].stat[priority];
                for (int l = 0; l < gauntlets.size(); l++) {
                    double w = z, d = c;
                    w += gauntlets[l].stat[13];
                    if (w >= ratio * max_load) {
                        continue;
                    }
                    d += gauntlets[l].stat[priority];
                    if (d > maxStat) {
                        maxStat = d;
                        maxSet[0] = k;
                        maxSet[1] = i;
                        maxSet[2] = l;
                        maxSet[3] = j;
                    }
                }
            }
        }
    }
    Exclusion:
    maxStat = 100 - (1 - helms[maxSet[0]].stat[priority] / 100) *
                    (1 - chests[maxSet[1]].stat[priority] / 100) *
                    (1 - gauntlets[maxSet[2]].stat[priority] / 100) *
                    (1 - legs[maxSet[3]].stat[priority] / 100) * 100;
    cout << "Maximum attribute achieved - " << maxStat << '\n'
         << "Recommended set:" << '\n'
         << "1 - " << helms[maxSet[0]].name << '\n'
         << "2 - " << chests[maxSet[1]].name << '\n'
         << "3 - " << gauntlets[maxSet[2]].name << '\n'
         << "4 - " << legs[maxSet[3]].name << '\n';
    cout << "Do you lack one of these? Enter number from 1 to 4 to exclude the "
            "item from further calculations or 0 to exit the program."
         << '\n';
    int input2;
    cin >> input2;
    switch (input2) {
        case 0:
            break;
        case 1:
            helms.erase(helms.begin() + maxSet[0]);
            goto Calculations;
        case 2:
            chests.erase(chests.begin() + maxSet[1]);
            goto Calculations;
        case 3:
            gauntlets.erase(gauntlets.begin() + maxSet[2]);
            goto Calculations;
        case 4:
            legs.erase(legs.begin() + maxSet[3]);
            goto Calculations;
        default:
            cout << "Wrong input" << '\n';
            goto Exclusion;
    }
}
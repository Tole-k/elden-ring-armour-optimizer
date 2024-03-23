#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class armour {
public:
    string name;
    double weight{};
    double stat[13]{};
};
vector<armour> preProcess(vector<armour> &armours, double remaining_weight, int priority) {
    map<double,armour> m;
    for (int i = 0; i < armours.size(); i++) {
        if (armours[i].weight > remaining_weight) {
            armours.erase(armours.begin() + i);
            i--;
        }
        else
        {
            if(m.find(armours[i].weight) == m.end())
            {
                m[armours[i].weight] = armours[i];
            }
            else
            {
                if(armours[i].stat[priority] > m[armours[i].weight].stat[priority])
                {
                    m[armours[i].weight] = armours[i];
                }
            }
        }
    }
    vector<armour> newArmours;
    newArmours.reserve(m.size());
for(auto & i : m)
    {
        newArmours.insert(newArmours.begin(),i.second);
    }
    return newArmours;
}
vector<armour> readData(const string& path) {
    ifstream file;
    file.open(path);
    if (!file.is_open()) {
        cout << "Error opening file" << '\n';
        return {};
    }
    vector<armour> armours;
    while (!file.eof()) {
        armour a;
        getline(file, a.name, ';');
        for (double & i : a.stat) {
            string s;
            getline(file, s, ';');
            i = atof(s.c_str());
        }
        string s;
        getline(file, s);
        a.weight = atof(s.c_str());
        armours.push_back(a);
    }
    file.close();
    return armours;
}
int main() {
    vector<armour> helms = readData(R"(../res/Helms.csv)");
    vector<armour> chests = readData(R"(../res/ChestArmour.csv)");
    vector<armour> gauntlets = readData(R"(../res/Gauntlets.csv)");
    vector<armour> legs = readData(R"(../res/LegArmour.csv)");
    attributeSelect:
    cout << "Which attribute do you want to maximise?" << '\n'
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
    if (priority < 0 || priority > 12) {
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
    double remaining_weight = ratio * max_load - current_weight;
    helms = preProcess(helms, remaining_weight, priority);
    chests = preProcess(chests, remaining_weight, priority);
    gauntlets = preProcess(gauntlets, remaining_weight, priority);
    legs = preProcess(legs, remaining_weight, priority);
    Calculations:
    double maxStat = 0;
    int maxSet[4] = {0};
    for (int i = 0; i < chests.size(); i++) {
        double x = 0, a = 0;
        x += chests[i].weight;
        if (x >= remaining_weight) {
            continue;
        }
        a += chests[i].stat[priority];
        for (int j = 0; j < legs.size(); j++) {
            double y = x, b = a;
            y += legs[j].weight;
            if (y >= remaining_weight) {
                continue;
            }
            b += legs[j].stat[priority];
            for (int k = 0; k < helms.size(); k++) {
                double z = y, c = b;
                z += helms[k].weight;
                if (z >= remaining_weight) {
                    continue;
                }
                c += helms[k].stat[priority];
                for (int l = 0; l < gauntlets.size(); l++) {
                    double w = z, d = c;
                    w += gauntlets[l].weight;
                    if (w >= remaining_weight) {
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
    cout << "Do you lack one of these? Enter number from 1 to 4 to exclude that"
            "item and rerun the computations or 0 to exit the program."
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
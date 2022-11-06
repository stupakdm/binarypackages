#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

struct package
{
    string name;
    int epoch;
    string version;
    string release;
    string arch;
    string disttag;
    long int buildtime;
    string source;
};

class binaryPackages
{
private:
    vector<struct package> packages_1;
    vector<struct package> packages_2;

    vector<struct package> packages_1_result1;
    vector<struct package> packages_2_result1;
    vector<struct package> packages_result2;
    vector<string> split_string(string s, char del);
    void unique(vector<struct package> pack1, vector<struct package> pack2);
    bool cmp(string val1, string val2);
    void check_vers_release(vector<struct package> package1, vector<struct package> package2);

public:
    binaryPackages();
    void get_packages(string url1, string url2);
    vector<struct package> parse_json(string json);
};

vector<string> binaryPackages::split_string(string s, char del)
{
    vector<string> split_string;
    stringstream ss(s);
    string word;
    while (!ss.eof())
    {
        getline(ss, word, del);
        split_string.push_back(word);
    }
    return split_string;

}

void binaryPackages::unique(vector<struct package> pack1, vector<struct package> pack2)
{
    map<string, vector<struct package>> mp;
    vector<string> keys = {"name", "epoch", "version", "release", "arch", "disttag", "buildtime", "source"};
    for (size_t i =0;i<pack1.size();i++)
    {
        string name = pack1[i].name;
        auto item = mp.find(name);
        if (item != mp.end())
        {
            mp[name].push_back(pack1[i]);
        }
        else
        {
            vector<struct package> prob_json;
            prob_json.push_back(pack1[i]);
            mp[name] = prob_json;
        }
    }

    int i = 0;
    int l2 = pack2.size();
    while (i<l2)
    {
        string name = pack2[i].name;
        auto item = mp.find(name);
        if (item != mp.end())
        {
            int j = 0;
            for (j=0;j<(int)mp[name].size();j++)
            {
                int compare = 0;
                if (mp[name][j].name == pack2[i].name)
                    compare++;

                if (mp[name][j].epoch == pack2[i].epoch)
                    compare++;

                if (mp[name][j].version == pack2[i].version)
                    compare++;

                if (mp[name][j].release == pack2[i].release)
                    compare++;

                if (mp[name][j].arch == pack2[i].arch)
                    compare++;

                if (mp[name][j].disttag == pack2[i].disttag)
                    compare++;

                if (mp[name][j].buildtime == pack2[i].buildtime)
                    compare++;

                if (mp[name][j].source == pack2[i].source)
                    compare++;

                if (compare == (int)keys.size())
                {
                    mp[name].erase(mp[name].begin()+j);
                    if (mp[name].size() == 0)
                    {
                        mp[name].clear();
                    }
                    pack2.erase(pack2.begin()+i);
                    i--;
                    l2 = pack2.size();
                    break;
                }

            }

        }
        i++;
    }
    packages_2_result1.clear();
    packages_1_result1.clear();
    copy(pack2.begin(), pack2.end(), back_inserter(packages_2_result1));

    for (auto iter = mp.begin();iter!=mp.end();iter++)
    {
        size_t z=0;
        for (z = 0;z<iter->second.size();z++)
        {
            packages_1_result1.push_back(iter->second[z]);
        }
    }


}


bool binaryPackages::cmp(string val1, string val2)
{
    vector<string> val_1 = split_string(val1, '.');
    vector<string> val_2 = split_string(val2, '.');
    size_t size_max = val_1.size() > val_2.size() ? val_1.size() : val_2.size();
    for (size_t t = 0;t<size_max;t++)
    {
        if (t == val_1.size())
            break;
        if (t >= val_2.size())
            return true;
        if (atoi(val_1[t].c_str()) > atoi(val_2[t].c_str()))
            return true;
        else if (atoi(val_1[t].c_str()) < atoi(val_2[t].c_str()))
            return false;
    }
    return false;
}

void binaryPackages::check_vers_release(vector<struct package> package1, vector<struct package> package2)
{
    size_t size_pack = package1.size();
    map<string, map<string, string>> mp;
    for (size_t t = 0;t<package2.size();t++)
    {
        string arch = package2[t].arch+package2[t].name;
        auto item = mp.find(arch);
        if ( item != mp.end())
        {
            if (cmp(package2[t].version, mp[arch]["version"]))
            {
                mp[arch]["version"] = package2[t].version;
            }
            if (cmp(package2[t].version, mp[arch]["release"]))
            {
                mp[arch]["release"] = package2[t].release;
            }
        }
        else
        {
            mp[arch]["release"] = package2[t].release;
            mp[arch]["version"] = package2[t].version;
        }
    }

    size_t i = 0;
    while (i<size_pack)
    {
        string arch = package1[i].arch+package1[i].name;
        auto item = mp.find(arch);
        if (item != mp.end())
        {
            package1.erase(package1.begin()+i);
            i--;
            size_pack = package1.size();
        }
        else
        {
            if (not cmp(package1[i].version, mp[arch]["version"]))
            {
                package1.erase(package1.begin()+i);
                i--;
                size_pack = package1.size();
            }
            else if (not cmp(package1[i].release, mp[arch]["release"]))
            {
                package1.erase(package1.begin()+i);
                i--;
                size_pack = package1.size();
            }
        }
        i++;
    }
    copy(package1.begin(), package1.end(), back_inserter(packages_result2));

}

binaryPackages::binaryPackages()
{

}



string read_file(const string filename)
{
    string data;
    ifstream in(filename.c_str());
    getline(in, data, string::traits_type::to_char_type(string::traits_type::eof()));
    return data;
}

int write_file(const string filename, vector<struct package> pack)
{
    ofstream myfile;

    myfile.open(filename);
    myfile << "{" << endl;
    myfile << "  " <<"length: " << pack.size() << ',' << endl;
    myfile << "  " << "packages: [" << endl;

    for (struct package pack_l: pack)
    {
        myfile << "{";
        myfile << "name: ";
        myfile << pack_l.name;
        myfile << ',';
        myfile << "epoch: ";
        myfile       << to_string(pack_l.epoch);
        myfile       << ',' ;
        myfile << "version: ";
        myfile       << pack_l.version;
        myfile       << ',';
        myfile << "release: ";
        myfile       << pack_l.release;
        myfile       << ',';
        myfile << "arch: ";
        myfile       << pack_l.arch;
        myfile       << ',';
        myfile << "disttag: ";
        myfile       << pack_l.disttag ;
        myfile       << ',' ;
        myfile << "buildtime: ";
        myfile       << to_string(pack_l.buildtime);
        myfile       << ',';
        myfile << "source: ";
        myfile       << pack_l.source;
        myfile       << "},";
    }
    myfile << "  ]" << endl;
    myfile << "}";

    myfile.close();
    return 0;
}

void binaryPackages::get_packages(string url1, string url2)
{
    system("rm temp1.txt");
    system("rm temp2.txt");
    system("rm packages11_result.txt");
    system("rm packages12_result.txt");
    system("rm packages2_result.txt");
    string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    system((string("curl ")+url+url1+string(" >> temp1.txt")).c_str());

    string json = read_file("temp1.txt");
    packages_1 = parse_json(json);
    json.clear();
    system("rm temp1.txt");

    system((string("curl ")+url+url2+string(" >> temp2.txt")).c_str());
    json = read_file("temp2.txt");
    packages_2 = parse_json(json);
    json.clear();
    system("rm temp2.txt");


    unique(packages_1, packages_2);

    check_vers_release(packages_1, packages_2);
    packages_1.clear();
    packages_2.clear();
    write_file("packages11_result.txt", packages_1_result1);
    write_file("packages12_result.txt", packages_2_result1);

    write_file("packages2_result.txt", packages_result2);



    return;
}

vector<struct package> binaryPackages::parse_json(string json)
{

    vector<string> to_find = {"name", "epoch", "version", "release", "arch", "disttag", "buildtime", "source"};
    string length = "length";
    vector<struct package> packages_get;
    size_t found = json.find(length);
    
    if (found!= string::npos)
    {
        json = json.substr(found+length.size()+2, json.size());
        size_t found = json.find(',');
        int amount = atoi(json.substr(0, found).c_str());//min(1000, atoi(json.substr(0, found).c_str()));

        for (int i =0;i<amount;i++)
        {
            struct package pack;
            for (int j=0;j<(int)to_find.size();j++)
            {
                found = json.find(to_find[j])+to_find[j].size()+4;
                if (to_find[j] == "epoch" || to_find[j] == "buildtime")
                {
                    found--;
                }
                json = json.substr(found, json.size());
                if (to_find[j] == "source")
                    found = json.find('}');
                else
                    found = json.find(',');
                if (to_find[j] != "epoch" && to_find[j] != "buildtime")
                {
                    found--;
                }

                string substr = json.substr(0, found);
                switch (j) {
                    case 0:
                        pack.name = substr;
                        break;
                    case 1:
                        pack.epoch = atoi(substr.c_str());
                        break;
                    case 2:
                        pack.version = substr;
                        break;
                    case 3:
                        pack.release = substr;
                        break;
                    case 4:
                        pack.arch = substr;
                        break;
                    case 5:
                        pack.disttag = substr;
                        break;
                    case 6:
                        pack.buildtime = atoi(substr.c_str());
                        break;
                    case 7:
                        pack.source = substr;
                        break;
                }

            }
            packages_get.push_back(pack);

        }
    }
    return packages_get;

}


int main(int argc, char* argv[])
{

    if (argc > 3)
    {
    	printf("Необходимо задать 2 параметра\n");
    	return -1;
    }
    binaryPackages packages;
    packages.get_packages(argv[1], argv[2]);


}

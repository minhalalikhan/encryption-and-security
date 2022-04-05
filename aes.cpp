#include <iostream>
#include <bits/stdc++.h>
#include <math.h>
#include <vector>
#include <string>

using namespace std;

void print(vector<int> array)
{

    for (int i = 0; i < array.size(); i++)
        cout << " " << array[i];
}

void s_box(vector<int> &input, vector<int> table)
{
    for (int i = 0; i < input.size(); i++)
    {
        input[i] = table[input[i]];
    }
}

void addkey(vector<int> &input, vector<int> key)
{
    for (int i = 0; i < input.size(); i++)
    {
        input[i] = input[i] ^ key[i];
    }
}

void shiftrow(vector<int> &input, int dir)
{

    vector<vector<int>> mat(4, vector<int>(4, 0));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[j][i] = input[4 * i + j];

    // shift rows
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (dir > 0)
            {
                int tmp = mat[i][0];
                mat[i].erase(mat[i].begin());
                mat[i].push_back(tmp);
            }
            else
            {
                int tmp = mat[i][3];
                mat[i].erase(mat[i].begin() + 3);
                mat[i].insert(mat[i].begin(), tmp);
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            input[4 * i + j] = mat[j][i];
        }
    }
}

int mul(int a_, int b_)
{
    unsigned char a = (unsigned char)a_;
    unsigned char b = (unsigned char)b_;
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for (counter = 0; counter < 8; counter++)
    {
        if ((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 128);
        a <<= 1;
        if (hi_bit_set == 128)
            a ^= 27;
        b >>= 1;
    }
    int res = (int)p;
    return res;
}

void mixcol(vector<int> &input, int mixcol[4][4])
{
    int mat[4];
    for (int z = 0; z < 4; z++)
    {
        for (int i = 0; i < 4; i++)
            mat[i] = input[z * 4 + i];

        //  int res[4];
        for (int i = 0; i < 4; i++)
        {
            int tmp = 0;
            for (int j = 0; j < 4; j++)
            {
                int tmp2 = mul(mat[j], mixcol[i][j]);
                tmp = tmp ^ tmp2;
            }
            input[z * 4 + i] = tmp;
        }
    }
}

void getroundkey(vector<int> &roundkey, vector<int> key, int n)
{
    for (int i = 0; i < key.size(); i++)
        roundkey[i] = key[i];
    // left shift
    for (int i = 0; i < n; i++)
    {
        int tmp = roundkey[0];
        roundkey.erase(roundkey.begin());
        roundkey.push_back(tmp);
    }
}

int main()
{ //                                                    SIMPLIFIED  AES
    system("cls");
    vector<int> input(16, 0);
    vector<int> key(16, 0);
    vector<int> sbox_table;
    vector<int> in_sbox_table(256, 0);

    // fill s-box table
    for (int i = 0; i < 256; i++)
        sbox_table.push_back(i);

    random_shuffle(sbox_table.begin(), sbox_table.end());

    for (int i = 0; i < 256; i++)
        in_sbox_table[sbox_table[i]] = i;

    int in_mixcol_mat[4][4] = {{14, 11, 13, 9},
                               {9, 14, 11, 13},
                               {13, 9, 14, 11},
                               {11, 13, 9, 14}};

    int mixcol_mat[4][4] = {{2, 3, 1, 1},
                            {1, 2, 3, 1},
                            {1, 1, 2, 3},
                            {3, 1, 1, 2}};

    int rounds = 5, roundcount = 1;
    string key_string = "", input_string = "";

    cout << " \n rounds  : " << rounds;
    cout << " \n enter key : ";
    cin >> key_string;
    cout << " \n enter input : ";
    cin >> input_string;

    for (int i = 0; i < key_string.size() && i < 16; i++)
        key[i] = stoi(key_string.substr(i, 1));
    for (int i = 0; i < input_string.size() && i < 16; i++)
        input[i] = stoi(input_string.substr(i, 1));

    // initial key add

    addkey(input, key);

    vector<int> roundkey(key);

    for (int i = 0; i < rounds - 1; i++)
    {
        s_box(input, sbox_table);
        shiftrow(input, 1);
        mixcol(input, mixcol_mat);
        getroundkey(roundkey, key, roundcount++);
        addkey(input, roundkey);
    }

    // final round
    s_box(input, sbox_table);
    shiftrow(input, 1);
    getroundkey(roundkey, key, roundcount);
    addkey(input, roundkey);

    cout << " \n final result : ";
    print(input);

    // // decryption part

    cout << "\n\n decrypting \n";
    addkey(input, roundkey);
    shiftrow(input, -1); // inverse shoft row
    s_box(input, in_sbox_table);

    for (int i = 0; i < rounds - 1; i++)
    {
        getroundkey(roundkey, key, --roundcount);
        addkey(input, roundkey);
        mixcol(input, in_mixcol_mat);
        shiftrow(input, -1);
        s_box(input, in_sbox_table);
    }

    // final key add
    addkey(input, key);
    cout << "\n deciphered text : ";
    print(input);
    return 0;
}
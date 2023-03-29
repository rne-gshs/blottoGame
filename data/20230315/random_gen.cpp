#include <iostream>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

void rgen(int n, int k, std::vector<int> &b)
{
    b.resize(n);
    std::vector<char> v(n+k-1, 0);
    std::fill(v.begin(), v.begin() + k, 1);
    std::shuffle(v.begin(), v.end(), gen);
    
    int cnt = 0, idx = 0;
    for(char &i:v)
    {
        if(i)
            cnt++;
        else
        {
            b[idx++] = cnt;
            cnt = 0;
        }
    }
    b[n-1] = cnt;
}


int main()
{
    int n, k, t;
    std::cin >> n >> k >> t;
    
    std::vector<int> A(k+1, 0);
    std::vector<int> v;
    while(t--)
    {
        rgen(n, k, v);
        for(int i = 0; i < n; i++)
            A[v[i]]++;
    }
    
    for(int i = 0; i <= k; i++)
        std::cout << A[i] << " ";
    
    return 0;
}
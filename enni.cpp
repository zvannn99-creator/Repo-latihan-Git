#include<iostream>
using namespace std;

int main(){
    int a,b;
    int *pa = &a, *pb = &b;
    int sum, absDifference;

    cout << "a = ";
    cin >> a;
    cout << "b = ";
    cin >> b;

    sum = *pa + *pb;

    if(a > b) 
        absDifference = *pa - *pb;
    else absDifference = *pb - *pa;

    cout << "a + b = " << sum << "\n|a-b| = " << absDifference << endl;

    return 0;
}
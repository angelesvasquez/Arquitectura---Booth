#include <iostream>
#include <bitset>
#include <vector>
#include <cmath>
#include <string> 
using namespace std;

class Booth{
    vector<bool> A, M, Q;
    int num_bits;
    vector<bool> relle_ceros(vector<bool> x);
    vector<bool>comp_2(vector<bool> num_bin);
    vector<bool>dec_bin(int num);
    void print_bits(vector<bool> cad);
    vector<bool> voltear(vector<bool> x);
    vector<bool>suma_bin(vector<bool> A, vector<bool> M);
    vector<bool>resta_bin(vector<bool> A, vector<bool> M);
    long long bin_dec(vector<bool> R);
    void corr_aritm();
public:
    int Q_1 = 0, Q_0 = 0; //bit adicional y el menos significativo
    Booth(int m, int q){
        int num_mayor= max(abs(m),abs(q));
        num_bits = ceil(log2(num_mayor)) + 1; //calculamos el numero de bits necesarios + 1 bit de signo
        M = dec_bin(m); //inicializamos el multiplicando (binario)
        Q = dec_bin(q); //inicializamos el multiplicador (binario)
        A = relle_ceros(A); //iniciamos A con ceros
        Q_0 = Q[num_bits-1]; //Q0 el 1er bit de Q
    }
    void multiplicacion();

};
void Booth::print_bits(vector<bool> cad){
    for(bool x: cad){
            cout<<x;
    }
}
vector<bool> Booth::voltear(vector<bool> x){
    vector<bool> rest;  
    for(int i = num_bits-1; i>=0; i-- ){  
        rest.push_back(x[i]);
    }
    return rest;
}
vector<bool> Booth::relle_ceros(vector<bool> x){
    while(x.size()<num_bits){
        x.push_back(0);
    }
    return x;
}
vector<bool>Booth::comp_2 (vector<bool> num_bin){
    vector<bool> comp_2;
    //invertimos todos los bits
    for(int i = 0; i <num_bits ; i++){
        if(num_bin[i]) comp_2.push_back(0);
        else comp_2.push_back(1);
    }
    bool acarreo = true;
    for(int i = num_bits-1; i >= 0 ; i--){
        if(acarreo) {
            if(comp_2[i]) comp_2[i]=0;
            else { comp_2[i]=1; acarreo=false; }

        }
    }
    return comp_2;
}
vector<bool>Booth::dec_bin(int num){
    vector<bool> num_binario;
    bool negativo = num < 0;
    if(negativo) num*=-1;
    while (num > 0){
        if(num%2==0) num_binario.push_back(0);
        else num_binario.push_back(1);
        num/=2;
    }
    int bits_add=num_bits - num_binario.size();
    while(bits_add> 0){
        num_binario.push_back(0);
        bits_add--;
    }
    vector<bool> rest = voltear(num_binario);  
    if(negativo) {
        return comp_2(rest);
    }
    return rest;
}
vector<bool>Booth::resta_bin(vector<bool> A, vector<bool> M){
    return suma_bin(A, comp_2(M));
}
vector<bool>Booth::suma_bin(vector<bool> A, vector<bool> M){
    vector<bool> suma;
    bool acarreo = false;
    for(int i = num_bits-1; i>=0; i-- ){   
        bool temp_sum = A[i] ^ M[i] ^ acarreo;
        acarreo = (A[i] & M[i]) ^ (A[i] & acarreo) ^ (acarreo & M[i]); 
        suma.push_back(temp_sum);
    }  
    return voltear(suma);
}
void Booth::corr_aritm(){
    //desplazamos Q
    Q_1 = Q_0;
    for(int i = num_bits-1; i>0; i--){
        Q[i] = Q[i-1];
    }
    Q[0]=A[num_bits-1];
    Q_0 = Q[num_bits-1];
    //desplazamos A
    for(int i = num_bits-1; i>0; i--){
        A[i] = A[i-1];
    }
}
long long Booth::bin_dec(vector<bool> R){
    bool negativo = false;
    long long prod_D = 0;
    int cont = num_bits*2-2;
    if(R[0]==1) negativo = true;
    if(negativo){
        prod_D -= pow(2,num_bits*2-1);
    }       
    for (int i = 1; i < num_bits*2; i++, cont--) {
        prod_D += R[i]*pow(2,cont);
    }
    return prod_D;
}
void Booth::multiplicacion(){
    int count = num_bits;
    string operacion = "";
    cout<<"\n|----------------------- ALGORITMO DE BOOTH -----------------------|"<<endl;
    cout<<"|      A      |      Q      | Q_1 |      M      |    Operaciones"<<endl;  
    cout<<"------------------------------------------------------------------------"<<endl;  
    cout<<"|  ";print_bits(A); cout<<"  |  "; print_bits(Q); cout<<"  | "<<Q_1; cout<<" |  ";  print_bits(M); cout<<"  |  Variables Iniciales"<<endl;
    cout<<"------------------------------------------------------------------------"<<endl;  
    while(count > 0 ){  
        if(Q_0 == 1 && Q_1 == 0){
            operacion="  | RESTA";
            A = resta_bin(A,M);
            
        } 
        else if (Q_0 == 0 && Q_1 == 1) {
            operacion= "  | SUMA ";
            A = suma_bin(A,M);
        }
        cout<<"|  ";print_bits(A); cout<<"  |  "; print_bits(Q); cout<<"  | "<<Q_1; cout<<" |  ";  print_bits(M); cout<<operacion<<endl;
        corr_aritm();
        cout<<"|  ";print_bits(A); cout<<"  |  "; print_bits(Q); cout<<"  | "<<Q_1; cout<<" |  ";  print_bits(M); cout<<"  | Corrimiento Aritmetico | COUNT:  "<<count<<endl;
        count--;
        cout<<"------------------------------------------------------------------------"<<endl; 
        operacion = "";
    }
    vector<bool> producto(num_bits*2);
    for (int i = 0; i < num_bits; i++) {
        producto[i] = A[i];
        producto[i+num_bits] = Q[i];
    }
    cout<<"\n--> Resultado en binario: ";
    print_bits(producto);
    cout<<"\n--> Resultado en decimal: ";
    cout<<bin_dec(producto);
}

int main(){
    int  m, q;
    cout<<"\nIngresa el multiplicando: ";
    cin>>m;
    cout<<"\nIngresa el multiplicador: ";
    cin>>q;
    Booth b(m,q);
    b.multiplicacion();
    return 0;
}
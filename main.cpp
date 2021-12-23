#include"arvore.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<utility>


using namespace std;
void separar (const string &s, char delimitador, vector<string>& dados)
{
    stringstream ss,dss;
    ss.str(s);
    string item, d;
    dss << delimitador;
    dss >> d;
    while (getline(ss, item, delimitador))
    {
        if(item != d)
            dados.push_back(item);
    }
}

vector<string> separar(const string &s, char delimitador)
{
    vector<string> dados;
    separar(s,delimitador,dados);
    return dados;
}
int main()
{
    ifstream arquivo;
	string nomedoregistro, linhadoarquivo;
	vector<string> aux, data, tipo;
	vector<pair<int, int> > produtos;
	cout<<"Digite o nome do registro desejado adicionando txt"<<endl;
	cin>>nomedoregistro;
	int contadordeversao = 0;
	if (arquivo.is_open())
		cout << "Arquivo já aberto!" << endl;
	else {
		arquivo.open(nomedoregistro);
		if (!arquivo.is_open()) {
			cout << "Erro ao abrir arquivo ou arquivo inexistente!" << endl;
			return 0;
		} else {
			while (!arquivo.eof()) {
				getline(arquivo, linhadoarquivo);
				if (!linhadoarquivo.empty() && (linhadoarquivo.back()) == '\r')// identifica caractere de quebra de linha '\r' adicionado pelo Windows
					linhadoarquivo.erase(remove(linhadoarquivo.begin(), linhadoarquivo.end(), '\r'), linhadoarquivo.end());
				if (linhadoarquivo.length() > 0)
					aux.push_back(linhadoarquivo);
			}
			arquivo.close();
		}
	}
	// construindo definições dos históricos


	for (int i = 0; i < static_cast<int>(aux.size()); i++) {
		vector<string> temp = separar(aux[i], ' ');
		data.push_back(temp[0] + " " + temp[1]);
		produtos.push_back(make_pair(stoi(temp[3]), stoi(temp[4])));
		if (temp[2] == "V") // marca como negativo em caso de venda
			produtos[i].second *= -1;
	}
	aux.erase(aux.begin(),aux.end());

	contadordeversao = static_cast<int>(data.size());
	/// MÉTODO DA CÓPIA COMPLETA --> DESCOMENTE PARA REALIZAR O MÉTODO DA CÓPIA E COMENTE O MÉTODO PERSISTENTE
	/*arvore historico[contadordeversao];
	for (int i = 0; i < contadordeversao; i++) {
		if (i == 0) {
			arvore arvoreaux;
			arvoreaux.atualizar(produtos[i]);
			historico[i] = arvoreaux;
		} else {
			arvore temp = historico[i - 1];
			temp.atualizar(produtos[i]);
			historico[i] = temp;
		}
	}*/
	/// MÉTODO PERSISTENTE --> COMENTE PARA REALIZAR O MÉTODO DA CÓPIA
	arvore historicopers [contadordeversao];
	for (int i = 0; i < contadordeversao; i++) {
		if (i == 0) {
			arvore arvoreaux;
			arvoreaux.atualizar(produtos[i]);
			historicopers[i] = arvoreaux;
		} else {
			arvore arvoreaux;
			arvoreaux.metodopers(produtos[i], historicopers[i - 1]);
			historicopers[i] = arvoreaux;
		}
	}
	/// BUSCA POR VERSÕES E ARQUIVO DE SAÍDA
    string abc;
    cout<<"Digite a data para recuperacao dos registros"<<endl;
    getchar();
    getline(cin,abc);
		//REALIZANDO A BUSCA
		int k = -1;
		for (int j = 0; j < static_cast<int>(data.size()); j++) {
		    //cout<<"abc:"<<abc<<" dta:"<<data[j]<<endl;
			if (abc == data[j]) {

				k = j;
				if (k != -1) {

			historicopers[k].printar();

			break;
		}
			}

		}

	//}
	return 0;
}

// NOIC - Ideia 3
// Exemplo 2
// Complexidade: O(n)
// Por Samyra Almeida

#include <bits/stdc++.h>

using namespace std;

const int maxn = 1e5+10;

int n, dist[maxn], vis[maxn], qtd[maxn], grau[maxn], soma[maxn];
int diametro = -1;
int c1 = -1, c2 = -1;
vector<int> arvore[maxn];

void bfs(int a)
{
	// inicializo os vetores como 0
	memset(vis, 0, sizeof (vis));
	memset(dist, 0, sizeof (dist));

	queue<int> fila;

	fila.push(a); // adiono a raiz na fila

	while(!fila.empty())
	{
		int b = fila.front();

		fila.pop();
		vis[b] = 1; // visito o vertice b

		for(int i = 0 ; i < arvore[b].size() ; i++) // percorro todos os filhos de b
		{
			int c = arvore[b][i]; 
			if(vis[c] == 0) // se eu ainda não visitei c
			{
				dist[c] = dist[b] + 1; // calculo sua distancia ate a raiz
				fila.push(c); // adiono c na fila para ser visitado
			}
		}
	}
}

void achar_centros(int d) // d é igual ao diametro da arvore
{

	// primeiro vamos achar o(s) centros da arvore
	// ultilizaremos um algoritmo muito semelhante a ordenação topologica aqui

	queue<int> fila;

	for(int i = 1 ; i <= n ; i++)
	{
		if(arvore[i].size() == 1) // se i é uma folha
		{
			fila.push(i); // adiciono ele
		}
	}

	// n_centros é o numero de centros que a arvore possui
	// falta é a quantidade de no que ainda não foram visitados
	int n_centros = 0, falta = n;
	
	if(d%2 == 0) n_centros = 1; // se o diametro for par, significa que a arvore so pussui um centro
	else n_centros = 2; // caso contrario possui dois

	while(falta > n_centros) // enquanto existirem vertices que não foram visitados alem do(s) centro(s)
	{
		int v = fila.front(); // visito o vertice v
		fila.pop(); 
	
		falta--; // desconto v de falta

		for(int i  = 0 ; i < arvore[v].size() ; i++) // percorro todos os visinhos de v
		{
			int u = arvore[v][i]; 
			grau[u]--; 

			if(grau[u] == 1) fila.push(u); // se o grau se tornar 1, ou seja se u virar uma folha, adiciono u na fila
		}
	}

	while(!fila.empty()) // o(s) vertice(s) que ainda estão na fila são o(s) centro(s)
	{
		if(c1 == -1)
		{
			c1 = fila.front();
		}else
		{
			c2 = fila.front();
		}
		fila.pop();
	}
}

int dfs(int u){
	vis[u] = 1; // marco  u como visitado

	for(int i = 0 ; i < arvore[u].size() ; i++) //percorro todos os filhos de u
	{	
		int v = arvore[u][i];
		if(!vis[v]) // se v ainda não foi visitado
		{
			dist[v] = dist[u] + 1; // calculo sua distancia ate a raiz
			dfs(v); // visito v

			if(dist[v] == (diametro/2)) // se a distancia de v ate a raiz for metade do diametro
			{
				qtd[v]++; // atualizo qtd[v]
			}
			qtd[u] += qtd[v]; // atualizo qtd[u]
		}
	}
}

int main()
{
	cin >> n;

	// monto a arvore
	for(int i = 1 ; i < n ; i++)
	{
		int u, v;
		cin >> u >> v;

		arvore[u].push_back(v);
		arvore[v].push_back(u);

		grau[u]++; grau[v]++;
	}

	// faço R = 1
	bfs(1);
	
	int maior = -1, v = -1;

	for(int i = 1 ; i <= n ; i++) // procuramos o vertice com a maior distancia do vertice 1
	{
		
		if(dist[i] > maior)  // checamos se a distancia de 1 a i é a maior ate agora
		{                    // se sim
			maior = dist[i]; // atualizamos maior distancia
			v = i;           // atualizamos o vertice
		}
	}

	// enraizamos a arvore em v
	bfs(v);

	int u = -1;
	
	for(int i = 1 ; i <= n ; i++) // procuramos o vertice com a maior distancia de v
	{
		
		if(dist[i] > diametro)  // checamos se a distancia de v a i é a maior ate agora
		{                    // se sim
			diametro = dist[i]; // atualizamos o diametro
		}
	}
	
	achar_centros(diametro); // acho o(s) centro(s) da arvore
	
	int total = 0; // quantidade de diametros

	if(c2 == -1) // se a arvore possui um unico centro
	{
		// inicializo os vetores
		memset(vis, 0, sizeof (vis));
		memset(dist, 0, sizeof (dist));

		dfs(c1); // chamo a dfs para calcular o qtd das sub-ávores de c1
		
		for(int i = 0 ; i < arvore[c1].size() ; i++) // percorro as sub-ávores do centro
		{
			int v = arvore[c1][i]; 
			
			soma[i+1] = soma[i] + qtd[v]; // faço a otimização 
		}

		for(int i = 0 ; i < arvore[c1].size() ; i++) // percorro as sub-ávores do centro
		{
			int v = arvore[c1][i];

			total += qtd[v]*soma[i]; // calculo a quantidade de diametros
		}		
	}else // a arvore possui dois centros
	{
		// inicializo os vetores
		memset(vis, 0, sizeof (vis));
		memset(dist, 0, sizeof (dist));

		dfs(c1); // chamo a dfs para calcular o qtd das sub-ávores de c1

		for(int i = 0 ; i < arvore[c1].size() ; i++) // percorro as sub-ávores de c1
		{
			int v = arvore[c1][i];

			soma[i+1] = soma[i] + qtd[v]; // faço a otimização 
		}
		
		int n1 = 0; // n1 é o numero de diametros considerando somente c1
		
		for(int i = 0 ; i < arvore[c1].size() ; i++) // percorro as sub-ávores do centro
		{
			int v = arvore[c1][i];
			
			n1 += qtd[v]*soma[i]; // calculo a quantidade de diametros em c1
		}

		// inicilizo os vetores como 0
		memset(vis, 0, sizeof (vis));
		memset(dist, 0, sizeof (dist));
		memset(qtd, 0, sizeof (qtd));
		memset(soma, 0, sizeof (soma));

		dfs(c2); // chamo a dfs para calcular o qtd das sub-ávores de c2
		
		for(int i = 0 ; i < arvore[c2].size() ; i++) // percorro as sub-ávores de c2
		{
			int v = arvore[c2][i];
			
			soma[i+1] = soma[i] + qtd[v]; // faço a otimização
		}
	
		int n2 = 0;
	
		for(int i = 0 ; i < arvore[c2].size() ; i++) // percorro as sub-ávores de c2
		{
			int v = arvore[c2][i];

			n2 += qtd[v]*soma[i]; // calculo a quantidade de diametros em c2
		}
		
		total = n1*n2; // calculo a quantidade total de diametros
	}
	
	cout << total << "\n"; // imprimo o numero de diametros
	
	return 0;
}

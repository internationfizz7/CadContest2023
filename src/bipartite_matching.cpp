#include "bipartite_matching.h"

bool GREATER(double A, double B)
{
    return ((A) - (B) > EPSILON);
}

bool LESS(double A, double B)
{
    return ((B) - (A) > EPSILON);
}

bool EQUAL(double A, double B)
{
    return (fabs((A) - (B)) < EPSILON);
}

bool GREATER_EQUAL(double A, double B)
{
    return (GREATER((A),(B)) || EQUAL((A),(B)));
}

bool LESS_EQUAL(double A, double B)
{
    return (LESS((A),(B)) || EQUAL((A),(B)));
}

/// main ///
vector<pair<int, int> > Bipartite_Matching(int n, int m, vector<pair<int, int> > edge, vector<double> edge_cost)
{
    vector<pair<int, int> > result;
    result = MinimumCostPerfectMatching(n, m, edge, edge_cost);

    return result;
}

pair< Graph, vector<double> > CreateRandomGraph()
{
	//random seed
	int x;
	cin >> x;
	srand( x );

	//Please see Graph.h for a description of the interface
	int n = 50;

	Graph G(n);
	vector<double> cost;
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++)
			if(rand()%10 == 0)
			{
				G.AddEdge(i, j);
				cost.push_back(rand()%1000);
			}

	return make_pair(G, cost);
}

Graph ReadGraph(string filename)
{
	//Please see Graph.h for a description of the interface

	ifstream file;
	file.open(filename.c_str());

	string s;
	getline(file, s);
	stringstream ss(s);
	int n;
	ss >> n;
	getline(file, s);
	ss.str(s);
	ss.clear();
	int m;
	ss >> m;

	Graph G(n);
	for(int i = 0; i < m; i++)
	{
		getline(file, s);
		ss.str(s);
		ss.clear();
		int u, v;
		ss >> u >> v;

		G.AddEdge(u, v);
	}

	file.close();
	return G;
}

pair< Graph, vector<double> > ReadWeightedGraph(int n, int m, vector<pair<int, int> > edge, vector<double> edge_cost)
{
	//Please see Graph.h for a description of the interface

	/*ifstream file;
	file.open(filename.c_str());

	string s;
	getline(file, s);
	stringstream ss(s);
	int n;
	ss >> n;
	getline(file, s);
	ss.str(s);
	ss.clear();
	int m;
	ss >> m;

	Graph G(n);
	vector<double> cost(m);
	for(int i = 0; i < m; i++)
	{
		getline(file, s);
		ss.str(s);
		ss.clear();
		int u, v;
		double c;
		ss >> u >> v >> c;

		G.AddEdge(u, v);
		cost[G.GetEdgeIndex(u, v)] = c;
	}

	file.close();*/

	Graph G(n);
	for(int i = 0; i < m; i++)
    {
        G.AddEdge(edge[i].first, edge[i].second);
    }
	return make_pair(G, edge_cost);
}

vector<pair<int, int> > MinimumCostPerfectMatching(int n, int m, vector<pair<int, int> > edge, vector<double> edge_cost)
{
	Graph G;
	vector<double> cost;

	//Read the graph
	pair< Graph, vector<double> > p = ReadWeightedGraph(n, m, edge, edge_cost);
	//pair< Graph, vector<double> > p = CreateRandomGraph();
	G = p.first;
	cost = p.second;

	//Create a Matching instance passing the graph
	Matching M(G);

	//Pass the costs to solve the problem
	pair< list<int>, double > solution = M.SolveMinimumCostPerfectMatching(cost);

	list<int> matching = solution.first;
	double obj = solution.second;
	vector<pair<int, int> > result;

	cout << "Optimal matching cost: " << obj << endl;
	cout << "Edges in the matching:" << endl;
	for(list<int>::iterator it = matching.begin(); it != matching.end(); it++)
	{
		pair<int, int> e = G.GetEdge( *it );
		result.push_back(e);

		cout << e.first << " " << e.second << endl;
	}

	return result;
}

void MaximumMatchingExample(string filename)
{
	Graph G = ReadGraph(filename);
	Matching M(G);

	list<int> matching;
	matching = M.SolveMaximumMatching();

	cout << "Number of edges in the maximum matching: " << matching.size() << endl;
	cout << "Edges in the matching:" << endl;
	for(list<int>::iterator it = matching.begin(); it != matching.end(); it++)
	{
		pair<int, int> e = G.GetEdge( *it );

		cout << e.first << " " << e.second << endl;
	}
}

/// ======================================================================///

/// Graph.cpp ///
Graph::Graph(int n, const list< pair<int, int> > & edges):
	n(n),
	m(edges.size()),
	adjMat(n, vector<bool>(n, false)),
	adjList(n),
	edges(),
	edgeIndex(n, vector<int>(n, -1))
{
	for(list< pair<int, int> >::const_iterator it = edges.begin(); it != edges.end(); it++)
	{
		int u = (*it).first;
		int v = (*it).second;

		AddEdge(u, v);
	}
}

pair<int, int> Graph::GetEdge(int e) const
{
	if(e > (int)edges.size())
		throw "Error: edge does not exist";

	return edges[e];
}

int Graph::GetEdgeIndex(int u, int v) const
{
	if( u > n or
		v > n )
		throw "Error: vertex does not exist";

	if(edgeIndex[u][v] == -1)
		throw "Error: edge does not exist";

	return edgeIndex[u][v];
}

void Graph::AddVertex()
{
	for(int i = 0; i < n; i++)
	{
		adjMat[i].push_back(false);
		edgeIndex[i].push_back(-1);
	}
	n++;
	adjMat.push_back( vector<bool>(n, false) );
	edgeIndex.push_back( vector<int>(n, -1) );
	adjList.push_back( list<int>() );
}

void Graph::AddEdge(int u, int v)
{
	if( u > n or
		v > n )
		throw "Error: vertex does not exist";

	if(adjMat[u][v]) return;

	adjMat[u][v] = adjMat[v][u] = true;
	adjList[u].push_back(v);
	adjList[v].push_back(u);

	edges.push_back(pair<int, int>(u, v));
	edgeIndex[u][v] = edgeIndex[v][u] = m++;
}

const list<int> & Graph::AdjList(int v) const
{
	if(v > n)
		throw "Error: vertex does not exist";

	return adjList[v];
}

const vector< vector<bool> > & Graph::AdjMat() const
{
	return adjMat;
}
/// ======================================================================///

/// BinaryHeap.cpp ///
void BinaryHeap::Clear()
{
	key.clear();
	pos.clear();
	satellite.clear();
}

void BinaryHeap::Insert(double k, int s)
{
	//Ajust the structures to fit new data
	if(s >= (int)pos.size())
	{
		pos.resize(s+1, -1);
		key.resize(s+1);
		//Recall that position 0 of satellite is unused
		satellite.resize(s+2);
	}
	//If satellite is already in the heap
	else if(pos[s] != -1)
	{
		throw "Error: satellite already in heap";
	}

	int i;
	for(i = ++size; i/2 > 0 && GREATER(key[satellite[i/2]], k); i /= 2)
	{
		satellite[i] = satellite[i/2];
		pos[satellite[i]] = i;
	}
	satellite[i] = s;
	pos[s] = i;
	key[s] = k;
}

int BinaryHeap::Size()
{
	return size;
}

int BinaryHeap::DeleteMin()
{
	if(size == 0)
		throw "Error: empty heap";

	int min = satellite[1];
	int slast = satellite[size--];


	int child;
	int i;
	for(i = 1, child = 2; child  <= size; i = child, child *= 2)
	{
		if(child < size && GREATER(key[satellite[child]], key[satellite[child+1]]))
			child++;

		if(GREATER(key[slast], key[satellite[child]]))
		{
			satellite[i] = satellite[child];
			pos[satellite[child]] = i;
		}
		else
			break;
	}
	satellite[i] = slast;
	pos[slast] = i;

	pos[min] = -1;

	return min;
}

void BinaryHeap::ChangeKey(double k, int s)
{
	Remove(s);
	Insert(k, s);
}

void BinaryHeap::Remove(int s)
{
	int i;
	for(i = pos[s]; i/2 > 0; i /= 2)
	{
		satellite[i] = satellite[i/2];
		pos[satellite[i]] = i;
	}
	satellite[1] = s;
	pos[s] = 1;

	DeleteMin();
}
/// ======================================================================///

/// Matching.cpp ///
Matching::Matching(const Graph & G):
	G(G),
	outer(2*G.GetNumVertices()),
	deep(2*G.GetNumVertices()),
	shallow(2*G.GetNumVertices()),
	tip(2*G.GetNumVertices()),
	active(2*G.GetNumVertices()),
	type(2*G.GetNumVertices()),
	forest(2*G.GetNumVertices()),
	root(2*G.GetNumVertices()),
	blocked(2*G.GetNumVertices()),
	dual(2*G.GetNumVertices()),
	slack(G.GetNumEdges()),
	mate(2*G.GetNumVertices()),
	m(G.GetNumEdges()),
	n(G.GetNumVertices()),
	visited(2*G.GetNumVertices())
{
}

void Matching::Grow()
{
	Reset();

	//All unmatched vertices will be roots in a forest that will be grown
	//The forest is grown by extending a unmatched vertex w through a matched edge u-v in a BFS fashion
	while(!forestList.empty())
	{
		int w = outer[forestList.front()];
		forestList.pop_front();

		//w might be a blossom
		//we have to explore all the connections from vertices inside the blossom to other vertices
		for(list<int>::iterator it = deep[w].begin(); it != deep[w].end(); it++)
		{
			int u = *it;

			int cont = false;
			for(list<int>::const_iterator jt = G.AdjList(u).begin(); jt != G.AdjList(u).end(); jt++)
			{
				int v = *jt;

				if(IsEdgeBlocked(u, v)) continue;

				//u is even and v is odd
				if(type[outer[v]] == ODD) continue;

				//if v is unlabeled
				if(type[outer[v]] != EVEN)
				{
					//We grow the alternating forest
					int vm = mate[outer[v]];

					forest[outer[v]] = u;
					type[outer[v]] = ODD;
					root[outer[v]] = root[outer[u]];
					forest[outer[vm]] = v;
					type[outer[vm]] = EVEN;
					root[outer[vm]] = root[outer[u]];

					if(!visited[outer[vm]])
					{
						forestList.push_back(vm);
						visited[outer[vm]] = true;
					}
				}
				//If v is even and u and v are on different trees
				//we found an augmenting path
				else if(root[outer[v]] != root[outer[u]])
				{
					Augment(u,v);
					Reset();

					cont = true;
					break;
				}
				//If u and v are even and on the same tree
				//we found a blossom
				else if(outer[u] != outer[v])
				{
					int b = Blossom(u,v);

					forestList.push_front(b);
					visited[b] = true;

					cont = true;
					break;
				}
			}
			if(cont) break;
		}
	}

	//Check whether the matching is perfect
	perfect = true;
	for(int i = 0; i < n; i++)
		if(mate[outer[i]] == -1)
			perfect = false;
}

bool Matching::IsAdjacent(int u, int v)
{
	return (G.AdjMat()[u][v] and not IsEdgeBlocked(u, v));
}

bool Matching::IsEdgeBlocked(int u, int v)
{
	return GREATER(slack[ G.GetEdgeIndex(u, v) ], 0);
}

bool Matching::IsEdgeBlocked(int e)
{
	return GREATER(slack[e], 0);
}

//Vertices will be selected in non-decreasing order of their degree
//Each time an unmatched vertex is selected, it is matched to its adjacent unmatched vertex of minimum degree
void Matching::Heuristic()
{
	vector<int> degree(n, 0);
	BinaryHeap B;

	for(int i = 0; i < m; i++)
	{
		if(IsEdgeBlocked(i)) continue;

		pair<int, int> p = G.GetEdge(i);
		int u = p.first;
		int v = p.second;

		degree[u]++;
		degree[v]++;
	}

	for(int i = 0; i < n; i++)
		B.Insert(degree[i], i);

	while(B.Size() > 0)
	{
		int u = B.DeleteMin();
		if(mate[outer[u]] == -1)
		{
			int min = -1;
			for(list<int>::const_iterator it = G.AdjList(u).begin(); it != G.AdjList(u).end(); it++)
			{
				int v = *it;

				if(IsEdgeBlocked(u, v) or
					(outer[u] == outer[v]) or
					(mate[outer[v]] != -1) )
					continue;

				if(min == -1 or degree[v] < degree[min])
					min = v;
			}
			if(min != -1)
			{
				mate[outer[u]] = min;
				mate[outer[min]] = u;
			}
		}
	}
}

//Destroys a blossom recursively
void Matching::DestroyBlossom(int t)
{
	if((t < n) or
		(blocked[t] and GREATER(dual[t], 0))) return;

	for(list<int>::iterator it = shallow[t].begin(); it != shallow[t].end(); it++)
	{
		int s = *it;
		outer[s] = s;
		for(list<int>::iterator jt = deep[s].begin(); jt != deep[s].end(); jt++)
			outer[*jt] = s;

		DestroyBlossom(s);
	}

	active[t] = false;
	blocked[t] = false;
	AddFreeBlossomIndex(t);
	mate[t] = -1;
}

void Matching::Expand(int u, bool expandBlocked = false)
{
	int v = outer[mate[u]];

	int index = m;
	int p = -1, q = -1;
	//Find the regular edge {p,q} of minimum index connecting u and its mate
	//We use the minimum index to grant that the two possible blossoms u and v will use the same edge for a mate
	for(list<int>::iterator it = deep[u].begin(); it != deep[u].end(); it++)
	{
		int di = *it;
		for(list<int>::iterator jt = deep[v].begin(); jt != deep[v].end(); jt++)
		{
			int dj = *jt;
			if(IsAdjacent(di, dj) and G.GetEdgeIndex(di, dj) < index)
			{
				index = G.GetEdgeIndex(di, dj);
				p = di;
				q = dj;
			}
		}
	}

	mate[u] = q;
    mate[v] = p;
	//If u is a regular vertex, we are done
	if(u < n or (blocked[u] and not expandBlocked)) return;

	bool found = false;
	//Find the position t of the new tip of the blossom
	for(list<int>::iterator it = shallow[u].begin(); it != shallow[u].end() and not found; )
	{
		int si = *it;
		for(list<int>::iterator jt = deep[si].begin(); jt != deep[si].end() and not found; jt++)
		{
			if(*jt == p )
				found = true;
		}
		it++;
		if(not found)
		{
			shallow[u].push_back(si);
			shallow[u].pop_front();
		}
	}

	list<int>::iterator it = shallow[u].begin();
	//Adjust the mate of the tip
	mate[*it] = mate[u];
	it++;
	//
	//Now we go through the odd circuit adjusting the new mates
	while(it != shallow[u].end())
	{
		list<int>::iterator itnext = it;
		itnext++;
		mate[*it] = *itnext;
		mate[*itnext] = *it;
		itnext++;
		it = itnext;
	}

	//We update the sets blossom, shallow, and outer since this blossom is being deactivated
	for(list<int>::iterator it = shallow[u].begin(); it != shallow[u].end(); it++)
	{
		int s = *it;
		outer[s] = s;
		for(list<int>::iterator jt = deep[s].begin(); jt != deep[s].end(); jt++)
			outer[*jt] = s;
	}
	active[u] = false;
	AddFreeBlossomIndex(u);

	//Expand the vertices in the blossom
	for(list<int>::iterator it = shallow[u].begin(); it != shallow[u].end(); it++)
		Expand(*it, expandBlocked);

}

//Augment the path root[u], ..., u, v, ..., root[v]
void Matching::Augment(int u, int v)
{
	//We go from u and v to its respective roots, alternating the matching
	int p = outer[u];
	int q = outer[v];
    int outv = q;
	int fp = forest[p];
	mate[p] = q;
	mate[q] = p;
	Expand(p);
	Expand(q);
	while(fp != -1)
	{
		q = outer[forest[p]];
		p = outer[forest[q]];
		fp = forest[p];

		mate[p] = q;
		mate[q] = p;
		Expand(p);
		Expand(q);
	}

	p = outv;
	fp = forest[p];
	while(fp != -1)
	{
		q = outer[forest[p]];
		p = outer[forest[q]];
		fp = forest[p];

		mate[p] = q;
		mate[q] = p;
		Expand(p);
		Expand(q);
	}
}

void Matching::Reset()
{
	for(int i = 0; i < 2*n; i++)
	{
		forest[i] = -1;
		root[i] = i;

		if(i >= n and active[i] and outer[i] == i)
			DestroyBlossom(i);
	}

	visited.assign(2*n, 0);
	forestList.clear();
	for(int i = 0; i < n; i++)
	{
		if(mate[outer[i]] == -1)
		{
			type[outer[i]] = 2;
			if(!visited[outer[i]])
				forestList.push_back(i);
			visited[outer[i]] = true;
		}
		else type[outer[i]] = 0;
	}
}

int Matching::GetFreeBlossomIndex()
{
	int i = free.back();
	free.pop_back();
	return i;
}

void Matching::AddFreeBlossomIndex(int i)
{
	free.push_back(i);
}

void Matching::ClearBlossomIndices()
{
	free.clear();
	for(int i = n; i < 2*n; i++)
		AddFreeBlossomIndex(i);
}

//Contracts the blossom w, ..., u, v, ..., w, where w is the first vertex that appears in the paths from u and v to their respective roots
int Matching::Blossom(int u, int v)
{
	int t = GetFreeBlossomIndex();

	vector<bool> isInPath(2*n, false);

	//Find the tip of the blossom
	int u_ = u;
	while(u_ != -1)
	{
		isInPath[outer[u_]] = true;

		u_ = forest[outer[u_]];
	}

	int v_ = outer[v];
	while(not isInPath[v_])
		v_ = outer[forest[v_]];
	tip[t] = v_;

	//Find the odd circuit, update shallow, outer, blossom and deep
	//First we construct the set shallow (the odd circuit)
	list<int> circuit;
	u_ = outer[u];
	circuit.push_front(u_);
	while(u_ != tip[t])
	{
		u_ = outer[forest[u_]];
		circuit.push_front(u_);
	}

	shallow[t].clear();
	deep[t].clear();
	for(list<int>::iterator it = circuit.begin(); it != circuit.end(); it++)
	{
		shallow[t].push_back(*it);
	}

	v_ = outer[v];
	while(v_ != tip[t])
	{
		shallow[t].push_back(v_);
		v_ = outer[forest[v_]];
	}

	//Now we construct deep and update outer
	for(list<int>::iterator it = shallow[t].begin(); it != shallow[t].end(); it++)
	{
		u_ = *it;
		outer[u_] = t;
		for(list<int>::iterator jt = deep[u_].begin(); jt != deep[u_].end(); jt++)
		{
			deep[t].push_back(*jt);
			outer[*jt] = t;
		}
	}

	forest[t] = forest[tip[t]];
	type[t] = EVEN;
	root[t] = root[tip[t]];
	active[t] = true;
	outer[t] = t;
	mate[t] = mate[tip[t]];

	return t;
}

void Matching::UpdateDualCosts()
{
	double e1 = 0, e2 = 0, e3 = 0;
	int inite1 = false, inite2 = false, inite3 = false;
	for(int i = 0; i < m; i++)
	{
		int u = G.GetEdge(i).first,
			v = G.GetEdge(i).second;

		if( (type[outer[u]] == EVEN and type[outer[v]] == UNLABELED) or (type[outer[v]] == EVEN and type[outer[u]] == UNLABELED) )
		{
			if(!inite1 or GREATER(e1, slack[i]))
			{
				e1 = slack[i];
				inite1 = true;
			}
		}
		else if( (outer[u] != outer[v]) and type[outer[u]] == EVEN and type[outer[v]] == EVEN )
		{
			if(!inite2 or GREATER(e2, slack[i]))
			{
				e2 = slack[i];
				inite2 = true;
			}
		}
	}
	for(int i = n; i < 2*n; i++)
	{
		if(active[i] and i == outer[i] and type[outer[i]] == ODD and (!inite3 or GREATER(e3, dual[i])))
		{
			e3 = dual[i];
			inite3 = true;
		}
	}
	double e = 0;
	if(inite1) e = e1;
	else if(inite2) e = e2;
	else if(inite3) e = e3;

	if(GREATER(e, e2/2.0) and inite2)
		e = e2/2.0;
	if(GREATER(e, e3) and inite3)
		e = e3;

	for(int i = 0; i < 2*n; i++)
	{
		if(i != outer[i]) continue;

		if(active[i] and type[outer[i]] == EVEN)
		{
			dual[i] += e;
		}
		else if(active[i] and type[outer[i]] == ODD)
		{
			dual[i] -= e;
		}
	}

	for(int i = 0; i < m; i++)
	{
		int u = G.GetEdge(i).first,
			v = G.GetEdge(i).second;

		if(outer[u] != outer[v])
		{
			if(type[outer[u]] == EVEN and type[outer[v]] == EVEN)
				slack[i] -= 2.0*e;
			else if(type[outer[u]] == ODD and type[outer[v]] == ODD)
				slack[i] += 2.0*e;
			else if( (type[outer[v]] == UNLABELED and type[outer[u]] == EVEN) or (type[outer[u]] == UNLABELED and type[outer[v]] == EVEN) )
				slack[i] -= e;
			else if( (type[outer[v]] == UNLABELED and type[outer[u]] == ODD) or (type[outer[u]] == UNLABELED and type[outer[v]] == ODD) )
				slack[i] += e;
		}
	}
	for(int i = n; i < 2*n; i++)
	{
		if(GREATER(dual[i], 0))
		{
			blocked[i] = true;
		}
		else if(active[i] and blocked[i])
		{
			//The blossom is becoming unblocked
			if(mate[i] == -1)
			{
				DestroyBlossom(i);
			}
			else
			{
				blocked[i] = false;
				Expand(i);
			}
		}
	}
}

pair< list<int>, double> Matching::SolveMinimumCostPerfectMatching(const vector<double> & cost)
{
	SolveMaximumMatching();
	if(!perfect)
		throw "Error: The graph does not have a perfect matching";

	Clear();

	//Initialize slacks (reduced costs for the edges)
	slack = cost;

	PositiveCosts();

	//If the matching on the compressed graph is perfect, we are done
	perfect = false;
	while(not perfect)
	{
		//Run an heuristic maximum matching algorithm
		Heuristic();
		//Grow a hungarian forest
		Grow();
		UpdateDualCosts();
		//Set up the algorithm for a new grow step
		Reset();
	}

	list<int> matching = RetrieveMatching();

	double obj = 0;
	for(list<int>::iterator it = matching.begin(); it != matching.end(); it++)
		obj += cost[*it];

	double dualObj = 0;
	for(int i = 0; i < 2*n; i++)
	{
		if(i < n) dualObj += dual[i];
		else if(blocked[i]) dualObj += dual[i];
	}

	return pair< list<int>, double >(matching, obj);
}

void Matching::PositiveCosts()
{
	double minEdge = 0;
	for(int i = 0; i < m ;i++)
		if(GREATER(minEdge - slack[i], 0))
			minEdge = slack[i];

	for(int i = 0; i < m; i++)
		slack[i] -= minEdge;
}

list<int> Matching::SolveMaximumMatching()
{
	Clear();
	Grow();
	return RetrieveMatching();
}

//Sets up the algorithm for a new run
void Matching::Clear()
{
	ClearBlossomIndices();

	for(int i = 0; i < 2*n; i++)
	{
		outer[i] = i;
		deep[i].clear();
		if(i<n)
			deep[i].push_back(i);
		shallow[i].clear();
		if(i < n) active[i] = true;
		else active[i] = false;

		type[i] = 0;
		forest[i] = -1;
		root[i] = i;

		blocked[i] = false;
		dual[i] = 0;
		mate[i] = -1;
		tip[i] = i;
	}
	slack.assign(m, 0);
}

list<int> Matching::RetrieveMatching()
{
	list<int> matching;

	for(int i = 0; i < 2*n; i++)
		if(active[i] and mate[i] != -1 and outer[i] == i)
			Expand(i, true);

	for(int i = 0; i < m; i++)
	{
		int u = G.GetEdge(i).first;
		int v = G.GetEdge(i).second;

		if(mate[u] == v)
			matching.push_back(i);
	}
	return matching;
}
/// ======================================================================///
/// ========================= above from github source code to solving bipartite matching ========================= ///

/// ========================= below using LEMON NetworkFlow to solving bipartite matching ========================= ///

void LEMON_SolveNetworkFlow(Legalization_INFO &LG_INFO)
{
    cout << "\n========   Now Run LEMON BIPARTITE MATCHING STAGE\t========"<< endl;
    // for example :
    // ss = 4
	// ts = 4
	// The graph picture will be:
    //
    //  source side        target side
    //        -- s1 ----- t1 --
    //       /                 \
    //      /--- s2 ----- t2 ---\
    //   src                     snk
    //      \--- s3 ----- t3 ---/
    //       \                 /
    //        -- s4 ----- t4 --
    //                |
    //                |
    //          you created arcs

//	// first create graph g to save data, ex nodes,arcs
//	lemon::SmartDigraph g;
//	// create map of cost,capacity,supply
//	lemon::SmartDigraph::ArcMap<int> cost(g);
//	lemon::SmartDigraph::ArcMap<int> lcap(g);
//	lemon::SmartDigraph::ArcMap<int> ucap(g);
//	lemon::SmartDigraph::NodeMap<int> supl(g);

	// set you want to create the count of nodes
    //int ss = this_region-> big_STDGroupID.size();
    //int ts = EmptyBins.size();
    //cout << "[INFO] Number of cell clusters: "<< ss << endl;
    //cout << "[INFO] Number of bins: "<< ts << endl;
    int ss = 0;
    int ts = 0;
    vector<ORDER_REGION> &region = LG_INFO.region;
    vector<Macro*> &MacroClusterSet = LG_INFO.MacroClusterSet;

    for(int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION &region_tmp = region[i];

        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];

                //Boundary enclose_all_macros;  // the bounding box can enclose all the same type macros in this region
                //Boundary_Assign_Limits(enclose_all_macros);
                // record before exchange lg
                for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int macro_id = type_tmp.macro_id[l];
                    Macro* macro_temp = MacroClusterSet[macro_id];
                    macro_temp->lg_no_bi_matching = macro_temp->lg;

                    //Cal_BBOX(macro_temp->set->bbx, enclose_all_macros);
                }

                // first create graph g to save data, ex nodes,arcs
                lemon::SmartDigraph g;
                // create map of cost,capacity,supply
                lemon::SmartDigraph::ArcMap<int> cost(g);
                lemon::SmartDigraph::ArcMap<int> lcap(g);
                lemon::SmartDigraph::ArcMap<int> ucap(g);
                lemon::SmartDigraph::NodeMap<int> supl(g);

                // set you want to create the count of nodes
                ss = type_tmp.macro_id.size();
                ts = type_tmp.macro_id.size();

                // create node & arc container
                // s : source side nodes
                // t : target side nodes
                lemon::SmartDigraph::Node src,snk;
                vector<lemon::SmartDigraph::Node> s,t;
                vector<lemon::SmartDigraph::Arc> arcs;

                // then start action to create nodes & arcs
                LEMON_Create_Node_Arc(g,src,snk,s,t,arcs,ss,ts);

                // set capacity & cost & supply
                LEMON_Set_Cap_Cost(src,snk,s,t,arcs,cost,lcap,ucap,supl, type_tmp.macro_id, MacroClusterSet);

                // use network simplex container ns
                // first let cost,cap,supply into ns, then run Check_NS
                lemon::NetworkSimplex<lemon::SmartDigraph, int> ns(g);
                ns.costMap(cost).lowerMap(lcap).upperMap(ucap).supplyMap(supl);
                LEMON_Check_NS(g, cost, ns, arcs, s.size(), t.size(), type_tmp.macro_id, MacroClusterSet);  // get the result

                //LEMON_Writer(g,ns,cost,lcap,ucap,supl,src,snk); // [DEBUG] can used this function // output the data of all graph,nodes,arcs
            }
        }
    }
}

void LEMON_Create_Node_Arc(lemon::SmartDigraph &g,
                     lemon::SmartDigraph::Node &src,
					 lemon::SmartDigraph::Node &snk,
                     vector<lemon::SmartDigraph::Node> &s,
                     vector<lemon::SmartDigraph::Node> &t,
                     vector<lemon::SmartDigraph::Arc> &arcs,
					 int ss, int ts)
{
	// first push src & snk into g
	src = g.addNode();
	snk = g.addNode();
    // create nodes of source side & target side
    // note : whether it is source side or target side;
    //        the first item must be "source node"(src) or "sink node"(snk).
    for(unsigned int i=0 ; i<ss ; ++i)
		s.push_back(g.addNode());
	for(unsigned int i=0 ; i<ts ; ++i)
		t.push_back(g.addNode());

    // create arcs between nodes;
    // we first set every nodes of source side link to every nodes of target side
	for(unsigned int i=0 ; i<s.size() ; ++i)
	{
		for(unsigned int j=0 ; j<t.size() ; ++j)
			arcs.push_back(g.addArc(s[i],t[j]));
	}
	// then create arcs to link src or snk
	for(int i=0 ; i<ss ; ++i)
		arcs.push_back(g.addArc(src,s[i]));
	for(int i=0 ; i<ts ; ++i)
		arcs.push_back(g.addArc(t[i],snk));
	// so the arcs have two part : "between s side & t side" + "link to src or snk"

}

void LEMON_Set_Cap_Cost(lemon::SmartDigraph::Node &src,
                        lemon::SmartDigraph::Node &snk,
                        vector<lemon::SmartDigraph::Node> &s,
                        vector<lemon::SmartDigraph::Node> &t,
                        vector<lemon::SmartDigraph::Arc> &arcs,
                        lemon::SmartDigraph::ArcMap<int> &cost,
                        lemon::SmartDigraph::ArcMap<int> &lcap,
                        lemon::SmartDigraph::ArcMap<int> &ucap,
                        lemon::SmartDigraph::NodeMap<int> &supl,
                        vector<int> &macro_id,
                        vector<Macro*> &MacroClusterSet)
{

    for(unsigned int i=0 ; i<arcs.size() ; ++i)
    {
		lcap[arcs[i]] = 0;
		ucap[arcs[i]] = 1;
        if(i < ( s.size() * t.size() ) )
            cost[arcs[i]] = -1;
        else
            cost[arcs[i]] = 0;
    }

    for(unsigned int i=0 ; i<s.size() ; ++i)
        supl[s[i]] = 0;
    for(unsigned int i=0 ; i<t.size() ; ++i)
        supl[t[i]] = 0;

    supl[src] = s.size();
    supl[snk] = 0 - t.size();

	// set you want to change the cost of the arcs
	// you can change,add or delete the cost to look how the result is

    for(unsigned int i=0 ; i<s.size() ; i++)
    {
        int macro_s_id = macro_id[i];
        Macro *macro_s = MacroClusterSet[macro_s_id];

        // calculate macro_s net degree
        int net_degree = 0;
        set<string> connected;
        for(unsigned int j = 0; j < macro_s->NetID.size(); j++)
        {
            int net_index = macro_s->NetID[j];

            for(int m = 0; m < net_list[net_index].macro_idSet.size(); m++)
            {
                int macro_index = net_list[net_index].macro_idSet[m];
                Macro *tmp = macro_list[macro_index];
                connected.insert(tmp->macro_name);
            }
            for(int m = 0; m < net_list[net_index].cell_idSet.size(); m++)
            {
                int cell_index = net_list[net_index].cell_idSet[m];
                Macro *tmp = macro_list[cell_index];
                connected.insert(tmp->macro_name);
            }
        }
        net_degree = connected.size();

        for(unsigned int j=0 ; j<t.size() ; j++)
        {
            int macro_t_id = macro_id[j];
            Macro *macro_t = MacroClusterSet[macro_t_id];
            MacrosSet *set_t = macro_t->set;

            // displacement cost
            int deltaX = fabs(macro_s->gp.llx / PARA - macro_t->lg.llx / PARA);
            int deltaY = fabs(macro_s->gp.lly / PARA - macro_t->lg.lly / PARA);
            int distance = sqrt(deltaX * deltaX + deltaY * deltaY);

            // net degree cost
            int n_distance = 0;
            int net_degree_cost = 0;
            if(set_t->aspect_ratio_decrease == true && set_t->aspect_ratio_increase == false)   // long->x
            {
                if(set_t->occupy_ratio[2] > set_t->occupy_ratio[3]) // left->urx
                {
                    n_distance = fabs(set_t->bbx.urx - macro_t->lg_no_bi_matching.llx);
                }
                else if(set_t->occupy_ratio[2] <= set_t->occupy_ratio[3])  // right->llx
                {
                    n_distance = fabs(set_t->bbx.llx - macro_t->lg_no_bi_matching.llx);
                }
            }
            else if(set_t->aspect_ratio_decrease == false && set_t->aspect_ratio_increase == true)   // wide->y
            {
                if(set_t->occupy_ratio[0] > set_t->occupy_ratio[1]) // up->lly
                {
                    n_distance = fabs(set_t->bbx.lly - macro_t->lg_no_bi_matching.lly);
                }
                else if(set_t->occupy_ratio[0] <= set_t->occupy_ratio[1])  // down->ury
                {
                    n_distance = fabs(set_t->bbx.ury - macro_t->lg_no_bi_matching.lly);
                }
            }
            else if(set_t->aspect_ratio_decrease == true && set_t->aspect_ratio_increase == true)   // Am/Ab ~= 1
            {
                if(set_t->bbx.urx - set_t->bbx.llx <= set_t->bbx.ury - set_t->bbx.lly)   // long->x
                {
                    if(set_t->occupy_ratio[2] > set_t->occupy_ratio[3]) // left->urx
                    {
                        n_distance = fabs(set_t->bbx.urx - macro_t->lg_no_bi_matching.llx);
                    }
                    else if(set_t->occupy_ratio[2] <= set_t->occupy_ratio[3])  // right->llx
                    {
                        n_distance = fabs(set_t->bbx.llx - macro_t->lg_no_bi_matching.llx);
                    }
                }
                else if(set_t->bbx.urx - set_t->bbx.llx > set_t->bbx.ury - set_t->bbx.lly)  // wide->y
                {
                    if(set_t->occupy_ratio[0] > set_t->occupy_ratio[1]) // up->lly
                    {
                        n_distance = fabs(set_t->bbx.lly - macro_t->lg_no_bi_matching.lly);
                    }
                    else if(set_t->occupy_ratio[0] <= set_t->occupy_ratio[1])  // down->ury
                    {
                        n_distance = fabs(set_t->bbx.ury - macro_t->lg_no_bi_matching.lly);
                    }
                }
            }
            net_degree_cost = net_degree * (n_distance / PARA);

            cost[arcs[t.size() * i + j]] = distance + net_degree_cost;
//            int groupid = this_region-> big_STDGroupID[i];
//            int deltaX = fabs(STDGroupBy2[groupid]-> GCenter_X/PARA - EmptyBins[j]-> centX/PARA);
//            int deltaY = fabs(STDGroupBy2[groupid]-> GCenter_Y/PARA - EmptyBins[j]-> centY/PARA);
//            int distance = sqrt(deltaX * deltaX + deltaY * deltaY);
//
//            //cout << "STDG cent: " << STDGroupBy2[groupid]-> GCenter_X/PARA << "," << STDGroupBy2[groupid]-> GCenter_Y/PARA << endl;
//            //cout << "Bin cent: " << EmptyBins[j]-> centX/PARA << "," << EmptyBins[j]-> centY/PARA << endl;
//            //cout << " i:" << i << " j:" << j << " "<< distance  << endl;
//            cost[arcs[ t.size() * i + j ]] = distance * distance;
        }
    }
}

void LEMON_Check_NS(lemon::SmartDigraph &g,
                    lemon::SmartDigraph::ArcMap<int> &cost,
                    lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
                    vector<lemon::SmartDigraph::Arc> &arcs,
                    int ss, int ts,
                    vector<int> &macro_id,
                    vector<Macro*> &MacroClusterSet)
{
	//cout << "========== Reseult ==========" << endl;

	// set found to check this case can be feasible
    lemon::NetworkSimplex<lemon::SmartDigraph,int>::ProblemType found = ns.run();

	// if found is 1, it is feasible
	// then show the total cost and which arcs between src side & tar side is selected;
    switch(found)
    {
        case 0:
            cout << "[ERROR] This case is INFEASIBLE" << endl;
            cout << "[ERROR] Can't solve" << endl;
            break;
        case 1:
            //cout << "[INFO] This case is OPTIMAL." << endl;
            //cout << "[INFO] Total Cost : " << ns.totalCost() << endl;

            // update macro lg
            for(int i=0 ; i< ss * ts ; ++i)
            {
                if(ns.flow(arcs[i]))
				{
				    int s_id = g.id(g.source(arcs[i])) -1 -1;
				    int macro_s_id = macro_id[s_id];
				    Macro *macro_s = MacroClusterSet[macro_s_id];

				    int t_id = g.id(g.target(arcs[i])) -ss -1 -1;
				    int macro_t_id = macro_id[t_id];
				    Macro *macro_t = MacroClusterSet[macro_t_id];

				    macro_s->lg = macro_t->lg_no_bi_matching;
//				    int group_id = g.id(g.source(arcs[i])) -1 -1;
//				    int clusterID = this_region-> big_STDGroupID[group_id];
//				    int binID = g.id(g.target(arcs[i])) -ss -1 -1;
//				    STDGroupBy2[clusterID]-> packingX = EmptyBins[binID]-> centX;
//                    STDGroupBy2[clusterID]-> packingY = EmptyBins[binID]-> centY;
//                    EmptyBins[binID]-> packing_std.push_back(clusterID);
				}
            }

            break;
        case 2:
            cout << "[ERROR] This case is UNBOUNDED" << endl;
            cout << "[ERROR] There is a directed cycle having negative total cost and infinite upper bound." << endl;
            break;
    }
}

void LEMON_Writer(lemon::SmartDigraph &g,
            lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
            lemon::SmartDigraph::ArcMap<int> &cost,
            lemon::SmartDigraph::ArcMap<int> &lcap,
            lemon::SmartDigraph::ArcMap<int> &ucap,
            lemon::SmartDigraph::NodeMap<int> &supl,
			lemon::SmartDigraph::Node src,
			lemon::SmartDigraph::Node snk)
{
	cout << "========== Graph ==========" << endl;

	lemon::SmartDigraph::ArcMap<int> flow(g);
	ns.flowMap(flow);

	digraphWriter(g).                  // write g to the standard output
			node("source", src).       // write s to 'source'
	        node("target", snk).       // write t to 'target'
	        arcMap("cost", cost).      // write 'cost' for arcs
	        arcMap("flow", flow).      // write 'flow' for arcs
	        arcMap("lcap", lcap).      // write 'capacity' for arcs
	        arcMap("ucap", ucap).
			nodeMap("supply",supl).    // write 'supply' for nodes
	        run();
}

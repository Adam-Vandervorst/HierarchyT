# HierarchyT
A prototype graph framework where edges are tagged with other nodes and graphs can be hierarchically structured and merged.

This concept is explained on [my research blog](https://research.adamv.be/#Unifyingrichgraphswithedgestoedges).

This implementation is competing with [NestedT](https://github.com/Adam-Vandervorst/NestedT).
## Motivation
The basis is extremely small and flexible. T-Graphs contain a vector of nodes and nodes can store atoms (strings, integers, doubles and functions). A node is connected to a node *via* another node. This allows you to associate arbitrary information with an edge.
T-Graphs can be based on other T-graphs, inheriting their nodes and edges. Importantly, edges are stored in the T-graphs and not in the nodes, this is in contrast with [NestedT](https://github.com/Adam-Vandervorst/NestedT).

Aside of this there are (for now) two ways to do computations intrinsic to the graph: graph rewriting and recursion schemes.
Both are explained and motivated beautifully [here](https://blog.opencog.org/2013/03/24/why-hypergraphs/) and [here](https://jtobin.io/time-traveling-recursion) respectively.

## Basic usage
This is an **extremely experimental** implementation and has absolutely **no guarantees**. 

- A T-graphs layer is created with `auto l = new Layer();` and destroyed with `l->free_tree(); delete l;`
- Nodes can be created with `auto [n, p, q] = l->wrap(42, "p", "q");` with content `string, int, double`
- A node `p` can be connected to another node `q` via `n` by writing `auto e = l->connect(p, n, q);`
- Similarly you can disconnect `p` and `q` via `n` by writing `l.disconnect(p, n, q);`
- Edges can also be deleted by writing `l.disconnect(e);`
- A list of edges (property, destination) starting from `n` can be obtained by `l->conn[n]`
- When you need the destinations from starting node `n` via `p` you can use the utility `l->get_objects(n, p);`
- The storage content of node `n` is accesed via `l[n]`
## Hierarchy usage
A T-graph in HierarchyT is just one layer. The aforementioned declaration `auto base = new Layer();` creates a root. 
On this base layer other layers can be created with `auto content = new Layer(base, "Name");`. All changes made in this layer are confined to this layer.
All nodes and edges of layers below the current are accesible, the ones made on top of this one are not.
You can merge changes from graph `h` into a lower layer `g` by writing `*g <<= h;`
## TODO's
Open to pull-requests.

- [ ] Add documentation and code comments in the frequent code paths
- [ ] Implement a [NEAT](http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf)-like neural-network
- [ ] Implement edge delete for stacked layers and merging
- [ ] Implement node delete for stacked layers and merging
- [ ] Add basic tests for Layer
- [ ] Add basic tests for stacked layers
- [ ] Construct more elaborate benchmarks including ones for graph-rewriting
- [ ] Add a function-type to the allowed node storage types
- [ ] Allow for more heterogeneous computation, using node-centric operations
- [ ] Add more TODO's

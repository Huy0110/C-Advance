#include "cgraphv1.h"

int main(int argc, char const *argv[])
{
    Graph g = importFile("file.txt", UNDIRECT_GRAPH);

    // int numV = numVertex(g);
    // int length = 0;
    // int path[numV];

    // Dijkstra(g, 0, 2, path, &length);
    // print2Dot(g, "dothi.dot");
    // BFS(g, 0);
    Coloring(g);

    // MST(g);

    // for (int i = 0; i < length; i++)
    // {
    //     printf("%d ", path[i]);
    // }

    JRB node;
    jrb_traverse(node,g.vertices)
      printf("%d\n",jval_i(node->key));
    dropGraph(g);
    // dropGraph(gReverse);
    return 0;
}

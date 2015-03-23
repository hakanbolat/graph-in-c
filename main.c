#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/* Function to create an adjacency list node*/
adjlist_node_p createNode(int v)
{
    adjlist_node_p newNode = (adjlist_node_p)malloc(sizeof(adjlist_node_t));
    if(!newNode)
        err_exit("Unable to allocate memory for new node");

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph_p createGraph(int n, graph_type_e type)
{
    int i;
    graph_p graph = (graph_p)malloc(sizeof(graph_t));
    if(!graph)
        err_exit("Unable to allocate memory for graph");
    graph->num_vertices = n;
    graph->type = type;

    /* Create an array of adjacency lists*/
    graph->adjListArr = (adjlist_p)malloc(n * sizeof(adjlist_t));
    if(!graph->adjListArr)
        err_exit("Unable to allocate memory for adjacency list array");

    for(i = 0; i < n; i++)
    {
        graph->adjListArr[i].head = NULL;
        graph->adjListArr[i].num_members = 0;
    }

    return graph;
}

/*Destroys the graph*/
void destroyGraph(graph_p graph)
{
    if(graph)
    {
        if(graph->adjListArr)
        {
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->num_vertices; v++)
            {
                adjlist_node_p adjListPtr = graph->adjListArr[v].head;
                while (adjListPtr)
                {
                    adjlist_node_p tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->adjListArr);
        }
        /*Free the graph*/
        free(graph);
    }
}

/* Adds an edge to a graph*/
void addEdge(graph_t *graph, int src, int dest)
{
    /* Add an edge from src to dst in the adjacency list*/
    adjlist_node_p newNode = createNode(dest);
    newNode->next = graph->adjListArr[src].head;
    graph->adjListArr[src].head = newNode;
    graph->adjListArr[src].num_members++;

    if(graph->type == UNDIRECTED)
    {
        /* Add an edge from dest to src also*/
        newNode = createNode(src);
        newNode->next = graph->adjListArr[dest].head;
        graph->adjListArr[dest].head = newNode;
        graph->adjListArr[dest].num_members++;
    }
}

/* Function to print the adjacency list of graph*/
void displayGraph(graph_p graph)
{
    int i;
    for (i = 0; i < graph->num_vertices; i++)
    {
        adjlist_node_p adjListPtr = graph->adjListArr[i].head;
        printf("\n%d: ", i);
        while (adjListPtr)
        {
            printf("%d->", adjListPtr->vertex);
            adjListPtr = adjListPtr->next;
        }
        printf("NULL\n");
    }
}

int main()
{
    int i=0,j=0;
    int TotalEdge=0,NodeOrder=0;
    int matrix[5][5];
    FILE *fp;//file read

    fp = fopen("exam.txt","r"); // read mode

    if( fp == NULL )
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    while( !feof(fp) )
    {
        fscanf(fp,"%d %d %d %d %d\n",&matrix[i][0],&matrix[i][1],&matrix[i][2],&matrix[i][3],&matrix[i][4]);//to get list from exam.txt file
        i++;
    }


    fclose(fp);//close file



    graph_p undir_graph = createGraph(5, UNDIRECTED); // create graph
    for(i=0;i<5;i++)
        for(j=i+1;j<5;j++)
        {
            if(matrix[i][j]!=0)
            {
                addEdge(undir_graph, i, j);//add edge using adj_list
            }
        }

    printf("\nUNDIRECTED GRAPH");
    displayGraph(undir_graph);
    try_scan:
    printf("\nPlease insert vertex no, which u want to display the degree:\n");
    scanf("%d",&i);
    if(i<0||i>4)
    {
        printf("value out of range!!");
        goto try_scan;
    }
    else
    {
        adjlist_node_p adjListPtr = undir_graph->adjListArr[i].head;

        while (adjListPtr)
        {
            adjListPtr = adjListPtr->next;
            NodeOrder++;
        }
    printf("\nOrder of %d.Node = %d\n",i,NodeOrder);
    }


    for (i = 0; i < undir_graph->num_vertices; i++)
    {
        adjlist_node_p adjListPtr = undir_graph->adjListArr[i].head;
        while (adjListPtr)
        {
            adjListPtr = adjListPtr->next;
            TotalEdge++;
        }
    }

    printf("\nGraph total edge is=%d\n\n",TotalEdge/2);//each edge have two direction

    if(NodeOrder!=10)
    {
        printf("The Graph isn't a complete!\n(Each node doesn't have edge with all of other node)\n");
    }
    else
    {
        printf("The Graf is a complete!\n");
    }

    destroyGraph(undir_graph);
    getch();

    return 0;
}

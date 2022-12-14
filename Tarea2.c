//Tarea 2 - Algoritmos y Estructuras de Datos
//Ignacio Marzotta (23.601.779-6)
//Cristobal Zapata (19.962.025-8)

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>

//Definicion de constantes
#define infinite 3000000
#define stationNameMaxSize 40

//Funcion encargada de inicializar la lista de bools Visitados a falso, utilizado para el algoritmo de Dijkstra.
void initializeVisited(int nodeCount, bool Visited[nodeCount])
{
    int i;
    for(i=0; i<nodeCount; i++)
    {
        Visited[i] = false;
    }
} 

//Funcion que muestra la ruta o costo entre 2 nodos en base a la seleccion del usuario.
void showRouteAndCost(int nodeCount, int v0, int vf, int path[nodeCount], int previousNode[nodeCount], int Cost[nodeCount],char stationNames[nodeCount][stationNameMaxSize], int selection)
{
    int i, node;
    i = 0;
    path[i] = vf;
    i++;
    node = previousNode[vf];
    while(node != v0)
    {
        path[i] = node;
        i++;
        node = previousNode[node];
    }
    path[i] = node;
    if (Cost[vf] == infinite)
    {
        printf("\nNo existe camino.");
        return;
    }
    if(selection == 1)
    {
        double cost = Cost[vf]/1000.0;
        printf("\nCosto: %.1f km\n", cost);
    }
    if(selection == 2)
    {
        printf("Ruta: ");
        for (; i>0; i--)
        {
            node = path[i];
            printf("%s, ", stationNames[node]);
        }
        node = path[i];
        printf("%s.\n", stationNames[node]);
    }
}

//Funcion que calcula los nodos sin visitar en el grafo. Usado por dijkstra.
int totalNonVisitedNodes(int nodeCount, bool Visited[nodeCount])
{
    int i, cont;
    i = 0;
    cont = 0;
    for(i=0; i<nodeCount; i++)
    {
        if (!Visited[i])
        {
            cont++;
        }
    }
   return cont;
}

//Funcion para encontrar el nodo adyacente de minimo valor sin visitar, utilizado por dijkstra.
int cheapestNonVisitedNode(int nodeCount, bool Visited[nodeCount], int Cost[nodeCount])
{
    int i, nodo, minimum;
    bool isFirst;
    isFirst = true;
    i = 0;
    for(i=0; i<nodeCount; i++)
    {
        if (!Visited[i])
        {
            if (isFirst)
            {
                minimum = Cost[i];
                nodo = i;
                isFirst = false;
            }
            else
            {
                if (Cost[i] < minimum)
                {
                    minimum = Cost[i];
                    nodo = i;
                }
            }
        }
    }
    return nodo;
}

//Algoritmo de dijkstra. Da valor a todos los nodos del mainGraph en base a un nodo inicial v0. Modifica la matriz de costos.
void dijkstra(int nodeCount, int v0, bool Visited[nodeCount], int Graph[nodeCount][nodeCount], int Cost[nodeCount], int previousNode[nodeCount])
{
    int i, j, selectedNode;
    initializeVisited(nodeCount, Visited);  //Inicialmente todos los nodos están sin visitar (arreglo "Visitados" parte con "falsos".
    Visited[v0] = true;    //Se marca como visitado a v0
    for(i=0;i<nodeCount;i++)
    {
        if (Graph[v0][i] == 0)  //Si el nodo i no tiene un arco directo desde v0 entonces:
        {
            Cost[i] = infinite;     //El costo del nodo i es infinitoinito
        }
        else
        {
            Cost[i] = Graph[v0][i];    //Si i tiene un arco directo el costo al nodo es el costo del arco desde v0.
        }
        previousNode[i] = v0;   //El nodo anterior de todos los nodos es v0 (valor entre paréntesis)
    }
    while (totalNonVisitedNodes(nodeCount, Visited)>0)
    {
        selectedNode = cheapestNonVisitedNode(nodeCount, Visited, Cost); //Escoge al nodo con menor costo y que no esté visitado.
        Visited[selectedNode] = true;  //El nodo escogido se marca como visitado.
        for(j=0;j<nodeCount;j++)
        {
            if ((Graph[selectedNode][j] != 0) && (!Visited[j])) //Si el nodo j es adyacente al nodo elegido, y no está visitado:
            {
                if (Cost[selectedNode] + Graph[selectedNode][j] < Cost[j]) //Si el costo es menor a través del nodo elegido:
                {
                    Cost[j] = Cost[selectedNode] + Graph[selectedNode][j]; //Se actualiza el costo para llegar al adyacente.
                    previousNode[j] = selectedNode; //Se actualiza el nodo anterior para llegar al adyacente.
                }
            }
        }
    }
}

//Funcion que busca la ID/Index de una estacion en base a su nombre. Recibe la cantidad de nodos, la lista de stationNames y el nombre de la estacion a buscar. Devuelve la ID como entero.
int getStationIndex(int nodeCount, char stationNames[nodeCount][stationNameMaxSize], char nomEstacion[stationNameMaxSize])
{
    int i;

    for(i=0; i<nodeCount; i++)
    {
        if ( strcmp(stationNames[i], nomEstacion) == 0)
        {
            return i;
        }
    }
    printf("\n ERROR: No se encontro la estacion %s.\n\n", nomEstacion);
}

//Funcion encargada de inicializar el mainGraph llenando con 0.
void initializeGraph(int nodeCount, int mainGraph[nodeCount][nodeCount])
{
    int i, j;

    for(i=0; i<nodeCount; i++)
    {
        for(j=0; j<nodeCount; j++)
        {
            mainGraph[i][j] = 0;
        }
    }
}

//Funcion de lectura de archivo. Rellena el mainGraph principal con los valores del fp proporcionado.
void populateGraph(int nodeCount, char stationNames[nodeCount][stationNameMaxSize], int mainGraph[nodeCount][nodeCount])
{
    FILE *fp;
    int i, length, node1, node2, distance;
    char line[200];
    char *station1;
    char *station2;
    char *strDistance;

    initializeGraph(nodeCount, mainGraph);

    fp = fopen("metro.txt", "r");
    if (fp == NULL)
    {
        printf("No se encuentra el archivo ""metro.txt"".");
    }
    fscanf(fp, "%i\n", &nodeCount);
    
    for (i=0; i<nodeCount; i++)
    {
        fgets(line, 200, fp);
        length = strlen(line);
        line[length-1] = '\0';
        strcpy(stationNames[i], line);
    }

    while (fgets(line, 400, fp) != 0)
    {
        station1 = strtok(line, ",");     //corta el primer string antes de la primera ","
        station2 = strtok(NULL, ",");      //corta el segundo string entre las dos ","
        strDistance = strtok(NULL, ",");   //corta el tercer string luego de la �ltima ","
        distance = atoi(strDistance);     //la funci�n "atoi" transforma el string que tiene la distance a un n�mero entero.
        node1 = getStationIndex(nodeCount, stationNames, station1);   //se obtiene el n�mero de la estaci�n1 a partir de su nombre.
        node2 = getStationIndex(nodeCount, stationNames, station2);   //se obtiene el n�mero de la estaci�n2 a partir de su nombre.
        mainGraph[node1][node2] = distance;    //llena el mainGraph con los arcos de las stationNames conectadas.
        mainGraph[node2][node1] = distance;    //llena el mainGraph sim�tricamente.
    }
    fclose(fp);
}

//Funcion encargada de definir la cantidad de stationNames/nodos
int getNodeCount()
{
    int x;
    char c[5];
    FILE *fptr;
    if ((fptr = fopen("metro.txt", "r")) == NULL) {
        printf("Error! File cannot be opened.");
        exit(1);
    }
    // reads text until newline is encountered
    fscanf(fptr, "%[^\n]", c);
    x = atoi(c);
    fclose(fptr);

    return x;
}

//Funcion que muestra el menu de seleccion con cada iteracion.
int showSelectionMenu()
{
    int x;              //
    char temp;          //Variable temporal para limpiar el buffer. Sirve para casos en los que se le solicite al usuario un string luego de ingresar un entero.
    printf("\nSeleccione opcion: \n");
    printf("1) Distancia entre estaciones. \n");
    printf("2) Ruta entre estaciones. \n");
    printf("3) Tramo alternativo en caso de corte. \n");
    printf("4) Salir.\n>> ");
    
    scanf("%d", &x);    //Solicitar input de seleccion

    //Extracto necesario para que el programa no se caiga al ingresar chars en la seleccion de menu.
    while(!(x == 1) && !(x == 2) && !(x == 3) && !(x == 4))
    {
        printf("Por favor, ingrese una seleccion valida. \n>>");
        scanf("%c", &temp);     
        scanf("%d", &x);
    }

    return x;
}

//Funcion que solicita el nombre de una estacion (puede ser la de origen, destino o la interrumpida de la opcion 3) y devuelve la ID de dicha estacion utilizando la funcion getStationIndex.
int setStation(int nodeCount, char stationNames[nodeCount][stationNameMaxSize],int inputType)
{
    int x;                                      //Variable para almacenar temporalmente la ID de la estacion.
    char temp;                                  //Variable temporal para limpiar el buffer. Sirve para casos en los que se le solicite al usuario un string luego de ingresar un entero.
    char stationName[stationNameMaxSize];       //Nombre de la estacion a solicitar

    //En base a la estacion que se necesita, mostrar un mensaje distinto al usuario
    switch (inputType)
    {
        case 1:
            printf("Estacion de origen: ");
            break;
        case 2:
            printf("Estacion de destino: ");
            break;
        case 3:
            printf("Estacion cortada: ");
            break;
    }

    scanf("%c", &temp);                //Limpiar buffer
    scanf("%[^\n]", stationName);      //Solicitar nombre de estacion
    
    x = getStationIndex(nodeCount, stationNames, stationName);

    return x;
}

int main()
{
    //Definicion de variables principales.
    int selection = 0;          //Seleccion de menu
    int v0, vf, vx;             //IDs de estaciones a solicitar
    int nodeCount = getNodeCount();       //Cantidad total de estaciones, rescatado de la primera linea de metro.txt

    //Definicion de listas y matrices principales.
    bool Visited[nodeCount];                                //Lista de nodos visitados, utilizado por funcion dijkstra.
    int Cost[nodeCount];                                    //Lista de costos usada por Dijkstra
    int previousNode[nodeCount];                            //Lista de nodos anteriores utilizada por Dijkstra
    int path[nodeCount];                               //Lista que almacena el camino de la ruta actual, usado por dijkstra.
    char stationNames[nodeCount][stationNameMaxSize];       //Matriz que almacena los 
    int mainGraph[nodeCount][nodeCount];                    //Matriz de adyacencia principal

    //Llamada a popular el grafo usando el archivo metro.txt
    populateGraph(nodeCount, stationNames, mainGraph);

    //Ciclo principal de menu de seleccion
    while (selection != 4)
    {
        //Mostrar menu de seleccion
        selection = showSelectionMenu();

        if(selection == 1 || selection == 2)
        {
            //Solicitar estaciones de origen y destino.
            v0 = setStation(nodeCount, stationNames, 1);
            vf = setStation(nodeCount, stationNames, 2);
            
            dijkstra(nodeCount, v0, Visited, mainGraph, Cost, previousNode);
            showRouteAndCost(nodeCount, v0, vf, path, previousNode, Cost, stationNames, selection);
        }
        else if(selection == 3)
        {
            //Solicitar estaciones de origen, destino e interrumpida.
            v0 = setStation(nodeCount, stationNames, 1);
            vf = setStation(nodeCount, stationNames, 2);
            vx = setStation(nodeCount, stationNames, 3);

            //Definicion e inicializacion de la matriz de conexiones para la estacion que se va a cortar. Se asume que una estacion no puede tener mas de 5 conexiones.
            int disconnectedStations[5][2] = {{0,0},{0,0},{0,0},{0,0},{0,0}};
            
            //Eliminar los adyacentes de la estacion cortada y agregarlos a la lista temporal para luego agregarlos de nuevo.
            int cont = 0;
            for (int i = 0; i < nodeCount; i++)
            {
                if(mainGraph[i][vx] != 0)
                {
                    disconnectedStations[cont][0] = i;
                    disconnectedStations[cont][1] = mainGraph[i][vx];
                    
                    cont++;
                }
                mainGraph[i][vx] = 0;
                mainGraph[vx][i] = 0;
            }

            //Aplicar dijkstra en base a la estacion inicial.
            dijkstra(nodeCount, v0, Visited, mainGraph, Cost, previousNode);
            //Mostrar el resultado
            showRouteAndCost(nodeCount, v0, vf, path, previousNode, Cost, stationNames, 2);

            //Agregar de vuelta todos los adyacentes de la estacion conectada
            for (int i = 0; i < 5; i++)
            {
                mainGraph[vx][disconnectedStations[i][0]] = disconnectedStations[i][1];
                mainGraph[disconnectedStations[i][0]][vx] = disconnectedStations[i][1];
            }   
        }
    }
    return 0;
}
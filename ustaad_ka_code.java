import java.util.*;
import java.lang.*;

public class wirelessNetwork
{
	protected String[] name;	// 1-d array to store the names of the vertices
	protected double[] posx;	// 1-d array to store all the x-coordinates
	protected double[] posy;	// 1-d array to store all the y-coordinates
	protected int numNodes;		// number of nodes in the wireless network
	protected int numEdges;		// number of edges in the wireless network
	protected myGraph graph;	// the corresponding graph of the wireless network
	protected String[] visited;	// 1-d array to store the previous vertices in the route
	protected int last;			// index of the last element in the visited array
	protected int maxDegree;	// Maximum degree in the wireless network
	
	// Default constructor. 
	public wirelessNetwork( )		
	{
		wirelessNetwork network = new wirelessNetwork(10, 500);
	}

	// Constructor that takes two parameters
	public wirelessNetwork(double size, int n)	
	{
		graph = new myGraph(n);  // Construct the corresponding graph
		posx = new double[n];
		posy = new double[n];
		name = new String[n];
		for (int i = 0; i < n; i++)
		{
			double x = size*Math.random();  // randomly generate the x-coordinate
			double y = size*Math.random();  // randomly generate the y-coordinate
			posx[i] = x;
			posy[i] = y;
			name[i] = "a"+i;			// the name of a node is an "a" followed by a number
			graph.addVertex(name[i]);	// add the vertex to the graph
		}
		numNodes = n;
		
		// Check the distance of each pair of vertices
		for (int i = 0; i<numNodes; i++)
		{
			for (int j = 0; j<i; j++)
			{
				// If the distance <= 1, add an edge in the corresponding graph
				if (dist(i, j)<=1)
				{
					graph.addEdge(name[i],name[j], dist(i, j)); 
					numEdges++;
				}
			}
		}
		
		visited = new String[n];
		last = 0;
	}
	
	// Calculate the distance between any two vertices, given their names
	public double distance(String vertex1, String vertex2)
	{
		int i1 = getIndex(vertex1);
		int i2 = getIndex(vertex2);
		if((i1==-1)||(i2==-1))		// If at least one vertex does not exist
			return -1;
		else return dist(i1, i2);
	}
	
	// Calculate the distance between any two vertices, given their indices
	public double dist(int index1, int index2)
	{
		return (Math.sqrt((posx[index1]-posx[index2])*(posx[index1]-posx[index2])
		 + (posy[index1]-posy[index2])*(posy[index1]-posy[index2])));
	} 
	
	// Get the index of a vertex, given the name
	public int getIndex(String vertex)
	{
		for(int i = 0; i < numNodes; i++)
			if(vertex.equals(name[i]))
				return i;
		return -1;
	}
	
	// Topology Control
	public void topologyControl()
	{
		// Use tempGraph to store the graph and tempGraph will not be modified.
		// When check the distance and neighbors of the nodes, check the tempGraph
		// When deleting an edge, delete in graph
		myGraph tempGraph = graph;
		int n = graph.numVertices;
		
		for (int i = 0; i<n; i++)  // for each node u in the network
		{
			String node = tempGraph.Vertices[i];
			int numNeighbors = tempGraph.degree(i);
			String[] neighbors = tempGraph.getNeighbors(node);
			
			for (int j = 0; j<numNeighbors; j++)  // for each neighbor v of current node u
			{
				String neighbor = neighbors[j];
				int jIndex = tempGraph.getIndex(neighbor);
				String[] neighborOfNeighbor = tempGraph.getNeighbors(neighbor);
				int numNeighborOfJ = tempGraph.degree(jIndex);
				
				for (int k = 0; k<numNeighborOfJ; k++)  // for each neighbor w of neighbor v
				{
					String neighborK = neighborOfNeighbor[k];
					
					// If |uw|<|uv| and |vw|<|uv|, delete edge |uv|
					if((distance(node, neighborK) < distance(node, neighbor))&&
					(distance(neighborK, neighbor) < distance(node, neighbor)))	
						graph.deleteEdge(node, neighbor);
				}
			}	
				
		}
	}
	
	// Check whether a vertex is already visited in compass routing
	// This method will prevent a node from being visited twice 
	// Being visited twice leads to endless recursive operations, i.e. non-termination
	protected boolean isVisited(String vertex, String[] array)
	{
		for (int i = 0; i<array.length; i++)
		{
			if (vertex.equals(array[i]))
				return true;
		}
		return false;
	}
	
	// Compass Routing methods, given s as the source, t as the destination
	public String[] compassRouting(String s, String t)
	{
		int is = getIndex(s);
		int it = getIndex(t);
		if ((is == -1) || (it == -1))   // Check whether the two vertices all exist
		{
			System.out.println("Routing cannot be found. Source or destination does not exists.");
		}
		
		// If the source node is the destination node, terminate
		if (s.equals(t))
		{
			// Generate the route which contains exactly one node -- the source node
			String[] route = new String[1];
			route[0] = t;
			// Reset the visited array and the last index for future use
			visited = new String[numNodes];
			last = 0;
			return route;
		}
		
		else
		{
			String[] neighbors = graph.getNeighbors(s);
			int num = neighbors.length;
			
			// If the current node has no neighbors, return an empty route
			if (num==0)
			{
				String[] emptyRoute = new String[0];
				visited = new String[numNodes];
				last = 0;
				return emptyRoute;
			}
			
			// If the current node has only one neighbor that is already visited,
			// Which means it receives the message from its neighbor and 
			// cannot send the message back to its neighbor,
			// So the route cannot be found. Return an empty route 
			else if ((num==1)&&(isVisited(neighbors[0], visited)))
			{
				String[] emptyRoute = new String[0];
				visited = new String[numNodes];
				last = 0;
				return emptyRoute;
			}
			
			// Other cases...				
			else
			{
				double maxCosine = -2;   // initial value of maxCosine can be any number less than -1
				String next = s;
				
				// Find the neighbor with the largest cosine value (smalles angle)
				for (int k = 0; k<num; k++)
				{
					String neighbor = neighbors[k];
					if ((cosine(s, t, neighbor) > maxCosine)&&(!(isVisited(neighbor, visited))))
					{
						maxCosine = cosine(s, t, neighbor);
						next = neighbor;
					}
				}
				
				// Pick this neighbor to do recursive compassRouting
				// Add this neighbor to visited list so that it won't be visited again
				visited[last++] = s;
				return merge(s, compassRouting(next, t));
			}
		}
	}
	
	// Merge an element and an array, this function is called in compassRouting()
	protected String[] merge(String head, String[] body)
	{
		int size = body.length;
		String[] merged = new String[size+1];
		merged[0] = head;
		for (int i=1; i<=size; i++)
		{
			merged[i]=body[i-1];
		}
		return merged;
	}
	
	// Calculate the cosine value of an angle. 
	// Cosine value changes from 1 to -1 when the angle increases from 0 to PI/2
	// i.e. larger cosine value indicates smaller angle
	protected double cosine(String current, String dest, String next)
	{
		double a = distance(dest, next);
		double b = distance(dest, current);
		double c = distance(current, next);
		return (b*b + c*c -a*a)/(2*b*c);
	}
	
	// Get the max degree in the wireless network, for experimental purpose
	protected int getMaxDegree()
	{
		maxDegree = 0;
		for (int i = 0; i<graph.numVertices; i++)
		{
			String node = graph.Vertices[i];
			int degree = graph.degree(node);
			if (degree > maxDegree)
				maxDegree = degree;
		}
		return maxDegree;
	}
	
}
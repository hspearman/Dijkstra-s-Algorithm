#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <limits.h>

using std::string;
using std::ifstream;
using std::vector;
using std::list;
using std::make_heap;
using std::sort_heap;
using std::pop_heap;
using std::map;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

static void GenerateVertexLookup();
static void findShortestPath( int numOfVerts );
static void outputShortestPath( int dest );

const int MAX_CHARS		= 10;
const int MAX_WEIGHT	= 10000000;

// All the information we'll keep track of while executing Djikstra's algorithm
vector< int > key;
vector< int > predecessor;
vector< int > heap;
vector< list< pair< int, int > > > neighbors;
map< int, string > vertexNameLookup;
map< string, int > vertexNumberLookup;

// Custom comparator for our min heap. The comparator helps sort the heap based on a vertex's key value.
class HeapComparator
{
public:
	bool operator()( const int &a, const int &b ) const
	{
		int a_key = key[ a ];
		int b_key = key[ b ];

		return a_key > b_key;
	}
};

int main()
{
	GenerateVertexLookup();

	string fileNames [] = { "Case1.txt",
							"Case2.txt",
							"Case3.txt" };

	int numOfFiles = sizeof( fileNames ) / sizeof( string );

	for ( int index = 0; index < numOfFiles; index++ )
	{
		// Open the file
		string filename = fileNames[ index ];
		ifstream file;
		file.open( filename );

		// Temporarily stores each line
		char line [ MAX_CHARS ];

		// Get the # of vertices (first line of the file)
		file.getline( line, MAX_CHARS );
		int numOfVerts = atoi( line );

		// Set up each vertex
		for ( int vertex = 0; vertex < numOfVerts; vertex++ )
		{
			key.push_back( MAX_WEIGHT );
			neighbors.push_back( list< pair< int, int > >() );
			predecessor.push_back( -1 );
			heap.push_back( vertex );
		}

		// Set the start vertex's weight to zero according to the algorithm
		key[ 0 ] = 0;

		// Read in the edges and record them
		while ( file.getline( line, MAX_CHARS ) )
		{
			string head( line, 2, 1 );
			string tail( line, 0, 1 );

			int weight = atoi( &line[ 4 ] );

			// Get the numbers corresponding to the vertex names (since we store vertex information via integer indexes)
			int vertex		= vertexNumberLookup[ tail ];
			int neighbor	= vertexNumberLookup[ head ];
			
			// Record the edge connected to vertex (bundle the weight and the neighbor's vertex number together).
			pair< int, int > edge = make_pair( neighbor, weight );
			( neighbors[ vertex ] ).push_back( edge );
		}
		
		// Make a minimum heap using our comparator
		make_heap( heap.begin(), heap.end(), HeapComparator() );

		findShortestPath( numOfVerts );

		// Output the shortest path information for a chosen vertex
		int destination = vertexNumberLookup[ "B" ];
		cout << key[ destination ] << endl;
		outputShortestPath( destination );
		cout << endl;

		// Clear out the data for the next file
		key.clear();
		neighbors.clear();
		predecessor.clear();
		heap.clear();
	}

	return 0;
}

// Finds the shortest path from start vertex to all other vertices using Djikstra's algorithm
static void findShortestPath( int numOfVerts )
{
	for ( int counter = 0; counter < numOfVerts; counter++ )
	{
		// Pop the vertex with the greatest key off the heap
		pop_heap( heap.begin(), heap.end(), HeapComparator() );
		int vertex = heap[ heap.size() - 1 ];
		heap.pop_back();

		// Iterate through the neighbors
		list< pair< int, int > > nborList = neighbors[ vertex ];
		list< pair< int, int > >::iterator iterator;
		for ( iterator = nborList.begin(); iterator != nborList.end(); iterator++ )
		{
			int neighbor		= iterator->first;
			int edgeWeight		= iterator->second;
			int totalPathWeight	= ( key[ vertex ] + edgeWeight );

			// If this is the cheapest path to "neighbor" we've encountered so far, then update it
			if ( key[ neighbor ] > totalPathWeight )
			{
				key [ neighbor ]		= totalPathWeight;
				predecessor[ neighbor ]	= vertex;

				// Resort the heap since we updated "neighbor"
				make_heap( heap.begin(), heap.end(), HeapComparator() );
			}
		}
	}
}

// Prints the shortest path from start vertex to destination vertex
static void outputShortestPath( int dest )
{
	string destination( vertexNameLookup[ dest ] );

	// Base case: we've recursively reached the start ( vertex 0 == start )
	if ( 0 == dest )
	{
		cout << destination << " ";
	}
	// Base case: no path exists
	else if ( -1 == predecessor[ dest ] )
	{
		cout << "No path from start to " << destination << endl;
		cout.flush();
	}
	else
	{
		outputShortestPath( predecessor[ dest ] );
		cout << destination << " ";
	}
}

/*	Populates the vertex lookup tables with entries. An entry associates a vertex number with a string name.
	The two tables allow you to look up a vertex by name ( "A" corresponds to 0 ) or by number ( 0 corresponds to "A" ). */	
static void GenerateVertexLookup()
{
	string names [] = { "A", "B", "C", "D", 
						"E", "F", "G", "H",
						"I", "J", "K", "L",
						"M", "N", "O", "P",
						"Q", "R", "S", "T",
						"U", "V", "W", "X",
						"Y", "Z" };

	int numOfNames = sizeof( names ) / sizeof( string );

	for ( int index = 0; index < numOfNames; index++ )
	{
		vertexNameLookup[ index ] = names[ index ];
		vertexNumberLookup[ names[ index ] ] = index;
	}
}
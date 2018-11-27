#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <math.h>
#include <float.h>
#include <algorithm>
#include <map>

using namespace std;
//------------- NODE STRUCTURE -----------------

struct node_c
{
    int id;
    int x, y;
    vector<node_c*> neighbours = {};
};
//-----------------------------------------------

bool operator == (node_c n, const node_c& c) {
    if(n.id == c.id && n.x == c.x && n.y == c.y){
        return true;
    }
    return false;
}
//-----------------------------------------------------------------

//--------- function to calc euclidean distance (used for node h - score)---------
double distance(node_c *node_i, node_c *node_j)
{
    return(sqrt(((node_i->x-node_j->x)*(node_i->x-node_j->x)) + ((node_i->y-node_j->y)*(node_i->y-node_j->y))));
}
//----------------------------------------------------------------

//---------- function to create a path of nodes ------------------
vector<int> create_path(map<int, int> came_from, node_c *current, vector<int> path, vector<node_c*> nodes)
{
    path.push_back(current->id);
    while(came_from[current->id] > 0)
    {
        path.push_back(came_from[current->id]);
        for (int i=0; i<nodes.size(); i++)
        {
            if(nodes[i]->id == came_from[current->id])
                current = nodes[i];
        }
    }
    return path;
}
//----------------------------------------------------------------

//====================== A STAR SEARCH ALGORITHM ===========================

vector<int> a_star(node_c *start_node, node_c *end_node, vector<node_c> nodes) {
    //-------Variables-------
    node_c *current_node;
    map<int, int> parent_map;
    map<int, double> g_score_map;
    map<int, double> f_score_map;
    vector<node_c*> open_list;
    vector<node_c*> closed_list;
    vector<int> path;
    //--------------------------

    //----------init start node scores--------
    g_score_map.insert(pair<int, double>(1,0));
    f_score_map.insert(pair<int, double>(1, distance(start_node, end_node)));
    open_list.push_back(start_node);
    //------------------------------------------

    //------------------whilst open list is not empty------------------
    while (!open_list.empty())
    {
        node_c neighbour;
        double lowest_f = FLT_MAX;
        //-----------node with lowest f score is current node-----------
        for (int i = 0; i < open_list.size(); i++) {
            if(f_score_map[open_list[i]->id] < lowest_f)
            {
                current_node = open_list[i];
                lowest_f = f_score_map[open_list[i]->id];
            }
        }

        //--------------find neighbours of node----------------
        for(int i=0; i<nodes.size(); i++) //fix
        {
            if(i+1 == current_node->id)
            {
                current_node->neighbours = nodes[i].neighbours;
            }
        }

        //---------if current node is the final node-> build path-------
        if(current_node->id == end_node->id) {
            path = create_path(parent_map, current_node, path, closed_list);
            for(int i=0; i< path.size(); i++)
            {
                cout << path[i] << endl;
            }
            return path;
        }

        //-------- remove current node from open list----------------
        if (find(open_list.begin(), open_list.end(), current_node) != open_list.end())
        {
            open_list.erase(remove(open_list.begin(), open_list.end(), current_node), open_list.end());
        }
        //--------- add current node to closed list------------
        closed_list.push_back(current_node);



        for(int i=0; i< current_node->neighbours.size(); i++)
        {
            node_c *neighbour = current_node->neighbours[i];
            if(find(closed_list.begin(), closed_list.end(), neighbour) != closed_list.end()) {
                continue;
            }
            double temp_g_score = g_score_map[current_node->id] + distance(current_node, neighbour);

            if (!(find(open_list.begin(), open_list.end(), neighbour) != open_list.end())) {
                open_list.push_back(neighbour);
            }

            else if(temp_g_score >= g_score_map[neighbour->id]) {
                continue;
            }
            parent_map.insert(pair<int, int>(neighbour->id, current_node->id));
            g_score_map.insert(pair<int, double>(neighbour->id, temp_g_score));
            f_score_map.insert(pair<int, double>(neighbour->id, g_score_map[neighbour->id] + distance(neighbour, end_node)));
        }
    }
    //---------------------if no path found, return 0-----------------
    path.push_back(0);
    for(int i=0; i< path.size(); i++)
    {
        cout << path[i] << endl;
    }
    return path;

}

int main(int argc, char* argv[]){
    //----------- check argument being used appropriately--------
    if(argc!=2)
        cout << "Usage: /program arg1.cav" <<endl;
    else {
        //-------------open cav file------------------
        ifstream cav_file(argv[1]);
        if(!cav_file.is_open())
            cout<<"Could not open file";
        else{
            vector<int> data_input;
            vector<node_c> nodes_list;
            vector<int> list_of_cons;
            int **connected_nodes = nullptr;

            //-----------------get ints between commas from file-------------
            string num;
            while(getline(cav_file, num, ',')) {
                data_input.push_back(stoi(num));
            }
            cav_file.close();

            int no_nodes = data_input[0];
            int tot_nodes = 2*(no_nodes);
            int node_id =1;

            //--------------get info for each node---------------
            for(int i=1; i< tot_nodes; i= i+2)
            {
                node_c temp;

                temp.id = node_id;
                node_id++;
                temp.x = data_input[i];
                temp.y = data_input[i+1];
                nodes_list.push_back(temp);
            }

            connected_nodes = new int*[no_nodes];
            for(int i=0; i < no_nodes; i++)
            {
                connected_nodes[i] = new int[no_nodes];
                for(int j=0; j< no_nodes; j++){
                    connected_nodes[i][j] = data_input[(((no_nodes*2)+1)+j) + no_nodes*i];
                }
            }

            for(int i=0; i< no_nodes; i++){
                for(int j=0; j< no_nodes; j++){
                    if(connected_nodes[i][j ]==1){
                        nodes_list[j].neighbours.push_back(&nodes_list[i]);
                    }
                }
            }

            //============= run A Star function ===================
            a_star(&nodes_list[0], &nodes_list[no_nodes-1], nodes_list);

            cout << "\n debug" << endl;




        }//end else
    }//end else
    return 0;
}//end main
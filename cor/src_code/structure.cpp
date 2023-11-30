#include "structure.h"


USER_SPECIFIED_PARAMETER PARAMETER;

double Absolute(double variable)
{
    if(variable < 0)
    {
        variable = variable * -1 ;
    }
    return variable ;
}

void Row_Delete(map<double,Row*> &for_delete_row)
{
    //cout<<" Delete_Row "<<endl;
    for(map<double,Row*>::iterator iter = for_delete_row.begin(); iter != for_delete_row.end(); iter++)
    {
        Node* ptr = iter->second->head;
        while(ptr != iter->second->tail)
        {
            Node* for_delete = ptr;
            ptr = ptr->next;
            delete for_delete;
        }
        delete ptr;
        Row* row_delete = iter->second;
        delete row_delete;
    }
    for_delete_row.clear();
    //cout<<" Clear... "<<endl;

}

void Insert_Node(Node* a, Node* b)  // a insert the location infront of b
{
    a -> next = b ;
    a -> prev = b -> prev ;
    b -> prev -> next = a ;
    b -> prev = a ;
}

void Delete_Node(Node* a )
{
    a -> prev -> next = a -> next ;
    a -> next -> prev = a -> prev ;
    delete a ;
}

double Max_cal(double a, double b)
{
    if(a > b)
    {
        return a;
    }
    return b;
}

double Min_cal(double a, double b)
{
    if(a < b)
    {
        return a;
    }
    return b;
}

string int2str(int i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}

void Boundary_Cout(Boundary a)
{
    cout<<"Boundary.llx : "<<a.llx<<endl;
    cout<<"Boundary.lly : "<<a.lly<<endl;
    cout<<"Boundary.urx : "<<a.urx<<endl;
    cout<<"Boundary.ury : "<<a.ury<<endl;
}

void Boundary_Assign(Boundary &a, int llx, int lly, int urx, int ury)
{
    a.llx = llx;
    a.lly = lly;
    a.urx = urx;
    a.ury = ury;
}

void Boundary_Assign_Limits(Boundary &a)
{
    a.llx = numeric_limits<int>::max();
    a.lly = numeric_limits<int>::max();
    a.urx = numeric_limits<int>::min();
    a.ury = numeric_limits<int>::min();
}

void Cal_BBOX(Boundary &src, Boundary &tar)
{
    if(src.llx < tar.llx)
    {
        tar.llx = src.llx;
    }
    if(src.lly < tar.lly)
    {
        tar.lly = src.lly;
    }
    if(src.urx > tar.urx)
    {
        tar.urx = src.urx;
    }
    if(src.ury > tar.ury)
    {
        tar.ury = src.ury;
    }
}

Boundary Overlap_Box(Boundary &region_box, Boundary &Blank_Space)
{

    Boundary sol;
    if(region_box.llx > Blank_Space.urx || region_box.urx < Blank_Space.llx || region_box.lly > Blank_Space.ury || region_box.ury < Blank_Space.lly){
        sol.llx = 0;
        sol.lly = 0;
        sol.urx = 0;
        sol.ury = 0;
        return sol;
    }
    if(region_box.llx > Blank_Space.llx)
    {
        sol.llx = region_box.llx;
    }
    else
    {
        sol.llx = Blank_Space.llx;
    }

    if(region_box.lly > Blank_Space.lly)
    {
        sol.lly = region_box.lly;
    }
    else
    {
        sol.lly = Blank_Space.lly;
    }

    if(region_box.urx < Blank_Space.urx)
    {
        sol.urx = region_box.urx;
    }
    else
    {
        sol.urx = Blank_Space.urx;
    }

    if(region_box.ury < Blank_Space.ury)
    {
        sol.ury = region_box.ury;
    }
    else
    {
        sol.ury = Blank_Space.ury;
    }

    return sol;
}

void Pair_max_min(float src, pair<float, float> &tar)
{
    tar.first = max(tar.first, src);
    tar.second = min(tar.second, src);
}


float Scaling_Cost(float range, float cost)
{
    float scaling_cost = 0;
    if(range == 0)
    {
        if(cost != 0)
        {
            return 1;
        }
        return 0;
    }
    scaling_cost = cost / range;
    return scaling_cost;
}

/*********modify 108.03.21*********/
int shifting_X;
int shifting_Y;
int PARA;
string def_file;  ///modify 108.03.26 // = def file name // used for ??
bool dataflow_PARA;         ///plus 108.04.23
bool Cluster_PARA;          ///plus 108.04.24
int dataflow_heir_num;      ///plus 108.06.04
bool Congestion_PARA= true;        ///plus 108.07.26
bool Debug_PARA = false;
/*****************************/
bool Refinement_Flag = false;
bool Build_Set = false;
int ITER = 0;

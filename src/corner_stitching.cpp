#include "corner_stitching.h"
//#include "macro_legalizer.h"

vector<CornerNode*> PreplacedInCornerStitching; /// For every iteration a all new clean fixed blocks & blanks /// If u want to get the info please use func : UpdateInfoInPreplacedInCornerStitching
vector<int> FixedPreplacedInCornerStitching; /// record the ID numbers of fixed occupied tiles

/// ADD 2021.05
vector<MacrosSet*> Macros_Set;
//////


void Coner_stitching_const()
{
    //cout<<"[INFO] Coner stitching is construct"<<endl;
    /// Here temp stores the fixed blocks
    /// AllCornerNode stores fixed blocks & blanks
    vector<CornerNode*> AllCornerNode;

    /// construct corner node & link node horizontally
    ConerNode_const(AllCornerNode);
    Row_Delete(row_vector);

    /// link node vertically
    ConerNode_Vert_link_const(AllCornerNode);

    /// merge corner node
    //ConerNode_merge(AllCornerNode);   // COMMENT 2021.03.23

    /// update container - PreplacedInCornerStitching
    UpdateInfoInPreplacedInCornerStitching(AllCornerNode, PreplacedInCornerStitching);

    /// update container - FixedPreplacedInCornerStitching
    UpdateFixedPreplacedInCornerStitching();

    /// release AllCornerNode memory
    DeleteCornerNode(AllCornerNode);
    vector<pair<int, Boundary> > for_plot;
    for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        for_plot.push_back(make_pair(PreplacedInCornerStitching[i]->NodeType, PreplacedInCornerStitching[i]->rectangle));
    }
    Plot_rectangle_w_color(for_plot, "./output/corner_stitching_graph/PreplacedInCornerStitching.m");
    ///cout<<"Coner_stitching_const end !!"<<endl;

}



void ConerNode_const(vector<CornerNode*> &ConerNode_temp)
{

    map<double, Row*>::iterator iter_end = row_vector.end();
    iter_end--;
    vector<Boundary> for_plot;
    for(map<double, Row*>::iterator iter = row_vector.begin(); iter != iter_end; iter++)
    {
        Node* ptr = iter->second->head;
        Node* tail = iter->second->tail;
        map<double, Row*>::iterator iter_next = iter;
        CornerNode* CornerNode_ptr = NULL;
        iter_next++;
        while(ptr != tail)
        {
            double blank_interval = ptr->next->rectangle.llx - ptr->rectangle.urx;

            if(ptr != iter->second->head)
            {
                CornerNode* temp = new CornerNode(Fixed, ConerNode_temp.size(), ptr->rectangle.llx, iter->first, ptr->rectangle.urx, iter_next->first);
                Boundary a;
                a.llx = ptr->rectangle.llx;
                a.lly = iter->first;
                a.urx = ptr->rectangle.urx;
                a.ury = iter_next->first;
                for_plot.push_back(a);
                ConerNode_temp.push_back(temp);

                if(CornerNode_ptr != NULL)
                {
                    CornerNode_hori_connect(CornerNode_ptr, temp);
                }
                CornerNode_ptr = temp;
            }

            if(blank_interval != 0)
            {
                CornerNode* temp = new CornerNode(Blank, ConerNode_temp.size(), ptr->rectangle.urx, iter->first, ptr->next->rectangle.llx, iter_next->first);
                Boundary a;
                a.llx = ptr->rectangle.urx;
                a.lly = iter->first;
                a.urx = ptr->next->rectangle.llx;
                a.ury = iter_next->first;
                for_plot.push_back(a);
                ConerNode_temp.push_back(temp);

                if(CornerNode_ptr != NULL)
                {
                    CornerNode_hori_connect(CornerNode_ptr, temp);
                }
                CornerNode_ptr = temp;
            }


            ptr = ptr->next;
        }
    }

    Plot_rectangle(for_plot, "./output/corner_stitching_graph/corner.m");
}

void CornerNode_hori_connect(CornerNode* ptr, CornerNode* next)
{
    ptr->E = next;
    next->W = ptr;
}

void CornerNode_vert_connect(CornerNode* ptr, CornerNode* next)
{
    ptr->N = next;
    next->S = ptr;
}

bool CornerNode_sort_x(CornerNode* a, CornerNode* b)
{
    if(a->rectangle.lly == b->rectangle.lly)
    {
        return a->rectangle.llx < b->rectangle.llx;
    }
    return a->rectangle.lly < b->rectangle.lly;
}

void ConerNode_Vert_link_const(vector<CornerNode*> &ConerNode_temp)
{
    //cout<<"ConerNode_Vert_link_const"<<endl;
    map<double, vector<pair<int,int> > > CornerNode_row_vector;
    vector<CornerNode*> temp;
    temp.resize(ConerNode_temp.size());
    for(unsigned int i = 0; i < ConerNode_temp.size(); i++)
    {
        temp[i] = ConerNode_temp[i];
    }

    for(unsigned int i = 0; i < ConerNode_temp.size(); i++)
    {
        vector<pair<int,int> > tmp;
        CornerNode_row_vector.insert(make_pair(ConerNode_temp[i]->rectangle.llx, tmp));
        CornerNode_row_vector.insert(make_pair(ConerNode_temp[i]->rectangle.urx, tmp));
    }


    for(unsigned int i = 0; i < temp.size(); i++)
    {
        map<double, vector<pair<int,int> > >::iterator iter = CornerNode_row_vector.find(temp[i]->rectangle.llx);
        pair<int,int> HeadTail_id = make_pair(0, temp[i]->CornerNode_id);
        iter->second.push_back(HeadTail_id);
        HeadTail_id.first = 1;
        iter++;
        int count_row_height = 1;
        for( ; iter->first != temp[i]->rectangle.urx; iter++)
        {
            iter->second.push_back(HeadTail_id);
            HeadTail_id.first = -1;
            count_row_height++;
        }
        iter--;
        iter->second[iter->second.size()-1].first = 2;
        if(count_row_height == 1)
        {
            iter->second[iter->second.size()-1].first = 3;
        }
    }


    for(map<double, vector<pair<int,int> > >::iterator iter = CornerNode_row_vector.begin(); iter != CornerNode_row_vector.end(); iter++)
    {
//        cout<<"Row : "<<k++<<endl;
//        cout<<"ID : ";
//        for(int i = 0; (unsigned int)i < iter->second.size(); i++)
//        {
//            cout<<iter->second[i].second<<"**"<<iter->second[i].first<<"  ";
//        }
//        cout<<endl;
        if(iter->second.size() < 2)
        {
            continue;
        }

        for(int i = 0; i < (int)iter->second.size(); i++)
        {

            unsigned int macro_id_ptr = iter->second[i].second;
            if((iter->second[i].first == 0 || iter->second[i].first == 3) && !(i - 1 < 0) )
            {
                unsigned int macro_id_prev = iter->second[i-1].second;
                ConerNode_temp[macro_id_ptr]->S = ConerNode_temp[macro_id_prev];
            }

            if((iter->second.at(i).first == 2  || iter->second[i].first == 3) && (i + 1) < (int) iter->second.size())
            {

                unsigned int macro_id_next = iter->second.at(i+1).second;
                ConerNode_temp[macro_id_ptr]->N = ConerNode_temp[macro_id_next];

            }

        }
        iter->second.clear();
    }

    CornerNode_row_vector.clear();

}

void Cout_CornerNode_linkID(vector<CornerNode*> ConerNode_temp)
{
    vector<int> exceed_id;
    vector<int> area_0;
    vector<int> wrong_N_link;
    vector<int> wrong_S_link;
    vector<int> wrong_W_link;
    vector<int> wrong_E_link;
    string a, b;
    a = "./outout/Link/link";
    b = int2str((int) ConerNode_temp.size());
    a += b;
    a += ".link";
    ofstream fout(a.c_str());
    int size_cornernode = (int) ConerNode_temp.size();
    bool exit_flag = 0;
    bool exit_flag0 = 0;
    for(unsigned int i = 0; i < ConerNode_temp.size(); i++)
    {
        if(ConerNode_temp.at(i) == NULL)
        {
            continue;
        }
        if((ConerNode_temp[i]->rectangle.urx - ConerNode_temp[i]->rectangle.llx) < 0 || (ConerNode_temp[i]->rectangle.ury - ConerNode_temp[i]->rectangle.lly) < 0)
        {
            area_0.push_back(i);
            exit_flag0 = 1;
        }
        fout<<"ID : "<<ConerNode_temp[i]->CornerNode_id<<" llx : "<<(float)ConerNode_temp[i]->rectangle.llx<<" urx : "<<(float)ConerNode_temp[i]->rectangle.urx<<" lly : ";
        fout<<ConerNode_temp[i]->rectangle.lly<<" ury : "<<ConerNode_temp[i]->rectangle.ury;
        fout<<"Nodetype : "<<ConerNode_temp[i]->NodeType<<endl;
        fout<<"area : "<<(ConerNode_temp[i]->rectangle.urx - ConerNode_temp[i]->rectangle.llx) * (ConerNode_temp[i]->rectangle.ury - ConerNode_temp[i]->rectangle.lly) <<endl;
        fout<<" N : ";
        if(ConerNode_temp[i]->N != NULL)
        {
            fout<<ConerNode_temp[i]->N->CornerNode_id<<" S : ";
            if(!(ConerNode_temp[i]->N->CornerNode_id < size_cornernode))
            {
                exit_flag = 1;
                int id = ConerNode_temp[i]->N->CornerNode_id;
                if(exceed_id.size() == 0)
                {
                    exceed_id.push_back(i);
                }
                else if(id != exceed_id.back())
                {
                    exceed_id.push_back(i);
                }
            }
            if(ConerNode_temp[i]->rectangle.ury == ConerNode_temp[i]->N->rectangle.lly && !(ConerNode_temp[i]->rectangle.urx > ConerNode_temp[i]->N->rectangle.urx) &&
                    ConerNode_temp[i]->rectangle.urx > ConerNode_temp[i]->N->rectangle.llx)
            {

            }
            else
            {
                wrong_N_link.push_back(ConerNode_temp[i]->CornerNode_id);
            }
        }
        else
        {
            fout<<"NULL"<<" S : ";
            if(ConerNode_temp[i]->rectangle.ury != chip_boundary.ury)
            {
                wrong_N_link.push_back(ConerNode_temp[i]->CornerNode_id);;
            }
        }
        if(ConerNode_temp[i]->S != NULL)
        {
            fout<<ConerNode_temp[i]->S->CornerNode_id<<" W : ";
            int id = ConerNode_temp[i]->S->CornerNode_id;
            if(!(id < size_cornernode))
            {
                exit_flag = 1;
                if(exceed_id.size() == 0)
                {
                    exceed_id.push_back(i);
                }
                else if(id != exceed_id.back())
                {
                    exceed_id.push_back(i);
                }
            }
            if(ConerNode_temp[i]->rectangle.lly == ConerNode_temp[i]->S->rectangle.ury && ConerNode_temp[i]->rectangle.llx < ConerNode_temp[i]->S->rectangle.urx &&
                    !(ConerNode_temp[i]->rectangle.llx < ConerNode_temp[i]->S->rectangle.llx))
            {

            }
            else
            {
                wrong_S_link.push_back(ConerNode_temp[i]->CornerNode_id);
            }
        }
        else
        {
            fout<<"NULL"<<" W : ";
            if(ConerNode_temp[i]->rectangle.lly != chip_boundary.lly)
            {
                wrong_S_link.push_back(ConerNode_temp[i]->CornerNode_id);;
            }
        }
        if(ConerNode_temp[i]->W != NULL)
        {
            fout<<ConerNode_temp[i]->W->CornerNode_id<<" E : ";
            int id = ConerNode_temp[i]->W->CornerNode_id;
            if(!(id < size_cornernode))
            {
                exit_flag = 1;
                if(exceed_id.size() == 0)
                {
                    exceed_id.push_back(i);
                }
                else if(id != exceed_id.back())
                {
                    exceed_id.push_back(i);
                }
            }
            if(ConerNode_temp[i]->rectangle.llx == ConerNode_temp[i]->W->rectangle.urx && !(ConerNode_temp[i]->rectangle.lly < ConerNode_temp[i]->W->rectangle.lly) &&
                    ConerNode_temp[i]->rectangle.lly < ConerNode_temp[i]->W->rectangle.ury)
            {

            }
            else
            {
                wrong_W_link.push_back(ConerNode_temp[i]->CornerNode_id);
            }

        }
        else
        {
            fout<<"NULL"<<" E : ";
            if(ConerNode_temp[i]->rectangle.llx != chip_boundary.llx)
            {
                wrong_W_link.push_back(ConerNode_temp[i]->CornerNode_id);
            }
        }
        if(ConerNode_temp[i]->E != NULL)
        {
            fout<<ConerNode_temp[i]->E->CornerNode_id;
            int id = ConerNode_temp[i]->E->CornerNode_id;
            if(!(id < size_cornernode))
            {
                exit_flag = 1;
                if(exceed_id.size() == 0)
                {
                    exceed_id.push_back(i);
                }
                else if(id != exceed_id.back())
                {
                    exceed_id.push_back(i);
                }
            }
            if(ConerNode_temp[i]->rectangle.urx == ConerNode_temp[i]->E->rectangle.llx && !(ConerNode_temp[i]->rectangle.ury > ConerNode_temp[i]->E->rectangle.ury) &&
                    ConerNode_temp[i]->rectangle.ury > ConerNode_temp[i]->E->rectangle.lly)
            {

            }
            else
            {
                wrong_E_link.push_back(ConerNode_temp[i]->CornerNode_id);
            }
        }
        else
        {
            fout<<"NULL";
            if(ConerNode_temp[i]->rectangle.urx != chip_boundary.urx)
            {
                wrong_E_link.push_back(ConerNode_temp[i]->CornerNode_id);
            }
        }
        fout<<endl;

    }
    if(exit_flag == 1)
    {
        fout<<"exceed_id : ";
        for(unsigned int i = 0; i < exceed_id.size(); i++)
        {
            fout<<exceed_id[i]<<"  ";
        }
        fout<<endl;
    }
    if(exit_flag0 == 1)
    {
        fout<<"area_0 : ";
        for(unsigned int i = 0; i < area_0.size(); i++)
        {
            fout<<area_0[i]<<"  ";
        }
        fout<<endl;

    }
    if(wrong_N_link.size() != 0)
    {
        fout<<"wrong_N_link : ";
        for(unsigned int i = 0; i < wrong_N_link.size(); i++)
        {
            fout<<wrong_N_link[i]<<"  ";
        }
        fout<<endl;
    }
    if(wrong_S_link.size() != 0)
    {
        fout<<"wrong_S_link : ";
        for(unsigned int i = 0; i < wrong_S_link.size(); i++)
        {
            fout<<wrong_S_link[i]<<"  ";
        }
        fout<<endl;
    }
    if(wrong_W_link.size() != 0)
    {
        fout<<"wrong_W_link : ";
        for(unsigned int i = 0; i < wrong_W_link.size(); i++)
        {
            fout<<wrong_W_link[i]<<"  ";
        }
        fout<<endl;
    }
    if(wrong_E_link.size() != 0)
    {
        fout<<"wrong_E_link : ";
        for(unsigned int i = 0; i < wrong_E_link.size(); i++)
        {
            fout<<wrong_E_link[i]<<"  ";
        }
        fout<<endl;
    }
    if(exit_flag == 1 || exit_flag0 == 1 || wrong_N_link.size() != 0 || wrong_S_link.size() != 0 || wrong_W_link.size() != 0 || wrong_E_link.size() != 0)
    {
        cout<<"exit !!"<<endl;
        cout<<"CORNER STITCHING SHUT DOWN!!!"<<endl;
        exit(1);
    }
    fout.close();
}

void ConerNode_merge(vector<CornerNode*> &ConerNode_temp)
{
    vector<int> delete_ID_vect;
    for(unsigned int i = 0; i < ConerNode_temp.size(); i++)
    {
        if(ConerNode_temp[i]->N == NULL)
        {
            continue;
        }
        if(ConerNode_temp[i]->NodeType == ConerNode_temp[i]->N->NodeType && ConerNode_temp[i]->CornerNode_id != ConerNode_temp[i]->N->CornerNode_id)
        {
            if(ConerNode_temp[i]->rectangle.llx == ConerNode_temp[i]->N->rectangle.llx && ConerNode_temp[i]->rectangle.urx == ConerNode_temp[i]->N->rectangle.urx)
            {
                CornerNode *new_N, *new_E;
                unsigned int delete_ID = ConerNode_temp[i]->N->CornerNode_id;
                ConerNode_temp[ConerNode_temp[i]->CornerNode_id]->rectangle.ury = ConerNode_temp[i]->N->rectangle.ury;
                new_N = ConerNode_temp[i]->N->N;
                new_E = ConerNode_temp[i]->N->E;
                ConerNode_temp[delete_ID]->CornerNode_id = ConerNode_temp[i]->CornerNode_id;
                delete_ID_vect.push_back(delete_ID);
                //ConerNode_temp[delete_ID] = ConerNode_temp[i];
                ConerNode_temp[ConerNode_temp[i]->CornerNode_id]->N = new_N;
                ConerNode_temp[ConerNode_temp[i]->CornerNode_id]->E = new_E;
            }
        }
    }

    set<int> ConerNode_ID;
    for(unsigned int i = 0; i < ConerNode_temp.size(); i++)
    {
        ConerNode_ID.insert(ConerNode_temp[i]->CornerNode_id);
        if(ConerNode_temp[i]->N != NULL)
        {
            ConerNode_temp[i]->N = ConerNode_temp[ ConerNode_temp[i]->N->CornerNode_id];
        }
        if(ConerNode_temp[i]->E != NULL)
        {
            ConerNode_temp[i]->E = ConerNode_temp[ ConerNode_temp[i]->E->CornerNode_id];
        }
        if(ConerNode_temp[i]->W != NULL)
        {
            ConerNode_temp[i]->W = ConerNode_temp[ ConerNode_temp[i]->W->CornerNode_id];
        }
        if(ConerNode_temp[i]->S != NULL)
        {
            ConerNode_temp[i]->S = ConerNode_temp[ ConerNode_temp[i]->S->CornerNode_id];
        }
    }
    for(unsigned int i = 0; i < delete_ID_vect.size(); i++)
    {
        if(ConerNode_temp[delete_ID_vect[i]] == NULL)
        {
            continue;
        }
        delete ConerNode_temp[delete_ID_vect[i]];
        ConerNode_temp[delete_ID_vect[i]] = NULL;
    }
    vector<CornerNode*> temp;
    for(int i = 0; i < (int)ConerNode_temp.size(); i++)
    {
        CornerNode* corner_temp = ConerNode_temp[i];
        if(corner_temp != NULL)
        {
            corner_temp->CornerNode_id = temp.size();
            temp.push_back(corner_temp);
        }
    }
    swap(temp, ConerNode_temp);
}







CornerNode* Vertical_search(pair<double, double> macro_coordinate, CornerNode* ptr)
{
    double lly = macro_coordinate.second;
    if(ptr->rectangle.lly > lly)
    {
        if(ptr->S != NULL)
        {
            ptr = Vertical_search(macro_coordinate, ptr->S);
        }
        else
        {
//            cout<<"ptr->S == NULL"<<endl;
        }
    }
    else
    {
        if(ptr->rectangle.ury <= lly)
        {
            if(ptr->N != NULL)
            {
                ptr = Vertical_search(macro_coordinate, ptr->N);
            }
            else
            {
//                cout<<"ptr->N == NULL"<<endl;
            }
        }
    }
    return ptr;
}

CornerNode* CornerNodePointSearch(pair<int, int> macro_coordinate, CornerNode* ptr)
{
    while(1)
    {

//        cout<<"Vertical_search"<<endl;
        ptr = Vertical_search(macro_coordinate, ptr);
//        cout<<ptr->CornerNode_id<<endl;
//        cout<<"Horizontal_search"<<endl;
        ptr = Horizontal_search(macro_coordinate, ptr);
//        cout<<ptr->CornerNode_id<<endl;
//        cout<<"macro_coordinate First : "<<macro_coordinate.first<<"\tsecond : "<<macro_coordinate.second<<endl;
//        cout<<"chip_boundary          : "<<chip_boundary.urx<<"\t"<<chip_boundary.ury<<endl;
        if(macro_coordinate.second == chip_boundary.ury)
        {
            if(!(ptr->rectangle.lly > macro_coordinate.second) && ptr->rectangle.ury >= macro_coordinate.second)
            {
//                cout<<"FINAL : "<<ptr->CornerNode_id<<endl;
                return ptr;
            }
        }
        if(!(ptr->rectangle.lly > macro_coordinate.second) && ptr->rectangle.ury > macro_coordinate.second)
        {
//            cout<<"FINAL : "<<ptr->CornerNode_id<<endl;
            return ptr;
        }
    }
}

CornerNode* Horizontal_search(pair<double, double> macro_coordinate, CornerNode* ptr)
{
    double llx = macro_coordinate.first;
//    cout<<"llx : "<<llx<<endl;
//    cout<<ptr->CornerNode_id<<" : "<<ptr->rectangle.urx<<" "<<ptr->rectangle.llx<<endl;

    if(ptr->rectangle.llx > llx)
    {
        if(ptr->W != NULL)
        {
//            cout<<"W"<<endl;
            ptr = Horizontal_search(macro_coordinate, ptr->W);
        }
        else
        {
            cout<<"ptr->W == NULL"<<endl;
        }
    }
    else if(ptr->rectangle.urx <= llx)
    {

        if(ptr->E != NULL)
        {
//            cout<<"E"<<endl;
            ptr = Horizontal_search(macro_coordinate, ptr->E);
        }
        else
        {
            cout<<"ptr->E == NULL"<<endl;
        }
    }
    return ptr;
}






void Cout_PossibleCombination(vector<vector<CornerNode*> > PossibleCombination)
{
    for(unsigned int i = 0; i < PossibleCombination.size(); i++)
    {
        cout<<"Size : "<<PossibleCombination[i].size()<<endl;
        for(unsigned int j = 0; j < PossibleCombination[i].size(); j++)
        {
            cout<<PossibleCombination[i][j]->CornerNode_id<<"  ";
        }
        cout<<endl;
    }
    //getchar();
}

vector<CylinderInterval> DP_VertCommonInterval(vector<CornerNode*> CornerNode_temp, Macro* Macro_temp)
{
    /// ADD 2021.03
    sort(CornerNode_temp.begin(), CornerNode_temp.end(), CornerNode_sort_x);
    map<int, CornerNode*> temp;
    for(int i = 0; i < (int)CornerNode_temp.size(); i++)
    {
        CornerNode *tmp = new CornerNode(CornerNode_temp[i]->NodeType, 0, CornerNode_temp[i]->rectangle.llx, CornerNode_temp[i]->rectangle.lly, CornerNode_temp[i]->rectangle.urx, CornerNode_temp[i]->rectangle.ury);
        pair<map<int, CornerNode*>::iterator, bool> check;
        check = temp.insert(make_pair(CornerNode_temp[i]->rectangle.lly, tmp));
        if(check.second == false)
        {
            check.first->second->rectangle.urx += tmp->rectangle.urx - tmp->rectangle.llx;
        }
    }
    vector<CornerNode*> CornerNodetemp;
    for(map<int, CornerNode*>::iterator it = temp.begin(); it != temp.end(); it++)
    {
        //cout<<it->first<<", "<<it->second->rectangle.llx<<", "<<it->second->rectangle.lly<<endl;
        CornerNodetemp.push_back(it->second);
    }//getchar();
    //////
    /// MODIFY 2021.03, CornerNode_temp -> CornerNodetemp
    vector<CylinderInterval> CylinderInterval_temp;
    CylinderInterval_temp.reserve(10);

    int MacroHeightShrink = Macro_temp->cal_h_wo_shrink * Macro_temp->HeightShrinkPARA;
    int MacroWidthShrink = Macro_temp->cal_w_wo_shrink * Macro_temp->WidthShrinkPARA;
    int CountHeight = 0;
    pair<int, int> LR = make_pair(CornerNodetemp[0]->rectangle.llx, CornerNodetemp[0]->rectangle.urx);

//    cout<<"CornerNode_temp.size() : "<<CornerNode_temp.size()<<endl;

    for(unsigned int i = 0; i < CornerNodetemp.size(); i++)
    {
        CornerNode* corner_temp = CornerNodetemp[i];
        CountHeight += corner_temp->rectangle.ury - corner_temp->rectangle.lly;

        LR.first = max(LR.first, corner_temp->rectangle.llx);
        LR.second = min(LR.second, corner_temp->rectangle.urx);

//        cout<<"MacroHeightShrink : "<<MacroHeightShrink<<endl;
//        getchar();

        CylinderInterval cy_temp;
        cy_temp.Height = CountHeight;
        cy_temp.LR = make_pair(corner_temp->rectangle.llx, corner_temp->rectangle.urx);
        cy_temp.LRBound = LR;
        cy_temp.Width = LR.second - LR.first;
        cy_temp.FinalCount = i;
//        cout<<"CountWidth : "<<cy_temp.Width<<endl;
//        cout<<"MacroWidthShrink : "<<MacroWidthShrink<<endl;
        if(MacroWidthShrink > cy_temp.Width)
        {
//            cout<<"CornerNode ID: "<<corner_temp->CornerNode_id<<endl;
//            cout<<"Macro ID: "<<Macro_temp->macro_id<<", "<<Macro_temp->cal_w<<endl;
//            cout<<"LR: "<<LR.first<<", "<<LR.second<<endl;
//            cout<<"Width: "<<LR.second - LR.first<<endl;
            CylinderInterval_temp.clear();
            break;
        }
        CylinderInterval_temp.push_back(cy_temp);


    }
    /*cout<<"CylinderInterval_temp"<<endl;
    for(int i = 0; i < (int) CylinderInterval_temp.size(); i++)
    {
        cout<<CylinderInterval_temp[i].LR.first<<", "<<CylinderInterval_temp[i].LR.second<<endl;
    }
    getchar();*/

    return CylinderInterval_temp;
}



void Before_UpdateMacroInDatastruct(vector<CornerNode*> &AllCornerNode, Macro* Macro_temp, vector<CornerNode*> CornerNode_temp)
{
    CornerNode *Head, *Tail;
    Head = Tail = NULL;
//    cout<<"macro_id : "<<Macro_temp->macro_id<<endl;
    for(unsigned int i = 0; i < CornerNode_temp.size(); i++)
    {
        CornerNode_temp[i]->Top = NULL;
        CornerNode_temp[i]->Bottom = NULL;
//        cout<<"new "<<CornerNode_temp[i]->CornerNode_id<<endl;
        if(!(CornerNode_temp[i]->rectangle.lly > Macro_temp->lg.ury || CornerNode_temp[i]->rectangle.ury < Macro_temp->lg.lly))
        {
            if(Head == NULL)
            {
//                cout<<"head "<<CornerNode_temp[i]->CornerNode_id<<endl;
                Head = CornerNode_temp[i];
                Tail = CornerNode_temp[i];
                continue;
            }
            //Top Bot link
            Top_Bot_Link(Tail, CornerNode_temp[i]);
            //update tail
            Tail = CornerNode_temp[i];

        }
        else
        {
            cout<<"GG"<<endl;
        }
    }
    CornerNode* ptr = Head;
    while(ptr != NULL)
    {
//        cout<<ptr->CornerNode_id<<" jjj ";
        ptr = ptr->Top;
    }
    UpdateMacroInDatastruct(AllCornerNode, Macro_temp, Head, Tail);
}

void UpdateMacroInDatastruct(vector<CornerNode*> &AllCornerNode, Macro* Macro_temp, CornerNode* Head, CornerNode* Tail)
{
    vector<CornerNode*> For_SearchObjectAroundMacro;
    CornerNode* ptr = Head;
//    cout<<"******Cornernode ori List : ";
//    while(ptr != NULL)
//    {
//        cout<<"\nID : "<<ptr->CornerNode_id<<"\n ";
//        Boundary_Cout(ptr->rectangle);
//
//        if(ptr->rectangle.urx - Macro_temp->lg.urx < 0)
//        {
//            cout<<"ERROR : MACRO OUTOF  BLANK"<<endl;
//            cout<<"********"<<ptr->rectangle.urx - Macro_temp->lg.urx<<endl;
//            exit(1);
//            //getchar();
//        }
//
//        For_SearchObjectAroundMacro.push_back(ptr);
//        ptr = ptr->Top;
//    }
//    cout<<endl;


    if(Head->rectangle.ury == Macro_temp->lg.lly)
    {
        Head = Head->Top;
        if(Head->Bottom != NULL)
        {
            Head->Bottom->Top = NULL;
            Head->Bottom = NULL;
        }
    }
    if(Head->rectangle.lly != Macro_temp->lg.lly)
    {
        CornerNode* CornerNode_temp = new CornerNode(Blank, (int)AllCornerNode.size(), Head->rectangle.llx, Macro_temp->lg.lly, Head->rectangle.urx, Head->rectangle.ury);
        Head->rectangle.ury = Macro_temp->lg.lly;
        AllCornerNode.push_back(CornerNode_temp);
        Head->rectangle.ury = Macro_temp->lg.lly;

        CornerNode_temp->N = Head->N;
        CornerNode_temp->E = Head->E;
        CornerNode_temp->S = Head;
        Head->N = CornerNode_temp;


        // Update Top edge
        ptr = CornerNode_temp->N;
        UpdateTopEdge(ptr, CornerNode_temp);
//        cout<<"Update Top edge"<<endl;
        // Update Right edge
        ptr = CornerNode_temp->E;
        UpdateRightEdge(ptr, CornerNode_temp);
        Head->E = ptr;
        // Update Left edge
        ptr = Head->W;
        UpdateLeftEdge(ptr, Head);
        CornerNode_temp->W = ptr;
        UpdateLeftEdge(ptr, CornerNode_temp);
//        cout<<"Update Left edge"<<endl;

        ///2021.01
        CornerNode* SliceE = Head->E;
        CornerNode* SliceW = CornerNode_temp->W;
        while(SliceE != NULL && SliceE->NodeType == Blank)
        {
            if(SliceE->rectangle.lly == Macro_temp->lg.lly || SliceE->rectangle.ury == Macro_temp->lg.lly)
                break;
            CornerNode* CornerNode_temp = new CornerNode(Blank, (int)AllCornerNode.size(), SliceE->rectangle.llx, Macro_temp->lg.lly, SliceE->rectangle.urx, SliceE->rectangle.ury);
            ///2021.01
            //CornerNode_temp->Overlap_Macros.insert(SliceE->Overlap_Macros.begin(), SliceE->Overlap_Macros.end());
            //////
            SliceE->rectangle.ury = Macro_temp->lg.lly;
            AllCornerNode.push_back(CornerNode_temp);
            SliceE->rectangle.ury = Macro_temp->lg.lly;

            CornerNode_temp->N = SliceE->N;
            CornerNode_temp->E = SliceE->E;
            CornerNode_temp->S = SliceE;
            SliceE->N = CornerNode_temp;

            // Update Top edge
            ptr = CornerNode_temp->N;
            UpdateTopEdge(ptr, CornerNode_temp);
            // Update Right edge
            ptr = CornerNode_temp->E;
            UpdateRightEdge(ptr, CornerNode_temp);
            SliceE->E = ptr;
            // Update Left edge
            ptr = SliceE->W;
            UpdateLeftEdge(ptr, SliceE);
            CornerNode_temp->W = ptr;
            UpdateLeftEdge(ptr, CornerNode_temp);

            SliceE = SliceE->E;
            continue;
        }
        while(SliceW != NULL && SliceW->NodeType == Blank)
        {
            if(SliceW->rectangle.lly == Macro_temp->lg.lly || SliceW->rectangle.ury == Macro_temp->lg.lly)
                break;
            CornerNode* CornerNode_temp = new CornerNode(Blank, (int)AllCornerNode.size(), SliceW->rectangle.llx, Macro_temp->lg.lly, SliceW->rectangle.urx, SliceW->rectangle.ury);
            ///2021.01
            //CornerNode_temp->Overlap_Macros.insert(SliceW->Overlap_Macros.begin(), SliceW->Overlap_Macros.end());
            //////
            SliceW->rectangle.ury = Macro_temp->lg.lly;
            AllCornerNode.push_back(CornerNode_temp);
            SliceW->rectangle.ury = Macro_temp->lg.lly;

            CornerNode_temp->N = SliceW->N;
            CornerNode_temp->E = SliceW->E;
            CornerNode_temp->S = SliceW;
            SliceW->N = CornerNode_temp;

            // Update Top edge
            ptr = CornerNode_temp->N;
            UpdateTopEdge(ptr, CornerNode_temp);
            // Update Right edge
            ptr = CornerNode_temp->E;
            UpdateRightEdge(ptr, CornerNode_temp);
            SliceW->E = ptr;
            // Update Left edge
            ptr = SliceW->W;
            UpdateLeftEdge(ptr, SliceW);
            CornerNode_temp->W = ptr;
            UpdateLeftEdge(ptr, CornerNode_temp);

            SliceW = SliceW->W;
            continue;
        }
        //////

        // Remove Head from list
        if(Head == Tail)
        {
            Head = Tail = CornerNode_temp;
        }
        else
        {
            Top_Bot_Link(CornerNode_temp, Head->Top);
            Head->Top = NULL;
            Head = CornerNode_temp;
        }
//        cout<<"Remove Head from list"<<endl;

    }
    if(Tail->rectangle.ury != Macro_temp->lg.ury)
    {
//        cout<<"Tail ID : "<<Tail->CornerNode_id<<endl;
//        cout<<"Tail->rectangle.ury"<<endl;
//        Boundary_Cout(Tail->rectangle);
        CornerNode* CornerNode_temp = new CornerNode(Blank, (int)AllCornerNode.size(), Tail->rectangle.llx, Tail->rectangle.lly, Tail->rectangle.urx, Macro_temp->lg.ury);
//        AllCornerNode[Tail->CornerNode_id]->rectangle.lly = Macro_temp->lg.ury;
        Tail->rectangle.lly = Macro_temp->lg.ury;
//        cout<<"Update Tail->rectangle.lly = Macro_temp->lg.ury : "<<Tail->rectangle.lly<<"   "<<Macro_temp->lg.ury<<endl;
//        cout<<"AllCornerNode[6] : "<<AllCornerNode[6]->rectangle.lly<<endl;
//        cout<<"Update Tail->rectangle.lly = Macro_temp->lg.ury : "<<Tail->rectangle.lly<<"   "<<Macro_temp->lg.ury<<endl;
        AllCornerNode.push_back(CornerNode_temp);

        CornerNode_temp->S = Tail->S;
        CornerNode_temp->W = Tail->W;
        CornerNode_temp->N = Tail;
        Tail->S = CornerNode_temp;

        // Update Bottom edge
//        cout<<"AllCornerNode[6] : "<<AllCornerNode[6]->rectangle.lly<<endl;
        ptr = CornerNode_temp->S;
        UpdateBottomEdge(ptr, CornerNode_temp);
//        cout<<"AllCornerNode[6] : "<<AllCornerNode[6]->rectangle.lly<<endl;
//        cout<<"Update Bottom edge"<<endl;
        // Update Right edge
        ptr = Tail->E;
        UpdateRightEdge(ptr, Tail);
        CornerNode_temp->E = ptr;
        UpdateRightEdge(ptr, CornerNode_temp);
//        cout<<"AllCornerNode[6] : "<<AllCornerNode[6]->rectangle.lly<<endl;
//        cout<<"Update Right edge"<<endl;
        // Update left edge
        ptr = CornerNode_temp->W;
        UpdateLeftEdge(ptr, CornerNode_temp);
        Tail->W = ptr;
//        cout<<"AllCornerNode[6] : "<<AllCornerNode[6]->rectangle.lly<<endl;
//        cout<<"Update left edge"<<endl;
        //Remove Tail from list

        ///2021.01
        CornerNode* SliceE = CornerNode_temp->E;
        CornerNode* SliceW = Tail->W;
        while(SliceE != NULL && SliceE->NodeType == Blank)
        {
            if(SliceE->rectangle.ury == Macro_temp->lg.ury || SliceE->rectangle.lly == Macro_temp->lg.ury)
                break;
            CornerNode* CornerNode_temp = new CornerNode(Blank, (int)AllCornerNode.size(), SliceE->rectangle.llx, SliceE->rectangle.lly, SliceE->rectangle.urx, Macro_temp->lg.ury);
            ///2021.01
            //CornerNode_temp->Overlap_Macros.insert(SliceE->Overlap_Macros.begin(), SliceE->Overlap_Macros.end());
            //////
            SliceE->rectangle.lly = Macro_temp->lg.ury;
            AllCornerNode.push_back(CornerNode_temp);

            CornerNode_temp->S = SliceE->S;
            CornerNode_temp->W = SliceE->W;
            CornerNode_temp->N = SliceE;
            SliceE->S = CornerNode_temp;

            ptr = CornerNode_temp->S;
            UpdateBottomEdge(ptr, CornerNode_temp);

            ptr = SliceE->E;
            UpdateRightEdge(ptr, SliceE);
            CornerNode_temp->E = ptr;
            UpdateRightEdge(ptr, CornerNode_temp);

            ptr = CornerNode_temp->W;
            UpdateLeftEdge(ptr, CornerNode_temp);
            SliceE->W = ptr;

            SliceE = SliceE->E;
            continue;
        }
        while(SliceW != NULL && SliceW->NodeType == Blank)
        {
            if(SliceW->rectangle.ury == Macro_temp->lg.ury || SliceW->rectangle.lly == Macro_temp->lg.ury)
                break;
            CornerNode* CornerNode_temp = new CornerNode(Blank, (int)AllCornerNode.size(), SliceW->rectangle.llx, SliceW->rectangle.lly, SliceW->rectangle.urx, Macro_temp->lg.ury);
            ///2021.01
            //CornerNode_temp->Overlap_Macros.insert(SliceW->Overlap_Macros.begin(), SliceW->Overlap_Macros.end());
            //////
            SliceW->rectangle.lly = Macro_temp->lg.ury;
            AllCornerNode.push_back(CornerNode_temp);

            CornerNode_temp->S = SliceW->S;
            CornerNode_temp->W = SliceW->W;
            CornerNode_temp->N = SliceW;
            SliceW->S = CornerNode_temp;

            ptr = CornerNode_temp->S;
            UpdateBottomEdge(ptr, CornerNode_temp);

            ptr = SliceW->E;
            UpdateRightEdge(ptr, SliceW);
            CornerNode_temp->E = ptr;
            UpdateRightEdge(ptr, CornerNode_temp);

            ptr = CornerNode_temp->W;
            UpdateLeftEdge(ptr, CornerNode_temp);
            SliceW->W = ptr;

            SliceW = SliceW->W;
            continue;
        }
        //////

        if(Tail == Head)
        {
            Tail = Head = CornerNode_temp;
        }
        else
        {
            Top_Bot_Link(Tail->Bottom, CornerNode_temp);
            Tail->Bottom = NULL;
            Tail = CornerNode_temp;
        }
//        cout<<"AllCornerNode[6] : "<<AllCornerNode[6]->rectangle.lly<<endl;
//        cout<<"Remove Tail from list"<<endl;
    }
    ptr = Head;
//    cout<<"LIST"<<endl;
//    cout<<"Macro lly : "<<Macro_temp->lg.lly<<" ury : "<<Macro_temp->lg.ury<<" llx : "<<Macro_temp->lg.llx<<" urx : "<<Macro_temp->lg.urx<<endl;
//    while(ptr != NULL)
//    {
//        cout<<"ID : "<<ptr->CornerNode_id<<" lly : "<<ptr->rectangle.lly<<" ury : "<<ptr->rectangle.ury<<" llx : "<<ptr->rectangle.llx<<" urx : "<<ptr->rectangle.urx<<endl;
//        if(ptr->E != NULL)
//            cout<<"E : "<<ptr->E->CornerNode_id<<endl;
//        ptr = ptr->Top;
//    }
    //getchar();

//    cout<<"Update"<<endl;
//    cout<<"Macro CornerNode : "<<AllCornerNode.size()<<endl;
    CornerNode* CornerNode_temp_for_Macro = new CornerNode(Movable, (int)AllCornerNode.size(), Macro_temp->lg.llx, Macro_temp->lg.lly, Macro_temp->lg.urx, Macro_temp->lg.ury);
    CornerNode_temp_for_Macro->lef_type_ID = Macro_temp->lef_type_ID;
    /// ADD 2021.05
    CornerNode_temp_for_Macro->macro = Macro_temp;
    //////
    AllCornerNode.push_back(CornerNode_temp_for_Macro);

    pair<vector<vector<CornerNode*> >, vector<vector<CornerNode*> > > left_right_merge_space;
    ptr = Head;
    vector<int> delete_cornernode;
    set<int> List_Member_ID;

    delete_cornernode.reserve(AllCornerNode.size());
    while(ptr != NULL)
    {
//        cout<<"*************start "<<ptr->CornerNode_id<<endl;
        CornerNode* Ptr = Head;
//        while(Ptr != NULL)
//        {
//            cout<<"ID : "<<Ptr->CornerNode_id<<" lly : "<<Ptr->rectangle.lly<<" ury : "<<Ptr->rectangle.ury<<" llx : "<<Ptr->rectangle.llx<<" urx : "<<Ptr->rectangle.urx<<endl;
//            if(Ptr->W != NULL)
//                cout<<"W : "<<Ptr->W->CornerNode_id<<endl;
//            if(Ptr->E != NULL)
//                cout<<"E : "<<Ptr->E->CornerNode_id<<endl;
//            Ptr = Ptr->Top;
//        }
        bool flag = 0;
        CornerNode* CornerNode_temp;
        if(ptr->rectangle.llx == Macro_temp->lg.llx)
        {
            CornerNode* ptr_W = ptr->W;
            vector<CornerNode*> temp;
            while(1)
            {
                if(ptr_W == NULL || ptr_W->rectangle.ury > ptr->rectangle.ury)
                {
                    break;
                }
                ptr_W->E = CornerNode_temp_for_Macro;
//                cout<<"ptr_W : "<<ptr_W->CornerNode_id<<endl;
                temp.push_back(ptr_W);
                ptr_W = ptr_W->N;
            }
            if(temp.size() == 0)
            {
                if(ptr->W != NULL)
                {
                    temp.push_back(ptr->W);
                    left_right_merge_space.first.push_back(temp);
                }
                else
                {
//                    cout<<"UPDATE MACRO S 1089"<<endl;
//                    cout<<"S ID : "<<ptr->CornerNode_id<<endl;
                    CornerNode_temp_for_Macro->W = NULL;
                    CornerNode_temp_for_Macro->S = ptr->S;
                }
            }
            else
            {
                left_right_merge_space.first.push_back(temp);
            }
//                cout<<"left delete : "<<ptr->CornerNode_id<<endl;
//                delete_cornernode.push_back(ptr->CornerNode_id);

        }
        else
        {
            /*if(left_right_merge_space.first.size() != 0 && left_right_merge_space.first.back()[0]->NodeType == Blank
                    && left_right_merge_space.first.back()[0]->rectangle.llx == ptr->rectangle.llx)
            {
                left_right_merge_space.first.back().push_back(ptr);
            }
            else
            {*/
                CornerNode_temp = ptr;
                flag = 1;
            //}     //************ commented by m108yjhuang, this comment is used for not merging. 2021/03/02 ************
        }
        if(flag == 1)
        {
            vector<CornerNode*> temp;
            temp.resize(1);
            temp[0] = CornerNode_temp;
            left_right_merge_space.first.push_back(temp);
        }
//        cout<<"left "<<ptr->CornerNode_id<<endl;
        Ptr = Head;
        while(Ptr != NULL)
        {
//            cout<<"ID : "<<Ptr->CornerNode_id<<" lly : "<<Ptr->rectangle.lly<<" ury : "<<Ptr->rectangle.ury<<" llx : "<<Ptr->rectangle.llx<<" urx : "<<Ptr->rectangle.urx<<endl;
//            if(Ptr->E != NULL)
//                cout<<"E : "<<Ptr->E->CornerNode_id<<endl;
            Ptr = Ptr->Top;
        }

        flag = 0;
        if(ptr->rectangle.urx == Macro_temp->lg.urx)
        {
            CornerNode* ptr_E = ptr->E;
            vector<CornerNode*> temp;
            while(ptr_E != NULL)
            {
                if(ptr_E->rectangle.lly < ptr->rectangle.lly)
                {
                    break;
                }
                ptr_E->W = CornerNode_temp_for_Macro;
                ptr_E = ptr_E->S;
            }
            if(temp.size() == 0)
            {
                if(ptr->E != NULL)
                {
                    temp.push_back(ptr->E);
                    delete_cornernode.push_back(ptr->CornerNode_id);
                    left_right_merge_space.second.push_back(temp);
                }
                else
                {
                    CornerNode_temp_for_Macro->E = NULL;
                    CornerNode_temp_for_Macro->N = ptr->N;
                }
            }
            else
            {
                delete_cornernode.push_back(ptr->CornerNode_id);
                left_right_merge_space.second.push_back(temp);
            }
//            cout<<"right delete : "<<ptr->CornerNode_id<<endl;

        }
        else
        {
            /*if(left_right_merge_space.second.size() != 0 && left_right_merge_space.second.back()[0]-> NodeType == Blank
                    && left_right_merge_space.second.back()[0]->rectangle.urx == ptr->rectangle.urx)
            {
                left_right_merge_space.second.back().push_back(ptr);
            }
            else
            {*/
                CornerNode_temp = ptr;
                flag = 1;
            //}     //************ commented by m108yjhuang, this comment is used for not merging. 2021/03/02 ************
        }
        if(flag == 1)
        {
            vector<CornerNode*> temp;
            temp.resize(1);
            temp[0] = CornerNode_temp;
            left_right_merge_space.second.push_back(temp);
        }
        List_Member_ID.insert(ptr->CornerNode_id);
//        cout<<"right "<<ptr->CornerNode_id<<endl;
        Ptr = Head;
//        while(Ptr != NULL)
//        {
//            cout<<"ID : "<<Ptr->CornerNode_id<<" lly : "<<Ptr->rectangle.lly<<" ury : "<<Ptr->rectangle.ury<<" llx : "<<Ptr->rectangle.llx<<" urx : "<<Ptr->rectangle.urx<<endl;
//            if(Ptr->E != NULL)
//                cout<<"E : "<<Ptr->E->CornerNode_id<<endl;
//            if(Ptr->S != NULL)
//                cout<<"S : "<<Ptr->S->CornerNode_id<<endl;
//            Ptr = Ptr->Top;
//        }
        ptr = ptr->Top;
    }
    // Generate Left Blank space && Update in Data struct
//    cout<<"Generate Left Blank space && Update in Data struct"<<endl;

    CornerNode* LeftTopNewCornerNode = NULL;
    CornerNode* RightTopNewCornerNode = NULL;
//    for(int i = 0; i < (int)left_right_merge_space.first.size(); i++)
//    {
//        if((int)left_right_merge_space.first[i].size() == 0)
//        {
//            cout<<"(int)left_right_merge_space.first[i].size() == 0"<<endl;
//        }
//        for(int j = 0; j < (int)left_right_merge_space.first[i].size(); j++)
//        {
////            cout<<" i : "<<i<<" j : "<<j<<endl;
//            if(left_right_merge_space.first[i][j] == NULL)
//            {
//                cout<<"left_right_merge_space.first[i][j] == NULL"<<endl;
//            }
//            else
//                cout<<left_right_merge_space.first[i][j]->CornerNode_id<<endl;
//        }
//    }
    for(int i = 0; i < (int)left_right_merge_space.first.size(); i++)
    {
//        cout<<"i******"<<i<<"  "<<left_right_merge_space.first.size()<<endl;
        CornerNode* CornerNode_temp = NULL;
        if(left_right_merge_space.first[i].size() > 0 && left_right_merge_space.first[i][0]->NodeType == Blank)
        {
            /*double llx, lly, urx, ury;
            llx = left_right_merge_space.first[i][0]->rectangle.llx;
            lly = left_right_merge_space.first[i][0]->rectangle.lly;
            urx = CornerNode_temp_for_Macro->rectangle.llx;
            ury = left_right_merge_space.first[i].back()->rectangle.ury;
//            cout<<"left_right_merge_space.first[i].back() ID : "<<left_right_merge_space.first[i].back()->CornerNode_id<<endl;
            CornerNode* CornerNode_Temp = new CornerNode(Blank, (int)AllCornerNode.size(), llx, lly, urx, ury);
//            cout<<"New Cornernode : "<<(int)AllCornerNode.size()<<endl;
            AllCornerNode.push_back(CornerNode_Temp);
            CornerNode_Temp->W = left_right_merge_space.first[i][0]->W;
            CornerNode_Temp->E = CornerNode_temp_for_Macro;
            CornerNode_Temp->S = left_right_merge_space.first[i][0]->S;
            ptr = CornerNode_Temp->W;
            UpdateLeftEdge(ptr, CornerNode_Temp);
            LeftTopNewCornerNode = CornerNode_Temp;
            CornerNode_temp = CornerNode_Temp;*/ /// COMMENT 2021.03.19
//            cout<<"out"<<endl;
            ///2021.01
            if(left_right_merge_space.first[i][0]->rectangle.urx != Macro_temp->lg.llx)
            {
                double llx, lly, urx, ury;
                llx = left_right_merge_space.first[i][0]->rectangle.llx;
                lly = left_right_merge_space.first[i][0]->rectangle.lly;
                urx = CornerNode_temp_for_Macro->rectangle.llx;
                ury = left_right_merge_space.first[i].back()->rectangle.ury;
//                cout<<"left_right_merge_space.first[i].back() ID : "<<left_right_merge_space.first[i].back()->CornerNode_id<<endl;
                CornerNode* CornerNode_Temp = new CornerNode(Blank, (int)AllCornerNode.size(), llx, lly, urx, ury);
                ///2021.01
                //CornerNode_Temp->Overlap_Macros.insert(left_right_merge_space.first[i][0]->Overlap_Macros.begin(), left_right_merge_space.first[i][0]->Overlap_Macros.end());
                //////
//                cout<<"New Cornernode : "<<(int)AllCornerNode.size()<<endl;
                AllCornerNode.push_back(CornerNode_Temp);
                CornerNode_Temp->W = left_right_merge_space.first[i][0]->W;

                if(CornerNode_temp_for_Macro->rectangle.lly < CornerNode_Temp->rectangle.ury && CornerNode_temp_for_Macro->rectangle.ury >= CornerNode_Temp->rectangle.ury)
                    CornerNode_Temp->E = CornerNode_temp_for_Macro;

                CornerNode_Temp->S = left_right_merge_space.first[i][0]->S;
                ptr = CornerNode_Temp->W;
                UpdateLeftEdge(ptr, CornerNode_Temp);
                LeftTopNewCornerNode = CornerNode_Temp;
                CornerNode_temp = CornerNode_Temp;
//                cout<<"out"<<endl;
            }
            else
            {

                if(!(CornerNode_temp_for_Macro->rectangle.ury < left_right_merge_space.first[i][0]->rectangle.ury) && CornerNode_temp_for_Macro->rectangle.lly < left_right_merge_space.first[i][0]->rectangle.ury)
                    left_right_merge_space.first[i][0]->E = CornerNode_temp_for_Macro;
                ptr = left_right_merge_space.first[i][0]->W;
                UpdateLeftEdge(ptr, left_right_merge_space.first[i][0]);
                LeftTopNewCornerNode = left_right_merge_space.first[i][0];
                CornerNode_temp = left_right_merge_space.first[i][0];
            }
            //////
        }
        if(i != 0)
        {
            // Update prev N link
//            int k = 0;
//            cout<<"Update prev N link"<<endl;
            CornerNode *ptr_temp, *prev_temp;

            if(left_right_merge_space.first[i][0]->NodeType != Blank)
            {
                ptr_temp = left_right_merge_space.first[i][0];
            }
            else
            {
                ptr_temp = CornerNode_temp;
            }
//            cout<<k++<<endl;
            if(left_right_merge_space.first[i-1][0]->NodeType != Blank)
            {
                prev_temp = left_right_merge_space.first[i-1].back();
            }
            else if(left_right_merge_space.first[i][0]->NodeType == Blank)
            {
                prev_temp = AllCornerNode[AllCornerNode.size()-2];
            }
            else
            {
                prev_temp = AllCornerNode.back();
            }
//            cout<<k++<<endl;
//                prev_temp->N = ptr_temp;

            if(left_right_merge_space.first[i-1].back() == left_right_merge_space.first[i][0]->S)
            {
                if(left_right_merge_space.first[i-1][0]->NodeType == Blank)
                {
                    CornerNode *CornerNode_update_S;
                    if(left_right_merge_space.first[i][0]->NodeType == Blank)
                    {
                        CornerNode_update_S = AllCornerNode[AllCornerNode.size()-2];
                    }
                    else
                    {
                        CornerNode_update_S = AllCornerNode[AllCornerNode.size()-1];
                    }
                    if(CornerNode_temp != NULL)
                    {
                        //CornerNode_temp->S = CornerNode_update_S;
                        ptr = CornerNode_temp;
                    }
                    else
                    {
                        //left_right_merge_space.first[i][0]->S = CornerNode_update_S;
                        ptr = left_right_merge_space.first[i][0];
                    }
                    UpdateTopEdge(ptr, CornerNode_update_S);
                }
            }
            //Cout_CornerNode_linkID(AllCornerNode);cout<<"Come on !!"<<endl;//getchar();
//            if(prev_temp!=NULL && ptr_temp!=NULL)
//                cout<<"UpdateBottomEdge"<<prev_temp->CornerNode_id<<" **** "<<ptr_temp->CornerNode_id<<endl;
//            cout<<left_right_merge_space.first[i][0]->CornerNode_id<<endl;
//            if(ptr_temp == NULL)cout<<"ptr_temp==NULL"<<endl;
            ptr = ptr_temp->S;
            UpdateBottomEdge(ptr, ptr_temp);
            //Cout_CornerNode_linkID(AllCornerNode);cout<<"Come on again !!"<<endl;//getchar();
//            cout<<k++<<endl;
//            cout<<"Update prev N link"<<endl;
        }
        else
        {
            if(left_right_merge_space.first[i][0]->NodeType != Blank)
            {
                CornerNode_temp_for_Macro->W = left_right_merge_space.first[i][0];
            }
            else
            {
                CornerNode_temp_for_Macro->W  = CornerNode_temp;
            }
            // Update Bottom Edge
            ptr = Head->S;
            if(left_right_merge_space.first[i][0]->NodeType == Blank)
            {
                UpdateBottomEdge(ptr, CornerNode_temp);
            }
//            cout<<"UPDATE MACRO S 1332"<<endl;
            CornerNode_temp_for_Macro->S = ptr;
            UpdateBottomEdge(ptr, CornerNode_temp_for_Macro);
            Head->S = ptr;
            UpdateBottomEdge(ptr, Head);
        }
    }
//    cout<<"Update right blank space"<<endl;
    // Update right blank space
//    for(int i = 0; i < (int)left_right_merge_space.second.size(); i++)
//    {
//        for(int j = 0; j < (int)left_right_merge_space.second[i].size(); j++)
//        {
//            cout<<"i : "<<i<<" j : "<<j<<endl;
//            cout<<left_right_merge_space.second[i][j]->CornerNode_id<<endl;
//            if(left_right_merge_space.second[i][j]->E != NULL)
//                cout<<"E : "<<left_right_merge_space.second[i][j]->E->CornerNode_id<<endl;
//        }
//    }
    for(int i = 0; i < (int)left_right_merge_space.second.size(); i++)
    {
//        cout<<"left_right_merge_space.second ID : "<<left_right_merge_space.second[i][0]->CornerNode_id<<endl;
//        if(left_right_merge_space.second[i][0]->E != NULL)
//        {
//            cout<<left_right_merge_space.second[i].back()->E->CornerNode_id<<endl;
//        }
        if(left_right_merge_space.second[i].size() > 0 && left_right_merge_space.second[i][0]->NodeType == Blank)
        {
            unsigned int j;
            if(left_right_merge_space.second[i][0]->rectangle.urx == CornerNode_temp_for_Macro->rectangle.urx)
            {
                j = 0;
            }
            else
            {
                j = 1;
            }
            for( ; j < left_right_merge_space.second[i].size(); j++)
            {
//                cout<<"right delete : "<<left_right_merge_space.second[i][j]->CornerNode_id<<endl;
                delete_cornernode.push_back(left_right_merge_space.second[i][j]->CornerNode_id);
            }
        }

        ///2021.02
        if(left_right_merge_space.second[i].size() > 0)
        {
            CornerNode* S_tmp = CornerNode_temp_for_Macro->S;
            UpdateBottomEdge(S_tmp, CornerNode_temp_for_Macro);
            if(S_tmp != NULL && S_tmp->rectangle.ury == left_right_merge_space.second[i][0]->rectangle.lly)
                left_right_merge_space.second[i][0]->S = S_tmp;
        }
        //////
        if(left_right_merge_space.second[i].size() > 0 && left_right_merge_space.second[i][0]->NodeType == Blank)
        {
            left_right_merge_space.second[i][0]->rectangle.ury = left_right_merge_space.second[i].back()->rectangle.ury;
            left_right_merge_space.second[i][0]->rectangle.llx = CornerNode_temp_for_Macro->rectangle.urx;
            left_right_merge_space.second[i][0]->N = left_right_merge_space.second[i].back()->N;
            left_right_merge_space.second[i][0]->E = left_right_merge_space.second[i].back()->E;
            ///2021.01
            if(CornerNode_temp_for_Macro->rectangle.lly <= left_right_merge_space.second[i][0]->rectangle.lly && CornerNode_temp_for_Macro->rectangle.ury > left_right_merge_space.second[i][0]->rectangle.lly)
                left_right_merge_space.second[i][0]->W = CornerNode_temp_for_Macro;
            //////
            //left_right_merge_space.second[i][0]->W = CornerNode_temp_for_Macro;
            left_right_merge_space.second[i][0]->S = left_right_merge_space.second[i][0]->S;
            RightTopNewCornerNode = left_right_merge_space.second[i][0];
            ptr = left_right_merge_space.second[i][0]->E;
            UpdateRightEdge(ptr, left_right_merge_space.second[i][0]);
        }

        if(i != 0)
        {
            ptr = left_right_merge_space.second[i-1][0]->N;
            UpdateTopEdge(ptr, left_right_merge_space.second[i-1][0]);
        }
    }
//    cout<<"end"<<endl;
    ptr = Tail->N;
    if(left_right_merge_space.second.size() != 0 && left_right_merge_space.second.back()[0]->NodeType == Blank)
    {
        UpdateTopEdge(ptr, RightTopNewCornerNode);
    }
//    cout<<"end"<<endl;
    CornerNode_temp_for_Macro->N = ptr;
    UpdateTopEdge(ptr, CornerNode_temp_for_Macro);
//    cout<<"eeend"<<endl;
    /*if(left_right_merge_space.first.size() != 0 && left_right_merge_space.first.back()[0]->NodeType == Blank && LeftTopNewCornerNode != NULL)
    {
        LeftTopNewCornerNode->N = ptr;
        UpdateTopEdge(ptr, LeftTopNewCornerNode);
    }*/ /// COMMENT 2021.03.19

    ///2021.01
    if(left_right_merge_space.first.size() != 0 && left_right_merge_space.first.back()[0]->NodeType == Blank && LeftTopNewCornerNode != NULL
       && LeftTopNewCornerNode != ptr && LeftTopNewCornerNode->rectangle.ury == ptr->rectangle.lly)
    {
        LeftTopNewCornerNode->N = ptr;
        UpdateTopEdge(ptr, LeftTopNewCornerNode);
    }
    //////
//    cout<<"end"<<endl;
    if(left_right_merge_space.second.size() != 0)
        CornerNode_temp_for_Macro->E = left_right_merge_space.second.back()[0];
    ptr = Head;
//    cout<<"end"<<endl;
    while(ptr!=NULL)
    {
        CornerNode* temp = ptr;
        ptr = ptr->Top;
        temp->Bottom = NULL;
        temp->Top = NULL;
    }
//    cout<<"end"<<endl;
//    cout<<"delete_cornernode : ";
    for(unsigned int i = 0; i < delete_cornernode.size(); i++)
    {
//        cout<<delete_cornernode[i]<<"  ";
        if(AllCornerNode[delete_cornernode[i]] == NULL)
        {
            continue;
        }
        delete AllCornerNode[delete_cornernode[i]];
        AllCornerNode[delete_cornernode[i]] = NULL;
    }
//    cout<<endl;

///plot & debug
//    vector<pair<int,Boundary> > plot_boundary;
//    plot_boundary.resize(AllCornerNode.size());
//    for(unsigned int i = 0; i < AllCornerNode.size(); i++)
//    {
//        Boundary temp;
//        if(AllCornerNode[i] == NULL)
//        {
//            plot_boundary[i] = make_pair(-1, temp);
//            continue;
//        }
//        int color = 0;
//        temp.llx = AllCornerNode[i]->rectangle.llx;
//        temp.lly = AllCornerNode[i]->rectangle.lly;
//        temp.urx = AllCornerNode[i]->rectangle.urx;
//        temp.ury = AllCornerNode[i]->rectangle.ury;
//        if(AllCornerNode[i]->NodeType == Blank)
//        {
//            color = 2;
//        }
//        else if(AllCornerNode[i]->NodeType == Fixed)
//        {
//            color = 1;
//        }
//        else
//        {
//            color = 3;
//        }
//        plot_boundary[i] = make_pair(color, temp);
//    }
//    string a, b;
//    a = "./output/AllCornerNode/AllcornerNode";
//    b = int2str((int) AllCornerNode.size());
//    a += b;
//    a += "_macroID";
//    b = int2str(Macro_temp->macro_id);
//    a += b;
//    a += ".m";
//    if(AllCornerNode.size() > 0)
//    {
//        cout<<a<<endl;
//        Plot_rectangle_w_color(plot_boundary, a);
////        getchar();
//    }
//    Cout_CornerNode_linkID(AllCornerNode);

}



void Top_Bot_Link(CornerNode* a, CornerNode* b)
{
    a->Top = b;
    b->Bottom = a;
}


void ReturnLeftBotCoordinate(int &x, int &y, unsigned int PlaceCornerType, Macro* temp)
{
    switch(PlaceCornerType)
    {
    case LeftBot:
        break;
    case LeftTop:
        y -= temp->cal_h;
        break;
    case RightBot:
        x -= temp->cal_w;
        break;
    case RightTop:
        x -= temp->cal_w;
        y -= temp->cal_h;
        break;
    }
    return ;
}

void UpdateLeftEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode)
{
    while(1)
    {
        if(ptr == NULL || ptr->rectangle.ury > UpdateCornerNode->rectangle.ury)
        {
            break;
        }
        ptr->E = UpdateCornerNode;
        ptr = ptr->N;
    }
}

void UpdateRightEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode)
{
    while(ptr != NULL)
    {
        if(ptr->rectangle.lly < UpdateCornerNode->rectangle.lly)
        {
            break;
        }
        ptr->W = UpdateCornerNode;
        ptr = ptr->S;
    }
}

void UpdateBottomEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode)
{
    while(ptr != NULL)
    {
        if(ptr->rectangle.urx > UpdateCornerNode->rectangle.urx)
        {
            break;
        }
        else
        {
            ptr->N = UpdateCornerNode;
        }
        ptr = ptr->E;
    }
}

void UpdateTopEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode)
{
    while(ptr != NULL)
    {
        if(ptr->rectangle.llx < UpdateCornerNode->rectangle.llx)
        {
            break;
        }
        else
        {
            ptr->S = UpdateCornerNode;
        }
        ptr = ptr->W;
    }
}

vector<CornerNode*> NeighborFindingTopEdge(CornerNode* ptrCornerNode)
{
    CornerNode* ptr = ptrCornerNode->N;
    vector<CornerNode*> Neighborhood;
    while(ptr != NULL && ptr->rectangle.urx > ptrCornerNode->rectangle.llx)
    {
        if(ptr->NodeType == Blank)
            Neighborhood.push_back(ptr);
        ptr = ptr->W;
    }
    return Neighborhood;
}

vector<CornerNode*> NeighborFindingBottomEdge(CornerNode* ptrCornerNode)
{
    CornerNode* ptr = ptrCornerNode->S;
    vector<CornerNode*> Neighborhood;
    while(ptr != NULL && ptr->rectangle.llx < ptrCornerNode->rectangle.urx)
    {
        if(ptr->NodeType == Blank)
            Neighborhood.push_back(ptr);
        ptr = ptr->E;
    }
    return Neighborhood;
}

vector<CornerNode*> NeighborFindingLeftEdge(CornerNode* ptrCornerNode)
{
    CornerNode* ptr = ptrCornerNode->W;
    vector<CornerNode*> Neighborhood;
    while(ptr != NULL && ptr->rectangle.lly < ptrCornerNode->rectangle.ury)
    {
        if(ptr->NodeType == Blank)
            Neighborhood.push_back(ptr);
        ptr = ptr->N;
    }
    return Neighborhood;
}

vector<CornerNode*> NeighborFindingRightEdge(CornerNode* ptrCornerNode)
{
    CornerNode* ptr = ptrCornerNode->E;
    vector<CornerNode*> Neighborhood;
    while(ptr != NULL && ptr->rectangle.ury > ptrCornerNode->rectangle.lly)
    {
        if(ptr->NodeType == Blank)
            Neighborhood.push_back(ptr);
        ptr = ptr->S;
    }
    return Neighborhood;
}





void DeleteCornerNode(vector<CornerNode*> &deletecornernode)
{

    for(unsigned int i = 0; i < deletecornernode.size(); i++)
    {
        CornerNode* t = deletecornernode[i];
        if(t != NULL)
        {
            delete t;
            t = NULL;
        }
    }
    deletecornernode.clear();
    vector<CornerNode*> temp;
    swap(temp, deletecornernode);

}

void UpdateInfoInPreplacedInCornerStitching(vector<CornerNode*> temp, vector<CornerNode*> &temp2)
{

    temp2.resize(temp.size());
    for(int i = 0; i < (int)temp.size(); i++)
    {
        CornerNode* CornerNodetemp = new CornerNode(temp[i]->NodeType, temp[i]->CornerNode_id, temp[i]->rectangle.llx, temp[i]->rectangle.lly, temp[i]->rectangle.urx, temp[i]->rectangle.ury);
        temp2[i] = CornerNodetemp;
//        cout<<"ID : "<<CornerNodetemp->CornerNode_id<<endl;
    }
    for(int i = 0; i < (int)temp.size(); i++)
    {
//        cout<<"Size : "<<temp.size()<<endl;
        if(temp[i]->N != NULL)
        {
            temp2[i]->N = temp2[temp[i]->N->CornerNode_id];
//            cout<<"N : "<<temp2[i]->N->CornerNode_id<<endl;
        }
        else
        {
            temp2[i]->N = NULL;
//            cout<<"N : NULL"<<endl;
        }
        if(temp[i]->S != NULL)
        {
            temp2[i]->S = temp2[temp[i]->S->CornerNode_id];
//            cout<<"S : "<<temp2[i]->S->CornerNode_id<<endl;
        }
        else
        {
            temp2[i]->S = NULL;
//            cout<<"S : NULL"<<endl;
        }
        if(temp[i]->W != NULL)
        {
            temp2[i]->W = temp2[temp[i]->W->CornerNode_id];
//            cout<<"W : "<<temp2[i]->W->CornerNode_id<<endl;
        }
        else
        {
            temp2[i]->W = NULL;
//            cout<<"W : NULL"<<endl;
        }
        if(temp[i]->E != NULL)
        {
            temp2[i]->E = temp2[temp[i]->E->CornerNode_id];
//            cout<<"E : "<<temp2[i]->E->CornerNode_id<<endl;
        }
        else
        {
            temp2[i]->E = NULL;
//            cout<<"E : NULL"<<endl;
        }
    }

    temp2.reserve(MovableMacro_ID.size() * 4);
}








void UpdateFixedPreplacedInCornerStitching()
{
    for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if(corner_temp->NodeType == Fixed)
        {
            FixedPreplacedInCornerStitching.push_back(corner_temp->CornerNode_id);
        }
    }
}

void Directed_AreaEnumeration(Boundary &BBox, CornerNode* StartNode, queue<CornerNode*> &CornerNodeSet)
{
    if(!(BBox.urx - BBox.llx > 0) || !(BBox.ury - BBox.lly > 0))
    {
        cout<<"llx: "<<BBox.llx<<"\turx: "<<BBox.urx<<"\tlly: "<<BBox.lly<<"\tury: "<<BBox.ury<<endl;
        cout<<"Directed_AreaEnumeration BBox Area Error !!"<<endl;
        exit(1);
    }
    CornerNode* BBoxStarNode = CornerNodePointSearch(make_pair(BBox.llx, BBox.ury), StartNode);

    queue<CornerNode*> VerticalSearchingBBoxLeftEdgeNode;
    VerticalSearchingBBoxLeftEdgeNode.push(BBoxStarNode);
    Directed_VerticalSearchingBBoxLeftEdge(BBox, BBoxStarNode, VerticalSearchingBBoxLeftEdgeNode);
//    plot_Directed_AreaEnumeration(VerticalSearchingBBoxLeftEdgeNode, BBox);
    while(!VerticalSearchingBBoxLeftEdgeNode.empty())
    {
        CornerNodeSet.push(VerticalSearchingBBoxLeftEdgeNode.front());
        Directed_HorizontalEnumeration(VerticalSearchingBBoxLeftEdgeNode.front(), BBox, CornerNodeSet);
        VerticalSearchingBBoxLeftEdgeNode.pop();
    }
}

void Directed_VerticalSearchingBBoxLeftEdge(Boundary &BBox, CornerNode* StartNode, queue<CornerNode*> &VerticalSearchingBBoxLeftEdgeNode)
{
    CornerNode* ptr = StartNode->S;
    if(ptr == NULL || !(ptr->rectangle.ury > BBox.lly))
        return;
    while(!(ptr->rectangle.urx > BBox.llx))
    {
        if(ptr->E != NULL)
        {
            ptr = ptr->E;
        }
        else
        {
            return;
        }
    }

    VerticalSearchingBBoxLeftEdgeNode.push(ptr);
    Directed_VerticalSearchingBBoxLeftEdge(BBox, ptr, VerticalSearchingBBoxLeftEdgeNode);
}

void Directed_HorizontalEnumeration(CornerNode* StartNode, Boundary &BBox, queue<CornerNode*> &CornerNodeSet)
{
    if(!(StartNode->rectangle.urx < BBox.urx) || !(StartNode->rectangle.lly < BBox.ury))
        return;

    CornerNode* ptr = StartNode->E;
    queue<CornerNode*> RightNeighbor;
    if(StartNode->rectangle.lly > BBox.lly)
    {
        while(ptr != NULL && !(ptr->rectangle.lly < StartNode->rectangle.lly))
        {
            if(ptr->rectangle.lly < BBox.ury)
                RightNeighbor.push(ptr);
            ptr = ptr->S;
        }
    }
    else
    {
        while(ptr != NULL && ptr->rectangle.ury > BBox.lly)
        {
            RightNeighbor.push(ptr);
            ptr = ptr->S;
        }
    }
    while(!RightNeighbor.empty())
    {
        CornerNodeSet.push(RightNeighbor.front());
        Directed_HorizontalEnumeration(RightNeighbor.front(), BBox, CornerNodeSet);
        RightNeighbor.pop();
    }
}

/// ADD 2021.08
map<int, MacrosSet*> Find_Surrounding_Sets(vector<CornerNode*> &AllCornerNode, Macro* macro_temp, Boundary macro_lg)
{
    Boundary expand_macro_lg;
    expand_macro_lg.llx = macro_lg.llx - 1;
    expand_macro_lg.lly = macro_lg.lly - 1;
    expand_macro_lg.urx = macro_lg.urx + 1;
    expand_macro_lg.ury = macro_lg.ury + 1;

    int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
    CornerNode* startNode = AllCornerNode[startID];
    queue<CornerNode*> CornerNodeSet;

    Directed_AreaEnumeration(expand_macro_lg, startNode, CornerNodeSet);

//    ofstream fout("check_enumeration.m");
//    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;
//
//    //chip boundary
//    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
//    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
//    fout << "fill(block_x, block_y, 'w');" << endl;


    map<int, MacrosSet*> set_id;
    while(!CornerNodeSet.empty())
    {
        CornerNode *tmp = CornerNodeSet.front();
        if(tmp == NULL)
            continue;
//        string color;
//        double llx = tmp->rectangle.llx ;
//        double urx = tmp->rectangle.urx ;
//        double lly = tmp->rectangle.lly ;
//        double ury = tmp->rectangle.ury ;
//        if(tmp->NodeType == Blank)
//            color = "y";
//        else if(tmp->NodeType == Fixed)
//            color = "c";
//        else if(tmp->NodeType == Movable)
//            color = "g";
//        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
//        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
//        fout << "fill(block_x,block_y,'"<<color<<"','facealpha',0.5);" << endl;

        if(tmp->NodeType == Movable)
        {
            if(tmp->macro->OriginalMacro->macro_name == macro_temp->macro_name)
            {
//                fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << "M" << "','fontsize',7);" << endl << endl;
            }
            else if(tmp->macro->OriginalMacro->macro_name != macro_temp->macro_name && ( (tmp->rectangle.lly == macro_lg.ury && !(tmp->rectangle.urx <= macro_lg.llx || tmp->rectangle.llx >= macro_lg.urx) ) ||
                                                                                         (tmp->rectangle.llx == macro_lg.urx && !(tmp->rectangle.ury <= macro_lg.lly || tmp->rectangle.lly >= macro_lg.ury) ) ||
                                                                                         (tmp->rectangle.ury == macro_lg.lly && !(tmp->rectangle.urx <= macro_lg.llx || tmp->rectangle.llx >= macro_lg.urx) ) ||
                                                                                         (tmp->rectangle.urx == macro_lg.llx && !(tmp->rectangle.ury <= macro_lg.lly || tmp->rectangle.lly >= macro_lg.ury) ) ) )
            {
//                fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << "*" << "','fontsize',7);" << endl << endl;
                MacrosSet* set_tmp = tmp->macro->OriginalMacro->set;
                set_id.insert( make_pair(set_tmp->id, set_tmp) );
            }
        }
        CornerNodeSet.pop();
    }
//    getchar();
    return set_id;
}
//////

void PointPacking(vector<CornerNode*> &AllCornerNode, PACKING_INFO &pack_tmp, COST_INFO &cost_info)
{
    Macro* Macro_temp = pack_tmp.Macro_temp;
    CornerNode* ptr = pack_tmp.ptr;
    if(pack_tmp.occupy_ratio.size() == 0)
    {
        cout<<"ERROR"<<endl;exit(1);
    }
    Boundary &region_boundary = pack_tmp.region_boundary;

    unsigned int reserve_value = 10;
    int MacroWidthShrink = Macro_temp->cal_w_wo_shrink * Macro_temp->WidthShrinkPARA;
    int MacroHeightShrink = Macro_temp->cal_h_wo_shrink * Macro_temp->HeightShrinkPARA;
    Macro_temp->cal_h = MacroHeightShrink;
    Macro_temp->cal_w = MacroWidthShrink;

    /*if(ptr->rectangle.urx - ptr->rectangle.llx < MacroWidthShrink)
    {
        return;
    }*/ // COMMENT 2021.03.24

    Boundary PackingRect;
    PackingRect.llx = max(ptr->rectangle.llx, region_boundary.llx);
    PackingRect.lly = max(ptr->rectangle.lly, region_boundary.lly);
    PackingRect.urx = min(ptr->rectangle.urx, region_boundary.urx);
    PackingRect.ury = min(ptr->rectangle.ury, region_boundary.ury); /// COMMENT 2021.03

//    cout<<"PACKING RECT\n";
//    Boundary_Cout(PackingRect);
//    cout<<"\n ptr : "<<ptr->CornerNode_id<<"\n";
//    Boundary_Cout(ptr->rectangle);
//    bool Pack_Top = false;

    for(int count = 0; count < 2; count++)
    {
        /*vector<CornerNode*> VertCombination;
        VertCombination.reserve(reserve_value);*/ /// COMMENT 2021.03, due to this vector is useless.
        vector<vector<CornerNode*> > PossibleCombination;
        PossibleCombination.reserve(reserve_value);

        int macro_ury = 0;
        int macro_lly = 0;
        int Lmacro_llx = max(PackingRect.llx, chip_boundary.llx);
        int Rmacro_llx = max(PackingRect.urx - MacroWidthShrink, chip_boundary.llx);


        int Height = 0;

        if(count == 0) ///packing bot edge
        {
            Height = PackingRect.lly + MacroHeightShrink;
            macro_lly = PackingRect.lly;

            if(Height > chip_boundary.ury || PackingRect.lly == ptr->rectangle.ury /*|| Height > PackingRect.ury*/) /// MODIFY 2021.03
            {
                //cout<<"continue macro id: "<<Macro_temp->macro_id<<endl;
                continue;
            }
        }
        else           ///packing top edge
        {
            Height = PackingRect.ury - MacroHeightShrink;
            macro_lly = Height;

            if(Height < chip_boundary.lly || PackingRect.ury == ptr->rectangle.lly /*|| Height < PackingRect.lly*/) /// MODIFY 2021.03
            {
                //cout<<"continue macro id: "<<Macro_temp->macro_id<<endl;
                continue;
            }
        }

        macro_ury = macro_lly + MacroHeightShrink;



//        cout<<"\n ptr : "<<ptr->CornerNode_id<<"\n";
        PossibleCombination.reserve(4);
        if(macro_ury - macro_lly < MacroHeightShrink)
        {
            cout<<"macro_ury - macro_lly < MacroHeightShrink"<<endl;
            exit(1);
        }
//        cout<<"%"<<endl;
        /*int initial_size = 0;
        int after_pack_left_size = 0;
        bool L_Feasible = true;
        bool R_Feasible = true;*/


        for(int  i = 0; i < 2; i++)
        {
            Boundary macro_boundary;

            CornerNode* start_node;
            if(count == 0) /// pack bot
            {
                start_node = ptr;
            }
            else /// pack top
            {
                if(i == 0)
                {
                    start_node = CornerNodePointSearch(make_pair(Lmacro_llx, macro_lly), ptr);
                }
                else
                {
                    start_node = CornerNodePointSearch(make_pair(Rmacro_llx, macro_lly), ptr);
                }
                if(start_node->NodeType != Blank)
                {
                    continue;
                }
            }


            if(i == 0) ///pack left
            {
                Boundary_Assign(macro_boundary, Lmacro_llx, macro_lly, Lmacro_llx + MacroWidthShrink, macro_ury);
                Possible_Combination_update(PossibleCombination, start_node, Macro_temp, macro_boundary);
            }
            else
            {
                Boundary_Assign(macro_boundary, Rmacro_llx, macro_lly, Rmacro_llx + MacroWidthShrink, macro_ury);
                Possible_Combination_update(PossibleCombination, start_node, Macro_temp, macro_boundary);
            }

        }
//        cout<<"ptr : "<<ptr->CornerNode_id<<endl;
//        for(int j = 0; j < (int)PossibleCombination.size(); j++)
//        {
//            vector<CornerNode*> &combination = PossibleCombination[j];
//            cout<<" FINAL PILLIAR : ";
//            for(int i = 0; i < (int)combination.size(); i++)
//            {
//                cout<<combination[i]->CornerNode_id<<" ";
//            }
//            cout<<"\n";
//        }
/*if(Macro_temp->macro_id == 0)
{
    cout<<"Macro ID: "<<Macro_temp->macro_id<<endl;
    cout<<"PossibleCombination.size(): "<<PossibleCombination.size()<<endl;
    for(unsigned int i = 0; i < PossibleCombination.size(); i++)
    {
        for(unsigned int j = 0; j < PossibleCombination[i].size(); j++)
        {
            cout<<"tile id: "<<PossibleCombination[i][j]->CornerNode_id<<endl;
            cout<<PossibleCombination[i][j]->rectangle.llx<<", "<<PossibleCombination[i][j]->rectangle.lly<<"  "<<PossibleCombination[i][j]->rectangle.urx<<", "<<PossibleCombination[i][j]->rectangle.ury<<endl;
        }
        cout<<"--------------"<<endl;
    }
    getchar();
}*/
        for(unsigned int i = 0; i < PossibleCombination.size(); i++)
        {
            vector<CylinderInterval> CylinderInterval_temp;
            CylinderInterval_temp = DP_VertCommonInterval(PossibleCombination[i], Macro_temp);

            /// check feasibility
            if(CylinderInterval_temp.size() == 0 || CylinderInterval_temp.back().Width < MacroWidthShrink)
            {
                continue;
            }     /// COMMENT 2021.03

            int L_Bound = PackingRect.llx;
            int R_Bound = PackingRect.urx;

            float L_DeadSpace = 0;
            float R_DeadSpace = 0;

            float L_CenterDistance = 0;
            float R_CenterDistance = 0;

            bool L_Feasible = true;
            bool R_Feasible = true;

            if(L_Bound + MacroWidthShrink > CylinderInterval_temp.back().LRBound.second || L_Bound < CylinderInterval_temp.back().LRBound.first)
            {
                if(CylinderInterval_temp.back().LRBound.first < region_boundary.llx)
                {
                    L_Feasible = false;
                }
                else
                {
                    L_Bound = CylinderInterval_temp.back().LRBound.first;
                    Lmacro_llx = L_Bound;
                }
            }
            if(R_Bound - MacroWidthShrink < CylinderInterval_temp.back().LRBound.first || R_Bound > CylinderInterval_temp.back().LRBound.second)
            {
//                R_Feasible = false;
                if(CylinderInterval_temp.back().LRBound.second > region_boundary.urx)
                {
                    R_Feasible = false;
                }
                else
                {
                    R_Bound = CylinderInterval_temp.back().LRBound.second;
                    Rmacro_llx = R_Bound - MacroWidthShrink;
                }

            }
//            if(Macro_temp->macro_id == 0)
//                cout<<"L_Feasible: "<<L_Feasible<<", R_Feasible: "<<R_Feasible<<endl;
//            cout<<"LRBOUND : "<<CylinderInterval_temp.back().LRBound.first<<"\t"<<CylinderInterval_temp.back().LRBound.second<<endl;
//            cout<<"L "<<L_Feasible<<endl;
//            cout<<"R "<<R_Feasible<<endl;
            for(unsigned int j = 0; j < CylinderInterval_temp.size(); j++)
            {
                CylinderInterval &cy_temp = CylinderInterval_temp[j];
                float LDeadSpaceW, RDeadSpaceW, DeadSpaceH;
                LDeadSpaceW = 0;
                RDeadSpaceW = 0;
                DeadSpaceH = 0;

                CornerNode* corner_temp = PossibleCombination[i][j];

                if(count == 0)
                {
                    DeadSpaceH = min(corner_temp->rectangle.ury, Height) - max(corner_temp->rectangle.lly, PackingRect.lly);

                }
                else
                {
                    DeadSpaceH = min(corner_temp->rectangle.ury, PackingRect.ury) - max(corner_temp->rectangle.lly, Height);
                }

                if(L_Feasible)
                {
                    LDeadSpaceW = (L_Bound - cy_temp.LR.first);
                    L_DeadSpace += LDeadSpaceW / (float)PARA * DeadSpaceH/ (float)PARA;
                }

                if(R_Feasible)
                {
                    RDeadSpaceW = (cy_temp.LR.second - R_Bound);
                    R_DeadSpace += RDeadSpaceW / (float)PARA * DeadSpaceH/ (float)PARA;
                }

            }

            /// COST EVALUATION

            float L_Displacement = 0;
            float R_Displacement = 0;
            float Y_Displacement = 0;

            Y_Displacement = fabs(Macro_temp->gp.lly - macro_lly) / (float)PARA;
            L_Displacement = fabs(Macro_temp->gp.llx - Lmacro_llx) / (float)PARA + Y_Displacement;
            R_Displacement = fabs(Macro_temp->gp.llx - Rmacro_llx) / (float)PARA + Y_Displacement;


//            int max_distanceY = max(fabs(pack_tmp.WhiteSpaceCenterY - macro_lly), fabs(pack_tmp.WhiteSpaceCenterY - (macro_lly + MacroHeightShrink)));
//            int CenterDistanceY = 0;
//            CenterDistanceY = pow( max_distanceY / (float)PARA, 2);
//            int Lmax_distanceX = max(fabs(Lmacro_llx - pack_tmp.WhiteSpaceCenterX), fabs(Lmacro_llx + MacroWidthShrink - pack_tmp.WhiteSpaceCenterX));
//            int Rmax_distanceX = max(Rmacro_llx - pack_tmp.WhiteSpaceCenterX, fabs(MacroWidthShrink + Rmacro_llx - pack_tmp.WhiteSpaceCenterX));
//            L_CenterDistance = sqrt(pow(Lmax_distanceX / (float)PARA, 2) + CenterDistanceY);
//            R_CenterDistance = sqrt(pow(Rmax_distanceX / (float)PARA, 2) + CenterDistanceY);


            int max_distanceY = max(fabs(pack_tmp.WhiteSpaceCenterY - macro_lly), fabs(pack_tmp.WhiteSpaceCenterY - (macro_lly + MacroHeightShrink)));
            int CenterDistanceY = 0;
            CenterDistanceY = max_distanceY;
            int Lmax_distanceX = max(fabs(Lmacro_llx - pack_tmp.WhiteSpaceCenterX), fabs(Lmacro_llx + MacroWidthShrink - pack_tmp.WhiteSpaceCenterX));
            int Rmax_distanceX = max(Rmacro_llx - pack_tmp.WhiteSpaceCenterX, (int)(fabs(MacroWidthShrink + Rmacro_llx - pack_tmp.WhiteSpaceCenterX)));

            //L_CenterDistance = pow(pow(Lmax_distanceX, 2)  + pow(CenterDistanceY, 2), 0.5);
            //R_CenterDistance = pow(pow(Rmax_distanceX, 2)  + pow(CenterDistanceY, 2), 0.5);

            ///2021.02
            float exp_x = 10;
            float exp_y = 10;
            float scale_down = (float)1 / (float)10;

            float exp1 = 10;
            int full_W = full_boundary.urx - full_boundary.llx;
            int full_H = full_boundary.ury - full_boundary.lly;
            exp1 = (float)10 * (log(0.5*(float)min(full_W, full_H))) / (log(0.5*(float)max(full_W, full_H)));

            if(full_W > full_H)
                exp_x = exp1;
            if(full_H > full_W)
                exp_y = exp1;

            L_CenterDistance = pow(pow((float)Lmax_distanceX, exp_x)  + pow((float)CenterDistanceY, exp_y), scale_down);
            R_CenterDistance = pow(pow((float)Rmax_distanceX, exp_x)  + pow((float)CenterDistanceY, exp_y), scale_down);

            //L_CenterDistance = pow(pow(Lmax_distanceX, 10)  + pow(CenterDistanceY, 10), 0.1);
            //R_CenterDistance = pow(pow(Rmax_distanceX, 10)  + pow(CenterDistanceY, 10), 0.1);
            //////

            Boundary macro_boudary;
            Boundary_Assign(macro_boudary, Lmacro_llx, macro_lly, Lmacro_llx + MacroWidthShrink, macro_lly + MacroHeightShrink);

            Boundary group_BBOX = pack_tmp.group_boundary;
            Boundary type_BBOX = pack_tmp.type_boundary;

            Cal_BBOX(macro_boudary, group_BBOX);
            Cal_BBOX(macro_boudary, type_BBOX);

            int Group_DistY = group_BBOX.ury - group_BBOX.lly;
            int Type_DistY = type_BBOX.ury - type_BBOX.lly;

            int LGroup_DistX = group_BBOX.urx - group_BBOX.llx;
            int LType_DistX = type_BBOX.urx - type_BBOX.llx;

            /// ADD 2021.03.17, calculate congestion cost
            float LCongestion_Cost = 0;
            if(Refinement_Flag == true)
            {
                float cong_cost = 0;
                cong_cost = CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boudary, true);
                LCongestion_Cost += cong_cost;
                cong_cost = CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boudary, true);
                LCongestion_Cost += cong_cost;
            }

            /// ADD 2021.08, consider set H/W tendency
            float LRegularity_Cost = 0;
            if(Build_Set == true && ITER > 0)
            {
                map<int, MacrosSet*> set_id;
                set_id = Find_Surrounding_Sets(AllCornerNode, Macro_temp, macro_boudary);

                map<int, MacrosSet*>::iterator i;
                while(!set_id.empty())
                {
                    i = set_id.begin();
                    MacrosSet* set_tmp = i->second;

                    Boundary after_put_macro_to_set = macro_boudary;
                    Cal_BBOX(set_tmp->bbx, after_put_macro_to_set);

                    if(set_tmp->aspect_ratio_decrease == false && set_tmp->aspect_ratio_increase == true)   // long
                    {
                        float after_H_W_ratio = (float)(after_put_macro_to_set.ury - after_put_macro_to_set.lly) / (after_put_macro_to_set.urx - after_put_macro_to_set.llx);

                        if(after_H_W_ratio - set_tmp->H_W_ratio >= 0)
                        {
                            LRegularity_Cost += 0;
                        }
                        else if(after_H_W_ratio - set_tmp->H_W_ratio < 0)
                        {
                            LRegularity_Cost += fabs(after_H_W_ratio - set_tmp->H_W_ratio);
                            //cout<<"long: "<<LRegularity_Cost<<endl;getchar();
                        }
                    }
                    else if(set_tmp->aspect_ratio_decrease == true && set_tmp->aspect_ratio_increase == false)  // wide
                    {
                        float after_H_W_ratio = (float)(after_put_macro_to_set.ury - after_put_macro_to_set.lly) / (after_put_macro_to_set.urx - after_put_macro_to_set.llx);

                        if(after_H_W_ratio - set_tmp->H_W_ratio <= 0)
                        {
                            LRegularity_Cost += 0;
                        }
                        else if(after_H_W_ratio - set_tmp->H_W_ratio > 0)
                        {
                            LRegularity_Cost += fabs(after_H_W_ratio - set_tmp->H_W_ratio);
                            //cout<<"wide: "<<LRegularity_Cost<<endl;getchar();
                        }
                    }
                    else if(set_tmp->aspect_ratio_decrease == true && set_tmp->aspect_ratio_increase == true)   // let Am/Ab = 1
                    {

                    }
                    float after_total_area = set_tmp->total_area + Macro_temp->area;
                    float after_bbx_area = (float)( ( after_put_macro_to_set.urx - after_put_macro_to_set.llx ) / PARA ) * ( ( after_put_macro_to_set.ury - after_put_macro_to_set.lly ) / PARA );
                    float after_total_over_bbx = after_total_area / (after_bbx_area - set_tmp->bbx_overlap_area);

                    if(after_total_over_bbx - set_tmp->total_over_bbx >= 0)
                    {
                        LRegularity_Cost += 0;
                    }
                    else if(after_total_over_bbx - set_tmp->total_over_bbx < 0)
                    {
                        LRegularity_Cost += fabs(after_total_over_bbx - set_tmp->total_over_bbx);
                        //cout<<"Am/Ab close to 1: "<<LRegularity_Cost<<endl;getchar();
                    }

                    set_id.erase(i);
                }

                /*if(set_id.empty())  // Regularity cost is a constant value
                {
                    LRegularity_Cost = 0;
                }
                else if(set_id.size() == 1) // follow the set H/W tendency
                {
                    MacrosSet* set_tmp = set_id.begin()->second;

                    Boundary after_put_macro_to_set = macro_boudary;
                    Cal_BBOX(set_tmp->bbx, after_put_macro_to_set);

                    if(set_tmp->aspect_ratio_decrease == false && set_tmp->aspect_ratio_increase == true)   // long
                    {
                        float after_H_W_ratio = (float)(after_put_macro_to_set.ury - after_put_macro_to_set.lly) / (after_put_macro_to_set.urx - after_put_macro_to_set.llx);

                        if(after_H_W_ratio - set_tmp->H_W_ratio >= 0)
                        {
                            LRegularity_Cost = 0;
                        }
                        else if(after_H_W_ratio - set_tmp->H_W_ratio < 0)
                        {
                            LRegularity_Cost = fabs(after_H_W_ratio - set_tmp->H_W_ratio);
                            //cout<<"long: "<<LRegularity_Cost<<endl;getchar();
                        }
                    }
                    else if(set_tmp->aspect_ratio_decrease == true && set_tmp->aspect_ratio_increase == false)  // wide
                    {
                        float after_H_W_ratio = (float)(after_put_macro_to_set.ury - after_put_macro_to_set.lly) / (after_put_macro_to_set.urx - after_put_macro_to_set.llx);

                        if(after_H_W_ratio - set_tmp->H_W_ratio <= 0)
                        {
                            LRegularity_Cost = 0;
                        }
                        else if(after_H_W_ratio - set_tmp->H_W_ratio > 0)
                        {
                            LRegularity_Cost = fabs(after_H_W_ratio - set_tmp->H_W_ratio);
                            //cout<<"wide: "<<LRegularity_Cost<<endl;getchar();
                        }
                    }
                    else if(set_tmp->aspect_ratio_decrease == true && set_tmp->aspect_ratio_increase == true)   // let Am/Ab = 1
                    {
                        float after_total_area = set_tmp->total_area + Macro_temp->area;
                        float after_bbx_area = (float)( ( after_put_macro_to_set.urx - after_put_macro_to_set.llx ) / PARA ) * ( ( after_put_macro_to_set.ury - after_put_macro_to_set.lly ) / PARA );
                        float after_total_over_bbx = after_total_area / (after_bbx_area - set_tmp->bbx_overlap_area);

                        if(after_total_over_bbx - set_tmp->total_over_bbx >= 0)
                        {
                            LRegularity_Cost = 0;
                        }
                        else if(after_total_over_bbx - set_tmp->total_over_bbx < 0)
                        {
                            LRegularity_Cost = fabs(after_total_over_bbx - set_tmp->total_over_bbx);
                            //cout<<"Am/Ab close to 1: "<<LRegularity_Cost<<endl;getchar();
                        }
                    }
                }
                else if(set_id.size() > 1)  // merge these sets and determine H/W tendency
                {
                    // merge and calculate bbx
                    MacrosSet *merged_set = new MacrosSet;
                    MacrosSet *first_set = set_id.begin();

                    merged_set->bbx              = first_set->bbx;
                    merged_set->total_area       = first_set->total_area;
                    merged_set->bbx_overlap_area = first_set->bbx_overlap_area;
                    merged_set->members          = first_set->members;

                    for(map<int, MacrosSet*>::iterator it = ++set_id.begin(); it != set_id.end(); it++)
                    {
                        MacrosSet *set_tmp = it->second;

                        Cal_BBOX(set_tmp->bbx, merged_set->bbx);
                        merged_set->total_area += set_tmp->total_area;
                        merged_set->bbx_overlap_area += set_tmp->bbx_overlap_area;
                        merged_set->members.insert(set_tmp->members.begin(), set_tmp->members.end());
                    }
                    // merged set information
                    float merged_total_area = ; // merged macro area
                    float merged_bbx_area = ;   // merged set bbx
                    float merged_bbx_overlap_area = ;   // pre-placed area in merged set bbx
                    float merged_total_over_bbx = merged_total_area / (merged_bbx_area - merged_bbx_overlap_area);  //merged set Am/Ab
                    // determine H/W tendency
                }*/
            }
            //////

            macro_boudary.llx = Rmacro_llx;
            macro_boudary.urx = Rmacro_llx + MacroWidthShrink;
            int RGroup_DistX = max(macro_boudary.urx, pack_tmp.group_boundary.urx) - min(macro_boudary.llx, pack_tmp.group_boundary.llx);
            int RType_DistX = max(macro_boudary.urx, pack_tmp.type_boundary.urx) - min(macro_boudary.llx, pack_tmp.type_boundary.llx);


            float LGroup_Dist = (LGroup_DistX + Group_DistY)/(float)PARA;
            float RGroup_Dist = (RGroup_DistX + Group_DistY)/(float)PARA;
            float LType_Dist = (LType_DistX + Type_DistY)/(float)PARA;
            float RType_Dist = (RType_DistX + Type_DistY)/(float)PARA;

            /// ADD 2021.03.17, calculate congestion cost
            int RCongestion_Cost = 0;
            if(Refinement_Flag == true)
            {
                float cong_cost = 0;
                cong_cost = CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boudary, true);
                RCongestion_Cost += cong_cost;
                cong_cost = CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boudary, true);
                RCongestion_Cost += cong_cost;
            }
            /// ADD 2021.08, consider set H/W tendency
            float RRegularity_Cost = 0;
            if(Build_Set == true && ITER > 0)
            {
                map<int, MacrosSet*> set_id;
                set_id = Find_Surrounding_Sets(AllCornerNode, Macro_temp, macro_boudary);

                map<int, MacrosSet*>::iterator i;
                while(!set_id.empty())
                {
                    i = set_id.begin();
                    MacrosSet* set_tmp = i->second;

                    Boundary after_put_macro_to_set = macro_boudary;
                    Cal_BBOX(set_tmp->bbx, after_put_macro_to_set);

                    if(set_tmp->aspect_ratio_decrease == false && set_tmp->aspect_ratio_increase == true)   // long
                    {
                        float after_H_W_ratio = (float)(after_put_macro_to_set.ury - after_put_macro_to_set.lly) / (after_put_macro_to_set.urx - after_put_macro_to_set.llx);

                        if(after_H_W_ratio - set_tmp->H_W_ratio >= 0)
                        {
                            RRegularity_Cost += 0;
                        }
                        else if(after_H_W_ratio - set_tmp->H_W_ratio < 0)
                        {
                            RRegularity_Cost += fabs(after_H_W_ratio - set_tmp->H_W_ratio);
                            //cout<<"long: "<<RRegularity_Cost<<endl;getchar();
                        }
                    }
                    else if(set_tmp->aspect_ratio_decrease == true && set_tmp->aspect_ratio_increase == false)  // wide
                    {
                        float after_H_W_ratio = (float)(after_put_macro_to_set.ury - after_put_macro_to_set.lly) / (after_put_macro_to_set.urx - after_put_macro_to_set.llx);

                        if(after_H_W_ratio - set_tmp->H_W_ratio <= 0)
                        {
                            RRegularity_Cost += 0;
                        }
                        else if(after_H_W_ratio - set_tmp->H_W_ratio > 0)
                        {
                            RRegularity_Cost += fabs(after_H_W_ratio - set_tmp->H_W_ratio);
                            //cout<<"wide: "<<RRegularity_Cost<<endl;getchar();
                        }
                    }
                    else if(set_tmp->aspect_ratio_decrease == true && set_tmp->aspect_ratio_increase == true)   // let Am/Ab = 1
                    {

                    }
                    float after_total_area = set_tmp->total_area + Macro_temp->area;
                    float after_bbx_area = (float)( ( after_put_macro_to_set.urx - after_put_macro_to_set.llx ) / PARA ) * ( ( after_put_macro_to_set.ury - after_put_macro_to_set.lly ) / PARA );
                    float after_total_over_bbx = after_total_area / (after_bbx_area - set_tmp->bbx_overlap_area);

                    if(after_total_over_bbx - set_tmp->total_over_bbx >= 0)
                    {
                        RRegularity_Cost += 0;
                    }
                    else if(after_total_over_bbx - set_tmp->total_over_bbx < 0)
                    {
                        RRegularity_Cost += fabs(after_total_over_bbx - set_tmp->total_over_bbx);
                        //cout<<"Am/Ab close to 1: "<<RRegularity_Cost<<endl;getchar();
                    }

                    set_id.erase(i);
                }
            }
            //////

//            cout<<"ID : "<<Macro_temp->macro_id<<endl;


            int LCorner=0, RCorner=0;

            if(count == 0)
            {
                LCorner = LeftBot;
                RCorner = RightBot;
            }
            else
            {
                LCorner = LeftTop;
                RCorner = RightTop;
            }

            int macroCenterY = macro_lly + MacroHeightShrink / 2;
            int LmacroCenterX = Lmacro_llx + MacroWidthShrink / 2;
            int RmacroCenterX = Rmacro_llx + MacroWidthShrink / 2;

            float LThickness = Thickness_Cost(LmacroCenterX, macroCenterY, region_boundary, pack_tmp.occupy_ratio, LCorner);
            float RThickness = Thickness_Cost(RmacroCenterX, macroCenterY, region_boundary, pack_tmp.occupy_ratio, RCorner);

            float LWireLength = Cal_WireLength(Macro_temp, make_pair(LmacroCenterX, macroCenterY));
            float RWireLength = Cal_WireLength(Macro_temp, make_pair(RmacroCenterX, macroCenterY));
//            getchar();
            if(L_Feasible)
            {

                COST_TERM cost_term;

                Boundary MacroBoundary;
                Boundary_Assign(MacroBoundary, Lmacro_llx, macro_lly, Lmacro_llx+MacroWidthShrink, macro_lly+MacroHeightShrink);
                Boundary overlap_box = Overlap_Box(region_boundary, MacroBoundary);
                float overlap_area = (overlap_box.urx - overlap_box.llx) / (float)PARA * (overlap_box.ury - overlap_box.lly) / (float)PARA;



                cost_term.Center_Distance = L_CenterDistance;



                cost_term.Displacement = L_Displacement;
                cost_term.Packing_DeadSpace = L_DeadSpace;
                cost_term.Overlap_BoundaryArea = pack_tmp.Macro_temp->area - overlap_area;
                cost_term.Group_Distance = LGroup_Dist;
                cost_term.Type_Distance = LType_Dist;
                cost_term.Thickness = LThickness;
                cost_term.WireLength = LWireLength;
                if(Refinement_Flag == true)
                    cost_term.Congestion = LCongestion_Cost;    // ADD 2021.03.17
                if(Build_Set == true && ITER > 0)
                    cost_term.Regularity = LRegularity_Cost;    // ADD 2021.08

                cost_term.MacroCoor = make_pair(Lmacro_llx, macro_lly);
                cost_info.cost.push(cost_term);
                UpdateBoundingValue(cost_info, cost_term);
                /*if(Macro_temp->macro_id == 2)
                {
                    cout<<"L_Feasible"<<endl;
                    cout<<"cost_term.MacroCoor: "<<cost_term.MacroCoor.first<<", "<<cost_term.MacroCoor.second<<endl;
                    getchar();
                }*/
//                if(cost_term.MacroCoor.first == 630140 && cost_term.MacroCoor.second == 2693033 && Macro_temp->macro_id == 11)
//                {
//                    cout<<"\n\n%%%%\nFIND%%%%\n IT\n\n";
//                    exit(1);
//                }

            }

            if(R_Feasible)
            {

                COST_TERM cost_term;

                Boundary MacroBoundary;
                Boundary_Assign(MacroBoundary, Rmacro_llx, macro_lly, Rmacro_llx+MacroWidthShrink, macro_lly+MacroHeightShrink);
                Boundary overlap_box = Overlap_Box(region_boundary, MacroBoundary);
                float overlap_area = (overlap_box.urx - overlap_box.llx) / (float)PARA * (overlap_box.ury - overlap_box.lly) / (float)PARA;



                cost_term.Center_Distance = R_CenterDistance;





                cost_term.Displacement = R_Displacement;
                cost_term.Packing_DeadSpace = R_DeadSpace;
                cost_term.Overlap_BoundaryArea = pack_tmp.Macro_temp->area - overlap_area;
                cost_term.Group_Distance = RGroup_Dist;
                cost_term.Type_Distance = RType_Dist;
                cost_term.Thickness = RThickness;
                cost_term.WireLength = RWireLength;
                if(Refinement_Flag == true)
                    cost_term.Congestion = RCongestion_Cost;    // ADD 2021.03.17
                if(Build_Set == true && ITER > 0)
                    cost_term.Regularity = RRegularity_Cost;    // ADD 2021.08

                cost_term.MacroCoor = make_pair(Rmacro_llx, macro_lly);
                cost_info.cost.push(cost_term);
                UpdateBoundingValue(cost_info, cost_term);
                /*if(Macro_temp->macro_id == 2)
                {
                    cout<<"R_Feasible"<<endl;
                    cout<<"cost_term.MacroCoor: "<<cost_term.MacroCoor.first<<", "<<cost_term.MacroCoor.second<<endl;
                    getchar();
                }*/
//                if(cost_term.MacroCoor.first == 630140 && cost_term.MacroCoor.second == 2693033 && Macro_temp->macro_id == 11)
//                {
//                    cout<<"\n\n%%%%\nFIND%%%%\n IT\n\n";
//                    exit(1);
//                }
            }
//            cout<<"L COOR "<<Lmacro_llx<<"\t"<<Lmacro_llx + MacroWidthShrink<<endl;
//            cout<<"R COOR "<<Rmacro_llx<<"\t"<<Rmacro_llx + MacroWidthShrink<<endl;

        }
//            Cout_PossibleCombination(PossibleCombination);

    }
//    getchar();
/*queue<COST_TERM> test = cost_info.cost;
while(!test.empty())
{
    cout<<"  Displacement: "<<test.front().Displacement<<endl;
    cout<<"  Type_Distance: "<<test.front().Type_Distance<<endl;
    cout<<"  WireLength: "<<test.front().WireLength<<endl;
    test.pop();
}*/
}

int Cal_WireLength(Macro* macro_temp, pair<int, int> MacroCoor)
{
    int WireLength = 0;

    for(int i = 0; i < (int)macro_temp->NET_INFO.size(); i++)
    {
        Boundary BBox;
        BBox.urx = MacroCoor.first;
        BBox.ury = MacroCoor.second;
        BBox.llx = MacroCoor.first;
        BBox.lly = MacroCoor.second;

        for(int j = 0; j < (int)macro_temp->NET_INFO[i].size(); j++)
        {

            Macro* macro_tmp = macro_temp->NET_INFO[i][j];

            if(macro_tmp->LegalFlag)
            {
                pair<int, int> macroCoor = macro_tmp->Macro_Center;
                if(BBox.urx < macroCoor.first)
                {
                    BBox.urx = macroCoor.first;
                }
                if(BBox.ury < macroCoor.second)
                {
                    BBox.ury = macroCoor.second;
                }
                if(BBox.llx > macroCoor.first)
                {
                    BBox.llx = macroCoor.first;
                }
                if(BBox.lly > macroCoor.second)
                {
                    BBox.lly = macroCoor.second;
                }

            }
        }
//        Boundary_Cout(BBox);
        WireLength += ((BBox.urx - BBox.llx) + (BBox.ury - BBox.lly));
    }

    return WireLength;
}

int Thickness_Cost(int macroCenterX, int macroCenterY, Boundary &region_boundary, vector<float> &occupy_ratio, int PackCorner)
{
   /// occupy_ratio[0]  /// top boundary
   /// occupy_ratio[1]  /// bot boundary
   /// occupy_ratio[2]  /// lef boundary
   /// occupy_ratio[3]  /// rig boundary



    int ThicknessX=0, ThicknessY=0;
    Boundary _boundary = region_boundary;
    float threshold = 0.4;


    switch(PackCorner)
    {
    case LeftBot :


        if(occupy_ratio[1] < threshold)
        {
            ThicknessY = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessY = fabs(macroCenterY - _boundary.lly)*(occupy_ratio[1]);
        }
        if(occupy_ratio[2] < threshold)
        {
            ThicknessX = BenchInfo.chip_W+BenchInfo.chip_H;
        }
        else
        {
            ThicknessX = fabs(macroCenterX - _boundary.llx)*(occupy_ratio[2]);
        }
        break;
    case RightBot :


        if(occupy_ratio[1] < threshold)
        {
            ThicknessY = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessY = fabs(macroCenterY - _boundary.lly)*(occupy_ratio[1]);
        }

        if(occupy_ratio[3] < threshold)
        {
            ThicknessX = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessX = fabs(_boundary.urx - macroCenterX)*(occupy_ratio[3]);
        }

        break;
    case LeftTop :

        if(occupy_ratio[0] < threshold)
        {
            ThicknessY = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessY = fabs(_boundary.ury - macroCenterY)*(occupy_ratio[0]);
        }

        if(occupy_ratio[2] < threshold)
        {
            ThicknessX = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessX = fabs(macroCenterX - _boundary.llx)*(occupy_ratio[2]);
        }

        break;
    case RightTop :

        if(occupy_ratio[0] < threshold)
        {
            ThicknessY = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessY = fabs(_boundary.ury - macroCenterY)*(occupy_ratio[0]);
        }

        if(occupy_ratio[3] < threshold)
        {
            ThicknessX = BenchInfo.chip_H+BenchInfo.chip_W;
        }
        else
        {
            ThicknessX = fabs(_boundary.urx - macroCenterX)*(occupy_ratio[3]);
        }
        break;
    }

//    if(occupy_ratio[0] >= threshold && occupy_ratio[1] >= threshold && !(occupy_ratio[2] < threshold || occupy_ratio[3] < threshold))
//    {
//        ThicknessX = 0;
//        ThicknessY = sqrt(fabs(_boundary.ury - macroCenterY)*(occupy_ratio[0])) + sqrt(fabs(macroCenterY - _boundary.lly)*(occupy_ratio[1]));
//
//    }
//    else if(occupy_ratio[2] >= threshold && occupy_ratio[3] >= threshold && !(occupy_ratio[0] < threshold || occupy_ratio[1] < threshold))
//    {
//        ThicknessX = sqrt(fabs(_boundary.urx - macroCenterX)*(occupy_ratio[3])) + sqrt(fabs(macroCenterX - _boundary.llx)*(occupy_ratio[2]));
//        ThicknessY = 0;
//    }
//    cout<<ThicknessX<<"\t"<<ThicknessY<<endl;
//    cout<<"OUT"<<endl;
    return ThicknessX + ThicknessY;
}

void UpdateBoundingValue(COST_INFO &cost_info, COST_TERM &cost_term)
{

//    cout<<"cost_term.Displacement "<<cost_term.Displacement<<endl;
    cost_info.Displacement.first = min(cost_term.Displacement, cost_info.Displacement.first);
    cost_info.Displacement.second = max(cost_term.Displacement, cost_info.Displacement.second);

    cost_info.Center_Distance.first = min(cost_term.Center_Distance, cost_info.Center_Distance.first);
    cost_info.Center_Distance.second = max(cost_term.Center_Distance, cost_info.Center_Distance.second);

    cost_info.Overlap_BoundaryArea.first = min(cost_term.Overlap_BoundaryArea, cost_info.Overlap_BoundaryArea.first);
    cost_info.Overlap_BoundaryArea.second = max(cost_term.Overlap_BoundaryArea, cost_info.Overlap_BoundaryArea.second);

    cost_info.Packing_DeadSpace.first = min(cost_term.Packing_DeadSpace, cost_info.Packing_DeadSpace.first);
    cost_info.Packing_DeadSpace.second = max(cost_term.Packing_DeadSpace, cost_info.Packing_DeadSpace.second);

    cost_info.Group_Distance.first = min(cost_term.Group_Distance, cost_info.Group_Distance.first);
    cost_info.Group_Distance.second = max(cost_term.Group_Distance, cost_info.Group_Distance.second);

    cost_info.Type_Distance.first = min(cost_term.Type_Distance, cost_info.Type_Distance.first);
    cost_info.Type_Distance.second = max(cost_term.Type_Distance, cost_info.Type_Distance.second);

    cost_info.Thickness.first = min(cost_term.Thickness, cost_info.Thickness.first);
    cost_info.Thickness.second = max(cost_term.Thickness, cost_info.Thickness.second);

    cost_info.WireLength.first = min(cost_term.WireLength, cost_info.WireLength.first);
    cost_info.WireLength.second = max(cost_term.WireLength, cost_info.WireLength.second);

    // ADD 2021.03.17
    cost_info.Congestion.first = min(cost_term.Congestion, cost_info.Congestion.first);
    cost_info.Congestion.second = max(cost_term.Congestion, cost_info.Congestion.second);
}

void Possible_Combination_update(vector<vector<CornerNode*> > &PossibleCombination, CornerNode* start_node, Macro* macro_temp, Boundary &macro_boundary)
{

    vector<CornerNode*> combination;
    combination.reserve(10);
    /*combination.push_back(start_node);
//    cout<<"start_node : "<<start_node->CornerNode_id<<endl;
//    cout<<"MACRO BOUNDARY "<<endl;
//    Boundary_Cout(macro_boundary);
//    cout<<"\n\n";

    Find_topBlank(combination, macro_temp, macro_boundary.ury, macro_boundary);   *//// COMMENT 2021.03

    ///2021.01
    //if(Refinement_Flag == true)
    //{
        combination.clear();
        queue<CornerNode*> CornerNodeSet;
        vector<CornerNode*> CornerNodeSetV;
        Directed_AreaEnumeration(macro_boundary, start_node, CornerNodeSet);
        //plot_Directed_AreaEnumeration(CornerNodeSet, macro_boundary);
        while(!CornerNodeSet.empty())
        {
            CornerNodeSetV.insert(CornerNodeSetV.begin(), CornerNodeSet.front()); //let y from low to high
            CornerNodeSet.pop();
        }

        for(int i = 0; i < CornerNodeSetV.size(); i ++)
        {
            CornerNode* CornerNode_tmp = CornerNodeSetV[i];
            if(CornerNode_tmp->rectangle.lly >= macro_boundary.ury)
                continue;
            if(CornerNode_tmp->NodeType == Blank)
            {
                if(CornerNode_tmp->rectangle.lly < macro_boundary.ury)
                    combination.push_back(CornerNode_tmp);

                //if(combination.size() == 0)
                //{
                    //combination.push_back(CornerNode_tmp); //push_back start_node
                //}
                //else
                //{
                    //if(CornerNode_tmp->rectangle.lly < macro_boundary.ury && CornerNode_tmp->rectangle.lly == combination.back()->rectangle.ury)
                        //combination.push_back(CornerNode_tmp);
                    //else
                        //break;
                //}
            }
            else
            {
                combination.clear();
                combination.push_back(start_node);
                break;
            }
        }
    //}
    //////

    if(!(combination.back()->rectangle.ury < macro_boundary.ury))
        PossibleCombination.push_back(combination);


}

void Find_topBlank(vector<CornerNode*> &combination, Macro* Macro_temp, int Height, Boundary &PackingBoundary)
{
    CornerNode* ptr = combination.back()->N;
    if(ptr == NULL)
    {
        return;
    }
//    cout<<"\nHeight : "<<Height<<endl;
//    cout<<"ID : "<<ptr->CornerNode_id<<" Type : "<<ptr->NodeType<<" ptr->rectangle.lly : "<<ptr->rectangle.lly<<"   ptr->rectangle.ury : "<<ptr->rectangle.ury<<endl;
//    cout<<"ID : "<<ptr->CornerNode_id<<" Type : "<<ptr->NodeType<<" ptr->rectangle.llx : "<<ptr->rectangle.llx<<"   ptr->rectangle.urx : "<<ptr->rectangle.urx<<endl;

    if(ptr != NULL)
    {

        if(Height > ptr->rectangle.lly)
        {
//            cout<<"\n";
//            Boundary_Cout(PackingBoundary);
//            cout<<"\n";
//            Boundary_Cout(ptr->rectangle);
//            cout<<"\n";
            while(ptr != NULL && ptr->rectangle.llx > PackingBoundary.llx)
            {
                if(!(ptr->rectangle.llx > PackingBoundary.llx) && ptr->rectangle.urx > PackingBoundary.llx)
                {
                    if(ptr->NodeType == Blank)
                    {
                        combination.push_back(ptr);
                        Find_topBlank(combination, Macro_temp, Height, PackingBoundary);
                        return;
                    }
                    else
                    {
                        return;
                    }
                }

                ptr = ptr->W;
            }
            if(!(ptr->rectangle.llx > PackingBoundary.llx) && ptr->rectangle.urx > PackingBoundary.llx)
            {
                if(ptr->NodeType == Blank)
                {

                    combination.push_back(ptr);
                    Find_topBlank(combination, Macro_temp, Height, PackingBoundary);
                    return;
                }
                else
                {
                    return;
                }
            }
        }
    }
    else
    {
        cout<<"ptr == NULL"<<endl;
    }
}

void Find_Right_topBlank(vector<CornerNode*> &combination, Macro* Macro_temp, int Height, Boundary &PackingBoundary)
{
    CornerNode* ptr = combination.back()->N;


    if(ptr != NULL)
    {

        if(Height > ptr->rectangle.lly)
        {

            while(ptr != NULL && ptr->rectangle.llx > PackingBoundary.urx)
            {
                if(ptr->NodeType == Blank)
                {
                    if(!(ptr->rectangle.urx < PackingBoundary.urx) && !(ptr->rectangle.llx < PackingBoundary.urx))
                    {
                        combination.push_back(ptr);
                        Find_topBlank(combination, Macro_temp, Height, PackingBoundary);
                        break;
                    }
                }
                ptr = ptr->W;
            }
        }
    }
    else
    {
        cout<<"ptr == NULL"<<endl;
    }
}

void plot_Directed_AreaEnumeration(queue<CornerNode*> nodeset, Boundary region)
{
    vector<PLOT_BLOCK> plot_block;
    vector<PLOT_LINE> plot_line;
    int count = 0;
    while(!nodeset.empty())
    {
        PLOT_BLOCK blk_tmp;
        CornerNode* corner_temp = nodeset.front();
        if(corner_temp->NodeType == Blank)
        {
            blk_tmp.color = 'w';
        }
        else
        {
            blk_tmp.color = 'g';
        }
        blk_tmp.plotrectangle = corner_temp->rectangle;
        blk_tmp.int_TEXT = corner_temp->CornerNode_id;
//        blk_tmp.int_TEXT = count;
        count++;
        blk_tmp.TEXT = true;
        blk_tmp.int_stringTEXT = true;
        plot_block.push_back(blk_tmp);
        nodeset.pop();
    }
    PLOT_LINE line_tmp;
    line_tmp.color = 'k';
    Boundary_Assign(line_tmp.plotline, region.llx, region.lly, region.urx, region.lly);
    plot_line.push_back(line_tmp);
    Boundary_Assign(line_tmp.plotline, region.llx, region.lly, region.llx, region.ury);
    plot_line.push_back(line_tmp);
    Boundary_Assign(line_tmp.plotline, region.urx, region.lly, region.urx, region.ury);
    plot_line.push_back(line_tmp);
    Boundary_Assign(line_tmp.plotline, region.llx, region.ury, region.urx, region.ury);
    plot_line.push_back(line_tmp);
    PlotMatlab(plot_block, plot_line, true, false, "Directed_Area.m");

}

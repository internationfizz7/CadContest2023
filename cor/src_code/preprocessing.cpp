#include "preprocessing.h"

vector<int> MovableMacro_ID ;
vector<int> PreplacedMacro_ID ;
map<double,Row*> row_vector;
static map<double,Row*> row_vectorVert;
static double min_macro_width, min_macro_height;
static vector<Boundary> FixedHorizontalCut;
static vector<Boundary> FixedVerticalCut;
Bechmark_INFO BenchInfo;



void Call_preprocessing()   // merge overlap macro and cut pre-place macro by row
{
    //cout<<"Preprocessing..."<<endl;
    cout << "\n======== Now Run PREPROCESSING STAGE\t========"<< endl;

    /// Determine movable and preplaced macros
    Determine_Macro();

    /// shift macros which are out of chip boundary
    Macro_outofbouundary_shifting();

    /// update minimum height and width of macros
    Find_min_macro_height_width();

    /// check overlaps between movable macros
    bool GP_or_LG = true ; //true GP false LG
    Check_overlap(GP_or_LG) ;

    /// Merge blocks
    Update_fixed_blkage();

    /// one day, if you need to merge obstacles vertical priority, you can use this function
    Update_fixed_blkage_Vert();

    /// expand macros by "ShrinkPara"
    MacroInflation();
    //MacroInflation_new();

    /// shift macros which are out of chip boundary
    Macro_outofbouundary_shifting();

    /// update bench info
    Bechmark_analysis();

    if(Debug_PARA)
    {
        cout << "   Write FixedHorizontalCut.m..." << endl;
        Plot_rectangle(FixedHorizontalCut, "./output/corner_stitching_graph/FixedHorizontalCut.m");

        cout << "   Write FixedVerticalCut.m..." << endl;
        Plot_rectangle(FixedVerticalCut, "./output/corner_stitching_graph/FixedVerticalCut.m");
    }
    swap(BenchInfo.FixedHorizontalCut, FixedHorizontalCut);
    swap(BenchInfo.FixedVerticalCut, FixedVerticalCut);
    BenchInfo.min_macro_height = min_macro_height;
    BenchInfo.min_macro_width = min_macro_width;
//    cout<<"Preprocessing end..."<<endl;
}

void Bechmark_analysis()
{
    float chip_area, placeable_area, movable_macro_area, preplaced_macro_blkage_area, std_cell_area;
    float utilization, total_utilization, preplace_utilization, std_utilization;
    chip_area = placeable_area = movable_macro_area = preplaced_macro_blkage_area = std_cell_area = 0 ;

    chip_area = ((chip_boundary.urx - chip_boundary.llx) / (float)PARA * (chip_boundary.ury - chip_boundary.lly) / (float)PARA) ;
    int max_std_h = -1 ;
    int min_std_h = (numeric_limits<int>::max()) ;
    int max_pin_num = 0;
    for(unsigned int i = 0 ; i < macro_list.size() ; i++)
    {
        Macro* macro_temp = macro_list[i];
        if( macro_temp->macro_type == STD_CELL)
        {
            std_cell_area += macro_temp->area ;
            if(macro_list[i] -> cal_h > max_std_h)
            {
                max_std_h = macro_list[i] -> cal_h ;
            }
            if(macro_list[i] -> cal_h < min_std_h)
            {
                min_std_h = macro_list[i] -> cal_h ;
            }

        }
        else if(macro_list[i] -> macro_type == MOVABLE_MACRO)
        {
            movable_macro_area += macro_temp->area ;
            if(macro_temp->pin_num > max_pin_num)
            {
                max_pin_num = macro_temp->pin_num;
            }
        }
    }
    Calculate_placeable_area(preplaced_macro_blkage_area);
    placeable_area = chip_area - preplaced_macro_blkage_area ;
    utilization = (movable_macro_area + std_cell_area) / (double)placeable_area ;
    std_utilization = std_cell_area / (double)(placeable_area - movable_macro_area) ;
    total_utilization = (preplaced_macro_blkage_area + movable_macro_area + std_cell_area) / chip_area;
    preplace_utilization = preplaced_macro_blkage_area / chip_area;
    cout<<"[INFO] Max std H: "<<max_std_h<<"  Min std H: "<<min_std_h<<endl;
    cout<<"[INFO] Utilization: "<<utilization<<endl;
    cout<<"[INFO] Chip area: "<<chip_area<<endl;
    cout<<"[INFO] Placeable area: "<<placeable_area<<endl;
    cout<<"[INFO] Movable macro area: "<<movable_macro_area<<endl;
    cout<<"[INFO] Std cell area : "<<std_cell_area<<endl;
    cout<<"[INFO] Movable macro / Std cell area ratio: "<< movable_macro_area / std_cell_area <<endl;
    cout<<"[INFO] Preplaced macro & Blkage area: "<<preplaced_macro_blkage_area<<endl;
    cout<<"[INFO] Preplaced utilization: "<<preplace_utilization<<endl;
    cout<<"[INFO] Total utilization: "<<total_utilization<<endl;

    BenchInfo.chip_W = chip_boundary.urx - chip_boundary.llx;
    BenchInfo.chip_H = chip_boundary.ury - chip_boundary.lly;

    BenchInfo.chip_area = chip_area;
    BenchInfo.min_std_h = min_std_h;
    BenchInfo.max_std_h = max_std_h;
    BenchInfo.movable_macro_area = movable_macro_area;
    BenchInfo.placeable_area = placeable_area;
    BenchInfo.blank_area = placeable_area - movable_macro_area;
    BenchInfo.preplaced_macro_blkage_area = preplaced_macro_blkage_area;
    BenchInfo.std_cell_area = std_cell_area;
    BenchInfo.utilization = utilization;
    BenchInfo.total_utilization = total_utilization;
    BenchInfo.preplace_utilization = preplace_utilization;
    BenchInfo.std_utilization = std_utilization;
    BenchInfo.max_pin_number = max_pin_num;
    StdCenterCal();

    int MacroNum = 0;
    int CellNum = 0;
    cout << "[INFO] Total Number of the Nets: "<< net_list.size() << endl;
    for(unsigned int i = 0; i < net_list.size(); i++)
    {
        Net & this_net = net_list[i];
        if(this_net.macro_idSet.size() > 1 )
        {
            //cout << this_net.macro_idSet.size() << " " << this_net.cell_idSet.size() << endl;
            MacroNum++;
        }
        if(this_net.macro_idSet.size() > 0 )
        {
            CellNum += this_net.cell_idSet.size();
        }
    }
    cout << "[INFO] Number of Nets is connect between the Macro: " << MacroNum << endl;
    cout << "[INFO] Number of Nets connected to the cell of all Macros: " << CellNum << endl;
//    getchar();
    //getchar();
}

void StdCenterCal()
{
    int stdXCenter, stdYCenter;
    stdXCenter = stdYCenter = 0;
    float x, y;
    x = y = 0;

    for(int i = 0; i < (int)macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == STD_CELL)
        {
            float area_ratio = macro_temp->area / BenchInfo.std_cell_area;
            if(area_ratio <= 0 || area_ratio > 1)
            {
                cout<<"[ERROR] STDArea too small ! macro_legalizer.cpp StdCenterCal()...."<<endl;
                exit(1);
            }

            x += macro_temp->gp.llx * area_ratio;
            y += macro_temp->gp.lly * area_ratio;
        }
    }
    stdXCenter += (int)x;
    stdYCenter += (int)y;
    cout<<"[INFO] Std Center: (" << stdXCenter<<","<<stdYCenter<<")"<<endl;
    cout<<"[INFO] Chip Center: (" << chip_boundary.urx / 2 <<","<<chip_boundary.ury / 2 <<")"<<endl;
    BenchInfo.stdYCenter = stdYCenter;
    BenchInfo.stdXcenter = stdXCenter;
}


void Calculate_placeable_area(float &preplaced_macro_blkage_area)
{
    map<double,Row*>::iterator iter_end = row_vector.end() ;
    iter_end-- ;
    if(iter_end == row_vector.begin())
    {
        return ;
    }
    for(map<double,Row*>::iterator iter = row_vector.begin() ; iter != iter_end ; iter++)
    {
        Node* head = iter -> second -> head ;
        Node* tail = iter -> second -> tail ;
        Node* ptr = head -> next ;

        if(ptr != tail)
        {
            double width_occupy = 0;
            while(ptr != tail)
            {
                width_occupy += ptr -> rectangle.urx - ptr -> rectangle.llx ;
                ptr = ptr -> next ;
            }
            map<double,Row*>::iterator next = iter ;
            next++ ;
            preplaced_macro_blkage_area += (float)(((next -> first - iter -> first) / (float)PARA) * width_occupy / (float)PARA) ;
        }
    }
}

void Find_min_macro_height_width()
{
    min_macro_width = min_macro_height = (numeric_limits<double>::max()) ;
    for(unsigned int i = 0; i < MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
//        cout<<macro_list[macro_id] -> cal_w<<endl;
        if(macro_list[macro_id] -> cal_w < min_macro_width)
        {
            min_macro_width = macro_list[macro_id] -> cal_w ;
        }
        if(macro_list[macro_id] -> cal_h < min_macro_height)
        {
            min_macro_height = macro_list[macro_id] -> cal_h ;
        }
    }
}

void Determine_Macro()
{
    set<int> Macro_LEFtype;
    bool has_non_rect = false;

    for(unsigned int i = 0 ; i < macro_list.size() ; i++ )
    {
        int macro_type = macro_list[i]-> macro_type;
        int lef_macroid = macro_list[i]-> lef_type_ID;
        if(macro_type == MOVABLE_MACRO)
        {
            MovableMacro_ID.push_back(i);
            Macro_LEFtype.insert(macro_list[i]-> lef_type_ID);
        }
        else if(macro_type == PORT || macro_type == PRE_PLACED)
        {
            // MODIFY for non-rectangle pre-placed macros

            // its area is over the placeable region, therefore we do not consider the macro and the prea-placed macro region is cover by placement blockage
            // if u want to slove the problum, pls parser the non-rect macro info
            if(lef_macroid != -1)
                if( DEFLEFInfo.LEF_Macros[lef_macroid].polygon.size() != 0)
                {
                    cout << "[WARNING] Non-rect macro: " << macro_list[i]-> macro_name << endl;
                    has_non_rect = true;
                    continue;
                }

            PreplacedMacro_ID.push_back(i);
        }
    }
    cout << "[INFO] Number of Moveable Macro: " << MovableMacro_ID.size() << endl;
    cout << "[INFO] Number of different Macro types: " << Macro_LEFtype.size() << endl;
}

void Macro_outofbouundary_shifting()
{
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro* macro_temp = macro_list[macro_id];
        int llx = macro_temp -> gp.llx ;
        int lly = macro_temp -> gp.lly ;
        int urx = macro_temp -> gp.urx ;
        int ury = macro_temp -> gp.ury ;
        if(llx < chip_boundary.llx)
        {
            macro_temp -> gp.llx = chip_boundary.llx ;
            macro_temp -> gp.urx = macro_temp -> gp.llx + macro_temp -> cal_w ;
        }
        else if(urx > chip_boundary.urx)
        {
            macro_temp -> gp.llx = chip_boundary.urx - macro_temp -> cal_w ;
            macro_temp -> gp.urx = macro_temp -> gp.llx + macro_temp -> cal_w ;
        }
        if(lly < chip_boundary.lly)
        {
            macro_temp -> gp.lly = chip_boundary.lly ;
            macro_temp -> gp.ury = macro_temp -> gp.lly + macro_temp -> cal_h ;
        }
        else if(ury > chip_boundary.ury)
        {
            macro_temp -> gp.lly = chip_boundary.ury - macro_temp -> cal_h ;
            macro_temp -> gp.ury = macro_temp -> gp.lly + macro_temp -> cal_h ;
        }
    }

     for(int i = 0; i < (int)PreplacedMacro_ID.size(); i++)
    {
        int macro_id = PreplacedMacro_ID[i];
        Macro* macro_temp = macro_list[macro_id];
        int llx = macro_temp -> gp.llx ;
        int lly = macro_temp -> gp.lly ;
        int urx = macro_temp -> gp.urx ;
        int ury = macro_temp -> gp.ury ;
        if(llx < chip_boundary.llx)
        {
            macro_temp -> gp.llx = chip_boundary.llx ;
            macro_temp -> gp.urx = macro_temp -> gp.llx + macro_temp -> cal_w ;
        }
        else if(urx > chip_boundary.urx)
        {
            macro_temp -> gp.llx = chip_boundary.urx - macro_temp -> cal_w ;
            macro_temp -> gp.urx = macro_temp -> gp.llx + macro_temp -> cal_w ;
        }
        if(lly < chip_boundary.lly)
        {
            macro_temp -> gp.lly = chip_boundary.lly ;
            macro_temp -> gp.ury = macro_temp -> gp.lly + macro_temp -> cal_h ;
        }
        else if(ury > chip_boundary.ury)
        {
            macro_temp -> gp.lly = chip_boundary.ury - macro_temp -> cal_h ;
            macro_temp -> gp.ury = macro_temp -> gp.lly + macro_temp -> cal_h ;
        }
        macro_temp->lg = macro_temp->gp;
    }
}
void Produce_narrow_channel_rectangle(vector<Boundary> &rectangle, double min_width, double min_height, map<double,Row*> temp)
{
    map<double,Row*>::iterator iter_end = temp.end();
    iter_end--;
    for(map<double,Row*>::iterator iter = temp.begin(); iter != iter_end; iter++)
    {
        Node* ptr = iter->second->head;
        if(ptr->next == iter->second->tail)
        {
            continue;
        }
        while(ptr != iter->second->tail)
        {
            double narrow_channel_w = ptr->next->rectangle.llx - ptr->rectangle.urx;
            map<double,Row*>::iterator iter_next = iter;
            iter_next++;
            if(narrow_channel_w < min_width && narrow_channel_w != 0)
            {
                Boundary rectangle_temp;
                rectangle_temp.llx = ptr->rectangle.urx;
                rectangle_temp.urx = ptr->next->rectangle.llx;
                rectangle_temp.lly = iter->first;
                rectangle_temp.ury = iter_next->first;
//                if(rectangle_temp.ury - rectangle_temp.lly < min_macro_height)
//                {
                rectangle.push_back(rectangle_temp);
//                }
//                tmp.push_back(rectangle_temp);
            }
//            if(iter_next->first - iter->first < min_macro_height)
//            {
//                tmp.push_back(rectangle_temp);
//            }
            ptr = ptr->next;
        }
    }
//    Plot_rectangle(tmp, "rec.m");getchar();

}
void Update_fixed_blkage()
{
    vector<Boundary> rectangle ;
    map<double,Row*> row_horizontal_vector;
    unsigned int Numrectangle = PreplacedMacro_ID.size() + Placement_blockage.size();
    rectangle.resize(Numrectangle) ;
    for(unsigned int i = 0 ; i < PreplacedMacro_ID.size() ; i++)
    {
        unsigned int macro_id = PreplacedMacro_ID[i] ;
        rectangle.at(i) = macro_list.at(macro_id) -> gp ;
    }

    for(unsigned int i = PreplacedMacro_ID.size() ; i < Numrectangle  ; i++)
    {
        rectangle.at(i) = Placement_blockage.at(i - PreplacedMacro_ID.size()) ;
    }
    for(unsigned int i = 0; i < rectangle.size(); i++)
    {
        swap(rectangle.at(i).llx, rectangle.at(i).lly);
        swap(rectangle.at(i).urx, rectangle.at(i).ury);
    }
    Boundary rectangle_boundary = chip_boundary;
    swap(rectangle_boundary.llx, rectangle_boundary.lly);
    swap(rectangle_boundary.urx, rectangle_boundary.ury);
    row_horizontal_vector = Block_merge(rectangle, rectangle_boundary);
    Produce_narrow_channel_rectangle(rectangle, min_macro_height, min_macro_width, row_horizontal_vector);
    for(unsigned int i = 0; i < rectangle.size(); i++)
    {
        swap(rectangle.at(i).llx, rectangle.at(i).lly);
        swap(rectangle.at(i).urx, rectangle.at(i).ury);
    }
    row_vector = Block_merge(rectangle, chip_boundary);
    Produce_narrow_channel_rectangle(rectangle, min_macro_width, min_macro_height, row_vector);
    Row_Delete(row_vector);
    row_vector = Block_merge(rectangle, chip_boundary);
    for(map<double,Row*>::iterator iter = row_vector.begin() ; iter != row_vector.end() ; iter++)
    {
        Node* head = iter -> second -> head ;
        Node* tail = iter -> second -> tail ;
        Node* ptr = head -> next ;
        if(ptr != tail)
        {
            while(ptr != tail)
            {
                Boundary tmp ;
                map<double,Row*>::iterator iter_next = iter ;
                iter_next ++ ;
                tmp.llx = ptr -> rectangle.llx ;
                tmp.urx = ptr -> rectangle.urx ;
                tmp.lly = iter -> first ;
                tmp.ury = iter_next -> first ;
                FixedHorizontalCut.push_back(tmp);
                ptr = ptr -> next ;
            }
        }
    }
    if(Debug_PARA)
    {
        cout << "\tWrite Final_row.m..."<<endl;
            Plot_Row(row_vector, "./output/corner_stitching_graph/Final_row.m");
    }
    Row_Delete(row_horizontal_vector);
}

void Update_fixed_blkage_Vert()
{
    vector<Boundary> rectangle ;
    map<double,Row*> row_horizontal_vector;
    unsigned int Numrectangle = PreplacedMacro_ID.size() + Placement_blockage.size();
    rectangle.resize(Numrectangle) ;
    for(unsigned int i = 0 ; i < PreplacedMacro_ID.size() ; i++)
    {
        unsigned int macro_id = PreplacedMacro_ID[i] ;
        if(macro_list[macro_id]->macro_type != PRE_PLACED)
            continue;
        rectangle.at(i) = macro_list.at(macro_id) -> gp ;
    }

    for(unsigned int i = PreplacedMacro_ID.size() ; i < Numrectangle  ; i++)
    {
        rectangle.at(i) = Placement_blockage.at(i - PreplacedMacro_ID.size()) ;
    }

    row_horizontal_vector = Block_merge(rectangle, chip_boundary);
    Produce_narrow_channel_rectangle(rectangle, min_macro_height, min_macro_width, row_horizontal_vector);
    for(unsigned int i = 0; i < rectangle.size(); i++)
    {
        swap(rectangle.at(i).llx, rectangle.at(i).lly);
        swap(rectangle.at(i).urx, rectangle.at(i).ury);
    }
    Boundary rectangle_boundary = chip_boundary;
    swap(rectangle_boundary.llx, rectangle_boundary.lly);
    swap(rectangle_boundary.urx, rectangle_boundary.ury);
    row_vectorVert = Block_merge(rectangle, rectangle_boundary);
    Produce_narrow_channel_rectangle(rectangle, min_macro_height, min_macro_width, row_vectorVert);
    Row_Delete(row_vectorVert);
    row_vectorVert = Block_merge(rectangle, rectangle_boundary);
    for(map<double,Row*>::iterator iter = row_vectorVert.begin() ; iter != row_vectorVert.end() ; iter++)
    {
        Node* head = iter -> second -> head ;
        Node* tail = iter -> second -> tail ;
        Node* ptr = head -> next ;
        if(ptr != tail)
        {
            while(ptr != tail)
            {
                Boundary tmp ;
                map<double,Row*>::iterator iter_next = iter ;
                iter_next ++ ;
                tmp.llx = iter -> first  ;
                tmp.urx = iter_next -> first ;
                tmp.lly = ptr -> rectangle.llx;
                tmp.ury = ptr -> rectangle.urx ;
                FixedVerticalCut.push_back(tmp);
                ptr = ptr -> next ;
            }
        }
    }
    Row_Delete(row_horizontal_vector);
}

bool cmp_llx_urx(Boundary a, Boundary b)
{
    if(a.llx == b.llx)
        return a.urx > b.urx ;
    return a.llx < b.llx ;
}

void Row_head_tail_connect(Node* &a, Node* &b, Boundary rectangle_boundary)
{
    Node* head= new Node ;
    Node* tail = new Node ;
    head -> macro_id = 0;
    tail -> macro_id = 0;
    head->rectangle.urx = rectangle_boundary.llx;
    tail->rectangle.llx = rectangle_boundary.urx;
    head -> next = tail ;
    tail -> prev = head ;
    a = head ;
    b = tail ;
}



void row_initialize(Row* &tmp, double index, Boundary rectangle_boundary)
{
    tmp = new Row ;
    tmp -> index = index ;
    Row_head_tail_connect(tmp->head, tmp->tail, rectangle_boundary);
}

map<double,Row*> Block_merge(vector<Boundary> rectangle, Boundary rectangle_boundary)
{
    sort(rectangle.begin(), rectangle.end(), cmp_llx_urx);
    map<double,Row*> Row_y_index ;


    /// Input top and bottom edges of chip
    Row *tmp, *tmpp ;
    row_initialize(tmp, rectangle_boundary.lly, rectangle_boundary);
    Row_y_index.insert(make_pair(tmp->index, tmp));
    row_initialize(tmpp, rectangle_boundary.ury, rectangle_boundary);
    Row_y_index.insert(make_pair(tmpp->index, tmpp));

    /// Input top and bottom edges of obstacles
    for(unsigned int i = 0 ; i < rectangle.size() ; i++)
    {
        Row *tmp1, *tmp2 ;
        row_initialize(tmp1, rectangle[i].lly, rectangle_boundary);
        Row_y_index.insert(make_pair(tmp1 -> index, tmp1));
        row_initialize(tmp2, rectangle[i].ury, rectangle_boundary);
        Row_y_index.insert(make_pair(tmp2 -> index, tmp2));
    }

    /// Update Info of obstacles
    map<double,Row*>::iterator iter ;
    for(unsigned int i = 0 ; i < rectangle.size() ; i++)
    {

        iter = Row_y_index.find(rectangle[i].lly) ;

        if(iter == Row_y_index.end())
        {
            cout<<"error in preprocessing.cpp ...."<<endl;
            exit(1) ;
        }
        for(; iter -> first < rectangle[i].ury ; iter++)
        {
            Node* tmp_node = new Node ;
            tmp_node -> rectangle.llx = rectangle[i].llx ;
            tmp_node -> rectangle.urx = rectangle[i].urx ;
            tmp_node -> rectangle.lly = -1 ;
            tmp_node -> rectangle.ury = -1 ;
            iter -> second -> head -> macro_id++ ; // Node count
            Insert_Node(tmp_node, iter -> second -> tail);
        }
    }

    /// merge obstacles horizontally
    for(iter = Row_y_index.begin() ; iter != Row_y_index.end() ; iter++)
    {
        Node* head = iter -> second -> head ;
        Node* tail = iter -> second -> tail ;
        Node* ptr = head -> next ;
        if(ptr != tail)
        {
            while(ptr != tail)
            {
                if(ptr -> prev != head)
                {
                    Node* prev = ptr -> prev ;
                    if(ptr -> rectangle.llx <= prev -> rectangle.urx)
                    {
                        if(prev -> rectangle.urx < ptr -> rectangle.urx)
                        {
                            prev -> rectangle.urx = ptr -> rectangle.urx ;
                        }
                        Delete_Node(ptr);
                        ptr = prev ;
                        head -> macro_id-- ;
                    }
                }
                ptr = ptr -> next ;
            }
        }
    }
    if(Debug_PARA)
    {
        //cout << "\tWrite Plot_X.m.."<<endl;
        //Plot_Row(Row_y_index, "./output/corner_stitching_graph/Plot_X.m");
    }

    /// merge obstacles vertically
    if(Row_y_index.size() > 2)
    {
        map<double,Row*>::iterator iter_end = Row_y_index.end() ;
        map<double,Row*>::iterator iter_cmp = Row_y_index.begin();
        bool ptr_cmp_is_same = true ;
        iter = iter_cmp ;
        iter++ ;
        iter_end-- ;
        for(; iter != iter_end ; iter++)
        {
            Node* ptr_head = iter -> second -> head ;
            Node* cmp_head = iter_cmp -> second -> head ;
            unsigned int ptr_node_count = ptr_head -> macro_id ;
            unsigned int cmp_node_count = cmp_head -> macro_id ;
            if(ptr_node_count != cmp_node_count)
            {
                iter_cmp = iter  ;
                continue ;
            }
            else if(ptr_node_count != 0)
            {
                Node* ptr_ptr = ptr_head -> next ;
                Node* ptr_cmp = cmp_head -> next ;
                while(ptr_ptr != iter -> second -> tail)
                {
                    if(ptr_ptr -> rectangle.llx != ptr_cmp -> rectangle.llx || ptr_ptr -> rectangle.urx != ptr_cmp -> rectangle.urx)
                    {
                        ptr_cmp_is_same = false ;
                        break ;
                    }
                    ptr_ptr = ptr_ptr -> next ;
                    ptr_cmp = ptr_cmp -> next ;
                }
            }

            if(ptr_cmp_is_same == false)
            {
                iter_cmp = iter  ;
                ptr_cmp_is_same = true ;
            }
            else
            {
                map<double,Row*>::iterator iter_delete = iter ;
                Row_y_index.erase(iter_delete);
                iter = iter_cmp ;
            }
        }
    }
    if(Debug_PARA)
    {
        //cout << "\tWrite Plot_X.m.."<<endl;
        //Plot_Row(Row_y_index, "./output/corner_stitching_graph/Plot_Y.m");
    }

    return Row_y_index;
}

bool sort_x_coordinate(Macro* a, Macro* b)
{
    if(a->lg.llx == b->lg.llx)
    {
        return a->lg.lly < b->lg.lly ;
    }
    return a->lg.llx < b->lg.llx ;
}

void Check_overlap(bool GP_or_LG)
{
    //cout<<"*****"<<"  "<<"*   *"<<"  "<<"*****"<<"  "<<"*****"<<"  "<<"**  *"<<"  "<<"*****"<<"  "<<"*****"<<"  "<<"*****"<<endl;
    //cout<<"*    "<<"  "<<"*   *"<<"  "<<"*    "<<"  "<<"*    "<<"  "<<"** * "<<"  "<<"*    "<<"  "<<"*   *"<<"  "<<"*****"<<endl;
    //cout<<"*    "<<"  "<<"*****"<<"  "<<"*****"<<"  "<<"*    "<<"  "<<"***  "<<"  "<<"*****"<<"  "<<"*****"<<"  "<<" *** "<<endl;
    //cout<<"*    "<<"  "<<"*   *"<<"  "<<"*    "<<"  "<<"*    "<<"  "<<"** * "<<"  "<<"*    "<<"  "<<"* ** "<<"  "<<" *** "<<endl;
    //cout<<"*****"<<"  "<<"*   *"<<"  "<<"*****"<<"  "<<"*****"<<"  "<<"**  *"<<"  "<<"*****"<<"  "<<"*  **"<<"  "<<"  *  "<<endl;
    if(GP_or_LG == true)
    {
        for(unsigned int i = 0 ; i < macro_list.size() ; i++)
        {
            macro_list[i] -> lg = macro_list[i] -> gp ;
        }
    }
    vector<Macro*>  Check_overlap_movable_fixed_macro ;
    unsigned int bound = MovableMacro_ID.size() + PreplacedMacro_ID.size() ;
    Check_overlap_movable_fixed_macro.resize(bound) ;
    double max_width = -1 ;

    for(unsigned int i = 0 ; i < MovableMacro_ID.size() ; i++)      //movable macros
    {
        unsigned int macro_id = MovableMacro_ID[i] ;
        Check_overlap_movable_fixed_macro.at(i) = macro_list.at(macro_id) ;
        if(macro_list[macro_id] -> w > max_width)
        {
            max_width = macro_list[macro_id] -> w ;
        }
    }


    for(unsigned int i = MovableMacro_ID.size() ; i < bound  ; i++)     //preplaced Macros
    {
        unsigned int macro_id = PreplacedMacro_ID.at(i - MovableMacro_ID.size()) ;
        Check_overlap_movable_fixed_macro.at(i) = macro_list.at(macro_id) ;
        if(macro_list[macro_id] -> w > max_width)
        {
            max_width = macro_list[macro_id] -> w ;
        }
    }

    sort(Check_overlap_movable_fixed_macro.begin(), Check_overlap_movable_fixed_macro.end(), sort_x_coordinate) ;
    //unsigned int prev_ID = 0 ;
    vector<int> overlap_macro_name ;

    for(unsigned int i = 0 ; i < Check_overlap_movable_fixed_macro.size() ; i++ )
    {
        if(Check_overlap_movable_fixed_macro[i] -> macro_type == MOVABLE_MACRO)
        {
            unsigned int j = i ;
            double llx_bound = Check_overlap_movable_fixed_macro[i] -> lg.llx ;
            double llx_minus_max_width_bound = Max_cal(llx_bound - max_width, chip_boundary.llx) ;
            double urx_bound = Check_overlap_movable_fixed_macro[j] -> lg.urx ;
            double lly_i = Check_overlap_movable_fixed_macro[j] -> lg.lly ;
            double ury_i = Check_overlap_movable_fixed_macro[j] -> lg.ury ;


            while(++j < Check_overlap_movable_fixed_macro.size())
            {
                double llx_j = Check_overlap_movable_fixed_macro[j] -> lg.llx ;
                //  double urx_j = Check_overlap_movable_fixed_macro[j] -> lg.urx ;
                double lly_j = Check_overlap_movable_fixed_macro[j] -> lg.lly ;
                double ury_j = Check_overlap_movable_fixed_macro[j] -> lg.ury ;

                if(llx_j > urx_bound)
                {
                    break ;
                }
                if(!(ury_j <= lly_i || lly_j >= ury_i)) // overlap
                {

                    if(overlap_macro_name.size() != 0)
                    {
                        if(overlap_macro_name[overlap_macro_name.size()-1] == Check_overlap_movable_fixed_macro[i] -> macro_id)
                        {
                            continue ;
                        }
                    }

                    overlap_macro_name.push_back(Check_overlap_movable_fixed_macro[i] -> macro_id) ; // push into overlap_macro_name
                }
            }
            j = i ;
            while(j != 0)
            {
                --j ;
                double llx_j = Check_overlap_movable_fixed_macro[j] -> lg.llx ;
                double urx_j = Check_overlap_movable_fixed_macro[j] -> lg.urx ;
                double lly_j = Check_overlap_movable_fixed_macro[j] -> lg.lly ;
                double ury_j = Check_overlap_movable_fixed_macro[j] -> lg.ury ;
//                if(Check_overlap_movable_fixed_macro[i] -> macro_id == 305)
//                {
//                    cout<<"*********** "<<Check_overlap_movable_fixed_macro[j] -> macro_id<<" ***********"<<endl;
//                }
                if(llx_minus_max_width_bound > llx_j)
                {
                    break ;
                }
                if(urx_j < llx_bound)
                {
                    if(j == 0 )
                    {
                        break ;
                    }
                    continue ;
                }
                if(!(ury_j <= lly_i || lly_j >= ury_i))
                {
                    if(overlap_macro_name.size() != 0)
                    {
                        if(overlap_macro_name[overlap_macro_name.size()-1] == Check_overlap_movable_fixed_macro[i] -> macro_id)
                        {
                            continue ;
                        }
                    }
                    overlap_macro_name.push_back(Check_overlap_movable_fixed_macro[i] -> macro_id) ;
                }
                if(j == 0 )
                {
                    break ;
                }
            }
        }
    }
    if(GP_or_LG == false && overlap_macro_name.size() != 0)
    {


        cout << "[INFO] overlap number: " << overlap_macro_name.size() << endl;
        //cout<<"*****"<<"  "<<"*   *"<<"  "<<"*****"<<"  "<<"*****"<<"  "<<"*    "<<"  "<<"  *  "<<"  "<<"*****"<<"  "<<"*****"<<endl;
        //cout<<"*   *"<<"  "<<"*   *"<<"  "<<"*    "<<"  "<<"*   *"<<"  "<<"*    "<<"  "<<" * * "<<"  "<<"*   *"<<"  "<<"*****"<<endl;
        //cout<<"*   *"<<"  "<<" * * "<<"  "<<"*****"<<"  "<<"*****"<<"  "<<"*    "<<"  "<<"*****"<<"  "<<"*****"<<"  "<<" *** "<<endl;
        //cout<<"*   *"<<"  "<<" * * "<<"  "<<"*    "<<"  "<<"*  * "<<"  "<<"*    "<<"  "<<"*   *"<<"  "<<"*    "<<"  "<<" *** "<<endl;
        //cout<<"*****"<<"  "<<"  *  "<<"  "<<"*****"<<"  "<<"*  **"<<"  "<<"*****"<<"  "<<"*   *"<<"  "<<"*    "<<"  "<<"  *  "<<endl;
        //cout <<"******** overlap name ********"<< endl;

        for(unsigned int i = 0 ; i < overlap_macro_name.size() ; i++)
        {
            cerr << "[WARNING] overlap Macro ID: " << overlap_macro_name[i] << endl;
        }

        //cout << "******** overlap name end ********" << endl;
    }
    Check_overlap_movable_fixed_macro.clear() ;
}

//[NOTE] this function will define macro sapcing
void MacroInflation() // explane macro by spacing
{
    int total_pin_num = 0; // totla macro's pins
    for(int i = 0; i < (int)DEFLEFInfo.LEF_Macros.size(); i++)
    {
        LefMacroType &lef_tmp = DEFLEFInfo.LEF_Macros[i];

        // total macro's pin number
        //if(lef_tmp.type == MOVABLE_MACRO)
        //{
            total_pin_num += lef_tmp.macro_count * lef_tmp.Pins.size();
            //cout << " total_pin_num: " << total_pin_num << " lef_tmp.macro_count: " << lef_tmp.macro_count << " lef_tmp.Pins.size(): " << lef_tmp.Pins.size()<< endl;
        //}
    }

    float avg_pin_num; // average pin number of moveable macro
    avg_pin_num = total_pin_num / (float)MovableMacro_ID.size();

    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro* macro_assign = macro_list[macro_id];
        LefMacroType &lef_tmp = DEFLEFInfo.LEF_Macros[macro_assign->lef_type_ID];

        // if the pin number of the macro is bigger then the number moveable macro, the weight is bigger then one
        float pin_weight = lef_tmp.Pins.size() / avg_pin_num;
        float type_weight = lef_tmp.macro_count / (float)MovableMacro_ID.size();  // type_weight is the radio of // not used now

        int Spacing = PARAMETER._SPACING * (1 + pin_weight + type_weight * 1.4);
        //cout << "Macor spacing: " << Spacing << " PARA: " << PARA << " _Spacing: " <<  PARAMETER._SPACING << " pin_weight: " << pin_weight << " type_weight: " << type_weight << endl;

        macro_assign-> Spacing = Spacing;
        macro_assign->cal_h             = macro_assign->cal_h           + Spacing * 2;
        macro_assign->cal_h_wo_shrink   = macro_assign->cal_h_wo_shrink + Spacing * 2;
        macro_assign->cal_w             = macro_assign->cal_w           + Spacing * 2;
        macro_assign->cal_w_wo_shrink   = macro_assign->cal_w_wo_shrink + Spacing * 2;
        macro_assign->area              = macro_assign->cal_w_wo_shrink / (float)PARA * macro_assign->cal_h_wo_shrink / (float)PARA;
        macro_assign->gp.llx            = macro_assign->gp.llx;
        macro_assign->lg.llx            = macro_assign->lg.llx;
        macro_assign->gp.lly            = macro_assign->gp.lly;
        macro_assign->lg.lly            = macro_assign->lg.lly;
        macro_assign->gp.urx            = macro_assign->gp.llx + macro_assign->cal_w_wo_shrink;
        macro_assign->lg.urx            = macro_assign->gp.llx + macro_assign->cal_w_wo_shrink;
        macro_assign->gp.ury            = macro_assign->gp.lly + macro_assign->cal_h_wo_shrink;
        macro_assign->lg.ury            = macro_assign->gp.lly + macro_assign->cal_h_wo_shrink;
        macro_assign->h                 = macro_assign->h;
        macro_assign->w                 = macro_assign->w;
        macro_assign->HeightShrinkPARA  = ShrinkPara;
        macro_assign->WidthShrinkPARA   = ShrinkPara;
        macro_assign->macro_name        = macro_assign->macro_name;
        macro_assign->macro_type        = macro_assign->macro_type;
        macro_assign->HierStdArea       = macro_assign->HierStdArea;
        macro_assign->RefCoor           = make_pair(macro_assign->gp.llx, macro_assign->gp.lly);

    }

}


#define MAX_SPACING 20
#define MIN_SPACING 2.5

void MacroInflation_new() // 2020.06.10
{
    //int chip_hpwl =  ((chip_boundary.urx - chip_boundary.llx) / (float)PARA + (chip_boundary.ury - chip_boundary.lly) / (float)PARA);
    //cout << chip_hpwl << endl;

    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro* macro_assign = macro_list[macro_id];

        //float hpwl_radio = (float)(macro_assign-> w +  macro_assign-> h) / PARA / chip_hpwl;
        int Spacing =  PARAMETER._SPACING + macro_assign-> pin_num * PARAMETER._SPACING * 0.005;

        if(macro_assign-> h < 3* PARAMETER._SPACING) Spacing = MIN_SPACING * PARA;
        if(Spacing > MAX_SPACING * PARA)             Spacing = MAX_SPACING * PARA;

        //cout << " Spacing: " << Spacing << " Pin_num: "<< macro_assign-> pin_num << " Area: " << macro_assign-> area << endl;

        macro_assign-> Spacing = Spacing;
        macro_assign->cal_h             = macro_assign->cal_h + Spacing * 2;
        macro_assign->cal_h_wo_shrink   = macro_assign->cal_h_wo_shrink + Spacing * 2;
        macro_assign->cal_w             = macro_assign->cal_w + Spacing * 2;
        macro_assign->cal_w_wo_shrink   = macro_assign->cal_w_wo_shrink + Spacing * 2;
        macro_assign->area              = macro_assign->cal_w_wo_shrink / (float)PARA * macro_assign->cal_h_wo_shrink / (float)PARA;
        macro_assign->gp.llx            = macro_assign->gp.llx;
        macro_assign->lg.llx            = macro_assign->lg.llx;
        macro_assign->gp.lly            = macro_assign->gp.lly;
        macro_assign->lg.lly            = macro_assign->lg.lly;
        macro_assign->gp.urx            = macro_assign->gp.llx + macro_assign->cal_w_wo_shrink;
        macro_assign->lg.urx            = macro_assign->gp.llx + macro_assign->cal_w_wo_shrink;
        macro_assign->gp.ury            = macro_assign->gp.lly + macro_assign->cal_h_wo_shrink;
        macro_assign->lg.ury            = macro_assign->gp.lly + macro_assign->cal_h_wo_shrink;
        macro_assign->h                 = macro_assign->h;
        macro_assign->w                 = macro_assign->w;
        macro_assign->HeightShrinkPARA  = ShrinkPara;
        macro_assign->WidthShrinkPARA   = ShrinkPara;
        macro_assign->macro_name        = macro_assign->macro_name;
        macro_assign->macro_type        = macro_assign->macro_type;
        macro_assign->HierStdArea       = macro_assign->HierStdArea;
        macro_assign->RefCoor           = make_pair(macro_assign->gp.llx, macro_assign->gp.lly);
    }
}





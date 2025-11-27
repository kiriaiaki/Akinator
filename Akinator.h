#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include "Stack.h"

struct node_k
{
    unsigned long Verification;
    char* Str;
    node_k* Left;
    node_k* Right;
};

struct tree_k
{
    size_t Size;
    node_k* Root;
};

enum Name_Tree_Error
{
    Not_Error_Tree  =  0,
    Not_Struct_Tree =  1,
    Not_Root        =  2,
    Bad_Root        =  3,
    Bad_Left        =  4,
    Bad_Right       =  5,
    Bad_Size        =  6,
    Quantity_Error  =  7
};

enum Direction
{
    Left  =  1,
    Right =  2,
};

const char* Array_Name_Tree_Error[Quantity_Error] = {
    "NOT ERROR TREE",
    "!!!NOT STRUCT TREE!!!",
    "!!!NOT ROOT!!!",
    "!!!BAD ROOT!!!",
    "!!!BAD LEFT!!!",
    "!!!BAD RIGHT!!!",
    "!!!BAD SIZE!!!",
};

const int There_Are_Errors      =                -2902;
const int Canary                =                 2008;
const int Finish                =                    7;
const int Match                 =                    0;
const int Vary                  =                    1;
const char Name_Log[]           =      "Log_File.html";
const char Name_Base_Data[]     =      "Base_Data.txt";
const char New_Name_Base_Data[] =  "Base_Data_New.txt";
node_k* Stop_Pointer            = (node_k*) 0xDEADBEEF;

const double Minimum_Contrast_Ratio    =   6;

int Tree_Ctor_From_Base (tree_k* const Tree);
int New_Tree_Ctor       (tree_k* const Tree);
int Tree_Dtor           (tree_k* const Tree);

int Tree_Error         (const tree_k* const Tree);
int Check_Verification (const node_k* Node, size_t* const Counter_Element);

int Tree_Dump          (const tree_k* const Tree, const char* const Name_Function);
int Naming_File        (char* const Name_File);
int Naming_Command_Dot (char* const Name_Command, char* const Name_File);
int Dump_For_Graph     (const tree_k* const Tree, FILE* const file_graph);
int Dump_Node          (const node_k* const Node, FILE* const file_graph);
int Dump_For_Html      (const tree_k* const Tree, const char* const Name_File, const char* const Name_Function);

int Save_Tree_In_File  (const tree_k* const Tree);
int Print_Node         (const node_k* const Node, FILE* const Data_Base);
node_k* Read_Node      (char** const Current_Position, size_t* const Counter_Node);
int Check_End          (char** const Current_Position);
int Check_Nil          (char** const Current_Position);

node_k* Tree_Append    (node_k* const Node, tree_k* const Tree);
node_k* Append_Sons    (node_k* const Node);

int Delete_Subtree     (node_k* Node, size_t* const Counter_Delete);

int Launch           (tree_k* const Tree);
int Run              (tree_k* const Tree);
int Play             (tree_k* const Tree);
int Append_Person    (node_k* const Current_Node, tree_k* const Tree);
int User_Save        (const tree_k* const Tree);
int Definition_Node  (const tree_k* const Tree);
int Comparison_Nods  (const tree_k* const Tree);
int Table_Comparison (stack_k* const Stack_Return_1, stack_k* const Stack_Return_2, node_k* const Root);
node_k* Search_Node  (const char* const String, node_k* Node, stack_k* const Stack_Return);

int Start_Logfile           ();
int Print_Separator_In_Log  (const size_t Len_Separator, FILE* file_html);
int Reverse_Str             (char* const Str);
char* itoa_k                (int Number, char* const Str);
unsigned int Generate_Color (const void *Ptr, unsigned int Color_Text);
unsigned int Hash_Pointer   (const void* const ptr);
double Calculate_Contrast   (unsigned int Color, unsigned int Color_Text);
int Copy_File_In_Buffer     (char** const Buffer);
int Clean_Stdin             ();
char* Read_Answer           ();
int getline_k               (char** const Str);
int Delete_Slash_N          (char* const Str);

int print_k(const char* Format, ...) __attribute__((format(printf, 1, 2)));

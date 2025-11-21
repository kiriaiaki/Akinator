#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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
const char Name_Log[]           =      "Log_File.html";
const char Name_Base_Data[]     =      "Base_Data.txt";
const char New_Name_Base_Data[] =  "Base_Data_New.txt";
node_k* Stop_Pointer            = (node_k*) 0xDEADBEEF;

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

int Print_Tree_In_File (const tree_k* const Tree);
int Print_Node         (const node_k* const Node, FILE* const Data_Base);
node_k* Read_Node      (char** const Current_Position, size_t* const Counter_Node);
int Check_End          (char** const Current_Position);
int Check_Nil          (char** const Current_Position);

node_k* Tree_Append    (node_k* const Node, tree_k* const Tree);
node_k* Append_Sons    (node_k* const Node);

int Delete_Subtree     (node_k* Node, size_t* const Counter_Delete);

int Start_Logfile          ();
int Print_Separator_In_Log (const size_t Len_Separator, FILE* file_html);
int Reverse_Str            (char* const Str);
char* itoa_k               (int Number, char* const Str);
unsigned int Hash_Pointer  (const void* const ptr);
int Copy_File_In_Buffer    (char** const Buffer);


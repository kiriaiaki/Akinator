#include "Akinator.h"
#define DEBUG // включает верификаторы и создание logfile
//#define STOP_PROGRAMME // в случае выявления ошибок в списке программа останавливается
#define LIGHT_DUMP

int main ()
{
    #ifdef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
            return 0;
        }
    #endif // DEBUG

    tree_k Tree = {};
    if (Tree_Ctor_From_Base (&Tree) == There_Are_Errors)
    {
        return 0;
    }



    if (Print_Tree_In_File (&Tree) == There_Are_Errors)
    {
        return 0;
    }

    #ifndef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
            return 0;
        }

        if (Tree_Dump (&Tree, "After all function") == There_Are_Errors)
        {
            printf ("%s:%d: Error dump", __FILE__, __LINE__);
            return 0;
        }
    #endif // DEBUG

    Tree_Dtor (&Tree);
    return 0;
}


int New_Tree_Ctor       (tree_k* const Tree)
{
    Tree->Root = (node_k*) calloc (1, sizeof (node_k));
    if (Tree->Root == NULL)
    {
        printf ("%s:%d: Error allocation memory\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    Tree->Root->Verification = (uintptr_t) (Tree->Root) ^ Canary;

    Tree->Root->Left = (node_k*) calloc (1, sizeof (node_k));
    if (Tree->Root->Left == NULL)
    {
        printf ("%s:%d: Error allocation memory\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }
    (Tree->Root->Left)->Verification = (uintptr_t) (Tree->Root->Left) ^ Canary;
    (Tree->Root->Left)->Left = NULL;
    (Tree->Root->Left)->Right = NULL;

    Tree->Root->Right = (node_k*) calloc (1, sizeof (node_k));
    if (Tree->Root->Right == NULL)
    {
        printf ("%s:%d: Error allocation memory\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }
    (Tree->Root->Right)->Verification = (uintptr_t) (Tree->Root->Right) ^ Canary;
    (Tree->Root->Right)->Left = NULL;
    (Tree->Root->Right)->Right = NULL;

    Tree->Size = 3;

    size_t Len = 0;

    printf ("Введите первый вопрос: ");
    getline (&Tree->Root->Str, &Len, stdout);

    printf ("Введите на него ответ, если ответ \"ДА\": ");
    if (getline (&Tree->Root->Left->Str, &Len, stdout) == -1)
    {
        printf ("%s:%d: Error getline\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    printf ("Введите на него ответ, если ответ \"НЕТ\": ");
    if (getline (&Tree->Root->Right->Str, &Len, stdout) == -1)
    {
        printf ("%s:%d: Error getline\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    #ifdef DEBUG
        if (Tree_Dump (Tree, "New_Tree_Ctor (Tree)") == There_Are_Errors)
        {
            printf ("%s:%d: Error dump\n", __FILE__, __LINE__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int Tree_Ctor_From_Base (tree_k* const Tree)
{
    char* Buffer = NULL;
    if (Copy_File_In_Buffer (&Buffer) == There_Are_Errors)
    {
        return There_Are_Errors;
    }
    char* Copy_Buffer = Buffer;

    size_t Counter_Node = 0;

    Tree->Root = Read_Node (&Buffer, &Counter_Node);
    if (Tree->Root == Stop_Pointer)
    {
        printf ("%s:%d: Programme not start!!!\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    Tree->Size += Counter_Node;

    free (Copy_Buffer);

    #ifdef DEBUG
        if (Tree_Dump (Tree, "Tree_Ctor_From_Base (Tree, Buffer)") == There_Are_Errors)
        {
            printf ("%s:%d Error dump\n", __FILE__, __LINE__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int Tree_Dtor           (tree_k* const Tree)
{
    #ifdef DEBUG
        if (Tree_Error (Tree) != Not_Error_Tree)
        {
            if (Tree_Dump (Tree, "Tree_Dtor (Tree)") == There_Are_Errors)
            {
                printf ("%s:%d Error dump\n", __FILE__, __LINE__);
                return There_Are_Errors;
            }

            printf ("%s:%d Error start function Tree_Dtor (Tree)\n", __FILE__, __LINE__);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    size_t Counter_Delete = 0;

    Delete_Subtree (Tree->Root, &Counter_Delete);

    Tree->Size = 0;
    free (Tree->Root);
    Tree->Root = NULL;

    return 0;
}


int Tree_Error         (const tree_k* const Tree)
{
    if (Tree == NULL)
    {
        return Not_Struct_Tree;
    }

    if (Tree->Root == NULL)
    {
        return Not_Root;
    }

    if (Tree->Root->Verification != ((uintptr_t) Tree->Root ^ Canary))
    {
        return Bad_Root;
    }

    size_t Counter_Element = 0;
    int Er = Check_Verification (Tree->Root, &Counter_Element);
    if (Er != Not_Error_Tree)
    {
        return Er;
    }

    if (Counter_Element != Tree->Size)
    {
        return Bad_Size;
    }

    return Not_Error_Tree;
}

int Check_Verification (const node_k* Node, size_t* const Counter_Element)
{
    if (Node->Left != NULL)
    {
        if (Node->Left->Verification != ((uintptr_t) (Node->Left) ^ Canary))
        {
            return Bad_Left;
        }

        else
        {
            int A = Check_Verification (Node->Left, Counter_Element);
            if (A != Not_Error_Tree)
            {
                return A;
            }
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification != ((uintptr_t) (Node->Right) ^ Canary) )
        {
            return Bad_Right;
        }

        else
        {
            int B = Check_Verification (Node->Right, Counter_Element);
            if (B != Not_Error_Tree)
            {
                return B;
            }
        }
    }

    (*Counter_Element)++;
    return Not_Error_Tree;
}


int Tree_Dump          (const tree_k* const Tree, const char* const Name_Function)
{
    char Name_File_Graph[30] = "File_txt/File_";

    Naming_File (Name_File_Graph);

    FILE* file_graph = fopen (Name_File_Graph, "w");
    if (file_graph == NULL)
    {
        printf ("%s:%d Error open file for graph\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    Dump_For_Graph (Tree, file_graph);

    fclose (file_graph);

    char Command[64] = "dot ";
    Naming_Command_Dot (Command, Name_File_Graph);

    system (Command);

    if (Dump_For_Html (Tree, Name_File_Graph, Name_Function) != Not_Error_Tree)
    {
        return There_Are_Errors;
    }

    return 0;
}

int Naming_File        (char* const Name_File)
{
    static int Counter = 1;

    char Temp_Str[11];
    itoa_k (Counter, Temp_Str);

    strcat(Name_File, Temp_Str);
    strcat(Name_File, ".txt");

    Counter++;
    return 0;
}

int Naming_Command_Dot (char* const Name_Command, char* const Name_File)
{
    strcat(Name_Command, Name_File);
    strcat(Name_Command, " -T png -o ");

    Name_File[5] = 'p';
    Name_File[6] = 'n';
    Name_File[7] = 'g';
    size_t Len = strlen (Name_File);
    Name_File[Len - 3] = 'p';
    Name_File[Len - 2] = 'n';
    Name_File[Len - 1] = 'g';
    strcat(Name_Command, Name_File);

    return 0;
}

int Dump_For_Graph     (const tree_k* const Tree, FILE* const file_graph)
{
    fprintf (file_graph, "digraph G\n"
                   "{\n"
                   "    node[shape = \"doubleoctagon\", style = \"filled\", fillcolor = \"red\"];\n"
                   "    edge[color = \"red\", style = \"bold\"];\n");
    fprintf (file_graph, "\n");

    #ifndef LIGHT_DUMP
    fprintf (file_graph, "    root [shape = invhouse, label = \"root = %p\", style = \"filled\", fillcolor = \"#%06x\"];\n", Tree->Root, Hash_Pointer (Tree->Root));
    fprintf (file_graph, "    root -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Tree->Root, Hash_Pointer (Tree->Root));
    fprintf (file_graph, "\n");
    #endif // LIGHT_DUMP

    Dump_Node (Tree->Root, file_graph);

    fprintf (file_graph, "}\n");
    return 0;
}

int Dump_Node          (const node_k* const Node, FILE* const file_graph)
{
    #ifndef LIGHT_DUMP
    fprintf (file_graph, "    node_%lx [shape = Mrecord, label = <<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\" CELLPADDING = \"4\" BGCOLOR = \"lightgrey\">\n"
                         "        <TR> <TD COLSPAN = \"2\" BGCOLOR = \"#%06x\"> <b>ptr</b>   = %p  </TD> </TR>\n"
                         "        <TR> <TD COLSPAN = \"2\"> <b>ver</b> = %lx </TD> </TR>\n"
                         "        <TR> <TD COLSPAN = \"2\"> <b>str</b> = %s  </TD> </TR>\n"
                         "        <TR> <TD BGCOLOR = \"#%06x\"> <b>left</b> = %p </TD>\n"
                         "             <TD BGCOLOR = \"#%06x\"> <b>right</b> = %p </TD> </TR>\n"
                         "        </TABLE>>, style = \"filled\", fillcolor = \"lightgrey\"];\n\n", (uintptr_t) Node, Hash_Pointer (Node), Node, Node->Verification, Node->Str, Hash_Pointer (Node->Left), Node->Left, Hash_Pointer (Node->Right), Node->Right);
    #endif // LIGHT_DUMP

    #ifdef LIGHT_DUMP
    if (Node->Left == NULL && Node->Right == NULL)
    {
        fprintf (file_graph, "    node_%lx [shape = Mrecord, label = \"%s\", style = \"filled\", fillcolor = \"lightgreen\"];\n\n", (uintptr_t) Node, Node->Str);
    }

    else
    {
        fprintf (file_graph, "    node_%lx [shape = Mrecord, label = \"%s\", style = \"filled\", fillcolor = \"lightblue\"];\n\n", (uintptr_t) Node, Node->Str);
    }
    #endif // LIGHT_DUMP

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            #ifndef LIGHT_DUMP
            fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left, Hash_Pointer (Node->Left));
            Dump_Node (Node->Left, file_graph);
            #endif // LIGHT_DUMP

            #ifdef LIGHT_DUMP
            fprintf (file_graph, "    node_%lx -> node_%lx [color = \"green\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left);
            Dump_Node (Node->Left, file_graph);
            #endif // LIGHT_DUMP
        }

        else
        {
            fprintf (file_graph, "    node_%lx -> node_%lx [label = \"Невозможный указатель right\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left);
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary))
        {
            #ifndef LIGHT_DUMP
            fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right, Hash_Pointer (Node->Right));
            Dump_Node (Node->Right, file_graph);
            #endif // LIGHT_DUMP

            #ifdef LIGHT_DUMP
            fprintf (file_graph, "    node_%lx -> node_%lx [color = \"red\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right);
            Dump_Node (Node->Right, file_graph);
            #endif // LIGHT_DUMP
        }

        else
        {
            fprintf (file_graph, "    node_%lx -> node_%lx [label = \"Невозможный указатель right\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right);
        }
    }

    return 0;

}

int Dump_For_Html      (const tree_k* const Tree, const char* const Name_File_Graph, const char* const Name_Function)
{
    FILE* file = fopen (Name_Log, "a");
    if (file == NULL)
    {
        printf ("%s:%d Error open logfile\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    fprintf (file, "<u>Dump after function</u> %s\n\n", Name_Function);

    int Number_Error = Tree_Error (Tree);
    if (Number_Error != 0)
    {
        fprintf (file, "                          <b>%s</b>\n", Array_Name_Tree_Error[Number_Error]);
    }

    fprintf (file, "SIZE = %zu\n", Tree->Size);
    fprintf (file, "ROOT = %p\n", Tree->Root);
    fprintf (file, "\n");

    fprintf (file, "<img src = \"%s\">\n", Name_File_Graph);
    fprintf (file, "\n");

    Print_Separator_In_Log (400, file);
    fclose (file);

    #ifdef STOP_PROGRAMME
    return Number_Error;
    #endif // STOP_PROGRAMME

    return 0;
}


int Print_Tree_In_File (const tree_k* const Tree)
{
    FILE* Data_Base = fopen (New_Name_Base_Data, "w");
    if (Data_Base == NULL)
    {
        printf ("%s:%d: Error open file for new base data\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    Print_Node (Tree->Root, Data_Base);

    return 0;
}

int Print_Node         (const node_k* const Node, FILE* const Data_Base)
{
    fprintf (Data_Base, "(");
    fprintf (Data_Base, " \"%s\" ", Node->Str);

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            Print_Node (Node->Left, Data_Base);
        }
    }

    else
    {
        fprintf (Data_Base, "nil ");
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary))
        {
            Print_Node (Node->Right, Data_Base);
        }
    }

    else
    {
        fprintf (Data_Base, "nil ");
    }

    fprintf (Data_Base, ")");

    return 0;

}

node_k* Read_Node      (char** const Current_Position, size_t* const Counter_Node)
{
    if (**Current_Position == '(')
    {
        (*Current_Position)++;
        node_k* Node = (node_k*) calloc (1, sizeof (node_k));
        if (Node == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node\n", __FILE__, __LINE__);
            return Stop_Pointer;
        }
        Node->Str = (char*) calloc (100, sizeof (char));
        if (Node->Str == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node\n", __FILE__, __LINE__);
            return Stop_Pointer;
        }
        Node->Verification = (uintptr_t) Node ^ Canary;

        int Len = 0;
        sscanf ((*Current_Position), " \"%[^\"]\" %n", Node->Str, &Len);
        (*Current_Position) += Len;
        (*Counter_Node)++;

        Node->Left = Read_Node (Current_Position, Counter_Node);
        if (Node->Left == Stop_Pointer)
        {
            return Stop_Pointer;
        }

        Node->Right = Read_Node (Current_Position, Counter_Node);
        if (Node->Right == Stop_Pointer)
        {
            return Stop_Pointer;
        }

        if (Check_End (Current_Position) == There_Are_Errors)
        {
            return Stop_Pointer;
        }

        return Node;
    }

    else
    {
        if (Check_Nil (Current_Position) == There_Are_Errors)
        {
            return Stop_Pointer;
        }

        return NULL;
    }
}

int Check_End          (char** const Current_Position)
{
    if (**Current_Position == ')')
    {
        (*Current_Position)++;
        return 0;
    }

    else
    {
        printf ("%s:%d: Error on\n", __FILE__, __LINE__);
        printf ("%s:1:  Error: You have bad base data\n", Name_Base_Data);
        return There_Are_Errors;
    }
}

int Check_Nil          (char** const Current_Position)
{
    if (**Current_Position == 'n')
    {
        (*Current_Position)++;

        if (**Current_Position == 'i')
        {
            (*Current_Position)++;

            if (**Current_Position == 'l')
            {
                (*Current_Position)++;

                if (**Current_Position == ' ')
                {
                    (*Current_Position)++;

                    return 0;
                }
            }
        }
    }

    printf ("%s:%d: Error startup\n", __FILE__, __LINE__);
    printf ("%s:1:  Error: You have bad base data\n", Name_Base_Data);
    return There_Are_Errors;
}


node_k* Tree_Append    (node_k* const Node, tree_k* const Tree)
{
    #ifdef DEBUG
        char Name_Func[52];
        snprintf (Name_Func, sizeof (Name_Func), "Tree_Append (%p, Tree)", Node);

        if (Tree_Error (Tree) != Not_Error_Tree)
        {
            if (Tree_Dump (Tree, Name_Func) == There_Are_Errors)
            {
                printf ("%s:%d: Error dump %s\n", __FILE__, __LINE__, Name_Func);
                return NULL;
            }

            printf ("%s:%d: Error start function %s\n", __FILE__, __LINE__, Name_Func);
            #ifdef STOP_PROGRAMME
                return NULL;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    if (Node->Left != NULL || Node->Right != NULL)
    {
        printf ("%s:%d: Error bad Node in function Tree_Append (%p, Tree)\n", __FILE__, __LINE__, Node);
        return NULL;
    }

    node_k* New_Node = Append_Sons (Node);
    if (New_Node == NULL)
    {
        return NULL;
    }

    Tree->Size += 2;

    size_t Len = 0;

    printf ("Кого ты загадал: ");
    if (getline (&Tree->Root->Left->Str, &Len, stdout) == -1)
    {
        printf ("%s:%d: Error getline\n", __FILE__, __LINE__);
        return NULL;
    }

    printf ("Чем %s отличается от %s?\n", Node->Left->Str, Node->Right->Str);
    printf ("Дополни фразу: Он(а)... ");
    if (getline (&Tree->Root->Left->Str, &Len, stdout) == -1)
    {
        printf ("%s:%d: Error getline\n", __FILE__, __LINE__);
        return NULL;
    }

    #ifdef DEBUG
        if (Tree_Dump (Tree, Name_Func) == There_Are_Errors)
        {
            printf ("%s:%d: Error dump\n", __FILE__, __LINE__);
            return NULL;
        }
    #endif // DEBUG

    return New_Node;
}

node_k* Append_Sons    (node_k* const Node)
{
    if (Node->Verification == ((uintptr_t) (Node) ^ Canary))
    {
        Node->Left = (node_k*) calloc (1, sizeof (node_k));
        if (Node->Left == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node\n", __FILE__, __LINE__);
            return NULL;
        }

        Node->Right = (node_k*) calloc (1, sizeof (node_k));
        if (Node->Right == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node\n", __FILE__, __LINE__);
            return NULL;
        }

        (Node->Left)->Verification = (uintptr_t) (Node->Left) ^ Canary;
        (Node->Left)->Str = (char*) calloc (100, sizeof (char));
        if ((Node->Left)->Str == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node\n", __FILE__, __LINE__);
            return NULL;
        }
        (Node->Left)->Left = NULL;
        (Node->Left)->Right = NULL;

        (Node->Right)->Verification = (uintptr_t) (Node->Right) ^ Canary;
        (Node->Right)->Str = strdup (Node->Str);
        if ((Node->Right)->Str == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node\n", __FILE__, __LINE__);
            return NULL;
        }
        (Node->Right)->Left = NULL;
        (Node->Right)->Right = NULL;

        return Node->Left;
    }

    else
    {
        printf ("%s:%d: Error bad node\n", __FILE__, __LINE__);
        return NULL;
    }
}


int Delete_Subtree (node_k* Node, size_t* const Counter_Delete)
{
    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            Delete_Subtree (Node->Left, Counter_Delete);
            (*Counter_Delete)++;
            free (Node->Left->Str);
            free (Node->Left);
            Node->Left = NULL;
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary))
        {
            Delete_Subtree (Node->Right, Counter_Delete);
            (*Counter_Delete)++;
            free (Node->Right->Str);
            free (Node->Right);
            Node->Right = NULL;
        }
    }

    return 0;
}


int Start_Logfile          ()
{
    FILE* file_html = fopen (Name_Log, "w");
    if (file_html == NULL)
    {
        printf ("%s:%d: Error open logfile\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    fprintf (file_html, "<pre>\n\n");

    Print_Separator_In_Log (400, file_html);

    fclose (file_html);

    return 0;
}

int Print_Separator_In_Log (const size_t Len_Separator, FILE* file_html)
{
    for (size_t i = 0; i < Len_Separator / 2; i++)
    {
        fprintf (file_html, "+=");
    }

    fprintf (file_html, "\n\n");

    return 0;
}

int Reverse_Str            (char* const Str)
{
    size_t i = 0;
    size_t j = strlen (Str) - 1;

    while (i < j)
    {
        char Symbol = Str[i];
        Str[i] = Str[j];
        Str[j] = Symbol;
        i++;
        j--;
    }

    return 0;
}

char* itoa_k               (int Number, char* const Str)
{
    size_t i = 0;

    if (Number == 0)
    {
        Str[i] = '0';
        i++;

        Str[i] = '\0';

        return Str;
    }

    while (Number != 0)
    {
        int Rem = Number % 10;

        Str[i] = char (Rem) + '0';
        i++;

        Number = Number / 10;
    }

    Str[i] = '\0';

    Reverse_Str (Str);

    return Str;
}

unsigned int Hash_Pointer  (const void* const ptr)
{
    uintptr_t Ptr_Value = (uintptr_t) ptr;

    unsigned int Hash = 5381;

    unsigned char *Byte_Ptr = (unsigned char *) &Ptr_Value;
    size_t Pointer_Size = sizeof(Ptr_Value);

    for (size_t i = 0; i < Pointer_Size; i++)
    {
        Hash = ((Hash << 7) + Hash) + Byte_Ptr[i];
    }

    return Hash;
}

int Copy_File_In_Buffer    (char** const Buffer)
{
    struct stat Data_File = {};

    if (stat (Name_Base_Data, &Data_File) != 0)
    {
        printf ("%s:%d: Error stat\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    off_t Size_File  = Data_File.st_size;

    int File_Base = open (Name_Base_Data, O_RDONLY);
    if (File_Base == -1)
    {
        printf ("%s:%d: Error open file with base data\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    *Buffer = (char*) calloc (size_t (Size_File) + 1, sizeof (char));
    if (*Buffer == NULL)
    {
        printf ("%s:%d: Error allocation memory for buffer\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    if (read (File_Base, *Buffer, size_t (Size_File)) < Size_File)
    {
        printf ("%s:%d: Error read file base data\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }

    close (File_Base);

    return 0;
}



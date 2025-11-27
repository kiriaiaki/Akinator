#include "Akinator.h"

#define DEBUG // включает верификаторы и создание logfile
//#define STOP_PROGRAMME // в случае выявления ошибок в списке программа останавливается
#define LIGHT_DUMP
#define MAX(A, B) ((A) > (B) ? (A) : (B))


int main ()
{
    #ifdef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    tree_k Tree = {};

    int A = Launch (&Tree);

    if (A == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    if (A != Finish)
    {
        if (Run (&Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    printf ("Пока!\n");

    #ifndef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        if (Tree_Dump (&Tree, "After all function") == There_Are_Errors)
        {
            printf ("%s:%d: Error dump in %s", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    if (Tree.Root != NULL)
    {
        Tree_Dtor (&Tree);
    }

    return 0;
}


int New_Tree_Ctor       (tree_k* const Tree)
{
    Tree->Root = (node_k*) calloc (1, sizeof (node_k));
    if (Tree->Root == NULL)
    {
        printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Tree->Root->Verification = (uintptr_t) (Tree->Root) ^ Canary;

    Tree->Root->Left = (node_k*) calloc (1, sizeof (node_k));
    if (Tree->Root->Left == NULL)
    {
        printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }
    (Tree->Root->Left)->Verification = (uintptr_t) (Tree->Root->Left) ^ Canary;
    (Tree->Root->Left)->Left = NULL;
    (Tree->Root->Left)->Right = NULL;

    Tree->Root->Right = (node_k*) calloc (1, sizeof (node_k));
    if (Tree->Root->Right == NULL)
    {
        printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }
    (Tree->Root->Right)->Verification = (uintptr_t) (Tree->Root->Right) ^ Canary;
    (Tree->Root->Right)->Left = NULL;
    (Tree->Root->Right)->Right = NULL;

    Tree->Size = 3;

    printf ("Какой первый вопрос в твоем дереве, Он(а)...?\n");
    printf ("\nТвой ввод: ");
    if (getline_k (&Tree->Root->Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    printf ("\nКого ты загадал, если ответ \"ДА\"\n");
    printf ("\nТвой ввод: ");
    if (getline_k (&Tree->Root->Left->Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    printf ("\nКого ты загадал, если ответ \"НЕТ\"\n");
    printf ("\nТвой ввод: ");
    if (getline_k (&Tree->Root->Right->Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    #ifdef DEBUG
        if (Tree_Dump (Tree, "New_Tree_Ctor (Tree)") == There_Are_Errors)
        {
            printf ("%s:%d: Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }
    char* Copy_Buffer = Buffer;

    size_t Counter_Node = 0;

    Tree->Root = Read_Node (&Buffer, &Counter_Node);
    if (Tree->Root == Stop_Pointer)
    {
        printf ("%s:%d: Programme not start!!! in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Tree->Size += Counter_Node;

    free (Copy_Buffer);

    #ifdef DEBUG
        if (Tree_Dump (Tree, "Tree_Ctor_From_Base (Tree, Buffer)") == There_Are_Errors)
        {
            printf ("%s:%d Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
                printf ("%s:%d Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return There_Are_Errors;
            }

            printf ("%s:%d Error start in %s\n", __FILE__, __LINE__, __FUNCTION__);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    size_t Counter_Delete = 0;

    if (Tree->Root != NULL && Tree->Root->Verification == ((uintptr_t) Tree->Root ^ Canary))
    {
        Delete_Subtree (Tree->Root, &Counter_Delete);
    }

    Tree->Size = 0;
    free (Tree->Root->Str);
    free (Tree->Root);
    Tree->Root = NULL;

    return 0;
}


int Tree_Error         (const tree_k* const Tree)
{
    if (Tree == NULL)
    {
        printf ("%s:%d: Error ion %s\n", __FILE__, __LINE__, __FUNCTION__);
        return Not_Struct_Tree;
    }

    if (Tree->Root == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return Not_Root;
    }

    if (Tree->Root->Verification != ((uintptr_t) Tree->Root ^ Canary))
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return Bad_Root;
    }

    size_t Counter_Element = 0;
    int Er = Check_Verification (Tree->Root, &Counter_Element);
    if (Er != Not_Error_Tree)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return Er;
    }

    if (Counter_Element != Tree->Size)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return Bad_Left;
        }

        else
        {
            int A = Check_Verification (Node->Left, Counter_Element);
            if (A != Not_Error_Tree)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return A;
            }
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification != ((uintptr_t) (Node->Right) ^ Canary) )
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return Bad_Right;
        }

        else
        {
            int B = Check_Verification (Node->Right, Counter_Element);
            if (B != Not_Error_Tree)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
        printf ("%s:%d Error open file for graph in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Dump_For_Graph (Tree, file_graph);

    fclose (file_graph);

    char Command[64] = "dot ";
    Naming_Command_Dot (Command, Name_File_Graph);

    system (Command);

    if (Dump_For_Html (Tree, Name_File_Graph, Name_Function) != Not_Error_Tree)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
    fprintf (file_graph, "    root [shape = invhouse, label = \"root = %p\", style = \"filled\", fillcolor = \"#%06x\"];\n", Tree->Root, Generate_Color (Tree->Root, 0x000000));
    fprintf (file_graph, "    root -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Tree->Root, Generate_Color (Tree->Root, 0x000000));
    fprintf (file_graph, "\n");
    #endif // LIGHT_DUMP

    if (Tree->Root != NULL && Tree->Root->Verification == ((uintptr_t) Tree->Root ^ Canary))
    {
        Dump_Node (Tree->Root, file_graph);
    }

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
                         "        </TABLE>>, style = \"filled\", fillcolor = \"lightgrey\"];\n\n", (uintptr_t) Node, Generate_Color (Node, 0x000000), Node, Node->Verification, Node->Str, Generate_Color (Node->Left, 0x000000), Node->Left, Generate_Color (Node->Right, 0x000000), Node->Right);
    #endif // LIGHT_DUMP

    #ifdef LIGHT_DUMP
    if (Node->Left == NULL && Node->Right == NULL)
    {
        fprintf (file_graph, "    node_%lx [shape = Mrecord, label = \"%s\", style = \"filled\", fillcolor = \"lightgreen\"];\n\n", (uintptr_t) Node, Node->Str);
    }

    else
    {
        fprintf (file_graph, "    node_%lx [shape = record, label = \"%s?\", style = \"filled\", fillcolor = \"lightblue\"];\n\n", (uintptr_t) Node, Node->Str);
    }
    #endif // LIGHT_DUMP

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            #ifndef LIGHT_DUMP
            fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left, Generate_Color (Node->Left, 0x000000));
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
            fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right, Generate_Color (Node->Right, 0x000000));
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
        printf ("%s:%d Error open logfile in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    fprintf (file, "<u>Dump after function</u> %s\n\n", Name_Function);

    int Number_Error = Tree_Error (Tree);
    if (Number_Error != 0)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        fprintf (file, "                          <b>%s</b>\n", Array_Name_Tree_Error[Number_Error]);
    }

    fprintf (file, "SIZE = %zu\n", Tree->Size);
    fprintf (file, "ROOT = %p\n", Tree->Root);
    fprintf (file, "\n");

    fprintf (file, "<img src = \"%s\">\n", Name_File_Graph);
    fprintf (file, "\n");

    Print_Separator_In_Log (200, file);
    fclose (file);

    #ifdef STOP_PROGRAMME
    return Number_Error;
    #endif // STOP_PROGRAMME

    return 0;
}


int Save_Tree_In_File  (const tree_k* const Tree)
{
    FILE* Data_Base = fopen (New_Name_Base_Data, "w");
    if (Data_Base == NULL)
    {
        printf ("%s:%d: Error open file for new base data in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Print_Node (Tree->Root, Data_Base);

    fclose (Data_Base);

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
            printf ("%s:%d: Error allocation memory for new node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return Stop_Pointer;
        }
        Node->Str = (char*) calloc (100, sizeof (char));
        if (Node->Str == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return Stop_Pointer;
        }

        Node->Right = Read_Node (Current_Position, Counter_Node);
        if (Node->Right == Stop_Pointer)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return Stop_Pointer;
        }

        if (Check_End (Current_Position) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return Stop_Pointer;
        }

        return Node;
    }

    else
    {
        if (Check_Nil (Current_Position) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
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

    printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
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

            printf ("%s:%d: Error start in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    Tree->Size += 2;

    printf ("Кого ты загадал: ");
    if (getline_k (&Node->Left->Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }
    printf ("\n");

    printf ("Чем %s отличается от %s?\n", Node->Left->Str, Node->Right->Str);
    printf ("Дополни фразу: Он(а)... \n");

    printf ("\nТвой ввод: ");
    if (getline_k (&Node->Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }
    printf ("\n");


    #ifdef DEBUG
        if (Tree_Dump (Tree, Name_Func) == There_Are_Errors)
        {
            printf ("%s:%d: Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
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
            printf ("%s:%d: Error allocation memory for new node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        Node->Right = (node_k*) calloc (1, sizeof (node_k));
        if (Node->Right == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        (Node->Left)->Verification = (uintptr_t) (Node->Left) ^ Canary;
        (Node->Left)->Left = NULL;
        (Node->Left)->Right = NULL;

        (Node->Right)->Verification = (uintptr_t) (Node->Right) ^ Canary;
        (Node->Right)->Str = strdup (Node->Str);
        if ((Node->Right)->Str == NULL)
        {
            printf ("%s:%d: Error allocation memory for new node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }
        (Node->Right)->Left = NULL;
        (Node->Right)->Right = NULL;

        return Node->Left;
    }

    else
    {
        printf ("%s:%d: Error bad node in %s\n", __FILE__, __LINE__, __FUNCTION__);
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

        else
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
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

        else
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    return 0;
}


int Launch           (tree_k* const Tree)
{
    Clean_Stdin ();

    printf ("Привет!\n");
    printf ("Я программа \033[4mАкинатор\033[0m\n\n");

    Again_Start:

    printf ("Введи [Б], если хочешь взаимодействовать с моей базой\n");
    printf ("Введи [М], если хочешь создать свою базу и взаимодействовать с ней\n");
    printf ("Введи [К], если хочешь закончить\n");
    printf ("\nТвой ввод: ");

    char* Answer_User = Read_Answer ();
    if (Answer_User == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Clean_Stdin ();

    if (strcmp (Answer_User, "Б") == 0 || strcmp (Answer_User, "б") == 0)
    {
        if (Tree_Ctor_From_Base (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        printf ("Дерево успешно загружено из базы!\n\n");

        free (Answer_User);
        return 0;
    }

    else if (strcmp (Answer_User, "М") == 0 || strcmp (Answer_User, "м") == 0)
    {
        if (New_Tree_Ctor (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        Clean_Stdin ();

        printf ("Дерево успешно создано!\n\n");

        if (User_Save (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        free (Answer_User);
        return 0;
    }

    else if (strcmp (Answer_User, "К") == 0 || strcmp (Answer_User, "к") == 0)
    {
        free (Answer_User);
        return Finish;
    }

    else
    {
        printf ("Извини, но такой команды нету в списке, будь повнимательнее\n\n");

        goto Again_Start;
    }
}

int Run              (tree_k* const Tree)
{
    Again:

    printf ("Введи [И], если хочешь запустить отгадывание\n");
    printf ("Введи [О], если хочешь получить описание узла\n");
    printf ("Введи [С], если хочешь сравнить два узла\n");
    printf ("Введи [К], если хочешь закончить\n");

    printf ("\nТвой ввод: ");

    char* Answer_User = Read_Answer ();
    if (Answer_User == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Clean_Stdin ();

    if (strcmp (Answer_User, "И") == 0 || strcmp (Answer_User, "и") == 0)
    {
        if (Play (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        goto Again;
    }

    else if (strcmp (Answer_User, "О") == 0 || strcmp (Answer_User, "о") == 0)
    {
        if (Definition_Node (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        goto Again;
    }

    else if (strcmp (Answer_User, "С") == 0 || strcmp (Answer_User, "с") == 0)
    {
        if (Comparison_Nods (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        goto Again;
    }

    else if (strcmp (Answer_User, "К") == 0 || strcmp (Answer_User, "к") == 0)
    {
        free (Answer_User);
        return 0;
    }

    else
    {
        printf ("Извини, но такой команды нету в списке, будь повнимательнее\n\n");

        goto Again;
    }
}

int Play             (tree_k* const Tree)
{
    node_k* Current_Node = Tree->Root;

    while (1)
    {
        Again:

        if (Current_Node->Left != NULL && Current_Node->Right != NULL)
        {
            printf ("Он(а) %s?\n\n", Current_Node->Str);
        }

        else
        {
            printf ("Кажется, я догадываюсь, твой персонаж - %s?\n\n", Current_Node->Str);
        }

        printf ("Введи [Д], если ответ ДА\n");
        printf ("Введи [Н], если ответ НЕТ\n");

        printf ("\nТвой ввод: ");

        char* Answer_User = Read_Answer ();
        if (Answer_User == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        Clean_Stdin ();

        if (strcmp (Answer_User, "Д") == 0 || strcmp (Answer_User, "д") == 0)
        {
            if (Current_Node->Left == NULL && Current_Node->Right == NULL)
            {
                printf ("Я отгадал, твой персонаж это \033[4m%s\033[0m!\n\n", Current_Node->Str);

                free (Answer_User);
                return 0;
            }

            Current_Node = Current_Node->Left;
        }

        else if (strcmp (Answer_User, "Н") == 0 || strcmp (Answer_User, "н") == 0)
        {
            if (Current_Node->Left == NULL && Current_Node->Right == NULL)
            {
                if (Append_Person (Current_Node, Tree) == There_Are_Errors)
                {
                    printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                    return There_Are_Errors;
                }

                free (Answer_User);
                return 0;
            }

            Current_Node = Current_Node->Right;
        }

        else
        {
            printf ("Извини, но такой команды нету в списке, будь повнимательнее\n\n");

            goto Again;
        }
    }
}

int Append_Person    (node_k* const Current_Node, tree_k* const Tree)
{
    Again:

    printf ("Похоже, такого персонажа нету в данном дереве ;(\n");
    printf ("Хочешь добавить своего персонажа в дерево?\n\n");

    printf ("Введи [Д], если ответ ДА\n");
    printf ("Введи [Н], если ответ НЕТ\n");

    printf ("\nТвой ввод: ");

    char* Answer_User = Read_Answer ();
    if (Answer_User == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Clean_Stdin ();

    if (strcmp (Answer_User, "Д") == 0 || strcmp (Answer_User, "д") == 0)
    {
        if (Tree_Append (Current_Node, Tree) == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        Clean_Stdin ();

        printf ("Твой персонаж успешно добавлен!\n\n");

        if (User_Save (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        free (Answer_User);
        return 0;
    }

    else if (strcmp (Answer_User, "Н") == 0 || strcmp (Answer_User, "н") == 0)
    {
        free (Answer_User);
        return 0;
    }

    else
    {
        printf ("Извини, но такой команды нету в списке, будь повнимательнее\n\n");

        goto Again;
    }
}

int User_Save        (const tree_k* const Tree)
{
    Again:

    printf ("Хочешь сохранить дерево в базу?\n");
    printf ("Старое дерево будет утеряно\n\n");

    printf ("Введи [Д], если ответ ДА\n");
    printf ("Введи [Н], если ответ НЕТ\n");

    printf ("\nТвой ввод: ");

    char* Answer_User = Read_Answer ();
    if (Answer_User == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Clean_Stdin ();

    if (strcmp (Answer_User, "Д") == 0 || strcmp (Answer_User, "д") == 0)
    {
        if (Save_Tree_In_File (Tree) == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        printf ("Твое дерево успешно сохранено в базу!\n\n");

        free (Answer_User);
        return 0;
    }

    else if (strcmp (Answer_User, "Н") == 0 || strcmp (Answer_User, "н") == 0)
    {
        free (Answer_User);
        return 0;
    }

    else
    {
        printf ("Извини, но такой команды нету в списке, будь повнимательнее\n\n");

        goto Again;
    }
}

int Definition_Node  (const tree_k* const Tree)
{
    Again:

    printf ("Введи имя узла, для которого я составлю описание\n");
    printf ("\nТвой ввод: ");

    char* Answer_User = Read_Answer ();
    if (Answer_User == NULL)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Clean_Stdin ();

    stack_k Stack_Return = {};
    if (Stack_Ctor (&Stack_Return, 10) == There_Are_Errors_Stack)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        free (Answer_User);
        return There_Are_Errors;
    }

    node_k* You_Node = Search_Node (Answer_User, Tree->Root, &Stack_Return);
    if (You_Node == NULL)
    {
        printf ("Извини, но в данном дереве нет узла - %s\n\n\n", Answer_User);

        goto Again;
    }

    size_t Len_Way = Stack_Return.Size;
    node_k* Current_Node = Tree->Root;

    for (size_t i = 0; i < Len_Way; i++)
    {
        int Direction = Stack_Pop (&Stack_Return);
        if (Direction == There_Are_Errors_Stack)
        {
            printf ("%s:%d: Error pop from stack in %s", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        if (Direction == Right)
        {
            printf ("%zu) Он(а) НЕ %s;\n", i + 1, Current_Node->Str);

            Current_Node = Current_Node->Right;
        }

        else if (Direction == Left)
        {
            printf ("%zu) Он(а) %s;\n", i + 1, Current_Node->Str);

            Current_Node = Current_Node->Left;
        }
    }

    printf ("\n");

    Stack_Dtor (&Stack_Return);
    free (Answer_User);
    return 0;
}

int Comparison_Nods  (const tree_k* const Tree)
{
    Again_1:

    printf ("Введи имя первого узла, для которого я составлю сравнение\n");
    printf ("\nТвой ввод: ");

    char* Answer_User_1 = Read_Answer ();
    if (Answer_User_1 == NULL)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    stack_k Stack_Return_1 = {};
    if (Stack_Ctor (&Stack_Return_1, 10) == There_Are_Errors_Stack)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        free (Answer_User_1);
        return There_Are_Errors;
    }

    node_k* First_Node = Search_Node (Answer_User_1, Tree->Root, &Stack_Return_1);
    if (First_Node == NULL)
    {
        Clean_Stdin ();

        printf ("Извини, но в данном дереве нет узла - %s\n\n", Answer_User_1);

        goto Again_1;
    }

    Again_2:

    printf ("\n\nВведи имя второго узла, для которого я составлю сравнение\n");
    printf ("\nТвой ввод: ");

    char* Answer_User_2 = Read_Answer ();
    if (Answer_User_2 == NULL)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        free (Answer_User_1);
        return There_Are_Errors;
    }

    Clean_Stdin ();

    stack_k Stack_Return_2 = {};
    if (Stack_Ctor (&Stack_Return_2, 10) == There_Are_Errors_Stack)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        Stack_Dtor (&Stack_Return_1);
        free (Answer_User_1);
        free (Answer_User_2);
        return There_Are_Errors;
    }

    node_k* Second_Node = Search_Node (Answer_User_2, Tree->Root, &Stack_Return_2);
    if (Second_Node == NULL)
    {
        printf ("Введи имя первого узла, для которого я составлю сравнение\n");
        printf ("\nТвой ввод: %s\n\n\n", Answer_User_1);

        printf ("Извини, но в данном дереве нет узла - %s\n", Answer_User_2);

        goto Again_2;
    }

    printf ("+-----------------------------------------------+-----------------------------------------------+\n");
    printf ("|  %s%*s     |  %s%*s     |\n", First_Node->Str, int (40 - strlen (First_Node->Str) / 2), " ", Second_Node->Str, int (40 - strlen (Second_Node->Str) / 2), " ");
    printf ("+-----------------------------------------------+-----------------------------------------------+\n");
    printf ("|  %39s  Совпадения %39s  |\n", " ", " ");

    if (Table_Comparison (&Stack_Return_1, &Stack_Return_2, Tree->Root) == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
        Stack_Dtor (&Stack_Return_1);
        Stack_Dtor (&Stack_Return_2);
        free (Answer_User_1);
        free (Answer_User_2);
        return There_Are_Errors;
    }

    Stack_Dtor (&Stack_Return_1);
    Stack_Dtor (&Stack_Return_2);
    free (Answer_User_1);
    free (Answer_User_2);
    return 0;
}

int Table_Comparison (stack_k* const Stack_Return_1, stack_k* const Stack_Return_2, node_k* const Root)
{
    size_t Len_Way = MAX (Stack_Return_1->Size, Stack_Return_2->Size);
    node_k* Current_Node_1 = Root;
    node_k* Current_Node_2 = Root;
    int Flag = Match;

    for (size_t i = 0; i < Len_Way; i++)
    {
        int Direction_1 = 0;
        if (Stack_Return_1->Size != 0)
        {
            Direction_1 = Stack_Pop (Stack_Return_1);
            if (Direction_1 == There_Are_Errors_Stack)
            {
                printf ("%s:%d: Error pop from stack in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return There_Are_Errors;
            }
        }

        int Direction_2 = 0;
        if (Stack_Return_2->Size != 0)
        {
            Direction_2 = Stack_Pop (Stack_Return_2);
            if (Direction_2 == There_Are_Errors_Stack)
            {
                printf ("%s:%d: Error pop from stack in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return There_Are_Errors;
            }
        }


        if (Flag == Match)
        {
            if (Direction_1 != Direction_2)
            {
                if (Stack_Push (Stack_Return_1, Direction_1) == There_Are_Errors_Stack)
                {
                    printf ("%s:%d: Error push in stack in %s\n", __FILE__, __LINE__, __FUNCTION__);
                    return There_Are_Errors;
                }
                if (Stack_Push (Stack_Return_2, Direction_2) == There_Are_Errors_Stack)
                {
                    printf ("%s:%d: Error push in stack in %s\n", __FILE__, __LINE__, __FUNCTION__);
                    return There_Are_Errors;
                }

                printf ("+-----------------------------------------------+-----------------------------------------------+\n");
                printf ("|  %40s  Различия %40s  |\n", " ", " ");

                Flag = Vary;

                i--;
            }

            else
            {
                if (Direction_1 == Right)
                {
                    printf ("| Он(а) НЕ %s%*s     | Он(а) НЕ %s%*s     |\n", Current_Node_1->Str, int (32 - strlen (Current_Node_1->Str) / 2), " ", Current_Node_2->Str,  int (32 - strlen (Current_Node_2->Str) / 2), " ");

                    Current_Node_1 = Current_Node_1->Right;
                    Current_Node_2 = Current_Node_2->Right;
                }

                else if (Direction_1 == Left)
                {
                    printf ("| Он(а) %s%*s     | Он(а) %s%*s     |\n", Current_Node_1->Str, int (35 - strlen (Current_Node_1->Str) / 2), " ", Current_Node_2->Str,  int (35 - strlen (Current_Node_2->Str) / 2), " ");

                    Current_Node_1 = Current_Node_1->Left;
                    Current_Node_2 = Current_Node_2->Left;
                }
            }
        }

        else
        {
            if (Direction_1 == Right)
            {
                printf ("| Он(а) НЕ %s%*s     |", Current_Node_1->Str, int (32 - strlen (Current_Node_1->Str) / 2), " ");

                Current_Node_1 = Current_Node_1->Right;
            }

            else if (Direction_1 == Left)
            {
                printf ("| Он(а) %s%*s     |", Current_Node_1->Str, int (35 - strlen (Current_Node_1->Str) / 2), " ");

                Current_Node_1 = Current_Node_1->Left;
            }

            else if (Direction_1 == 0)
            {
                printf ("| %41s     |", " ");
            }


            if (Direction_2 == Right)
            {
                printf (" Он(а) НЕ %s%*s     |\n", Current_Node_2->Str, int (32 - strlen (Current_Node_2->Str) / 2), " ");

                Current_Node_2 = Current_Node_2->Right;
            }

            else if (Direction_2 == Left)
            {
                printf (" Он(а) %s%*s     |\n", Current_Node_2->Str, int (35 - strlen (Current_Node_2->Str) / 2), " ");

                Current_Node_2 = Current_Node_2->Left;
            }

            else if (Direction_2 == 0)
            {
                printf (" %41s     |\n", " ");
            }
        }
    }

    printf ("+-----------------------------------------------+-----------------------------------------------+\n");
    printf ("\n");

    return 0;
}

node_k* Search_Node  (const char* const String, node_k* Node, stack_k* const Stack_Return)
{
    if (Node->Left == NULL && Node->Right == NULL)
    {
        if (strcmp (Node->Str, String) == 0)
        {
            return Node;
        }

        else
        {
            return NULL;
        }
    }

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            node_k* Pointer = Search_Node (String, Node->Left, Stack_Return);
            if (Pointer != NULL)
            {
                if (Stack_Push (Stack_Return, Left) == There_Are_Errors_Stack)
                {
                    printf ("%s:%d Error push in stack in %s\n", __FILE__, __LINE__, __FUNCTION__);
                    return NULL;
                }

                return Pointer;
            }
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary))
        {
            node_k* Pointer = Search_Node (String, Node->Right, Stack_Return);

            if (Pointer != NULL)
            {
                if (Stack_Push (Stack_Return, Right) == There_Are_Errors_Stack)
                {
                    printf ("%s:%d Error push in stack in %s\n", __FILE__, __LINE__, __FUNCTION__);
                    return NULL;
                }

                return Pointer;
            }
        }
    }

    return NULL;
}


int Start_Logfile           ()
{
    FILE* file_html = fopen (Name_Log, "w");
    if (file_html == NULL)
    {
        printf ("%s:%d: Error open logfile in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    fprintf (file_html, "<pre>\n\n");

    Print_Separator_In_Log (200, file_html);

    fclose (file_html);

    return 0;
}

int Print_Separator_In_Log  (const size_t Len_Separator, FILE* file_html)
{
    for (size_t i = 0; i < Len_Separator / 2; i++)
    {
        fprintf (file_html, "+=");
    }

    fprintf (file_html, "\n\n");

    return 0;
}

int Reverse_Str             (char* const Str)
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

char* itoa_k                (int Number, char* const Str)
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

unsigned int Generate_Color (const void *Ptr, unsigned int Color_Text)
{
    if (Ptr == NULL)
    {
        return 0xD3D3D3;
    }

    unsigned int Color = Hash_Pointer (Ptr) & 0xFFFFFF;

    if (Calculate_Contrast (Color, Color_Text) < Minimum_Contrast_Ratio)
    {
        unsigned int Reverse_Color = (~Color) & 0xFFFFFF;
        return Reverse_Color;
    }

    return Color;
}

unsigned int Hash_Pointer   (const void* const Ptr)
{
    uintptr_t Ptr_Value = (uintptr_t) Ptr;

    unsigned int Hash = 5381;

    unsigned char *Byte_Ptr = (unsigned char *) &Ptr_Value;
    size_t Pointer_Size = sizeof(Ptr_Value);

    for (size_t i = 0; i < Pointer_Size; i++)
    {
        Hash = ((Hash << 7) + Hash) + Byte_Ptr[i];
    }

    return Hash;
}

double Calculate_Contrast   (unsigned int Color, unsigned int Color_Text)
{
    double r_1 = ((Color >> 16) & 0xFF) / 255.0;
    double g_1 = ((Color >> 8) & 0xFF) / 255.0;
    double b_1 = (Color & 0xFF) / 255.0;
    double luminance_1 = 0.2126 * r_1 + 0.7152 * g_1 + 0.0722 * b_1;

    double r_2 = ((Color_Text >> 16) & 0xFF) / 255.0;
    double g_2 = ((Color_Text >> 8) & 0xFF) / 255.0;
    double b_2 = (Color_Text & 0xFF) / 255.0;
    double luminance_2 = 0.2126 * r_2 + 0.7152 * g_2 + 0.0722 * b_2;

    return (fmax(luminance_1, luminance_2) + 0.05) / (fmin(luminance_1, luminance_2) + 0.05);
}

int Copy_File_In_Buffer     (char** const Buffer)
{
    struct stat Data_File = {};

    if (stat (Name_Base_Data, &Data_File) != 0)
    {
        printf ("%s:%d: Error stat in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    off_t Size_File  = Data_File.st_size;

    int File_Base = open (Name_Base_Data, O_RDONLY);
    if (File_Base == -1)
    {
        printf ("%s:%d: Error open file with base data in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    *Buffer = (char*) calloc (size_t (Size_File) + 1, sizeof (char));
    if (*Buffer == NULL)
    {
        printf ("%s:%d: Error allocation memory for buffer in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    if (read (File_Base, *Buffer, size_t (Size_File)) < Size_File)
    {
        printf ("%s:%d: Error read file base data in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    close (File_Base);

    return 0;
}

int Clean_Stdin             ()
{
    printf ("\033[H\033[J");
    fflush(stdout);

    return 0;
}

char* Read_Answer           ()
{
    char* Answer_User = NULL;

    if (getline_k (&Answer_User) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    return Answer_User;
}

int getline_k               (char** const Str)
{
    size_t Len = 0;

    if (getline (Str, &Len, stdin) == -1)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    if (Delete_Slash_N (*Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    return 0;

}

int Delete_Slash_N          (char* const Str)
{
    size_t Len = strlen (Str);

    if (Len > 0 && Str[Len - 1] == '\n')
    {
        Str[Len - 1] = '\0';
        return 0;
    }

    else
    {
        printf ("%s:%d: Error delete in string\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//declar o lista dublu inlantuita
typedef struct nod
{
    char c;
    struct nod *next;
    struct nod *prev;
} Nod, *DList;
// declar o coada
struct qnode
{
    char *key;
    struct qnode *next;
};
struct queue
{
    struct qnode *primul, *ultimul;
};
//declar o structura ce retine contorul
typedef struct Contor
{
    int coloana;
    int linie;
} Cursor;
// declar o stiva
struct StackNode
{
    char data;
    struct StackNode *next;
};
//adaugare nou dupa pozitia pos- lista
void addDL(Nod *nou, Nod *pos)
{
    if (pos->next == NULL)
    {
        pos->next = nou;
        nou->prev = pos;
        nou->next = NULL;
    }
    else
    {
        nou->next = pos->next;
        pos->next->prev = nou;
        nou->prev = pos;
        pos->next = nou;
    }
}
//creez o stiva
struct StackNode *newNode(char data)
{
    struct StackNode *stackNode = (struct StackNode *)malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}
//verific daca stiva e goala
int isEmpty(struct StackNode *root)
{
    return !root;
}
// adaug un element la stiva
void push(struct StackNode **root, char data)
{
    struct StackNode *stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
}
//sterg un element din stiva
char pop(struct StackNode **root)
{
    if (isEmpty(*root))
        return '0';
    struct StackNode *temp = *root;
    *root = (*root)->next;
    char popped = temp->data;
    free(temp);
    return popped;
}
// returneaza ultimul element din stiva fara sa il stearga
char peek(struct StackNode *root)
{
    if (isEmpty(root))
        return '0';
    return root->data;
}
//functia de backspace
void backspace(Nod **head_ref, Nod *del)
{
    if (*head_ref == NULL || del == NULL)
        return;
    // daca e primul nod
    if (*head_ref == del)
        *head_ref = del->next;
    //daca nu e ultimul nod
    if (del->next != NULL)
        del->next->prev = del->prev;
    //daca nu e primul nod
    if (del->prev != NULL)
        del->prev->next = del->next;
    free(del);
}
//functia ce sterge o linie
void deleteline(Nod **head, int liniee, Cursor **cursor)
{
    int i = 1;
    Nod *whereami = *head;
    //cazul in care linia este data ca parametru
    if (liniee != -1)
    {
        //se pozitioneaza pe caracterul de la care incepe stergerea
        while (i != liniee)
        {
            if (whereami->c == '\n')
                i++;
            whereami = whereami->next;
        }
        //sterge linia caracter cu caracter
        while (whereami->c != '\n')
        {
            whereami = whereami->next;
            backspace(head, whereami->prev);
        }
        //sterge '\n'
        whereami = whereami->next;
        backspace(head, whereami->prev);
        //actualizez pozitia cursorului
        if ((*cursor)->linie >= liniee)
            (*cursor)->linie--;
    }
    //cazul in care linia nu este data ca parametru
    else
    {
        //whereami ajunge pe pozitia cursorului
        while (i != (*cursor)->linie)
        {
            if (whereami->c == '\n')
                i++;
            whereami = whereami->next;
        }
        //sterge linia
        while (whereami->c != '\n')
        {
            whereami = whereami->next;
            backspace(head, whereami->prev);
        }
        //sterge '\n'
        whereami = whereami->next;
        backspace(head, whereami->prev);
        //actualizez pozitia cursorului
        (*cursor)->linie--;
        (*cursor)->coloana = 1;
    }
}
//functia ce sterge n caractere
void delete (Nod **head, Cursor **cursor, int nrchar)
{
    int i = 1, j = 0, x;
    Nod *whereami = *head;
    //whereami devine cursor
    while (1)
    {
        if (whereami->c == '\n')
        {
            i++;
            j = 0;
        }
        whereami = whereami->next;
        j++;
        if ((*cursor)->coloana == j && (*cursor)->linie == i)
            break;
    }
    //se sterg nrchar caractere folosind functia backspace
    for (x = 1; x <= nrchar; x++)
    {
        whereami = whereami->next;
        backspace(head, whereami->prev);
    }
}
//functia ce inlocuieste un cuvant
void replace(Nod **head, Cursor **cursor, char *old_word, char *new_word)
{
    int i = 1;
    int j = 0;
    int x;
    Nod *nou;
    Nod *whereami = *head;
    //whereami devine cursor
    while (1)
    {
        if (whereami->c == '\n')
        {
            i++;
            j = 0;
        }
        whereami = whereami->next;
        j++;
        if ((*cursor)->coloana == j && (*cursor)->linie == i)
            break;
    }
    whereami = whereami->next;
    int y, z;
    //caut prima aparitie a cuvantului old_word
    while (whereami->next != NULL)
    {
        int okay = 1;
        for (x = 0; x < strlen(old_word); x++)
        {
            if (whereami->c != old_word[x])
            {
                okay = 0;
            }
            whereami = whereami->next;
        }
        //daca nu l-a gasit, incepe iar cautarea de la pozitia urmatoare
        if (okay == 0)
        {
            for (y = 0; y < x - 1; y++)
                whereami = whereami->prev;
        }
        else
        {
            //daca cele 2 cuvinte au acelasi numar de litere
            if (strlen(old_word) == strlen(new_word))
            {
                for (y = 0; y < strlen(old_word); y++)
                {
                    whereami->c = new_word[y];
                    whereami = whereami->next;
                }
            }
            else
            {
                //daca vechiul cuvant are mai putine caractere decat
                //noul cuvant
                if (strlen(old_word) < strlen(new_word))
                {
                    for (y = 0; y < strlen(old_word); y++)
                    {
                        whereami->c = new_word[y];
                        whereami = whereami->next;
                    }
                    for (z = y; z < strlen(new_word); z++)
                    {
                        nou = (Nod *)malloc(sizeof(Nod));
                        nou->c = new_word[z];
                        addDL(nou, whereami);
                        whereami = nou;
                    }
                }
                else
                {
                    //daca noul cuvant are mai putine caractere decat
                    //vechiul cuvant
                    //se intoarce la inceputul cuvantului
                    for (y = 0; y < strlen(old_word); y++)
                        whereami = whereami->prev;
                    for (y = 0; y < strlen(new_word); y++)
                    {
                        whereami->c = new_word[y];
                        whereami = whereami->next;
                    }
                    for (z = y; z <= strlen(old_word); z++)
                    {
                        whereami = whereami->next;
                        backspace(head, whereami->prev);
                    }
                }
            }
            break;
        }
    }
}
//functia ce sterge un cuvant
void delete_word(Nod **head, Cursor **cursor, char *word)
{
    int i = 1;
    int j = 0;
    int x;
    Nod *whereami = *head;
    //whereami devine cursor
    while (1)
    {
        if (whereami->c == '\n')
        {
            i++;
            j = 0;
        }
        whereami = whereami->next;
        j++;
        if ((*cursor)->coloana == j && (*cursor)->linie == i)
            break;
    }
    whereami = whereami->next;
    int y, z;
    //caut prima aparitie a cuvantului word
    while (whereami->next != NULL)
    {
        int okay = 1;
        for (x = 0; x < strlen(word); x++)
        {
            if (whereami->c != word[x])
            {
                okay = 0;
            }
            whereami = whereami->next;
        }
        //daca nu l-a gasit, incepe iar cautarea de la pozitia urmatoare
        if (okay == 0)
        {
            for (y = 0; y < x - 1; y++)
                whereami = whereami->prev;
        }
        else
        {
            for (z = 0; z < strlen(word); z++)
            {
                whereami = whereami->prev;
                backspace(head, whereami->next);
            }
            break;
        }
    }
}
//functia ce inlocuieste toate aparitiile cuvantului old_word cu new_word
void replace_all(Nod **head, Cursor **cursor, char *old_word, char *new_word)
{
    int i = 1;
    int j = 1;
    int x;
    Nod *nou;
    Nod *whereami = *head;
    //whereami devine cursor
    while (1)
    {
        if (whereami->c == '\n')
        {
            i++;
            j = 1;
        }
        if ((*cursor)->coloana == j && (*cursor)->linie == i)
            break;
        whereami = whereami->next;
        j++;
    }
    int y, z;
    //caut prima aparitie a cuvantului word
    while (whereami->next != NULL)
    {
        int okay = 1;
        for (x = 0; x < strlen(old_word); x++)
        {
            if (whereami->c != old_word[x])
            {
                okay = 0;
            }
            whereami = whereami->next;
        }
        //daca l-a gasit
        if (okay == 1)
        {
            //daca cele 2 cuvinte au acelasi numar de litere
            if (strlen(old_word) == strlen(new_word))
            {
                for (y = 0; y < strlen(old_word); y++)
                {
                    whereami->c = new_word[y];
                    whereami = whereami->next;
                }
            }
            else
            {
                //daca vechiul cuvant are mai putine caractere decat
                //noul cuvant
                if (strlen(old_word) < strlen(new_word))
                {
                    for (y = 0; y < strlen(old_word); y++)
                    {
                        whereami->c = new_word[y];
                        whereami = whereami->next;
                    }
                    for (z = y; z < strlen(new_word); z++)
                    {
                        nou = (Nod *)malloc(sizeof(Nod));
                        nou->c = new_word[z];
                        addDL(nou, whereami);
                        whereami = nou;
                    }
                }
                else
                {
                    //se intoarce la inceputul cuvantului
                    for (y = 0; y < strlen(old_word); y++)
                        whereami = whereami->prev;
                    for (y = 0; y < strlen(new_word); y++)
                    {
                        whereami->c = new_word[y];
                        whereami = whereami->next;
                    }
                    for (z = y; z <= strlen(old_word); z++)
                    {
                        whereami = whereami->next;
                        backspace(head, whereami->prev);
                    }
                }
            }
        }
        else
        {
            for (y = 0; y < x; y++)
                whereami = whereami->prev;
        }
    }
}
//functie ce sterge toate aparitiile cuvantului word de dupa cursor
void delete_all(Nod **head, Cursor **cursor, char *word)
{
    int i = 1;
    int j = 1;
    int x;
    Nod *whereami = *head;
    //whereami devine cursor
    while (1)
    {
        if (whereami->c == '\n')
        {
            i++;
            j = 1;
        }
        if ((*cursor)->coloana == j && (*cursor)->linie == i)
            break;
        whereami = whereami->next;
        j++;
    }
    //whereami = whereami->next;
    int y, z;
    //caut prima aparitie a cuvantului word
    while (whereami->next != NULL)
    {
        int okay = 1;
        for (x = 0; x < strlen(word); x++)
        {
            if (whereami->c != word[x])
            {
                okay = 0;
            }
            whereami = whereami->next;
        }
        //daca l-a gasit
        if (okay == 1)
        {
            for (z = 0; z < strlen(word); z++)
            {
                whereami = whereami->prev;
                backspace(head, whereami->next);
            }
        }
        else
        {
            for (y = 0; y < x; y++)
                whereami = whereami->prev;
        }
    }
}
int main()
{
    //declar fisierul pt input si cel pentru output
    FILE *in;
    FILE *outt;
    char ch;
    char string[200];
    string[0]='\0';
    Nod *lst = NULL, *p = NULL, *whereami, *nou;
    Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
    in = fopen("editor.in", "r");
    struct StackNode *stivaundo = NULL;
    char shir[100];
    if (in != NULL)
    {
        //adaug textul in lista, caracter cu caracter
        ch = fgetc(in);
        nou = (Nod *)malloc(sizeof(Nod));
        nou->c = ch;
        cursor->linie = 1;
        cursor->coloana = 1;
        nou->next = nou->prev = NULL;
        lst = nou;
        p = lst;
        ch = fgetc(in);
        //ok e 1 daca se introduce text si 0 daca se introduc comenzi
        int okbun = 1;
        int okrau = 0;
        int k;
        char string[50];
        int coloana_inainte;
        Nod *lst2;
        int nr_caractere = 2;
        while (ch != EOF)
        {
            //pun text in lista
            if (ch != ':' && okbun == 1)
            {
                nou = (Nod *)malloc(sizeof(Nod));
                nou->c = ch;
                addDL(nou, p);
                p = nou;
                cursor->coloana++;
                coloana_inainte = cursor->coloana;
                ch = fgetc(in);
                nr_caractere++;
                //actualizez linia si coloana
                if (ch == '\n')
                {
                    cursor->linie++;
                    cursor->coloana = 0;
                }
                if (ch == ':')
                {
                    cursor->linie--;
                    cursor->coloana = coloana_inainte;
                }
            }
            if (ch == ':')
            {
                //sar peste "::i\n"
                ch = fgetc(in);
                ch = fgetc(in);
                ch = fgetc(in);
                ch = fgetc(in);
                //daca se afla un spatiu dupa "::i" sar peste
                if (ch == ' ')
                    ch = fgetc(in);
                //interschimb ok- urile pentru a sti in ce mod sunt
                //(inserare text/ inserare comenzi)
                int aux = okbun;
                okbun = okrau;
                okrau = aux;
                //introduc i in stiva pentru a sti ca ultima comanda efectuata
                //este cea de introducere text
                push(&stivaundo, 'i');
                if (okbun == 1)
                    nr_caractere = 0;
            }

            //transform fiecare comanda intr-un sir de caractere
            if (ch != ':' && okbun == 0)
            {
                k = 0;
                while (ch != '\n' && ch != EOF)
                {
                    string[k++] = ch;
                    ch = fgetc(in);
                    //k++;
                }
                string[k++] = '\n';
                string[k] = '\0';
                //verific ce comanda s-a citit
                //daca ultima comanda citita este undo
                if (strcmp(string, "u\n") == 0)
                {
                    //daca ultima operatie efectuata este introducere text
                    if (peek(stivaundo) == 'i')
                    {
                        int contor = 0;
                        int fr;
                        //sterg cate caractere s au introdus la ultima citire
                        for (fr = 1; fr <= nr_caractere; fr++)
                        {
                            shir[contor++] = p->c;
                            p = p->prev;
                            backspace(&lst, p->next);
                        }
                    }
                    //pun comanda de undo in stiva
                    push(&stivaundo, 'u');
                }
                //daca ultima comanda citita este redo
                if (strcmp(string, "r\n") == 0)
                {
                    //daca ultima comanda efectuata este undo introducere text
                    if (peek(stivaundo) == 'u')
                    {
                        int o;
                        //adaug iar textul sters in lista
                        for (o = nr_caractere - 2; o >= 0; o--)
                        {
                            nou = (Nod *)malloc(sizeof(Nod));
                            nou->c = shir[o];
                            addDL(nou, p);
                            p = nou;
                        }
                    }
                }
                //daca ultima comanda citita este save
                if (strcmp(string, "s\n") == 0)
                {
                    lst2 = lst;
                    outt = fopen("editor.out", "w");
                    //rescriu fisierul de output
                    while (lst2->next != NULL)
                    {
                        fputc(lst2->c, outt);
                        lst2 = lst2->next;
                    }
                    fputc('\n', outt);
                    fclose(outt);
                }
                //daca ultima comanda citita este quit
                if (strcmp(string, "q\n") == 0)
                {
                    //eliberez lista
                    while (lst->next != NULL)
                    {
                        lst = lst->next;
                        backspace(&lst, lst->prev);
                    }
                    struct StackNode *aux;
                    //eliberez stiva
                    while (stivaundo)
                    {
                        aux = stivaundo;
                        stivaundo = stivaundo->next;
                        free(aux);
                    }
                    free(cursor);
                    free(lst);
                    fclose(in);
                    break;
                }
                //daca ultima comanda citita este backspace
                if (strcmp(string, "b\n") == 0)
                {
                    whereami = lst;
                    char strundo[2];
                    strundo[0] = p->c;
                    strundo[1] = '\0';
                    int i = 1;
                    int j = 0;
                    //whereami devine cursor
                    while (1)
                    {
                        if (whereami->c == '\n')
                        {
                            i++;
                            j = 0;
                        }
                        whereami = whereami->next;
                        j++;
                        if (cursor->coloana == j && cursor->linie == i)
                            break;
                    }
                    //apelez functia de backspace
                    backspace(&lst, whereami);
                }
                //daca ce este citit poate fi format din mai multe cuvinte
                if (strcmp(string, "u\n") != 0 && strcmp(string, "r\n") != 0 &&
                    strcmp(string, "s\n") != 0 && strcmp(string, "q\n") != 0 && strcmp(string, "b\n") != 0)
                {
                    //impart comanda in cuvinte
                    char *pch = strtok(string, " ");
                    //daca ultima comanda citita este delete line
                    if (strcmp(pch, "dl\n") == 0)
                    {
                        pch = strtok(NULL, " ");
                        int linie = -1;
                        //daca nu se da linia, aceasta va ramane -1
                        if (pch != NULL)
                            linie = atoi(pch);
                        //apelez functia delete line
                        deleteline(&lst, linie, &cursor);
                    }
                    //daca ultima comanda citita este delete line cu parametru
                    else if (strcmp(pch, "dl") == 0)
                    {
                        pch = strtok(NULL, " ");
                        int linie = -1;
                        if (pch != NULL)
                            linie = atoi(pch);
                        //apelez functia delete line
                        deleteline(&lst, linie, &cursor);
                    }
                    //daca ultima comanda citita este go line
                    else if (strcmp(pch, "gl") == 0)
                    {
                        pch = strtok(NULL, " ");
                        int linie = atoi(pch);
                        //modific cursorul
                        cursor->linie = linie;
                        cursor->coloana = 1;
                    }
                    //daca ultima comanda citita este go column
                    else if (strcmp(pch, "gc") == 0)
                    {
                        pch = strtok(NULL, " ");
                        cursor->coloana = atoi(pch);
                        pch = strtok(NULL, " ");
                        int lin = -1;
                        //modific cursorul
                        if (pch != NULL)
                            lin = atoi(pch);
                        if (lin != -1)
                            cursor->linie = lin;
                    }
                    //daca ultima comanda citita este delete cu parametru
                    else if (strcmp(pch, "d") == 0)
                    {
                        pch = strtok(NULL, " ");
                        int nrcaractere = 1;
                        if (pch != NULL)
                            nrcaractere = atoi(pch);
                        //apelez functia delete
                        delete (&lst, &cursor, nrcaractere);
                    }
                    //daca ultima comanda citita este delete fara parametru
                    else if (strcmp(pch, "d\n") == 0)
                    {
                        pch = strtok(NULL, " ");
                        int nrcaractere = 1;
                        if (pch != NULL)
                            nrcaractere = atoi(pch);
                        //apelez functia delete
                        delete (&lst, &cursor, nrcaractere);
                    }
                    //daca ultima comanda citita este replace
                    else if (strcmp(pch, "re") == 0)
                    {
                        char *old;
                        char *new;
                        old = strtok(NULL, " ");
                        new = strtok(NULL, " ");
                        //apelez functia replace
                        replace(&lst, &cursor, old, new);
                    }
                    //daca ultima comanda citita este replace all
                    else if (strcmp(pch, "ra") == 0)
                    {
                        char *old;
                        char *new;
                        old = strtok(NULL, " ");
                        new = strtok(NULL, " ");
                        //apelez functia replace all
                    }
                    //daca ultima comanda citita este delete word
                    else if (strcmp(pch, "dw") == 0)
                    {
                        char *old;
                        old = strtok(NULL, " ");
                        //apelez functia delete word
                        delete_word(&lst, &cursor, old);
                    }
                    //daca ultima comanda citita este delete all
                    else if (strcmp(pch, "da") == 0)
                    {
                        char *old;
                        old = strtok(NULL, " ");
                        //apelez functia delete all
                    }
                    pch = NULL;
                }
                //reinitializez sirul in care se citesc comenzi
                string[0] = '\0';
                ch = fgetc(in);
            }
        }
    }
    return 0;
}
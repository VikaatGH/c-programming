#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

constexpr int PHONE_DIGITS = 10;

typedef struct TNode
{
    char                               * m_Name;
    struct TNode                       * m_Child[PHONE_DIGITS];
} TNODE;

typedef struct
{
    TNODE                              * m_Root;
    int                                  m_Size;
} TPHONEBOOK;

#endif /* __PROGTEST__ */


TNODE * createNode (){
    TNODE *new_node = (TNODE *)malloc (sizeof(TNODE));
    if (new_node == NULL) return NULL;

    new_node->m_Name = NULL;

    for (int i = 0; i < PHONE_DIGITS; i++){
        new_node->m_Child[i] = NULL;
    }
    return new_node;
}

bool         addPhone  ( TPHONEBOOK * book,
                         const char * phone,
                         const char * name )
{

    if (!book || !phone || !name) return false;
    size_t phone_len = strlen(phone);

    for (size_t i = 0; i < phone_len; i++){
        if (!isdigit(phone[i])) return false;
    }

    if (book->m_Root == NULL){
        book->m_Root = createNode();
        if (book->m_Root == NULL) return false;
    }

    TNODE *cur_node = book->m_Root;
    int digit;

    for ( size_t i = 0; i < phone_len; i++){
        digit = phone[i] - '0';

        if (cur_node->m_Child[digit] == NULL){
            cur_node->m_Child[digit] = createNode();

            if (cur_node->m_Child[digit] == NULL) return false;
        }
        cur_node = cur_node->m_Child[digit];
    }
    if (cur_node->m_Name == NULL && name != NULL) book->m_Size++;
    if (cur_node ->m_Name != NULL) free(cur_node->m_Name);
    size_t name_len = strlen(name) + 1;

    char * tmp_name = (char*)malloc(name_len * sizeof(char));
    if (!tmp_name) return false;
    strcpy(tmp_name, name);
    cur_node->m_Name = tmp_name;

    return true;
}

void delNode(TNODE *node, int *deleted_nodes){
    if (node == NULL) return;
    for (int i = 0; i < PHONE_DIGITS; i++){
        if (node->m_Child[i] != NULL) {
            delNode(node->m_Child[i], deleted_nodes);
            node->m_Child[i] = NULL;
        }
    }
    if (node->m_Name != NULL) {
        free(node->m_Name);
        node->m_Name = NULL;
        (*deleted_nodes)++;
    }
    free(node);
}

void         delBook   ( TPHONEBOOK * book )
{
    if (book == NULL || book->m_Root == NULL) return;
    int deleted_nodes = 0;
    delNode(book->m_Root, &deleted_nodes);
    book->m_Root = NULL;
    book->m_Size = 0;
    return;
}


bool         delPhone  ( TPHONEBOOK * book,
                         const char * phone )
{
    if (book == NULL || book->m_Root == NULL || phone == NULL) return false;

    size_t phone_len = strlen(phone);
    for (size_t i = 0; i < phone_len; i++) {
        if (!isdigit(phone[i])) {
            return false;
        }
    }


    TNODE ** node_path = (TNODE **)malloc((phone_len + 1) * sizeof(TNODE *));
    if (!node_path) return false;

    TNODE * cur_node = book->m_Root;
    int index = 0, digit;

    for (size_t i = 0; i < phone_len; i++){
        digit = phone[i] - '0';
        if (cur_node->m_Child[digit] == NULL) {
            free(node_path);
            return false;
        }
        node_path[index++] = cur_node;
        cur_node = cur_node->m_Child[digit];
    }
    node_path[index] = cur_node;

    if (cur_node->m_Name != NULL) {
        free(cur_node->m_Name);
        cur_node->m_Name = NULL;
        book->m_Size--;
    }

    for (int i = index; i >= 0; i--){


      bool  has_children = false;
        for (int j = 0; j < PHONE_DIGITS; j++){
            if (node_path[i]->m_Child[j] != NULL){
                has_children = true;
                break;
            }
        }

        if (has_children || node_path[i]->m_Name != NULL) break;

        if (i > 0) {
            digit = phone[i-1] - '0';
            node_path[i - 1]->m_Child[digit] = NULL;
            free(node_path[i]);

        }
        else  book->m_Root = NULL;

    }
    free(node_path);
    return true;
}


const char * findPhone ( TPHONEBOOK * book,
                         const char * phone )
{
    if (book == NULL || book->m_Root == NULL || phone == NULL) return NULL;

    TNODE *cur_node = book->m_Root;
    char * tmp_name = NULL;
    size_t phone_len = strlen(phone);

    for (size_t i = 0; i < phone_len; i++){
        if (!isdigit(phone[i])) return NULL;
        int digit = phone[i] - '0';

        if (cur_node->m_Child[digit] == NULL) break;
        cur_node = cur_node->m_Child[digit];

        if (cur_node->m_Name != NULL) tmp_name = cur_node->m_Name;
    }
    return tmp_name;

}


#ifndef __PROGTEST__
int main ()
{
    TPHONEBOOK b = { nullptr, 0 };
    char tmpStr[100];
    const char * name;
    assert ( addPhone ( &b, "420", "Czech Republic" ) );
    assert ( addPhone ( &b, "42022435", "Czech Republic CVUT" ) );
    assert ( addPhone ( &b, "421", "Slovak Republic" ) );
    assert ( addPhone ( &b, "44", "Great Britain" ) );
    strncpy ( tmpStr, "USA", sizeof ( tmpStr ) - 1 );
    assert ( addPhone ( &b, "1", tmpStr ) );
    strncpy ( tmpStr, "Guam", sizeof ( tmpStr ) - 1 );
    assert ( addPhone ( &b, "1671", tmpStr ) );
    assert ( addPhone ( &b, "44", "United Kingdom" ) );
    assert ( b . m_Size == 6 );
    assert ( ! b . m_Root -> m_Name );
    assert ( ! b . m_Root -> m_Child[0] );
    assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Name );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Name );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[0] );
    assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Name, "Guam" ) );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
    assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name, "Czech Republic" ) );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
    assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
    assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Name, "Slovak Republic" ) );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
    assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[4] -> m_Name, "United Kingdom" ) );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[9] );
    name = findPhone ( &b, "420800123456" );  assert ( !strcmp ( name, "Czech Republic" ) );
    name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
    name = findPhone ( &b, "42022435" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
    name = findPhone ( &b, "4202243" );  assert ( !strcmp ( name, "Czech Republic" ) );
    name = findPhone ( &b, "420224343258985224" );  assert ( !strcmp ( name, "Czech Republic" ) );
    name = findPhone ( &b, "42" );  assert ( !name );
    name = findPhone ( &b, "422" );  assert ( !name );
    name = findPhone ( &b, "4422" );  assert ( !strcmp ( name, "United Kingdom" ) );
    name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "Guam" ) );
    name = findPhone ( &b, "123456789123456789" );  assert ( !strcmp ( name, "USA" ) );
    assert ( delPhone ( &b, "420" ) );
    assert ( delPhone ( &b, "421" ) );
    assert ( delPhone ( &b, "44" ) );
    assert ( delPhone ( &b, "1671" ) );
    assert ( !delPhone ( &b, "1672" ) );
    assert ( !delPhone ( &b, "1671" ) );
    name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "USA" ) );
    name = findPhone ( &b, "4422" );  assert ( !name );
    name = findPhone ( &b, "420800123456" );  assert ( !name );
    name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
    assert ( b . m_Size == 2 );
    assert ( ! b . m_Root -> m_Name );
    assert ( ! b . m_Root -> m_Child[0] );
    assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
    assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
    assert ( ! b . m_Root -> m_Child[5] );
    assert ( ! b . m_Root -> m_Child[6] );
    assert ( ! b . m_Root -> m_Child[7] );
    assert ( ! b . m_Root -> m_Child[8] );
    assert ( ! b . m_Root -> m_Child[9] );
    assert ( delPhone ( &b, "1" ) );
    assert ( delPhone ( &b, "42022435" ) );
    assert ( !addPhone ( &b, "12345XYZ", "test" ) );
    assert ( b . m_Size == 0 );
    assert ( ! b . m_Root );

    delBook ( &b );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

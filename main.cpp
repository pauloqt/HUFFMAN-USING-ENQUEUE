#include <iostream>
#include <fstream>

using namespace std;

typedef struct node{
    char ch;
    int freq;
    struct node* nxt;
    struct node *left, *right;
}NODE;

char chars[256];                //array of characters to store each character's frequency
string codeword[256];           //array of characters to store each character's codeword.
NODE* head, *prevHead;
int i, length;
void scanFrequency();
void enqueue(char ch, int freq, NODE *left, NODE *right);
void dequeue();
void createTree();
int getCodeword(NODE* root, string code);
void displaySorted();
void displayCodeword();
void init();
int main()
{
    init();
    scanFrequency();

    for(i=1; i<256; i++){               //enqueue each character na may frequency.
        if(chars[i]==0){continue;}
        enqueue(i, chars[i], nullptr, nullptr);
    }

    displaySorted();
    cout <<endl;
    createTree();
    displaySorted();
    NODE *root= head;
    getCodeword(root, "");
    displayCodeword();
}

void scanFrequency(){
char fileName[41];
char ch;

    init();
    cout <<"Enter file name: ";
    cin >>fileName;
    ifstream in(fileName);              //ifstream has methods for input output operations on file

    if(in.fail()){
        cout <<"FILE ERROR!\n";
        system("pause");
    }

    else{
        while(in.get(ch)){              //scans character from file and stores in ch
            chars[ch]++;                //dagdagan ang frequency ng charac ch.
            length++;
        }
    }
}

void enqueue(char ch, int freq, NODE *left, NODE *right){
NODE *q, *p, *n;

    n= new NODE;                //allocates memory to n
    n->ch=ch;
    n->freq=freq;
    n->left= left;
    n->right= right;

    p=q=head;                   //set lahat sa head
    //cout <<n->freq;
    while(p!=NULL && n->freq >= p->freq){
        q=p;
        p=p->nxt;
    }

    if(p==head){                //If wala pang laman
        head=n;
    }
    else{q->nxt=n;}             //if may laman na

    n->nxt=p;                   //lagay NULL sa dulo which is p.
}

void createTree(){
NODE *left, *right;
int freq;

    while(head->nxt!=NULL){                 //hanggat hindi isa ang node
        dequeue(); left=prevHead;           //tanggalin ang head salin sa left node
        dequeue(); right=prevHead;          //tanggalin ang head salin sa right node
        freq= left->freq + right->freq;     //kunin ang sum ng frequency ng left and right
        enqueue(NULL, freq, left, right);   //enqueue para makabuo ng new node na may left and right child
    }
}

void dequeue(){             //deletes the last first node (head) of queue.
NODE *p ;

    p=head;
    head=p->nxt;            //i-move ang head sa kasunod.
    prevHead=p;             //isalin ang dating head sa prevHead variable
    prevHead->nxt=nullptr;  //gawing null ang nxt ng prevhead.
}

int getCodeword(NODE* root, string code){   //every call ng function, execute niya ito using kung saang node nag-move at ang updated codeword

    if(root==nullptr)                       //if walang laman ang tree
        return 0;

    if(!root->left && !root->right){        //if may na-detect na leaf node
        codeword[root->ch]= code;           //i-save ang codeword sa index ng character
    }

    getCodeword(root->left, code + "0");    //move sa left ang node and add "0" sa code ng call na ito
    getCodeword(root->right, code + "1");   //move sa right ang node and add "1" sa code ng call na ito
}   //basically, lahat ng node ay may sariling codeword dahil na-update ang codeword kada-move, pero ise-save lang ang codeword kapag na-reach ang leaf node
    //kapag natapos ma-reach ang leaf node, babalik ito sa previous node na hindi pa tapos i-execute and Left and right

void displaySorted(){
NODE *p;

    p=head;
    cout <<"\tDISPLAY RECORDS\n\n";
    while(p!=NULL){
        printf("%d\t%c\t%d\n", p->ch, p->ch, p->freq);
        p=p->nxt;
    }
}

void displayCodeword(){                             //displays codeword of each character
    cout <<"CODE WORDS:\n";

    for(int i=0;i<=256;i++){                        //loop to 256 characters
        if(codeword[i]==""){continue;}              //if walang lamang codeword, skip
        cout<< (char)i <<"\t" <<codeword[i] <<endl; //if meron, print caharcter and codeword.
    }
}

void init(){
    head= NULL;
    for(i=0; i<=256; i++)
        chars[i]=0;
}

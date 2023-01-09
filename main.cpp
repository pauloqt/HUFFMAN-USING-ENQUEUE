#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

typedef struct node{
    char ch;
    int freq;
    struct node* nxt;
    struct node *left, *right;
}NODE;

void setFontStyle(int FontSize);
void gotoxy(int x,int y);
HWND WINAPI GetConsoleWindowNT(void);
HANDLE m= GetStdHandle(STD_OUTPUT_HANDLE);

char chars[256];                //array of characters to store each character's frequency
string codeword[256];           //array of characters to store each character's codeword.
string huffmanCode;
NODE* head, *prevHead;
char fileName[41];
int i, length;
void scanFrequency();
void enqueue(char ch, int freq, NODE *left, NODE *right);
void dequeue();
void createTree();
int getCodeword(NODE* root, string code);
void combineCode();
void decompress(NODE* root, int &pos, string huffmanCode);
void saveCompressed();
void displaySorted();
void displayCodeword();
int menu();
void design();
void display();
void displayMenu();
void CodeDisplay();
void decodeDisplay();
void init();


int main()
{
    HWND hWnd=GetConsoleWindowNT();
    MoveWindow(hWnd,900,900,1500,900,TRUE);
    setFontStyle(13);

    int ch;
    int pos;
    init();
    scanFrequency();

    for(i=1; i<256; i++){               //enqueue each character na may frequency.
        if(chars[i]==0){continue;}
        enqueue(i, chars[i], nullptr, nullptr);
    }

    displaySorted();
    cout <<endl;
    createTree();
    cout <<"\n\n\t\t\t\t\t\t\t\t" <<"SIZE OF CHARACTERS: " <<head->freq;
    cout <<"\n\n\t\t\t\t\t\t\t\t"; system("pause");
    NODE *root= head;
    getCodeword(root, "");
    //displayCodeword();

    while (1){
            ch = menu();
                    switch (ch)
                {
                    case 1:system("cls"); CodeDisplay();displayCodeword();combineCode(); saveCompressed(); cout <<"\n\n\t\t\t\t\t\t\t\t"; system("pause");break;
                    case 2:system("cls"); design();decodeDisplay();gotoxy(101,20);cout << "C:\>_Decoded string is...";gotoxy(101,22);cout <<"";
                    pos=-1; while (pos < (int)huffmanCode.size() - 2) {
                    decompress(root, pos, huffmanCode);} system("pause"); break;
                    case 3:exit(0);gotoxy(67,49);system("pause");
                    default:  gotoxy(98,32);cout <<"Enter only numbers 1 to 3."<<endl;gotoxy(67,49);system("pause");
       }
    }
}

void scanFrequency(){
char ch;

    init();
    design();
    gotoxy(85,25);cout <<"Enter file name: ";
    cin >>fileName;
    ifstream in(fileName);              //ifstream has methods for input output operations on file

    if(in.fail()){
         gotoxy(90,27);cout <<"FILE ERROR!\n";
         gotoxy(60,49);system("pause");
    }

    else{
        while(in.get(ch)){              //scans character from file and stores in ch
            chars[ch]++;                //dagdagan ang frequency ng charac ch.
            length++;
        }
    }
    system("cls");
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

void displaySorted(){                       //displays frequency of each character
NODE *p;
int i=0;
    display();
    p=head;
    while(p!=NULL){

        gotoxy(30,11+i);printf("%d", p->ch);
        gotoxy(69,11+i);printf("%c", p->ch);
        gotoxy(125,11+i);printf("%d", p->freq);
        i+=2;
        p=p->nxt;
    }
}

void displayCodeword(){                               //displays codeword of each character
    system("cls");
    CodeDisplay();
    for(int i=0;i<256;i++){                           //loop to 256 characters
        if(codeword[i]==""){continue;}                //if walang lamang codeword, skip
          cout<<"\t\t\t\t\t\t\t\t"<< (char)i <<"\t\t\t" <<codeword[i] <<endl; //if meron, print caharcter and codeword.
    }
}

void combineCode(){                            //combines codeword of each character.
char ch;

    huffmanCode="";
    fstream in(fileName);                      //scan ang fileName
    while(in.get(ch)){                         //scan ang each character ng text sa file
        for(i=0; i<256; i++){                  //loop sa 256 characters
            if(ch==(char)i){                      //hanapin ang ch sa 256 characters
                huffmanCode+= codeword[i];     //i-concatenate sa huffmanCode na variable
            }
        }
    }
    gotoxy(79,28);cout<<"HUFFMAN CODE: ";
    gotoxy(40,30);cout<<huffmanCode;
}

void decompress(NODE* root, int &pos, string huffmanCode)
{
	if (root == nullptr) {                 //if walang laman root
		return;
	}

	if (!root->left && !root->right)       //if may na-detect na leaf node
	{
		cout << root->ch;
		return;
	}

	pos++;                               //move to next digit
	if (huffmanCode[pos] =='1')          //if 1 and digit, move sa right
		decompress(root->right, pos, huffmanCode);
	else                                   //if 0 ang digit, move sa left
		decompress(root->left, pos, huffmanCode);
}

void saveCompressed(){
fstream fp;                         //creates a file pointer. fstream para both in and out for iostream.
fp.open("compressed.txt", ios::out); //filePointerName.method ("file_name", ios::mode). -format to access file.

    if(!fp){
        gotoxy(64,40);cout <<"File error!";;
        gotoxy(60,49);system("pause");
    }
    else{
        fp<< huffmanCode;
    }

    gotoxy(105,36);cout <<"File Compressed successfully.";

    fp.close();
}

void init(){
    head= NULL;
    for(i=0; i<=256; i++)
        chars[i]=0;
}

int menu(){
int option;

    system("cls");design(); displayMenu();
    gotoxy(98,24);cout <<"[1] Compress";
    gotoxy(98,26);cout <<"[2] Decompress";
    gotoxy(98,28);cout <<"[3] Exit";
    gotoxy(98,30);cout <<"Select(1-3): ";
    cin >> option;
    return option;
}

//Sets a fixed window size
HWND WINAPI GetConsoleWindowNT(void)
{
    //declare function pointer type
    typedef HWND WINAPI(*GetConsoleWindowT)(void);
    //declare one such function pointer
    GetConsoleWindowT GetConsoleWindow;
    //get a handle on kernel32.dll
    HMODULE hk32Lib = GetModuleHandle(TEXT("KERNEL32.DLL"));
    //assign procedure address to function pointer
    GetConsoleWindow = (GetConsoleWindowT)GetProcAddress(hk32Lib
    ,TEXT("GetConsoleWindow"));
    //check if the function pointer is valid
    //since the function is undocumented
    if(GetConsoleWindow == NULL){
        return NULL;
    }
    //call the undocumented function
    return GetConsoleWindow();
}

//Sets a fixed font size
void setFontStyle(int FontSize){
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = FontSize;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // font style
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

//Sets the fixed place of displays
void gotoxy(int x,int y){
    COORD coord = {0,0};
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void design(){
gotoxy(30,5);cout << R"(

              (`  ).                   _
             (     ).              .:(`  )`.               .   \_,!,_/   ,
)           _(       '`.          :(   .    )               `.,'     `.,'
        .=(`(      .   )     .--  `.  (    ) )               /         \
       ((    (..__.:'-'   .+(   )   ` _`  ) )           ~ -- :         : -- ~
`.     `(       ) )       (   .  )     (   )  ._             \         /
  )      ` __.:'   )     (   (   ))     `-'.-(`  )          ,'`._   _.'`.
)  )  ( )       --'       `- __.'         :(      ))       '   / `!` \   `
.-'  (_.'          .')                    `(    )  ))         ;   :   ;
                  (_  )                     ` __.:'

                                                         .-------.-------.
          +-------------+                     ___        |  0000 |  0100 |
          |             |                     \ /]       |  0001 |  0110 |
          |             |        _           _(_)        |  0011 |       |
          |             |     ___))         [  | \___    |       |       |
          |             |     ) //o          | |     \   |       |       |
          |             |  _ (_    >         | |      ]  |       |       |
          |          __ | (O)  \__<          | | ____/   '-------'-------'
          |         /  o| [/] /   \)        [__|/_
          |             | [\]|  ( \         __/___\_____
          |             | [/]|   \ \__  ___|            |
          |             | [\]|    \___E/%%/|____________|_____
          |             | [/]|=====__   (_____________________)
          |             | [\] \_____ \    |                  |
          |             | [/========\ |   |                  |
          |             | [\]     []| |   |                  |
          |             | [/]     []| |_  |                  |
          |             | [\]     []|___) |                  |
        ====================================================================

            )";


 }

 void display(){
    gotoxy(10,4);cout << R"(
                         _  _  ___ __  __      ___  _  _    _    ___    _    ___  _____  ___  ___        ___  ___  ___  ___   _   _  ___  _  _   ___ __   __
                        | || || __|\ \/ /     / __|| || |  /_\  | _ \  /_\  / __||_   _|| __|| _ \      | __|| _ \| __|/ _ \ | | | || __|| \| | / __|\ \ / /
                        | __ || _|  >  <     | (__ | __ | / _ \ |   / / _ \| (__   | |  | _| |   /      | _| |   /| _|| (_) || |_| || _| | .` || (__  \ V /
                        |_||_||___|/_/\_\     \___||_||_|/_/ \_\|_|_\/_/ \_\\___|  |_|  |___||_|_\      |_|  |_|_\|___|\__\_\ \___/ |___||_|\_| \___|  |_|

                )";


 }
 void displayMenu(){
gotoxy(95,19);cout<<" __  __  ___  _  _  _   _";
gotoxy(95,20);cout<<"|  \\/  || __|| \\| || | | |";
gotoxy(95,21);cout<<"| |\\/| || _| | .` || |_| |";
gotoxy(95,22);cout<<"|_|  |_||___||_|\\_| \\___/ ";



 }
void CodeDisplay(){
     gotoxy(5,4);cout << R"(
                                                   ___ _  _    _    ___              ___  ___   ___   ___
                                                 / __|| || |  /_\  | _ \            / __|/ _ \ |   \ | __|
                                                | (__ | __ | / _ \ |   /           | (__| (_) || |) || _|
                                                 \___||_||_|/_/ \_\|_|_\            \___|\___/ |___/ |___|
                )";
 }
void decodeDisplay(){
gotoxy(95,17);cout<<"  _______________________________________________";
gotoxy(95,18);cout<<" /                                                \\";
gotoxy(95,19);cout<<"|    _________________________________________     |";
gotoxy(95,20);cout<<"|   |                                         |    |";
gotoxy(95,21);cout<<"|   |                                         |    |";
gotoxy(95,22);cout<<"|   |                                         |    |";
gotoxy(95,23);cout<<"|   |                                         |    |";
gotoxy(95,24);cout<<"|   |                                         |    |";
gotoxy(95,25);cout<<"|   |                                         |    |";
gotoxy(95,26);cout<<"|   |                                         |    |";
gotoxy(95,27);cout<<"|   |                                         |    |";
gotoxy(95,28);cout<<"|   |_________________________________________|    |";
gotoxy(95,29);cout<<"|                                                  |";
gotoxy(95,30);cout<<"\\_________________________________________________/";
gotoxy(95,31);cout<<"       \\___________________________________/";
gotoxy(95,32);cout<<"     ___________________________________________";
gotoxy(95,33);cout<<"  _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_";
gotoxy(95,34);cout<<"_-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_";


}





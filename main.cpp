//BY INDRI HIMAWAN FOR CONCEPTS OF PROGRAMMING LANGUAGE
//BUILDING A PARSER WITH RECURSIVE DECENT
//NOVEMBER 16, 2015

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <array>
#include <cstring>

using namespace std;

//for the scanner function
void scan(char character);
void scanner(char character);
void thisIsId(char character);
void thisIsNum();
void thisIsNumWithPoint();
void thisIsComment();
void isRead();
void isWrite();
void fill(string outputs);
void buildOutput(char nextchar);

string tokenList[100];
string tokenContent[100];

int tokenListIndex = 0;
int tokenContentIndex = 0;
bool flag = true;

//for the parser
bool flag2 = true;
void parser(char);
int currentToken = 0;
void program();
void stmt_list();
void stmt();
void expr();
void term_tail();
void term();
void fact_tail();
void factor();
void add_op();
void mult_op();

//printing the output
void addTag(string);
string tagList[400];
int tagListIndex = 0;
int closeTracker = 0;

FILE *fp;

int main(int argc, const char * argv[]){
    
    if (argc != 2){
        cout << "Please enter a valid command with a file input.";
        return 0;
    }
    
    if ((fp = fopen(argv[1], "r")) == NULL) {
        cout << "File cannot be opened.";
        return 0;
    }else{
        cout << "File open successful.\n\n";
    }
    
    char startchar = fgetc(fp);
    
    parser(startchar);

    
    fclose(fp);
    
}

void scanner(char character){
    
    scan(character);
    tokenList[tokenListIndex]="$$";

}

void parser(char character){
    
    scanner(character);
    
    if (!flag){
        cout << "there is an invalid token.";
        return;
    }
    
    program();

    if(flag2){
        int tabAmount = 0;
        char isClose;
        char prevIsClose = '\0';
        for (int index=0 ; index < tagListIndex ; index++){
            
            isClose = tagList[index][1];
            
            if (isClose=='/' || tagList[index]==")"){
                
                if (prevIsClose=='/' || tagList[index-1] == ")" || (tagList[index-1].substr(1,-1) == (tagList[index].substr(2,-1))) ){
                    tabAmount -= 1;
                }else{
                    tabAmount -= 2;
                }
                
                for (int j=0; j<tabAmount; j++){
                    cout << "\t";
                }
                
                cout << tagList[index] << "\n";
                

            }else{
                
                
                for (int j=0; j<tabAmount; j++){
                    cout << "\t";
                }
                
                cout << tagList[index] << "\n";
                tabAmount++;
                
            }
            
            prevIsClose = isClose;
        }
    }else{
        cout << "parse error.";
    }

}

void program(){
    
    addTag("<Program>");
    
    if (tokenList[currentToken]=="id" || tokenList[currentToken]=="read" || tokenList[currentToken]=="write" || tokenList[currentToken]=="$$"){
        stmt_list();
    }else{
        flag2=0;
    }
    
    addTag("</Program>");
}

void stmt_list(){
    
    addTag("<stmt_list>");
    
    if (tokenList[currentToken]=="id" || tokenList[currentToken]=="read" || tokenList[currentToken]=="write"){
        stmt();
        if (flag2) {
            stmt_list();
        }
        
    }else if (tokenList[currentToken]=="$$"){
        
    }
    
    addTag("</stmt_list>");
}

void stmt(){
    
    addTag("<stmt>");
    
    if (tokenList[currentToken]=="id"){ // id := expr
        
        //match
        addTag("<id>");
        addTag(tokenContent[currentToken++]);
        addTag("</id>");
        
        if (tokenList[currentToken] == "assign"){
            
            //match
            addTag("<assign>");
            addTag(tokenContent[currentToken++]);
            addTag("</assign>");
            
            expr();
            
        }else{
            flag2=0;
        }
        
    }else if (tokenList[currentToken]=="read"){ //read id
        
        //match
        addTag("<read>");
        addTag(tokenContent[currentToken++]);
        addTag("</read>");
        
        if (tokenList[currentToken]=="id"){
            
            //match
            addTag("<id>");
            addTag(tokenContent[currentToken++]);
            addTag("</id>");
            
            if (tokenList[currentToken]!="id" && tokenList[currentToken]!="read" && tokenList[currentToken]!="write" && tokenList[currentToken]!="$$"){
                flag2=0;
            }
            
        }else{
            
            flag2=0;
            
        }
        
    }else if (tokenList[currentToken]=="write"){ //write expr
        
        //write match
        addTag("<write>");
        addTag(tokenContent[currentToken++]);
        addTag("</write>");
        
        expr();
        
    }
    
    addTag("</stmt>");
}

void expr(){
    
    addTag("<expr>");
    if (tokenList[currentToken] == "id" || tokenList[currentToken] == "number" || tokenList[currentToken] == "lparen"){
        term();
        if (flag2){
            term_tail();
        }
    }else{
        flag2=0;
    }
    
    addTag("</expr>");
}

void term(){
    addTag("<term>");
    factor();
    if (flag2){
            fact_tail();
    }
    addTag("</term>");
}

void term_tail(){
    
    addTag("<term_tail>");
    
    if (tokenList[currentToken] == "id" || tokenList[currentToken] == "read" || tokenList[currentToken]=="write" || tokenList[currentToken] == "rparen" || tokenList[currentToken] == "$$"){

    }else if (tokenList[currentToken] == "plus" || tokenList[currentToken] == "minus"){
        add_op();
        if (flag2){
            term();
            if (flag2) {
                term_tail();
            }
        }
    }else{
        flag2=0;
    }
    
    addTag("</term_tail>");
}

void factor(){
    addTag("<factor>");
    
    if (tokenList[currentToken] == "lparen"){
        
        addTag("(");
        
        currentToken++; //match
        
        expr();
        if (flag2){
            if (tokenList[currentToken] == "rparen"){
                addTag(")");
                currentToken++; //match
            }else{
                flag2=0;
            }
        }
        
    }else if (tokenList[currentToken] == "id"){
        
        //match
        addTag("<id>");
        addTag(tokenContent[currentToken++]);
        addTag("</id>");
        
    }else if (tokenList[currentToken] == "number"){
        
        //match
        addTag("<number>");
        addTag(tokenContent[currentToken++]);
        addTag("</number>");
        
    }else{
        flag2=0;
    }
    
    addTag("</factor>");
}

void fact_tail(){
    addTag("<fact_tail>");
    
    if (tokenList[currentToken] == "id" || tokenList[currentToken] == "read" || tokenList[currentToken]=="write" || tokenList[currentToken]=="$$" || tokenList[currentToken]=="rparen" || tokenList[currentToken]=="plus" || tokenList[currentToken]=="minus" ){
        
    }else if (tokenList[currentToken]=="times" || tokenList[currentToken]=="div"){
        mult_op();
        if (flag2){
            factor();
            if (flag2){
                fact_tail();
            }        }
    }
    
    addTag("</fact_tail>");
}

void add_op(){
    addTag("<add_op>");
    if (tokenList[currentToken] == "plus" || tokenList[currentToken] == "minus"){
        //match
        addTag(tokenContent[currentToken++]);
        addTag("</add_op>");
    }else{
        flag2 = 0;
    }
}

void mult_op(){
    addTag("<mult_op>");
    if (tokenList[currentToken] == "times" || tokenList[currentToken] == "div"){
        //match
        addTag(tokenContent[currentToken++]);
        addTag("</mult_op>");
    }else{
        flag2 = 0;
    }
}


void scan(char character){
    
    while (character!=EOF && flag){
        
        while (character==' ' || character == '\n') { //ignores new lines and spaces and tabs
            character=fgetc(fp);
        }
        
        if(character==EOF)
            return;
        
        if (character == 'r'){ //recognize "read"
            buildOutput('r');
            isRead();
        }
        
        else if (character == 'w') { //recognize "write"
            buildOutput('w');
            isWrite();
        }
        
        else if (('a'<=character && character<='z') || ('A'<=character && character<='Z')) { //if starts with letter
            thisIsId(character); //then this is an ID
        }
        
        else if (character == '.' || ('0'<= character && character <= '9')){ //if starts with . or 0-9
            if (character == '.') {
                buildOutput(character);
                char nextchar = fgetc(fp);
                if ('0' <= nextchar && nextchar <= '9') {
                    buildOutput(nextchar);
                    thisIsNumWithPoint();
                }else{
                    flag=0;
                }
            }else{
                buildOutput(character);
                thisIsNum(); //then this is a number
            }
        }
        
        else if (character == '('){
            buildOutput('(');
            fill("lparen");
        }
        
        else if (character == ')'){
            buildOutput(')');
            fill("rparen");
        }
        
        else if (character == '+'){
            buildOutput('+');
            fill ("plus");
        }
        
        else if (character == '-'){
            buildOutput('-');
            fill ("minus");
        }
        
        else if (character == '*'){
            buildOutput('*');
            fill ("times");
            
        }else if (character == '/'){
            buildOutput('/');
            fill("div");
        }
        
        else if (character == ':'){ //if there is a ':', then the next one must be a '='
            buildOutput(':');
            char nextchar=fgetc(fp);
            if (nextchar == '=') {
                buildOutput('=');
                fill ("assign");
                
            }else{
                flag=0;
            }
        }
        
        else{
            flag=0;
        }
        
        character=fgetc(fp);
    }
    
}

void thisIsId(char character){
    
    while(('0'<= character && character <='9') || ('a'<=character && character<='z') || ('A'<=character && character<='Z')){
        buildOutput(character);
        character=fgetc(fp);
    }
    
    fill ("id");
    scan(character);
}

void thisIsNum(){
    char nextchar=fgetc(fp);
    while (('0' <= nextchar && nextchar <= '9')) {
        buildOutput(nextchar);
        nextchar=fgetc(fp);
    }
    if(nextchar=='.'){
        buildOutput(nextchar);
        char nextchar2 = fgetc(fp);
        if ('0' <= nextchar2 && nextchar2 <= '9'){
            buildOutput(nextchar2);
            thisIsNumWithPoint();
        }else{
            flag=0;
        }
    }else{
        fill ("number");
        scan(nextchar);
    }
}

void thisIsNumWithPoint(){
    char nextchar = fgetc(fp);
    while (('0' <= nextchar && nextchar <= '9')){
        buildOutput(nextchar);
        nextchar=fgetc(fp);
    }
    
    fill ("number");
    scan(nextchar);
    
}

void thisIsComment(){
    char nextchar = fgetc(fp);
    char nextchar2 = fgetc(fp);
    
    while (1){
        
        if (nextchar == '*'){
            if (nextchar2 == '/'){
                break;
            }
        }
        
        if (nextchar == EOF) {
            flag = 0;
            return;
        }
        
        nextchar = nextchar2;
        nextchar2 = fgetc(fp);
    }
    
    scan(fgetc(fp));
    
}

void isRead(){
    char character = fgetc(fp);
    if (character == 'e') {
        buildOutput(character);
        character = fgetc(fp);
        if (character == 'a'){
            buildOutput(character);
            character = fgetc(fp);
            if (character == 'd') {
                buildOutput(character);
                fill ("read");
            }else{
                thisIsId(character);
            }
        }else{
            thisIsId(character);
        }
    }else{
        thisIsId(character);
    }
}

void isWrite(){
    char character = fgetc(fp);
    
    if (character == 'r') {
        buildOutput(character);
        character = fgetc(fp);
        
        if (character == 'i'){
            buildOutput(character);
            character = fgetc(fp);
            
            if (character == 't') {
                buildOutput(character);
                character = fgetc(fp);
                
                if (character == 'e'){
                    buildOutput(character);
                    fill("write");
                }else{
                    thisIsId(character);
                }
            }else{
                thisIsId(character);
            }
        }else{
            thisIsId(character);
        }
    }else{
        thisIsId(character);
    }
}

void fill(string outputs){
    tokenList[tokenListIndex] = outputs;
    tokenListIndex++;
    tokenContentIndex++;
}

void buildOutput(char nextchar){
    tokenContent[tokenContentIndex] += nextchar;
}

void addTag(string tag){
    tagList[tagListIndex++] = tag;
}

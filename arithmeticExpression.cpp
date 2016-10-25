#include "arithmeticExpression.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;

arithmeticExpression::arithmeticExpression(const string &str) {
    infixExpression = str;
    root = 0;
}

int arithmeticExpression::priority(char op){
    int priority = 0;
    if(op == '('){
        priority =  3;
    }
    else if(op == '*' || op == '/'){
        priority = 2;
    }
    else if(op == '+' || op == '-'){
        priority = 1;
    }
    return priority;
}

string arithmeticExpression::infix_to_postfix(){
    stack<char> s;
    ostringstream oss;
    char c;
    for(unsigned i = 0; i< infixExpression.size();++i){
        c = infixExpression.at(i);
        if(c == ' '){
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){
                    oss << s.top();
                    s.pop();
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    while(!s.empty()){
        oss << s.top();
        s.pop();
    }
    return oss.str();
}

void arithmeticExpression::visualizeTree(const string &outputFilename){
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error opening "<< outputFilename<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}

void arithmeticExpression::visualizeTree(ofstream &outFS, TreeNode *curr) {
    if (curr == 0) {
        return;
    }
    
    outFS << curr->key << "[ label = \"" << curr->data << "\"];" << endl;
    if (curr->left != 0) {
        outFS << curr->key << "->" << curr->left->key << ";" << endl;
    }
    if (curr->right != 0) {
        outFS << curr->key << "->" << curr->right->key << ";" << endl;
    }
    
    visualizeTree(outFS, curr->left);
    visualizeTree(outFS, curr->right);
}

void arithmeticExpression::buildTree() {
    string postfixExpression = infix_to_postfix();
    stack<TreeNode *> st;
    char key = 'a';
    
    for (unsigned i = 0; i < postfixExpression.size(); ++i) {
        if (!isOperator(postfixExpression.at(i))) {
            st.push(new TreeNode(postfixExpression.at(i), key));
        }
        else {
            TreeNode *tempRight = st.top();
            st.pop();
            TreeNode *tempLeft = st.top();
            st.pop();
            
            TreeNode* temp = new TreeNode(postfixExpression.at(i), key);
            temp->left = tempLeft;
            temp->right = tempRight;
            
            st.push(temp);
        }
        ++key;
    }
    root = st.top();
}

void arithmeticExpression::infix() {
    infix(root);
}

void arithmeticExpression::prefix() {
    prefix(root);
}

void arithmeticExpression::postfix() {
    postfix(root);
}

void arithmeticExpression::infix(TreeNode *curr) {
    /* Helper function that outputs the infix notation of the arithmetic expression tree
       by performing the inorder traversal of the tree.
       An opening and closing parenthesis must be added at the 
       beginning and ending of each expression. */
    
    if (curr == 0) {
        return;
    }
    if (!(curr->left == 0 && curr->right == 0)) {
        cout << "(";
        infix(curr->left);
        cout << curr->data;
        infix(curr->right);
        cout << ")";
    }
    else {
        infix(curr->left);
        cout << curr->data;
        infix(curr->right);
    }
}

void arithmeticExpression::prefix(TreeNode *curr) {
    /* Helper function that outputs the prefix notation of the arithmetic expression tree
       by performing the preorder traversal of the tree. */
    
    if (curr == 0) {
        return;
    }
    
    cout << curr->data;
    prefix(curr->left);
    prefix(curr->right);
}

void arithmeticExpression::postfix(TreeNode *curr) {
    /* Helper function that outputs the postfix notation of the arithmetic expression tree
       by performing the postorder traversal of the tree. */
    
    if (curr == 0) {
        return;
    }
    
    postfix(curr->left);
    postfix(curr->right);
    cout << curr->data;
}

bool arithmeticExpression::isOperator(const char &c) {
    if (c == '*' || c == '/' || c == '+' || c == '-') {
        return true;
    }
    else {
        return false;
    }
    
}



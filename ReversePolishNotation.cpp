#include <iostream>
#include <string>

struct Node { //Creating the node structure
    std::string data;
    Node *nextNode;
};

void initStack(struct Node *&head, int data);

bool isStackFull(struct Node *head, int count, int maxSize);

bool isStackEmpty(struct Node *head);

void pushStack(struct Node *&head, int &count, std::string data, int maxSize);

void popStack(struct Node *&head, int &count);

std::string stackTop(struct Node *&head, int &count);

void initQueue(struct Node *&head, std::string data);

bool isQueueFull(struct Node *head, int count, int maxSize);

bool isQueueEmpty(struct Node *head);

void pushQueue(struct Node *&head, int &count, std::string data, int maxSize);

void popQueue(struct Node *&head, int &count);

std::string queueFront(struct Node *&head, int &count);

void outputQueue(struct Node *head);

void outputStack(struct Node *head);

int getPrecedence(std::string currentOperator);

void outputDataStructure(struct Node *head);

int main() {
    int maxStackSize = 20; //Initialising the stack data
    int stackCount = 0;
    struct Node *stack = new Node; //Creating the head of the linked list
    initStack(stack, 0); //Initialising the head of the linked list
    int maxQueueSize = 20; //Initialising the queue data
    int queueCount = 0;
    struct Node *queue = new Node; //Creating the head of the linked list
    initQueue(queue, "START"); //Initialising the head of the linked list
    //std::string tokens = "(5 + 3) / (3 * 2)"; //The sum to parse into Reverse Polish
    //std::string tokens = "(5 - 3) / 3";
    std::string tokens = "1+2*(3+4)";
    // std::string tokens = "3 + 4 * 2 - 1";
    std::string currentToken; //The current token parsed from char to string
    std::string top; //The top of the stack
    bool inBracket = false; //Are we currently in a bracket?
    std::string tempOperator; //Stores the operator from within the brackets
    for (char token : tokens) { //For each token in the tokens string
        if (token == ' ') { //If current token is a space then skip
            continue;
        }
        if (token == '1' || token == '2' || token == '3' || token == '4' || token == '5' || token == '6' ||
            token == '7' || token == '8' || token == '9') { //Checking if token is valid int
            currentToken = std::string(1, token); //Parsing from char to string
            pushQueue(queue, queueCount, currentToken, maxQueueSize); //Pushing token onto the output queue
        } else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '(' ||
                   token == ')') { //Checking if token is a valid operator
            if (inBracket && token !=
                             ')') { //If we are in a bracket and the token is not the closing bracket, then this is the operator
                tempOperator = tempOperator + token;
                continue;
            } else if (token == ')') { //Else we are finished with the bracket, push temp operator to output queue
                inBracket = false;
                pushQueue(queue, queueCount, tempOperator, maxQueueSize);
                tempOperator = "";
            }
            if (top.empty()) { //If stack top is empty then set top to null or empty string
                top = "";
            } else { //Else set top to top of stack
                top = stackTop(stack, stackCount);
            }
            if (token == ')') { //If token is closing bracket
                inBracket = false; //No longer in brackets
                tempOperator = "";
                while (top != "" && top != "(") { //While the top is not null and the token is not a opening bracket
                    pushQueue(queue, queueCount, top, maxQueueSize); //Push top to output
                    popStack(stack, stackCount); //Pop the top from the stack
                    top = stackTop(stack, stackCount); //Get new top after pop
                }
                popStack(stack, stackCount);
            } else if (token == '(') { //If token is opening bracket
                inBracket = true; //We are in brackets
                currentToken = std::string(1, token); //Parse current token to string
                pushStack(stack, stackCount, currentToken, maxStackSize); //Push to operators stack
            } else {
                currentToken = std::string(1, token); //Parse token to string
                if (!isStackEmpty(stack)) {
                    top = stackTop(stack, stackCount);
                }
                while (top != "" && getPrecedence(top) >= getPrecedence(
                        currentToken)) { //While top is not null and current top of stack has higher precedence than current token
                    pushQueue(queue, queueCount, top, maxQueueSize); //Push top to output queue
                    popStack(stack, stackCount); //Pop top from stack
                    if (!isStackEmpty(stack)) {
                        top = stackTop(stack, stackCount); //Get new top from stack after pop
                    } else {
                        top = "";
                    }
                }
                pushStack(stack, stackCount, currentToken, maxStackSize);
            }
        }
    }
    std::string token = "";
    while (!isStackEmpty(stack)) { //Pushing remaining tokens onto queue
        token = stackTop(stack, stackCount);
        pushQueue(queue, queueCount, token, maxQueueSize);
        popStack(stack, stackCount);
    }
    while (!isQueueEmpty(queue)) { //Outputting queue
        if (queueFront(queue, queueCount) == "START") {
            popQueue(queue, queueCount);
        }
        std::cout << queueFront(queue, queueCount) << " ";
        popQueue(queue, queueCount);
    }
    std::cin.get();
    std::cin.ignore();
    return 0;
}

int getPrecedence(std::string currentOperator) {
    if (currentOperator == "(") {
        return 1;
    } else if (currentOperator == "+" || currentOperator == "-") {
        return 2;
    } else if (currentOperator == "*" || currentOperator == "/") {
        return 3;
    }
    return 10;
}

void initStack(struct Node *&head, int data) { //Initialising the head of the stack
    head->data = data;
    head->nextNode = NULL;
}

bool isStackFull(struct Node *head, int count, int maxSize) {
    if (count >= maxSize) { //Checking if the stack is full
        return true;
    } else {
        return false;
    }
}

bool isStackEmpty(struct Node *head) {
    if (head->nextNode != NULL) { //Checking if the stack is empty
        return false;
    } else {
        return true;
    }
}

void pushStack(struct Node *&head, int &count, std::string data, int maxSize) {
    if (!isStackFull(head, count, maxSize)) { //Checking that the stack is not currently full
        struct Node *nodeToAdd = new Node; //Creating a new temporary node
        nodeToAdd->data = data; //Adding the data to the node
        nodeToAdd->nextNode = head; //Setting the pointer in the new node to be that of the current head
        head = nodeToAdd; //Setting head to be the new node
        count++; //Incrementing the count of the stack
    }
}

void popStack(struct Node *&head, int &count) {
    if (!isStackEmpty(head)) { //Checking that the stack is not currently empty
        struct Node *tempNode = head; //Creating a temporary node to hold the data from the current head
        head = head->nextNode; //Setting head to the node that the current head points to
        delete tempNode; //Deleting the temporary node
        count--; //Decrementing the current count of the stack
    } else {
        std::cout << "There is no data to pop!" << std::endl;
    }
}

std::string stackTop(struct Node *&head, int &count) {
    if (!isStackEmpty(head)) { //Checking that the stack is not currently empty
        return head->data;
    }
    return NULL;
}

void initQueue(struct Node *&head, std::string data) { //Initialising the head of the queue
    head->data = data;
    head->nextNode = NULL;
}

bool isQueueFull(struct Node *head, int count, int maxSize) {
    if (count >= maxSize) { //Checking if the queue is full
        return true;
    } else {
        return false;
    }
}

bool isQueueEmpty(struct Node *head) {
    if (head != NULL) { //Checking if the queue is empty
        return false;
    } else {
        return true;
    }
}

void pushQueue(struct Node *&head, int &count, std::string data, int maxSize) {
    if (!isQueueFull(head, count, maxSize)) { //Checking that the queue is not currently full
        Node *currentNode = head;
        Node *nodeToAdd = new Node; //Creating a temporary node
        nodeToAdd->nextNode = NULL; //Setting node data
        nodeToAdd->data = data;
        if (currentNode->nextNode == NULL) { //If next node is null then add the temp node here
            currentNode->nextNode = nodeToAdd;
            return;
        }
        while (currentNode->nextNode != NULL) { //Loop until reach end of queue
            currentNode = currentNode->nextNode;
            if (currentNode->nextNode == NULL) { //If next node is null then add temp node here
                currentNode->nextNode = nodeToAdd;
                count++; //Incrementing the count of the stack
                return;
            }
        }
    } else {
        std::cout << "Queue is full" << std::endl;
    }
}

void popQueue(struct Node *&head, int &count) {
    if (!isQueueEmpty(head)) { //Checking that the queue is not currently empty
        head = head->nextNode;
        count--; //Decrementing the current count of the queue
    }
}

std::string queueFront(struct Node *&head, int &count) {
    if (!isQueueEmpty(head)) { //Checking that the queue is not currently empty
        return head->data;
    }
    return NULL;
}

void outputDataStructure(struct Node *head) { //Outputting all of the current data in the stack
    Node *currentNode = head; //Creating a temporary node
    std::cout << "Head => " << currentNode->data << " | " << currentNode->nextNode << " => ";
    if (currentNode->nextNode == NULL) {
        std::cout << "NULL | ";
    }
    while (currentNode->nextNode != NULL) {
        currentNode = currentNode->nextNode;
        std::cout << currentNode->data << " | " << currentNode->nextNode << " => ";
        if (currentNode->nextNode == NULL) {
            std::cout << "NULL | ";
        }
    }
    std::cout << "End of linked list" << std::endl;
    std::cout << std::endl;
}
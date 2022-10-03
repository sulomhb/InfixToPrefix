#include <algorithm>
#include <iostream>
#include <string>

template <typename T> class Stack {
  int top = -1;
  static const int size = 100;
  T stack[size]{0};

public:
  bool is_full() const {
    bool result = (top == (size - 1));
    return result;
  }

  bool is_empty() const {
    bool result = (top == -1);
    return result;
  }

  void push(T input) {
    if (is_full()) {
      std::cout << "Stack overflow" << std::endl;
      return;
    }
    stack[++top] = input;
  }

  void pop() {
    if (is_empty()) {
      std::cout << "Stack is empty" << std::endl;
      return;
    }
    top--;
  }

  T peek() {
    if (is_empty()) {
      std::cout << "Stack is empty" << std::endl;
      return 0;
    }
    return stack[top];
  }
};

int operator_priority(char character) {
  switch (character) {
  case '^': {
    return 3;
  }

  case '*': {
  }
  case '/': {
    return 2;
  }

  case '+': {
  }
  case '-': {
    return 1;
  }

  default:
    return -1;
  }
}

bool is_operand(char character) {
  return (character >= '0' && character <= '9') ||
         (character >= 'a' && character <= 'z') ||
         (character >= 'A' && character <= 'Z');
}

bool is_operator(char character) {
  return (character == '+' || character == '-' || character == '*' ||
          character == '/' || character == '^' || character == '!' ||
          character == '@' || character == '#');
}

float calculate(float op1, char op, float op2) {
  switch (op) {
  case '+':
    return op1 + op2;
  case '-':
    return op1 - op2;
  case '*':
    return op1 * op2;
  case '/':
    return op1 / op2;
  default:
    return 0.0;
  }
}

void replace(std::string &subject, const std::string &search, const std::string &replace) {
  size_t position = 0;
  if (search.empty())
    return;
  while ((position = subject.find(search, position)) != std::string::npos) {
    subject.replace(position, search.length(), replace);
    position += replace.length();
  }
}

std::string convert(std::string inputExpression) {
  Stack<char> stack;

  replace(inputExpression, "sin", "!");
  replace(inputExpression, "cos", "@");
  replace(inputExpression, "log", "#");

  std::string prefixExpression;
  std::reverse(inputExpression.begin(), inputExpression.end());

  for (char character : inputExpression) {

    if (is_operand(character)) {
      prefixExpression += character;
    }

    else if (character == ')') {
      stack.push(character);
    }

    else if (character == '(') {
      while ((stack.peek() != ')') && (!stack.is_empty())) {
        prefixExpression += stack.peek();
        stack.pop();
      }
      if (stack.peek() == ')')
        stack.pop();
    }

    else if (is_operator(character)) {
      if (stack.is_empty()) {
        stack.push(character);
      } else {
        if (operator_priority(character) > operator_priority(stack.peek())) {
          stack.push(character);
        } else if ((operator_priority(character) ==
                    operator_priority(stack.peek())) &&
                   (character == '^')) {
          while ((operator_priority(character) ==
                  operator_priority(stack.peek()))) {
            prefixExpression += stack.peek();
            stack.pop();
          }
          stack.push(character);
        }

        else if (operator_priority(character) ==
                 operator_priority(stack.peek())) {
          stack.push(character);
        }

        else {
          while ((!stack.is_empty()) && (operator_priority(character) <
                                         operator_priority(stack.peek()))) {
            prefixExpression += stack.peek();
            stack.pop();
          }
          stack.push(character);
        }
      }
    }
  }

  while (!stack.is_empty()) {
    prefixExpression += stack.peek();
    stack.pop();
  }

  std::reverse(prefixExpression.begin(), prefixExpression.end());
  return prefixExpression;
}

float evaluatePrefix(std::string prefix) {
  Stack<float> operandStack;
  Stack<char> operatorStack;

  for (char character : prefix) {
    if (is_operand(character)) {
      std::cout << character << std::endl;
      operandStack.push(float(character) - '0');
    } else if (is_operator(character)) {
      operatorStack.push(character);
    }
    if (!operatorStack.is_empty()) {
      float op1 = operandStack.peek();
      operandStack.pop();

      float op2 = operandStack.peek();
      operandStack.pop();
      float result = calculate(op1, operatorStack.peek(), op2);
      operandStack.push(result);
      operatorStack.pop();
    }
  }

  return operandStack.peek();
}

int main() {

  std::cout << "Enter expression: ";
  std::string input;
  std::getline(std::cin, input);

  std::string prefix = convert(input);
  std::cout << prefix << std::endl;

  float result = evaluatePrefix(prefix);
  std::cout << result << std::endl;
  return 0;
}

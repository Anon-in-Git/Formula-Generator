#include "Formula_Checker.h"
#include <random>
#include <algorithm>
#include <sstream>
#include <cmath>

// Fraction 类实现
Fraction::Fraction(int num, int den) : numerator(num), denominator(den), whole(0) {
    if (denominator == 0) denominator = 1;
    toProperFraction();
    simplify();
}

Fraction::Fraction(int w, int num, int den) : numerator(num), denominator(den), whole(w) {
    if (denominator == 0) denominator = 1;
    toProperFraction();
    simplify();
}

void Fraction::simplify() {
    if (numerator == 0) {
        denominator = 1;
        return;
    }

    int a = std::abs(numerator);
    int b = denominator;

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    numerator /= a;
    denominator /= a;

    toProperFraction();
}

void Fraction::toProperFraction() {
    if (numerator >= denominator) {
        whole += numerator / denominator;
        numerator %= denominator;
    }

    if (numerator < 0) {
        whole -= 1;
        numerator += denominator;
    }
}

std::string Fraction::toString() const {
    std::stringstream ss;

    if (whole != 0) {
        ss << whole;
        if (numerator != 0) {
            ss << "'" << numerator << "/" << denominator;
        }
    }
    else {
        if (numerator == 0) {
            ss << "0";
        }
        else {
            ss << numerator << "/" << denominator;
        }
    }

    return ss.str();
}

double Fraction::toDouble() const {
    return whole + static_cast<double>(numerator) / denominator;
}

Fraction Fraction::operator+(const Fraction& other) const {
    int num1 = whole * denominator + numerator;
    int num2 = other.whole * other.denominator + other.numerator;

    int new_num = num1 * other.denominator + num2 * denominator;
    int new_den = denominator * other.denominator;

    return Fraction(new_num, new_den);
}

Fraction Fraction::operator-(const Fraction& other) const {
    int num1 = whole * denominator + numerator;
    int num2 = other.whole * other.denominator + other.numerator;

    int new_num = num1 * other.denominator - num2 * denominator;
    int new_den = denominator * other.denominator;

    return Fraction(new_num, new_den);
}

Fraction Fraction::operator*(const Fraction& other) const {
    int num1 = whole * denominator + numerator;
    int num2 = other.whole * other.denominator + other.numerator;

    int new_num = num1 * num2;
    int new_den = denominator * other.denominator;

    return Fraction(new_num, new_den);
}

Fraction Fraction::operator/(const Fraction& other) const {
    int num1 = whole * denominator + numerator;
    int num2 = other.whole * other.denominator + other.numerator;

    if (num2 == 0) return Fraction(0, 1);

    int new_num = num1 * other.denominator;
    int new_den = denominator * num2;

    return Fraction(new_num, new_den);
}

bool Fraction::operator==(const Fraction& other) const {
    return toDouble() == other.toDouble();
}

bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
}

bool Fraction::operator<(const Fraction& other) const {
    return toDouble() < other.toDouble();
}

bool Fraction::operator<=(const Fraction& other) const {
    return toDouble() <= other.toDouble();
}

bool Fraction::operator>(const Fraction& other) const {
    return toDouble() > other.toDouble();
}

bool Fraction::operator>=(const Fraction& other) const {
    return toDouble() >= other.toDouble();
}

// ExpressionNode 实现
ExpressionNode::ExpressionNode(const Fraction& val)
    : type(NodeType::NUMBER), value(val), op('\0'), left(nullptr), right(nullptr) {
}

ExpressionNode::ExpressionNode(char operation, ExpressionNode* l, ExpressionNode* r)
    : type(NodeType::OPERATOR), value(0, 1), op(operation), left(l), right(r) {
}

ExpressionNode::~ExpressionNode() {
    delete left;
    delete right;
}

std::string ExpressionNode::toString() const {
    if (type == NodeType::NUMBER) {
        return value.toString();
    }

    std::string leftStr = left->toString();
    std::string rightStr = right->toString();

    // 根据运算符优先级决定是否加括号
    if (left->type == NodeType::OPERATOR &&
        getOperatorPriority(left->op) < getOperatorPriority(op)) {
        leftStr = "(" + leftStr + ")";
    }

    if (right->type == NodeType::OPERATOR &&
        getOperatorPriority(right->op) <= getOperatorPriority(op)) {
        rightStr = "(" + rightStr + ")";
    }

    return leftStr + " " + std::string(1, op) + " " + rightStr;
}

Fraction ExpressionNode::evaluate() const {
    if (type == NodeType::NUMBER) {
        return value;
    }

    Fraction leftVal = left->evaluate();
    Fraction rightVal = right->evaluate();

    switch (op) {
    case '+': return leftVal + rightVal;
    case '-': return leftVal - rightVal;
    case '*': return leftVal * rightVal;
    case '/': return leftVal / rightVal;
    default: return Fraction(0, 1);
    }
}

ExpressionNode* ExpressionNode::clone() const {
    if (type == NodeType::NUMBER) {
        return new ExpressionNode(value);
    }
    else {
        return new ExpressionNode(op, left->clone(), right->clone());
    }
}

// ExpressionGenerator 实现
ExpressionGenerator::ExpressionGenerator(int r) : range(r) {
    std::random_device rd;
    rng.seed(rd());
}

Fraction ExpressionGenerator::generateRandomFraction() {
    std::uniform_int_distribution<int> dist(1, range - 1);
    std::uniform_int_distribution<int> typeDist(0, 2); // 0: 整数, 1: 真分数, 2: 带分数

    int type = typeDist(rng);

    switch (type) {
    case 0: // 整数
        return Fraction(dist(rng), 1);

    case 1: // 真分数
    {
        int num = dist(rng);
        int den = dist(rng);
        if (num >= den) std::swap(num, den);
        return Fraction(num, den);
    }

    case 2: // 带分数
    {
        int whole = dist(rng) / 2; // 整数部分小一些
        int num = dist(rng);
        int den = dist(rng);
        if (num >= den) std::swap(num, den);
        return Fraction(whole, num, den);
    }

    default:
        return Fraction(dist(rng), 1);
    }
}

ExpressionNode* ExpressionGenerator::generateExpression(int operatorCount) {
    if (operatorCount == 0) {
        return new ExpressionNode(generateRandomFraction());
    }

    std::uniform_int_distribution<int> opDist(0, 3);
    std::uniform_int_distribution<int> splitDist(0, operatorCount - 1);

    int leftOps = splitDist(rng);
    int rightOps = operatorCount - 1 - leftOps;

    char operators[] = { '+', '-', '*', '/' };
    char op = operators[opDist(rng)];

    ExpressionNode* left = generateExpression(leftOps);
    ExpressionNode* right = generateExpression(rightOps);

    // 确保减法和除法的合法性
    if (op == '-') {
        // 确保左操作数 >= 右操作数
        while (left->evaluate() < right->evaluate()) {
            delete left;
            delete right;
            left = generateExpression(leftOps);
            right = generateExpression(rightOps);
        }
    }
    else if (op == '/') {
        // 确保结果是真分数
        Fraction result = left->evaluate() / right->evaluate();
        while (result.getWhole() != 0 || result.getNumerator() == 0) {
            delete left;
            delete right;
            left = generateExpression(leftOps);
            right = generateExpression(rightOps);
            result = left->evaluate() / right->evaluate();
        }
    }

    return new ExpressionNode(op, left, right);
}

bool ExpressionGenerator::isEquivalent(ExpressionNode* expr1, ExpressionNode* expr2) {
    if (expr1->type != expr2->type) return false;

    if (expr1->type == NodeType::NUMBER) {
        return expr1->value == expr2->value;
    }

    // 对于运算符节点，检查是否通过交换律等价
    if (expr1->op == expr2->op) {
        // 对于加法和乘法，检查交换律
        if (expr1->op == '+' || expr1->op == '*') {
            return (isEquivalent(expr1->left, expr2->left) &&
                isEquivalent(expr1->right, expr2->right)) ||
                (isEquivalent(expr1->left, expr2->right) &&
                    isEquivalent(expr1->right, expr2->left));
        }
        else {
            return isEquivalent(expr1->left, expr2->left) &&
                isEquivalent(expr1->right, expr2->right);
        }
    }

    return false;
}

void ExpressionGenerator::normalizeExpression(ExpressionNode* node) {
    if (node->type != NodeType::OPERATOR) return;

    // 对加法和乘法进行规范化（左操作数 >= 右操作数）
    if (node->op == '+' || node->op == '*') {
        normalizeExpression(node->left);
        normalizeExpression(node->right);

        // 如果右操作数大于左操作数，交换
        if (node->right->evaluate() > node->left->evaluate()) {
            std::swap(node->left, node->right);
        }
    }
}

std::pair<std::string, std::string> ExpressionGenerator::generateSingleExpression() {
    std::uniform_int_distribution<int> opCountDist(1, 3);
    int opCount = opCountDist(rng);

    ExpressionNode* expr = generateExpression(opCount);
    normalizeExpression(expr);

    std::string expressionStr = expr->toString();
    std::string answerStr = expr->evaluate().toString();

    delete expr;

    return { expressionStr + " = ", answerStr };
}

std::vector<std::pair<std::string, std::string>>
ExpressionGenerator::generateExpressions(int count) {
    std::vector<std::pair<std::string, std::string>> expressions;
    std::vector<ExpressionNode*> generatedTrees;

    for (int i = 0; i < count; ++i) {
        bool isUnique = false;
        ExpressionNode* newExpr = nullptr;

        // 尝试生成不重复的表达式
        for (int attempt = 0; attempt < 100 && !isUnique; ++attempt) {
            std::uniform_int_distribution<int> opCountDist(1, 3);
            int opCount = opCountDist(rng);

            newExpr = generateExpression(opCount);
            normalizeExpression(newExpr);

            isUnique = true;
            for (ExpressionNode* existingExpr : generatedTrees) {
                if (isEquivalent(newExpr, existingExpr)) {
                    isUnique = false;
                    break;
                }
            }

            if (!isUnique) {
                delete newExpr;
            }
        }

        if (isUnique && newExpr) {
            generatedTrees.push_back(newExpr->clone());
            expressions.push_back({
                newExpr->toString() + " = ",
                newExpr->evaluate().toString()
                });
        }

        delete newExpr;
    }

    // 清理内存
    for (ExpressionNode* expr : generatedTrees) {
        delete expr;
    }

    return expressions;
}

// 工具函数实现
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int getOperatorPriority(char op) {
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    default: return 0;
    }
}

std::string fractionToString(const Fraction& frac) {
    return frac.toString();
}

Fraction stringToFraction(const std::string& str) {
    // 简化的字符串转分数实现
    // 实际项目中需要更完善的解析
    if (str.find('/') != std::string::npos) {
        size_t slashPos = str.find('/');
        int num = std::stoi(str.substr(0, slashPos));
        int den = std::stoi(str.substr(slashPos + 1));
        return Fraction(num, den);
    }
    else {
        return Fraction(std::stoi(str), 1);
    }
}
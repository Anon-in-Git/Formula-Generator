#include "Formula_Generator.h"
#include <random>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <fstream>
#include <iostream>

// ==================== Fraction 类实现 ====================

Fraction::Fraction(int num, int den) : numerator(num), denominator(den), whole(0) {
    if (denominator == 0) denominator = 1;
    simplify();
    toProperFraction();  // 先简化再转换为真分数
}

Fraction::Fraction(int w, int num, int den) : numerator(num), denominator(den), whole(w) {
    if (denominator == 0) denominator = 1;
    simplify();
    toProperFraction();  // 先简化再转换为真分数
}

void Fraction::simplify() {
    if (numerator == 0) {
        denominator = 1;
        return;
    }

    int a = std::abs(numerator);
    int b = denominator;

    // 欧几里得算法求最大公约数
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
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }

    // 如果分子为0，直接返回
    if (numerator == 0) {
        whole = 0;
        return;
    }

    while (numerator < 0) {
		whole -= 1;
		numerator += denominator;
    }

    // 处理分子绝对值大于等于分母的情况
    if (numerator >= denominator) {
        int addToWhole = numerator / denominator;
        whole += addToWhole;
        numerator = std::abs(numerator) % denominator;

        // 如果分子为0，重置分母为1
        if (numerator == 0) {
            denominator = 1;
        }
    }

    // 确保整数部分和分数部分的符号一致
    if (whole != 0 && numerator != 0) {
        if ((whole > 0 && numerator < 0) || (whole < 0 && numerator > 0)) {
            // 这种情况不应该出现，如果出现则调整
            if (whole > 0) {
                whole -= 1;
                numerator += denominator;
            }
            else {
                whole += 1;
                numerator -= denominator;
            }
        }
    }
    
}

std::string Fraction::toString() const {
    std::stringstream ss;

    // 计算实际的值（考虑整数部分）
    double actualValue = toDouble();

    // 如果是负数，需要在两边加括号
    bool isNegative = (actualValue < 0);

    if (isNegative) {
        ss << "(";
    }

    // 处理整数的显示
    if (whole != 0) {
        ss << std::abs(whole);  // 显示绝对值的整数部分
        if (numerator != 0) {
            ss << "'" << numerator << "/" << denominator;
        }
    }
    else {
        if (numerator == 0) {
            ss << "0";
        }
        else {
            ss << std::abs(numerator) << "/" << denominator;
        }
    }

    if (isNegative) {
        ss << ")";
    }

    return ss.str();
}

double Fraction::toDouble() const {
    double result = whole + static_cast<double>(numerator) / denominator;
    return result;
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
    long long num1 = static_cast<long long>(whole) * denominator + numerator;
    long long num2 = static_cast<long long>(other.whole) * other.denominator + other.numerator;

    return num1 * other.denominator == num2 * denominator;
}

bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
}

bool Fraction::operator<(const Fraction& other) const {
    long long num1 = static_cast<long long>(whole) * denominator + numerator;
    long long num2 = static_cast<long long>(other.whole) * other.denominator + other.numerator;

    return num1 * other.denominator < num2 * denominator;
}

bool Fraction::operator<=(const Fraction& other) const {
    return *this < other || *this == other;
}

bool Fraction::operator>(const Fraction& other) const {
    return !(*this <= other);
}

bool Fraction::operator>=(const Fraction& other) const {
    return !(*this < other);
}

// ==================== ExpressionNode 类实现 ====================

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
        // 对于数字节点，直接返回其字符串表示
        // Fraction::toString() 已经会为负数添加括号
        return value.toString();
    }

    std::string leftStr = left->toString();
    std::string rightStr = right->toString();

    // 为分数表达式添加括号，避免歧义
    bool leftIsFraction = (left->type == NodeType::NUMBER &&
        (left->value.toString().find('/') != std::string::npos ||
            left->value.toString().find('\'') != std::string::npos));

    bool rightIsFraction = (right->type == NodeType::NUMBER &&
        (right->value.toString().find('/') != std::string::npos ||
            right->value.toString().find('\'') != std::string::npos));

    // 如果操作数是负数（以负号开头），需要加括号
    bool leftIsNegative = (left->type == NodeType::NUMBER && left->value.toDouble() < 0);
    bool rightIsNegative = (right->type == NodeType::NUMBER && right->value.toDouble() < 0);

    // 如果当前是除法运算符，且操作数是分数或负数，需要加括号
    if (op == '/' && (leftIsFraction || leftIsNegative)) {
        // 检查是否已经有括号（负数分数在 Fraction::toString 中已经加了括号）
        if (leftStr[0] != '(') {
            leftStr = "(" + leftStr + ")";
        }
    }

    if (op == '/' && (rightIsFraction || rightIsNegative)) {
        if (rightStr[0] != '(') {
            rightStr = "(" + rightStr + ")";
        }
    }

    // 根据运算符优先级决定是否加括号
    if (left->type == NodeType::OPERATOR &&
        getOperatorPriority(left->op) < getOperatorPriority(op)) {
        leftStr = "(" + leftStr + ")";
    }

    if (right->type == NodeType::OPERATOR &&
        getOperatorPriority(right->op) <= getOperatorPriority(op)) {
        rightStr = "(" + rightStr + ")";
    }

    return leftStr + " " + op + " " + rightStr;
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

// ==================== ExpressionGenerator 类实现 ====================

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

    // 合法性检查：减法不产生负数，除法结果为真分数
    if (op == '-') {
        while (left->evaluate() < right->evaluate()) {
            delete left;
            delete right;
            left = generateExpression(leftOps);
            right = generateExpression(rightOps);
        }
    }
    else if (op == '/') {
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

    if (expr1->op == expr2->op) {
        // 对于加法和乘法，检查交换律
        if (expr1->op == '+' || expr1->op == '*') {
            return (isEquivalent(expr1->left, expr2->left) &&
                isEquivalent(expr1->right, expr2->right)) ||
                (isEquivalent(expr1->left, expr2->right) &&
                    isEquivalent(expr1->right, expr2->left));
        }
        else {
            // 减法和除法不满足交换律
            return isEquivalent(expr1->left, expr2->left) &&
                isEquivalent(expr1->right, expr2->right);
        }
    }

    return false;
}

void ExpressionGenerator::normalizeExpression(ExpressionNode* node) {
    if (node->type != NodeType::OPERATOR) return;

    // 递归规范化子树
    normalizeExpression(node->left);
    normalizeExpression(node->right);

    // 对加法和乘法进行规范化（左操作数 >= 右操作数）
    if (node->op == '+' || node->op == '*') {
        if (node->right->evaluate() > node->left->evaluate()) {
            std::swap(node->left, node->right);
        }
    }
}

// 修改：返回表达式和答案的对
std::pair<std::string, std::string> ExpressionGenerator::generateSingleExpression() {
    std::uniform_int_distribution<int> opCountDist(1, 3);
    int opCount = opCountDist(rng);

    ExpressionNode* expr = generateExpression(opCount);
    normalizeExpression(expr);

    std::string expressionStr = expr->toString() + " = ";
    std::string answerStr = expr->evaluate().toString();

    delete expr;

    return { expressionStr, answerStr };
}

// 修改：返回表达式和答案对的向量
std::vector<std::pair<std::string, std::string>> ExpressionGenerator::generateExpressions(int count) {
    std::vector<std::pair<std::string, std::string>> expressions;
    std::vector<ExpressionNode*> generatedTrees;

    for (int i = 0; i < count; ++i) {
        bool isUnique = false;
        ExpressionNode* newExpr = nullptr;

        // 尝试生成唯一表达式（最多100次）
        for (int attempt = 0; attempt < 100 && !isUnique; ++attempt) {
            std::uniform_int_distribution<int> opCountDist(1, 3);
            int opCount = opCountDist(rng);

            newExpr = generateExpression(opCount);
            normalizeExpression(newExpr);

            // 检查是否与已生成的表达式等价
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

        // 成功生成唯一表达式
        if (isUnique && newExpr) {
            generatedTrees.push_back(newExpr->clone());
            // 存储表达式和答案的对
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

// ==================== 工具函数实现 ====================

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

// 文件操作函数 - 写入题目到文件
bool writeExercisesToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int cnt = 1;
    for (const auto& expr : expressions) {
        file << cnt << '.' << expr.first << std::endl;  // 只写入表达式部分
        cnt++;
    }

    file.close();
    return true;
}

// 文件操作函数 - 写入答案到文件
bool writeAnswersToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int cnt = 1;
    for (const auto& expr : expressions) {
        file << cnt << '.' << expr.second << std::endl;  // 只写入答案部分
        cnt++;
    }

    file.close();
    return true;
}

void Formula_Generator(int count, int range) {
    ExpressionGenerator generator(range);
    auto expressions = generator.generateExpressions(count);
    std::string expressionfile = "Exercises.txt";
    std::string answerfile = "Answers.txt";
    if (writeExercisesToFile(expressions, expressionfile)) {
        std::cout << "Expressions successfully written to " << expressionfile << std::endl;
    }
    else {
        std::cout << "Failed to write expressions to file." << std::endl;
    }
    if (writeAnswersToFile(expressions, answerfile)) {
        std::cout << "Answers successfully written to " << answerfile << std::endl;
    }
    else {
        std::cout << "Failed to write answers to file." << std::endl;
    }
}
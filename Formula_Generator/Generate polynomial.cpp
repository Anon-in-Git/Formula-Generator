#include "Formula_Generator.h"
#include <random>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <fstream>
#include <iostream>

// ==================== Fraction ��ʵ�� ====================

Fraction::Fraction(int num, int den) : numerator(num), denominator(den), whole(0) {
    if (denominator == 0) denominator = 1;
    simplify();
    toProperFraction();  // �ȼ���ת��Ϊ�����
}

Fraction::Fraction(int w, int num, int den) : numerator(num), denominator(den), whole(w) {
    if (denominator == 0) denominator = 1;
    simplify();
    toProperFraction();  // �ȼ���ת��Ϊ�����
}

void Fraction::simplify() {
    if (numerator == 0) {
        denominator = 1;
        return;
    }

    int a = std::abs(numerator);
    int b = denominator;

    // ŷ������㷨�����Լ��
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

    // �������Ϊ0��ֱ�ӷ���
    if (numerator == 0) {
        whole = 0;
        return;
    }

    while (numerator < 0) {
		whole -= 1;
		numerator += denominator;
    }

    // ������Ӿ���ֵ���ڵ��ڷ�ĸ�����
    if (numerator >= denominator) {
        int addToWhole = numerator / denominator;
        whole += addToWhole;
        numerator = std::abs(numerator) % denominator;

        // �������Ϊ0�����÷�ĸΪ1
        if (numerator == 0) {
            denominator = 1;
        }
    }

    // ȷ���������ֺͷ������ֵķ���һ��
    if (whole != 0 && numerator != 0) {
        if ((whole > 0 && numerator < 0) || (whole < 0 && numerator > 0)) {
            // ���������Ӧ�ó��֣�������������
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

    // ����ʵ�ʵ�ֵ�������������֣�
    double actualValue = toDouble();

    // ����Ǹ�������Ҫ�����߼�����
    bool isNegative = (actualValue < 0);

    if (isNegative) {
        ss << "(";
    }

    // ������������ʾ
    if (whole != 0) {
        ss << std::abs(whole);  // ��ʾ����ֵ����������
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

// ==================== ExpressionNode ��ʵ�� ====================

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
        // �������ֽڵ㣬ֱ�ӷ������ַ�����ʾ
        // Fraction::toString() �Ѿ���Ϊ�����������
        return value.toString();
    }

    std::string leftStr = left->toString();
    std::string rightStr = right->toString();

    // Ϊ�������ʽ������ţ���������
    bool leftIsFraction = (left->type == NodeType::NUMBER &&
        (left->value.toString().find('/') != std::string::npos ||
            left->value.toString().find('\'') != std::string::npos));

    bool rightIsFraction = (right->type == NodeType::NUMBER &&
        (right->value.toString().find('/') != std::string::npos ||
            right->value.toString().find('\'') != std::string::npos));

    // ����������Ǹ������Ը��ſ�ͷ������Ҫ������
    bool leftIsNegative = (left->type == NodeType::NUMBER && left->value.toDouble() < 0);
    bool rightIsNegative = (right->type == NodeType::NUMBER && right->value.toDouble() < 0);

    // �����ǰ�ǳ�����������Ҳ������Ƿ�����������Ҫ������
    if (op == '/' && (leftIsFraction || leftIsNegative)) {
        // ����Ƿ��Ѿ������ţ����������� Fraction::toString ���Ѿ��������ţ�
        if (leftStr[0] != '(') {
            leftStr = "(" + leftStr + ")";
        }
    }

    if (op == '/' && (rightIsFraction || rightIsNegative)) {
        if (rightStr[0] != '(') {
            rightStr = "(" + rightStr + ")";
        }
    }

    // ������������ȼ������Ƿ������
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

// ==================== ExpressionGenerator ��ʵ�� ====================

ExpressionGenerator::ExpressionGenerator(int r) : range(r) {
    std::random_device rd;
    rng.seed(rd());
}

Fraction ExpressionGenerator::generateRandomFraction() {
    std::uniform_int_distribution<int> dist(1, range - 1);
    std::uniform_int_distribution<int> typeDist(0, 2); // 0: ����, 1: �����, 2: ������

    int type = typeDist(rng);

    switch (type) {
    case 0: // ����
        return Fraction(dist(rng), 1);

    case 1: // �����
    {
        int num = dist(rng);
        int den = dist(rng);
        if (num >= den) std::swap(num, den);
        return Fraction(num, den);
    }

    case 2: // ������
    {
        int whole = dist(rng) / 2; // ��������СһЩ
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

    // �Ϸ��Լ�飺�����������������������Ϊ�����
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
        // ���ڼӷ��ͳ˷�����齻����
        if (expr1->op == '+' || expr1->op == '*') {
            return (isEquivalent(expr1->left, expr2->left) &&
                isEquivalent(expr1->right, expr2->right)) ||
                (isEquivalent(expr1->left, expr2->right) &&
                    isEquivalent(expr1->right, expr2->left));
        }
        else {
            // �����ͳ��������㽻����
            return isEquivalent(expr1->left, expr2->left) &&
                isEquivalent(expr1->right, expr2->right);
        }
    }

    return false;
}

void ExpressionGenerator::normalizeExpression(ExpressionNode* node) {
    if (node->type != NodeType::OPERATOR) return;

    // �ݹ�淶������
    normalizeExpression(node->left);
    normalizeExpression(node->right);

    // �Լӷ��ͳ˷����й淶����������� >= �Ҳ�������
    if (node->op == '+' || node->op == '*') {
        if (node->right->evaluate() > node->left->evaluate()) {
            std::swap(node->left, node->right);
        }
    }
}

// �޸ģ����ر��ʽ�ʹ𰸵Ķ�
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

// �޸ģ����ر��ʽ�ʹ𰸶Ե�����
std::vector<std::pair<std::string, std::string>> ExpressionGenerator::generateExpressions(int count) {
    std::vector<std::pair<std::string, std::string>> expressions;
    std::vector<ExpressionNode*> generatedTrees;

    for (int i = 0; i < count; ++i) {
        bool isUnique = false;
        ExpressionNode* newExpr = nullptr;

        // ��������Ψһ���ʽ�����100�Σ�
        for (int attempt = 0; attempt < 100 && !isUnique; ++attempt) {
            std::uniform_int_distribution<int> opCountDist(1, 3);
            int opCount = opCountDist(rng);

            newExpr = generateExpression(opCount);
            normalizeExpression(newExpr);

            // ����Ƿ��������ɵı��ʽ�ȼ�
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

        // �ɹ�����Ψһ���ʽ
        if (isUnique && newExpr) {
            generatedTrees.push_back(newExpr->clone());
            // �洢���ʽ�ʹ𰸵Ķ�
            expressions.push_back({
                newExpr->toString() + " = ",
                newExpr->evaluate().toString()
                });
        }

        delete newExpr;
    }

    // �����ڴ�
    for (ExpressionNode* expr : generatedTrees) {
        delete expr;
    }

    return expressions;
}

// ==================== ���ߺ���ʵ�� ====================

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

// �ļ��������� - д����Ŀ���ļ�
bool writeExercisesToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int cnt = 1;
    for (const auto& expr : expressions) {
        file << cnt << '.' << expr.first << std::endl;  // ֻд����ʽ����
        cnt++;
    }

    file.close();
    return true;
}

// �ļ��������� - д��𰸵��ļ�
bool writeAnswersToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int cnt = 1;
    for (const auto& expr : expressions) {
        file << cnt << '.' << expr.second << std::endl;  // ֻд��𰸲���
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
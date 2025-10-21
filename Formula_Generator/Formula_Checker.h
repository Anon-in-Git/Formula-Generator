#ifndef EXPRESSION_GENERATOR_H
#define EXPRESSION_GENERATOR_H

#include <string>
#include <vector>
#include <utility>
#include <random>

// 分数类
class Fraction {
private:
    int numerator;    // 分子
    int denominator;  // 分母
    int whole;        // 整数部分（用于带分数）

    void simplify();  // 约分
    void toProperFraction(); // 转换为真分数形式

public:
    Fraction(int num = 0, int den = 1);
    Fraction(int w, int num, int den);

    // 获取各种表示
    std::string toString() const;
    double toDouble() const;

    // 算术运算
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    // 比较运算
    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;
    bool operator<(const Fraction& other) const;
    bool operator<=(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator>=(const Fraction& other) const;

    // 获取分量
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
    int getWhole() const { return whole; }
    bool isZero() const { return numerator == 0 && whole == 0; }
};

// 表达式节点类型
enum class NodeType {
    NUMBER,
    OPERATOR
};

// 表达式节点
struct ExpressionNode {
    NodeType type;
    Fraction value;  // 数值节点使用
    char op;         // 运算符节点使用
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(const Fraction& val);
    ExpressionNode(char operation, ExpressionNode* l, ExpressionNode* r);
    ~ExpressionNode();

    std::string toString() const;
    Fraction evaluate() const;
    ExpressionNode* clone() const;
};

// 表达式生成器
class ExpressionGenerator {
private:
    int range;
    std::mt19937 rng;

    Fraction generateRandomFraction();
    ExpressionNode* generateExpression(int operatorCount);
    bool isEquivalent(ExpressionNode* expr1, ExpressionNode* expr2);
    void normalizeExpression(ExpressionNode* node);

public:
    ExpressionGenerator(int r);

    // 生成单个表达式
    std::pair<std::string, std::string> generateSingleExpression();

    // 批量生成表达式
    std::vector<std::pair<std::string, std::string>>
        generateExpressions(int count);
};

// 工具函数
bool isOperator(char c);
int getOperatorPriority(char op);
std::string fractionToString(const Fraction& frac);
Fraction stringToFraction(const std::string& str);

#endif
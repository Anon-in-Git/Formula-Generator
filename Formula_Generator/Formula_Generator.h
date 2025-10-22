#ifndef FORMULA_GENERATOR_H
#define FORMULA_GENERATOR_H

#include <string>
#include <vector>
#include <utility>
#include <random>
#include <iostream>

// 分数类
class Fraction {
private:
    int numerator;    // 分子
    int denominator;  // 分母
    int whole;        // 整数部分（用于带分数）

    void simplify();  // 约分
    void toProperFraction(); // 转换为真分数形式

public:
    // 构造函数
    Fraction(int numerator = 0, int denominator = 1);
    Fraction(int whole, int numerator, int denominator);

    // 字符串表示和转换
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

    // 访问器
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

    // 构造函数和析构函数
    ExpressionNode(const Fraction& value);
    ExpressionNode(char operation, ExpressionNode* left, ExpressionNode* right);
    ~ExpressionNode();

    // 核心方法
    std::string toString() const;
    Fraction evaluate() const;
    ExpressionNode* clone() const;
};

// 表达式生成器
class ExpressionGenerator {
private:
    int range;
    std::mt19937 rng;

    // 私有方法
    Fraction generateRandomFraction();
    ExpressionNode* generateExpression(int operatorCount);
    bool isEquivalent(ExpressionNode* expr1, ExpressionNode* expr2);
    void normalizeExpression(ExpressionNode* node);

public:
    // 构造函数
    ExpressionGenerator(int range);

    // 公有接口 - 返回表达式和答案的对
    std::pair<std::string, std::string> generateSingleExpression();
    std::vector<std::pair<std::string, std::string>> generateExpressions(int count);
};

// 工具函数
bool isOperator(char c);
int getOperatorPriority(char op);
std::string fractionToString(const Fraction& frac);

// 文件操作函数 - 分别写入题目和答案
bool writeExercisesToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename);
bool writeAnswersToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename);

void Formula_Generator(int count, int range); // 生成指定数量和范围的算术表达式并写入文件

#endif
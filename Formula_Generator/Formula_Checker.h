#ifndef EXPRESSION_GENERATOR_H
#define EXPRESSION_GENERATOR_H

#include <string>
#include <vector>
#include <utility>
#include <random>

// ������
class Fraction {
private:
    int numerator;    // ����
    int denominator;  // ��ĸ
    int whole;        // �������֣����ڴ�������

    void simplify();  // Լ��
    void toProperFraction(); // ת��Ϊ�������ʽ

public:
    Fraction(int num = 0, int den = 1);
    Fraction(int w, int num, int den);

    // ��ȡ���ֱ�ʾ
    std::string toString() const;
    double toDouble() const;

    // ��������
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    // �Ƚ�����
    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;
    bool operator<(const Fraction& other) const;
    bool operator<=(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator>=(const Fraction& other) const;

    // ��ȡ����
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
    int getWhole() const { return whole; }
    bool isZero() const { return numerator == 0 && whole == 0; }
};

// ���ʽ�ڵ�����
enum class NodeType {
    NUMBER,
    OPERATOR
};

// ���ʽ�ڵ�
struct ExpressionNode {
    NodeType type;
    Fraction value;  // ��ֵ�ڵ�ʹ��
    char op;         // ������ڵ�ʹ��
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(const Fraction& val);
    ExpressionNode(char operation, ExpressionNode* l, ExpressionNode* r);
    ~ExpressionNode();

    std::string toString() const;
    Fraction evaluate() const;
    ExpressionNode* clone() const;
};

// ���ʽ������
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

    // ���ɵ������ʽ
    std::pair<std::string, std::string> generateSingleExpression();

    // �������ɱ��ʽ
    std::vector<std::pair<std::string, std::string>>
        generateExpressions(int count);
};

// ���ߺ���
bool isOperator(char c);
int getOperatorPriority(char op);
std::string fractionToString(const Fraction& frac);
Fraction stringToFraction(const std::string& str);

#endif
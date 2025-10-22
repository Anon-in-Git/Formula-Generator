#ifndef FORMULA_GENERATOR_H
#define FORMULA_GENERATOR_H

#include <string>
#include <vector>
#include <utility>
#include <random>
#include <iostream>

// ������
class Fraction {
private:
    int numerator;    // ����
    int denominator;  // ��ĸ
    int whole;        // �������֣����ڴ�������

    void simplify();  // Լ��
    void toProperFraction(); // ת��Ϊ�������ʽ

public:
    // ���캯��
    Fraction(int numerator = 0, int denominator = 1);
    Fraction(int whole, int numerator, int denominator);

    // �ַ�����ʾ��ת��
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

    // ������
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

    // ���캯������������
    ExpressionNode(const Fraction& value);
    ExpressionNode(char operation, ExpressionNode* left, ExpressionNode* right);
    ~ExpressionNode();

    // ���ķ���
    std::string toString() const;
    Fraction evaluate() const;
    ExpressionNode* clone() const;
};

// ���ʽ������
class ExpressionGenerator {
private:
    int range;
    std::mt19937 rng;

    // ˽�з���
    Fraction generateRandomFraction();
    ExpressionNode* generateExpression(int operatorCount);
    bool isEquivalent(ExpressionNode* expr1, ExpressionNode* expr2);
    void normalizeExpression(ExpressionNode* node);

public:
    // ���캯��
    ExpressionGenerator(int range);

    // ���нӿ� - ���ر��ʽ�ʹ𰸵Ķ�
    std::pair<std::string, std::string> generateSingleExpression();
    std::vector<std::pair<std::string, std::string>> generateExpressions(int count);
};

// ���ߺ���
bool isOperator(char c);
int getOperatorPriority(char op);
std::string fractionToString(const Fraction& frac);

// �ļ��������� - �ֱ�д����Ŀ�ʹ�
bool writeExercisesToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename);
bool writeAnswersToFile(const std::vector<std::pair<std::string, std::string>>& expressions,
    const std::string& filename);

void Formula_Generator(int count, int range); // ����ָ�������ͷ�Χ���������ʽ��д���ļ�

#endif
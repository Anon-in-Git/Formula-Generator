| 这个作业属于哪个课程 | [https://edu.cnblogs.com/campus/gdgy/Class12Grade23ComputerScience](https://edu.cnblogs.com/campus/gdgy/Class12Grade23ComputerScience) |
| ------------------ | --------------------------------------------------------------------------- |
| 这个作业要求在哪里 | [https://edu.cnblogs.com/campus/gdgy/Class12Grade23ComputerScience/homework/13470](https://edu.cnblogs.com/campus/gdgy/Class12Grade23ComputerScience/homework/13470) |
| 这个作业的目标 | 使用 **C++** 实现小学四则运算题目的命令行程序，通过 **GitHub** 与组员合作，记录各个版本并完成项目 |

## 一、GitHub链接与小组成员

仓库地址：[https://github.com/Anon-in-Git/Formula-Generator](https://github.com/Anon-in-Git/Formula-Generator)
</br> **Release** 地址：[https://github.com/Anon-in-Git/Formula-Generator/releases](https://github.com/Anon-in-Git/Formula-Generator/releases)

| 姓名 | 学号|
| --- | --- |
| 郭靖扬 | 3123004655 |
| 王耀廷 | 3123000586 |

## 二、PSP表格预估耗时

| **PSP2.1** | **Personal Software Process Stages** | **预估耗时（分钟）** | **实际耗时（分钟）** |
| --- | --- | --- | --- |
| Planning | 计划 | 10 | |
| Estimate | 估计这个任务需要多少时间 | 5 | |
| Development | 开发 | 10 | |
| Analysis | 需求分析（包括学习新技术） | 20 | |
| Design Spec | 生成设计文档 | 20 | |
| Design Review | 设计复审 | 10 | |
| Coding Standard | 代码规范（为目前的开发制定合适的规范） | 5 | |
| Design | 具体设计 | 10 | |
| Coding | 具体编码 | 30 | |
| Code Review | 代码复审 | 5 | |
| Test | 测试(自我测试，修改代码，提交修改) | 20 | |
| Reporting | 报告 | 15 | |
| Test Report | 测试报告 | 15 | |
| Size Measurement | 计算工作量 | 5 | |
| Postmortem & Process Improvement Plan | 事后总结，并提出过程改进计划| 10 | |
| Total | 合计 | 190 | |

## 三、效能分析

在进行生成算式的过程中，代码使用了一个分数类，重载了分数的加减乘除，优化了其中的时间，时间与空间的复杂度为 $O(n)$。

在进行检验算式的过程中，代码的时间与空间复杂度均为 $O(n)$，数组的总长规模不超过输入数据的总长度。

在进行性能分析的时候，发现 ```toString()``` 消耗的时间较大，重复调用较多，后续考虑对这一部分的优化。

![bbaf3b319f8a573f070e0bc1a99aab5](https://img2024.cnblogs.com/blog/3698606/202510/3698606-20251022194059625-956721094.png)

## 四、设计实现过程

代码主要分为两个部分，生成四则运算题目答案，和检验四则运算题目答案是否相符。

主函数中会检查各种边界条件，接着根据需要调用 ```Formula_Generator()``` 和 ```Formula_Checker()``` 进行生成和检验，它们分别位于 Formula_Generator.h 和 Formula_Checker.h 头文件中。

在生成中，代码有以下实现过程：

# 代码实现过程

## 1.代码组织结构

### fraction类(分数类)

#### 构造函数

```plaintext
Fraction(int numerator = 0, int denominator = 1) - 基本分数构造函数
Fraction(int whole, int numerator, int denominator) - 带分数构造函数
```

#### 核心方法

```plaintext
void simplify() - 分数约分（私有）
void toProperFraction() - 转换为真分数形式（私有）
std::string toString() const - 转换为字符串表示
double toDouble() const - 转换为双精度浮点数
bool isZero() const - 检查是否为零
```

#### 算术运算符重载

```plaintext
Fraction operator+(const Fraction& other) const - 加法
Fraction operator-(const Fraction& other) const - 减法
Fraction operator*(const Fraction& other) const - 乘法
Fraction operator/(const Fraction& other) const - 除法
```

#### 比较运算符重载

```plaintext
bool operator==(const Fraction& other) const - 等于
bool operator!=(const Fraction& other) const - 不等于
bool operator<(const Fraction& other) const - 小于
bool operator<=(const Fraction& other) const - 小于等于
bool operator>(const Fraction& other) const - 大于
bool operator>=(const Fraction& other) const - 大于等于
```

#### 访问器

```plaintext
int getNumerator() const - 获取分子
int getDenominator() const - 获取分母
int getWhole() const - 获取整数部分
```

### ExpressionNode类(表达式节点)

#### 构造函数和析构函数

```plaintext
ExpressionNode(const Fraction& value) - 数值节点构造函数
ExpressionNode(char operation, ExpressionNode* left, ExpressionNode* right) - 运算符节点构造函数
~ExpressionNode() - 析构函数
```

#### 核心方法

```plaintext
std::string toString() const - 生成表达式字符串
Fraction evaluate() const - 计算表达式值
ExpressionNode* clone() const - 深度复制表达式树
```

### ExpressionGenerator类(表达式生成)

#### 生成函数

```plaintext
ExpressionGenerator(int range) - 初始化生成器，设置数值范围
```

#### 私有函数

```plaintext
Fraction generateRandomFraction() - 生成随机分数
ExpressionNode* generateExpression(int operatorCount) - 递归生成表达式树
bool isEquivalent(ExpressionNode* expr1, ExpressionNode* expr2) - 检查表达式等价性
void normalizeExpression(ExpressionNode* node) - 规范化表达式树
```

#### 公有接口

```plaintext
std::string generateSingleExpression() - 生成单个表达式
std::vector<std::string> generateExpressions(int count) - 批量生成唯一表达式
```

### 工具函数

```plaintext
bool isOperator(char c) - 判断字符是否为运算符
int getOperatorPriority(char op) - 获取运算符优先级
std::string fractionToString(const Fraction& frac) - 分数转字符串
bool writeExpressionsToFile(const std::vector<std::string>& expressions, const std::string& filename) - 写入表达式到文件
```

## 2.类关系图

```plaintext
┌─────────────────┐   包含    ┌─────────────────┐   使用    ┌───────────────┐
│ ExpressionGenerator │───────>│  ExpressionNode  │────────>│    Fraction    │
│                 │          │                 │          │               │
│ - range         │          │ - type          │          │ - numerator   │
│ - rng           │          │ - value(Fraction)│          │ - denominator │
│                 │          │ - op            │          │ - whole       │
│ + generateSingle│          │ - left          │          │               │
│ + generateBatch │          │ - right         │          │ + toString()  │
└─────────────────┘          │                 │          │ + operators   │
                             │ + toString()    │          └───────────────┘
                             │ + evaluate()    │
                             │ + clone()       │
                             └─────────────────┘
```

## 3.关键函数流程图

<font size="3">__+ 表达式生成流程 (generateExpressions)__</font>

```plaintext
开始
  ↓
创建空表达式列表和树列表
  ↓
循环 count 次
  ↓
尝试生成唯一表达式 (最多100次)
  ├─ 随机选择运算符数量 (1-3)
  ├─ 递归构建表达式树
  ├─ 规范化表达式树
  ├─ 检查与已生成表达式是否等价
  └─ 如果唯一则添加到列表
  ↓
将表达式树转换为字符串
  ↓
添加到结果向量
  ↓
返回表达式列表
结束
```

<font size="3">__+ 表达式树生成流程 (generateExpression)__</font>

```plaintext
开始 (operatorCount)
  ↓
如果 operatorCount == 0
  ├─ 生成随机分数节点
  └─ 返回
  ↓
随机分配左右子树运算符数量
  ↓
递归生成左子树
  ↓
递归生成右子树
  ↓
随机选择运算符 (+, -, *, /)
  ↓
合法性检查：
  ├─ 如果是减法：确保左操作数 ≥ 右操作数
  └─ 如果是除法：确保结果为真分数
  ↓
创建运算符节点并返回
结束
```

<font size="3">__+ 分数规范化流程 (toProperFraction)__</font>

```plaintext
开始
  ↓
确保分母为正数
  ↓
如果分子 > 分母
  ├─ 提取整数部分
  └─ 更新分子
  ↓
处理负数情况
  ↓
确保分数部分为正
结束
```

---

在检验时，```Formula_Checker()``` 会先进行文本的读入，接着再送至 ```Recognize()``` 进行识别数字与符号，并存入 ```std::vector<int>``` 中。接着会使用```Check()```函数进行检验是否存在连续的四则运算或非法的括号序列。最后会将算式和答案分别送至 ```Calculate()``` 计算答案，统计并输出。

![draw](https://img2024.cnblogs.com/blog/3698606/202510/3698606-20251022165613331-1603054663.png)

<font color = red>**对于非法字符或空格，```Recognize()``` 会自行跳过。**
</br>**遇到由纯空格组成的行或者空行，```Recognize()``` 会自行跳过。**
</br>**```Recognize()``` 不会检查题目和答案的题号是否一致，只会根据相对顺序进行验证，请自行检查题目顺序。**
</br>**```Check()``` 会检查是否存在两个连续的运算符，在使用负数时请务必在左右加上括号。**
</br>**生成的 ```Grade.txt``` 默认生成在 exe 文件的目录下**</font>

检验使用的所有函数包装在 **FC** 域中，在主函数调用时需声明。
</br>检验使用的带分数会自动转换为假分数，检验时使用真假分数检验，这一过程对于用户不透明。

## 五、代码说明

在生成中，代码有以下说明：

# 代码说明

## 1. Fraction 类 - 分数处理核心

```cpp
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

    // 处理分子大于等于分母的情况
    if (numerator >= denominator) {
        whole += numerator / denominator;
        numerator %= denominator;
    }

    // 处理分子为负数的情况
    while (numerator < 0) {
         whole -= 1;
         numerator += denominator;
    }

}

std::string Fraction::toString() const {
    std::stringstream ss;

    // 处理负数的正确表示
    if (whole < 0 ) {
        if ( numerator == 0) {
            // 负整数
            ss << whole;
        }
        else {
            // 负带分数 - 这是不正确的表示，需要转换
            // 将 -a'b/c 转换为 -(a'b/c)
            ss << "-" <<whole<<"'"<<numerator << "/" << denominator;
        }
    }
    else if (whole == 0) {
        ss << "-" << numerator << "/" << denominator;
    }
    else {
        // 正数的正常表示
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
    }

    return ss.str();
}
```

### 设计思路

#### 1.自动处理分数约分和格式转换

#### 2.支持三种表示形式：整数、真分数、带分数

#### 3.精确的数学运算，避免浮点数精度问题

#### 4.符合项目要求的输出格式

## 2.ExpressionNode 类 - 表达式树结构

```cpp
struct ExpressionNode {
    NodeType type;
    Fraction value;
    char op;
    ExpressionNode* left, *right;
    
    std::string toString() const {
        if (type == NodeType::NUMBER) {
            return value.toString();
        }
        
        std::string leftStr = left->toString();
        std::string rightStr = right->toString();
        
        // 为分数表达式添加括号，避免歧义（如 15/17 / 27/19）
        bool leftIsFraction = (left->type == NodeType::NUMBER && 
                              (left->value.toString().find('/') != std::string::npos));
        
        bool rightIsFraction = (right->type == NodeType::NUMBER && 
                               (right->value.toString().find('/') != std::string::npos));
        
        // 如果当前是除法运算符，且操作数是分数，需要加括号
        if (op == '/' && leftIsFraction) {
            leftStr = "(" + leftStr + ")";
        }
        if (op == '/' && rightIsFraction) {
            rightStr = "(" + rightStr + ")";
        }
        
        // 根据运算符优先级自动添加括号
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
    
    Fraction evaluate() const {
        // 递归计算表达式值，用于合法性验证
        if (type == NodeType::NUMBER) return value;
        
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
    
    ExpressionNode* clone() const {
        // 深度复制支持表达式去重检查
        if (type == NodeType::NUMBER) {
            return new ExpressionNode(value);
        } else {
            return new ExpressionNode(op, left->clone(), right->clone());
        }
    }
};
```

### 设计思路

#### 1.使用二叉树表示任意复杂的四则运算表达式

#### 2.自动处理运算符优先级和括号

#### 3.递归计算表达式值，支持合法性验证

#### 4.深度复制支持表达式去重检查

## 3.ExpressionGenerator 类 - 核心生成逻辑

```cpp
class ExpressionGenerator {
private:
    int range;  // 数值范围
    std::mt19937 rng;  // 随机数生成器
    
    ExpressionNode* generateExpression(int operatorCount) {
        if (operatorCount == 0) {
            return new ExpressionNode(generateRandomFraction());
        }
        
        // 递归生成表达式树
        int leftOps = std::uniform_int_distribution<int>(0, operatorCount - 1)(rng);
        int rightOps = operatorCount - 1 - leftOps;
        
        char op = "+-*/"[std::uniform_int_distribution<int>(0, 3)(rng)];
        ExpressionNode* left = generateExpression(leftOps);
        ExpressionNode* right = generateExpression(rightOps);
        
        // 项目要求：减法不产生负数，除法结果为真分数
        if (op == '-') {
            while (left->evaluate() < right->evaluate()) {
                delete left; delete right;
                left = generateExpression(leftOps);
                right = generateExpression(rightOps);
            }
        } else if (op == '/') {
            Fraction result = left->evaluate() / right->evaluate();
            while (result.getWhole() != 0 || result.getNumerator() == 0) {
                delete left; delete right;
                left = generateExpression(leftOps);
                right = generateExpression(rightOps);
                result = left->evaluate() / right->evaluate();
            }
        }
        
        return new ExpressionNode(op, left, right);
    }
    
    bool isEquivalent(ExpressionNode* expr1, ExpressionNode* expr2) {
        // 检查表达式等价性（考虑交换律）
        if (expr1->type != expr2->type) return false;
        if (expr1->type == NodeType::NUMBER) {
            return expr1->value == expr2->value;
        }
        if (expr1->op == expr2->op) {
            if (expr1->op == '+' || expr1->op == '*') {
                // 检查交换律：a+b ≡ b+a
                return (isEquivalent(expr1->left, expr2->left) && 
                        isEquivalent(expr1->right, expr2->right)) ||
                       (isEquivalent(expr1->left, expr2->right) && 
                        isEquivalent(expr1->right, expr2->left));
            }
            return isEquivalent(expr1->left, expr2->left) && 
                   isEquivalent(expr1->right, expr2->right);
        }
        return false;
    }
    
    void normalizeExpression(ExpressionNode* node) {
        // 规范化表达式，减少等价表达式的产生
        if (node->type != NodeType::OPERATOR) return;
        
        normalizeExpression(node->left);
        normalizeExpression(node->right);
        
        // 对加法和乘法进行规范化（左操作数 >= 右操作数）
        if (node->op == '+' || node->op == '*') {
            if (node->right->evaluate() > node->left->evaluate()) {
                std::swap(node->left, node->right);
            }
        }
    }

public:
    std::vector<std::pair<std::string, std::string>> generateExpressions(int count) {
        std::vector<std::pair<std::string, std::string>> expressions;
        std::vector<ExpressionNode*> generatedTrees;
        
        for (int i = 0; i < count; ++i) {
            bool isUnique = false;
            ExpressionNode* newExpr = nullptr;
            
            // 最多尝试100次生成唯一表达式
            for (int attempt = 0; attempt < 100 && !isUnique; ++attempt) {
                int opCount = std::uniform_int_distribution<int>(1, 3)(rng);
                newExpr = generateExpression(opCount);
                normalizeExpression(newExpr);
                
                isUnique = true;
                for (ExpressionNode* existing : generatedTrees) {
                    if (isEquivalent(newExpr, existing)) {
                        isUnique = false;
                        break;
                    }
                }
                
                if (!isUnique) delete newExpr;
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
        for (ExpressionNode* expr : generatedTrees) delete expr;
        return expressions;
    }
};
```

### 关键算法思路：

#### 1.递归生成：使用分治策略构建表达式树，确保运算符数量限制

#### 2.合法性验证：实时检查减法和除法的约束条件

#### 3.规范化处理：对加法和乘法进行排序，减少等价表达式

#### 4.去重算法：基于表达式树结构比较，考虑运算符交换律

#### 5.内存管理：妥善处理动态分配的表达式树，防止内存泄漏

---

在检验中，关键代码为 ```Calculate()```。

遇到数字时，代码会根据之前的运算符进行乘除，或初始填入暂存的数据并根据减法变号；
</br>遇到加减法时，代码会将暂存的数据存入变长数组中，同时记录运算符号；
</br>遇到乘除法时，代码会记录运算符号；
</br>遇到左括号时，代码会递归调用自身，算出括号的结果，之后视为数字处理，并跳过括号的内容；
</br>遇到右括号时，代码会算出数组和暂存数据的总和并返回。

```cpp
// PII 即 pair<int, int>, first表示分子，second表示分母
static PII Calculate(const std::vector<int> &Ex, int dep, int &idx) {
        // 分子和分母的数组
        std::vector<int> res, div;
        // tmp表示暂存数据的分子，mag表示分母，op表示上一运算符
        int tmp = 1, mag = 1, op = 1;
        // init表示需要初始化暂存数据，rt表示需要立即返回数组和
        bool init = true, rt = false;
        for (int i = idx; i < Ex.size(); i ++) {
            // 暂存数据的初始化
            if (init && Ex[i] >= 0) {
                init = false;
                tmp = Ex[i] * op;
                mag = 1;
                continue;
            }
            // 分类讨论运算符
            switch (Ex[i]) {
                // 加法
                case -1:
                    if (!init) {
                        res.emplace_back(tmp);
                        div.emplace_back(mag);
                    }
                    init = true;
                    op = 1;
                    break;
                // 减法
                case -2:
                    if (!init) {
                        res.emplace_back(tmp);
                        div.emplace_back(mag);
                    }
                    init = true;
                    op = -1;
                    break;
                // 乘法
                case -3:
                    op = 3;
                    break;
                // 除法
                case -4:
                    op = 4;
                    break;
                default:
                    // 左括号
                    if (Ex[i] == -5) {
                        i ++;
                        // 递归算出括号内的值
                        auto t = Calculate(Ex, dep + 1, i);
                        // std::cout << i << ": " << t.first << " " << t.second << std::endl;
                        if (init) {
                            init = false;
                            tmp = t.first * op;
                            mag = t.second;
                        }
                        else {
                            // 乘以括号内的数
                            if (op == 3) {
                                tmp *= t.first;
                                mag *= t.second;
                                // 约分
                                int divi = std::gcd(tmp, mag);
                                if (divi > 0) {
                                    tmp /= divi;
                                    mag /= divi;
                                }
                            }
                            // 除以括号内的数
                            if (op == 4) {
                                tmp *= t.second;
                                mag *= t.first;
                                int divi = std::gcd(tmp, mag);
                                if (divi > 0) {
                                    tmp /= divi;
                                    mag /= divi;
                                }
                            }
                        }
                        break;
                    }
                    // 右括号
                    if (Ex[i] == -6) {
                        // 打标记后立即统计答案返回
                        rt = true;
                        break;
                    }
                    // 遇到正常数字
                    if (Ex[i] >= 0) {
                        // 乘法
                        if (op == 3) {
                            tmp *= Ex[i];
                            int divi = std::gcd(tmp, mag);
                            if (divi > 0) {
                                tmp /= divi;
                                mag /= divi;
                            }
                        }
                        // 除法
                        if (op == 4 && Ex[i] > 0) {
                            mag *= Ex[i];
                            int divi = std::gcd(tmp, mag);
                            if (divi > 0) {
                                tmp /= divi;
                                mag /= divi;
                            }
                        }
                    }
            }
            // 退出循环
            if (rt) {
                idx = i;
                break;
            }
        }
        // 塞入暂存数据
        if (!init) {
            res.emplace_back(tmp);
            div.emplace_back(mag);
        }
        // 算出总和
        PII ans = {0, 1};
        for (int i = 0; i < res.size(); i ++) {
            ans.first = ans.first * div[i] + ans.second * res[i];
            ans.second = ans.second * div[i];
            int divi = std::gcd(ans.first, ans.second);
            if (divi == 0)
                continue;
            ans.first /= divi;
            ans.second /= divi;
        }
        return ans;
    }
```

## 六、测试运行

运行程序时使用命令行 ```Formula_Generator.exe -r 10 -n 10 -e Exercises.txt -a Answers.txt```

生成的 ```Exercises.txt``` 如下

```plaintxt
1.(3/7) / (3'1/7) = 
2.(4'8/9) / 9 / ((3'5/7) / (4'6/7)) = 
3.4'1/3 - 3'2/5 = 
4.8 - 1 = 
5.(2/7) / (2'7/9) / (2/3) = 
6.4 * 2'1/4 - 3/4 - 4/9 = 
7.9 - 4'1/2 = 
8.(2/3 - 3/7) / (2/7) / (7/8) = 
9.(2'1/2) / (7 - 5/9) = 
10.3'3/7 - 1/7 = 
```

生成的 ```Answers.txt``` 如下

```plaintxt
1.3/22
2.748/1053
3.14/15
4.7
5.27/175
6.7'29/36
7.4'1/2
8.20/21
9.45/116
10.3'2/7
```

生成的 ```Grade.txt``` 如下

```plaintxt
Correct: 10 (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
Wrong: 0 ()
```

经过手工验算，十道题目均为正确答案。

同时也进行了额外的十组测试，并未发现问题。

## 七、PSP表格实际耗时

| **PSP2.1** | **Personal Software Process Stages** | **预估耗时（分钟）** | **实际耗时（分钟）** |
| --- | --- | --- | --- |
| Planning | 计划 | 10 | 10 |
| Estimate | 估计这个任务需要多少时间 | 5 | 5 |
| Development | 开发 | 10 | 30 |
| Analysis | 需求分析（包括学习新技术） | 20 | 10 |
| Design Spec | 生成设计文档 | 20 | 10 |
| Design Review | 设计复审 | 10 | 5 |
| Coding Standard | 代码规范（为目前的开发制定合适的规范） | 5 | 5 |
| Design | 具体设计 | 10 | 10 |
| Coding | 具体编码 | 30 | 60 |
| Code Review | 代码复审 | 5 | 60 |
| Test | 测试(自我测试，修改代码，提交修改) | 20 | 20 |
| Reporting | 报告 | 15 | 60 |
| Test Report | 测试报告 | 15 | 15 |
| Size Measurement | 计算工作量 | 5 | 5 |
| Postmortem & Process Improvement Plan | 事后总结，并提出过程改进计划| 10 | 10 |
| Total | 合计 | 190 | 315 |

## 八、项目小结

在这次的项目合作中，我们理解了标准化的重要性。在许多接口对接的过程中，我们不断协调彼此的代码与格式，修改了一版又一版的代码，终于完成这个项目，正是我们的勤劳协作和互相体谅造就了这一切，同时我们也理解了这个项目的来之不易。有一个比较好的建议就是将代码分成两个部分，一人完成一个部分，在调试和修改代码的时候就比较方便。

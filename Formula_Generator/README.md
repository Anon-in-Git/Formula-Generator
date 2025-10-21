公式生成函数在主函数中的使用方法：

int range   // 定义生成公式的范围
int count	// 定义生成公式的数量

ExpressionGenerator generator(range);	// 创建公式生成器对象

auto expressions = generator.generateExpressions(count);	// 生成指定数量的公式(返回值为vector<string>)

writeExpressionsToFile(expressions, "Exercises.txt");	// 将生成的公式写入文件
��ʽ���ɺ������������е�ʹ�÷�����

int range   // �������ɹ�ʽ�ķ�Χ
int count	// �������ɹ�ʽ������

ExpressionGenerator generator(range);	// ������ʽ����������

auto expressions = generator.generateExpressions(count);	// ����ָ�������Ĺ�ʽ(����ֵΪvector<string>)

writeExpressionsToFile(expressions, "Exercises.txt");	// �����ɵĹ�ʽд���ļ�
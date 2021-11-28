#include "Parser.h"
#include "Factory.h"
#include "Worker.h"
int main(int argc, char** argv) {
	WorkflowParser* parser = new WorkflowParser;
	parser->set_argc(argc);
	parser->set_argv(argv);
	if (parser->parse_file())
		cout << "OK" << '\n';
	else
		cout << "sth went wrong" << '\n';


	cout << "Factory is working...\n";
	Factory f;
	int ctr = 1;
	vector <string> *general_buffer = new vector <string>;
	vector <int> order_of_operations = parser->get_order();
	vector <int> ::iterator i = order_of_operations.begin();
	for (; i != order_of_operations.end(); i++) { 
		cout << "Operation " << ctr << "...\n";
		ctr++;
 		Worker* worker = f.Create(parser->get_methods()[*i].operation, parser->get_methods()[*i].arguments, general_buffer);
		worker->do_work();
		vector <string> ::iterator res_i = (*general_buffer).begin();
	}
	cout << "Factory finished successfully!\n";
	return 0;
}

//Parser
//
// |
//\|/
//
//Main										
//
///|\
// |
//
//Factory  		 Worker <-	. sort		+
//							. grep		+
//							. replace	+
//							. dump		+
//							. readfile  +
//							. writefile +
///
//
//	--------------------------------------------
//
//
//1) ������� ������ �������.
//2) �������� argc � argv � ������
//3) ��������� ������� ������� � �������
//	. ���������, ��� �������� ���� �������� � �� ������ (!filename.is_open() && filename.peek() != EOF)
//	. ������� �������� ������ ������ ����������������� ����� � ����� ��� "desc"
//	. ��������� stringstream ��������� �� ����� � ������ � ����� �� "csed"
//		(
//			������ ��������: 
//				id	 method arguments
//				1 = replace w1 w2
//			�������� ��������� 1 �������� � �����, ���� �����, �� ��� ������ �������� i (try/catch/throw)
//			��������� ������ '=', ���� �����, �� ��������� ��������, ���� �����, �� ��������� ���������
//			�������� � ������ ���� <i, block>, ��� block == block.operation � block.arguments
//			
//		)
//	. ���������� True, ���� ����� ����������
//4) �������� �������� ������ ���.
//5) � ����� ��������� ���������� ��������
//	. � ������� stringstream ��������� �� ����� � �������� ��������� ��������
//		! � Factory ��������� block, ������� �������� switch/case ����������
//		! � Factory �������� ������� � ���������� ����� "������"
//	. ���� �����, ������ ������
//	. ��������� �������, ���� �����, ����������
//6) �����
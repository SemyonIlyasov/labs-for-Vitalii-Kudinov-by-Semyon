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
//1) создать объект парсера.
//2) передать argc и argv в парсер
//3) Запустить парсинг методом в парсере
//	. проверить, что считаный файл считался и не пустой (!filename.is_open() && filename.peek() != EOF)
//	. считать отдельно первую строку конфигурационного файла и найти там "desc"
//	. используя stringstream разбиваем на слова и парсим в цикле до "csed"
//		(
//			пример парсинга: 
//				id	 method arguments
//				1 = replace w1 w2
//			пытаемся перевести 1 аргумент в число, если успех, то это индекс операции i (try/catch/throw)
//			считываем символ '=', если успех, то считываем операцию, если успех, то считываем аргументы
//			аппендим в список пару <i, block>, где block == block.operation и block.arguments
//			
//		)
//	. возвращаем True, если успех считывания
//4) геттером вытащить список пар.
//5) в цикле запустить считывание операций
//	. с помощью stringstream разбиваем на слова и пытаемся выполнить операции
//		! в Factory передаётся block, который парсится switch/case оператором
//		! в Factory создаётся рабочий и вызывается метод "работа"
//	. если успех, читаем дальше
//	. считываем стрелку, если успех, продолжаем
//6) конец
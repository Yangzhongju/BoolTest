#include "ExprLibFileHandler.h"
#include "ExpProgramCreater.h"

void main(int argc, char **argv)
{
	//����Ԥ����
	//����: CreateExp_GetAllFailTests TCAS.txt TCASMutant.txt TCAS.h TCASMutant.h exper.cpp
	if(argc!=6)return;
	char *origin_expr_file=argv[1];		//����1:���ԭʼ�������ʽ���ļ�
	char *mutant_expr_file=argv[2];		//����2:��ű��첼�����ʽ���ļ�

	char *origin_expr_cpp_file=argv[3];	//����3:���ԭʼ�������ʽ��.h�ļ�
	char *mutant_expr_cpp_file=argv[4];	//����4:��ű��첼�����ʽ��.h�ļ�

	char *program_src_file=argv[5];		//����5:����ʵ�����е�.cpp�ļ�

	//FOR DEBUG
	/*char *origin_expr_file="../Temp/TCAS.txt";
	char *mutant_expr_file="../Temp/TCASMutant.txt";
	char *origin_expr_cpp_file="../Temp/TCAS.h";
	char *mutant_expr_cpp_file="../Temp/TCASMutant.h";
	char *program_src_file="../Temp/AFT_exp.cpp";*/

	/*---------------------------------------------------------------*/

	std::vector<std::string> origin_fun_names;
	std::vector<std::vector<std::vector<std::string> > > mutant_fun_names;
	std::vector<std::vector<char> > all_argument_list;

	//����ԭʼ�������ʽ, ������.h�ļ�
	HandleOrigin(origin_expr_file,origin_expr_cpp_file,all_argument_list,origin_fun_names);

	//������첼�����ʽ, ������.h�ļ�
	HandleMutant(mutant_expr_file,mutant_expr_cpp_file,all_argument_list,mutant_fun_names);

	//����ʵ�����Դ����
	ExpProgramCreater prog_creater;
	prog_creater.CreateProgram(origin_fun_names,mutant_fun_names,all_argument_list,
							   origin_expr_cpp_file,mutant_expr_cpp_file,
							   program_src_file);
}
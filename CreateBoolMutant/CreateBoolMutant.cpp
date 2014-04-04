#include <fstream>
#include <iostream>
#include "AllCreater.h"
#include "../SolutionBase/OriginExprFileIO.h"
#include "../SolutionBase/MutantExprFileIO.h"

void main(int argc, char **argv)
{
	//����Ԥ����
	//����: CreateBoolMutant TCAS.txt FaultType.ini TCASMutant.txt
	if(argc!=4 && argc!=5)return;
	char *origin_expr_file=argv[1];//����1:���ԭʼ�������ʽ���ļ�
	char *fault_types_file=argv[2];//����2:ָ������������͵��ļ�
	char *mutant_expr_file=argv[3];//����3:������ñ�����ʽ���ļ�

	bool is_display=false;
	if(argc==5 && strcmp("-d",argv[4])==0)
		is_display=true;

	//FOR DEBUG
	/*char *origin_expr_file="../Temp/TCAS.txt";
	char *fault_types_file="../Temp/FaultType.ini";
	char *mutant_expr_file="../Temp/TCASMutant.txt";*/

	/*---------------------------------------------------------------*/

	//��origin_expr_file�ļ��ж������б��ʽ
	std::string expr_common_name;
	std::vector<std::string> expressions;
	ReadOriginExprFile(origin_expr_file,expr_common_name,expressions);

	//��fault_types_file�ļ��ж���ָ���ı�������
	std::vector<std::string> types;
	std::ifstream infile2;
	infile2.open(fault_types_file);
	if(!infile2)
	{
		std::cout<<"Error when reading file "<<fault_types_file;
		return;
	}
	try
	{
		char buffer[1024];
		while(infile2.getline(buffer,1024,'\n'))
			types.push_back(buffer);
	}
	catch (...)
	{
		infile2.close();
		std::cout<<"Error when reading file "<<fault_types_file;
		return;
	}
	infile2.close();

	//��ÿһ�����ʽ����������ָ�����͵ı�����ʽ
	std::vector<std::vector<std::vector<std::string> > > all_mutants(expressions.size());
	for(int i=0;i<expressions.size();i++)
	{
		AllCreater creater;
		creater.SetMutationType(types);
		creater.CreateGivenClassMutants(expressions[i],all_mutants[i]);

		if(is_display)
		{
			std::cout<<"The "<<i+1<<"-th original expression: "<<expressions[i]<<'\n';
			std::cout<<"The mutants of the "<<i+1<<"-th original expression include:\n";
			for(int j=0;j<all_mutants[i].size();j++)
			{
				std::cout<<"Mutation type = "<<types[j]<<" :\n";
				for(int k=0;k<all_mutants[i][j].size();++k)
					std::cout<<all_mutants[i][j][k]<<'\n';
			}
		}
	}

	//��������ʽд������ļ�mutant_expr_file
	WriteMutantExprFile(mutant_expr_file,origin_expr_file,expressions,types,all_mutants);
}
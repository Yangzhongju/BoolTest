#include <fstream>
#include <iostream>
#include "MuCreaterBuilder.h"
//#include "../SolutionBase/OriginExprFileIO.h"
//#include "../SolutionBase/MutantExprFileIO.h"
#include "ExpressionFileIO.h"

int main(int argc, char **argv)
{
    //����CreateBoolMutant /?
    if(argc==2 && strcmp("/?",argv[1])==0)
    {
        std::cout<<"CreateBoolMutant file_origin_expressions file_mutant_types file_mutant_expressions [-disp]\n";
        std::cout<<"\tfile_origin_expressions: file contains origin boolean expressions, one expression per line\n";
        std::cout<<"\tfile_mutant_types: file contains mutant types, one mutant type per line\n";
        std::cout<<"\tfile_mutant_expressions: output file contains mutant boolean expressions\n";
        std::cout<<"\t[-disp]: display context of output file\n";
        return 0;
    }
    
    //����ʹ��, ����: CreateBoolMutant TCAS.txt FaultType.ini TCASMutant.txt
    //����Ԥ����
    if(argc!=4 && argc!=5)
    {
        std::cout<<"Error arguments!";
        return 0;
    }
    char *origin_expr_file=argv[1];//����1:���ԭʼ�������ʽ���ļ�
    char *fault_types_file=argv[2];//����2:ָ������������͵��ļ�
    char *mutant_expr_file=argv[3];//����3:������ñ�����ʽ���ļ�

    bool is_display=false;
    if(argc==5 && strcmp("-disp",argv[4])==0)//����4:�����Ƿ�����Ļ�����
        is_display=true;

    /*---------------------------------------------------------------*/

    //��origin_expr_file�ļ��ж������б��ʽ
    std::vector<std::string> expressions;
    try
    {
        ReadOriginExprFile(origin_expr_file,expressions);
    }
    catch(...)
    {
        std::cout<<"Error when reading file "<<origin_expr_file;
        return 0;
    }

    //��fault_types_file�ļ��ж���ָ���ı�������
    std::vector<std::string> mutant_types;
    std::ifstream infile2;
    infile2.open(fault_types_file);
    if(!infile2)
    {
        std::cout<<"Error when reading file "<<fault_types_file<<'\n';
        return 0;
    }
    try
    {
        char buffer[8];
        while(infile2.getline(buffer,8,'\n'))
            mutant_types.push_back(buffer);
    }
    catch(...)//���ܶ�ȡԭʼ�������ʽ�ļ�ʱ�׳���"Fail to Open File!"��"Fail to Read File!"�쳣
    {
        infile2.close();
        std::cout<<"Error when reading file "<<fault_types_file<<'\n';
        return 0;
    }
    infile2.close();

    //����ָ���ı������ʹ�����Ӧ�ı���������
    MutantCreaterBuilder builder;
    std::vector<MutantCreater *> mutant_creaters(mutant_types.size());
    for(int i=0;i<mutant_creaters.size();i++)
    {
        mutant_creaters[i]=builder.Build(mutant_types[i]);
    }

    std::vector<std::vector<std::vector<std::string> > > all_mutants(expressions.size());
    //��ÿһ�����ʽ, ��������ָ�����͵ı�����ʽ
    for(int i=0;i<expressions.size();i++)
    {
        try{
            all_mutants[i].resize(mutant_creaters.size());
            //��ѭ��ÿ������һ��ָ�����͵ı�����ʽ
            for(int j=0;j<mutant_creaters.size();j++)
            {
                if(mutant_creaters[j]!=NULL)
                {
                    mutant_creaters[j]->CreateMutants(expressions[i],all_mutants[i][j]);
                }
            }

            if(is_display)
            {
                std::cout<<"The "<<i+1<<"-th original expression: "<<expressions[i]<<'\n';
                std::cout<<"The mutants of the "<<i+1<<"-th original expression include:\n";
                for(int j=0;j<all_mutants[i].size();j++)
                {
                    std::cout<<"Mutation type = "<<mutant_types[j]<<" :\n";
                    for(int k=0;k<all_mutants[i][j].size();++k)
                        std::cout<<all_mutants[i][j][k]<<'\n';
                }
            }
        }
        catch(char *exception_info)//���ܱ����������׳���"Boolean Expression Solecism!"�쳣
        {
            all_mutants[i].clear();
            std::cout<<exception_info<<"in "<<i+1<<"-th expression!\n";
        }
    }

    //�ͷű�����������Դ
    for(int i=0;i<mutant_creaters.size();i++)
    {
        delete mutant_creaters[i];
    }

    //��������ʽд������ļ�mutant_expr_file
    try
    {
        WriteMutantExprFile(mutant_expr_file,origin_expr_file,expressions,mutant_types,all_mutants);
    }
    catch(...)//�������ɼ�д����첼�����ʽ�ļ�ʱ�׳���"Fail to Create File!"��"Fail to Write File!"�쳣
    {
        std::cout<<"Error when creating file "<<mutant_expr_file;
        return 0;
    }
}
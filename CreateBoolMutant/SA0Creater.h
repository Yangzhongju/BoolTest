#ifndef SA0_CREATER_H
#define SA0_CREATER_H

#include "MutantCreater.h"

//An occurrence of a condition is replaced by 0
class SA0Creater : public MutantCreater
{
protected:
	virtual void Initialize(const std::string &expression);

	virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:
	
	std::vector<int> m_literal_appear;
};

#endif
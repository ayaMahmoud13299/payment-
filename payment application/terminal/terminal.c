#include "terminal.h"
#include <string.h>






EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	if (strlen(termData->transactionDate) == 10 && (termData->transactionDate[2] == termData->transactionDate[5] == '/'))
	{
		if ((termData->transactionDate[3] == 0 && termData->transactionDate[4] <= '9') || (termData->transactionDate[3] == 1 && (termData->transactionDate[4] <= '2')))
		{
			if ((termData->transactionDate[0] < '3' && termData->transactionDate[1] <= '9') || (termData->transactionDate[0] == '3' && termData->transactionDate[1] <= '1'))
			{
				if ((termData->transactionDate[6] == 2 && termData->transactionDate[7] == 0))
				
				{
					return OK_TER;
				}
			
					 
			}
		}
	}
	else 
			return WRONG_DATE;
}


EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{

	if (cardData.cardExpirationDate[0] > termData.transactionDate[3])
	{
		if (cardData.cardExpirationDate[1] > termData.transactionDate[4])
		{
			if (cardData.cardExpirationDate[3] > termData.transactionDate[0])
			{
				if (cardData.cardExpirationDate[4] > termData.transactionDate[1])
				{
					return OK_TER;
				}
			}
		}
	}
	else
		return EXPIRED_CARD;
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount >= termData->transAmount)
	{
		return OK_TER;
	}
	else
		return EXCEED_MAX_AMOUNT;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount >= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
		return OK_TER;
}
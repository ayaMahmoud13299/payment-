#include "server.h"
#include "string.h"
#include "../card/card.h"

ST_accountsDB_t account_database[255] ;

ST_transaction_t transaction_database[255];

void database_init(void)
{
	for (int i = 0; i < 255; i++)
	{
		account_database[i].balance = i * 1000;
		for (int j = 0; j < 10; j++)
		{
			account_database[i].primaryAccountNumber[j] = '0';
			account_database[i].primaryAccountNumber[j+7] = 'a'+j;
		}
		account_database[i].primaryAccountNumber[17] = i % 100 + '0';
		account_database[i].primaryAccountNumber[18] = (i % 100) / 10 + '0';
		account_database[i].primaryAccountNumber[19] = i % 10 +'0';


	}
}

EN_transStat_t recieveTransactionData(ST_transaction_t* transData)
{
	unsigned char card_here = 0;
	EN_terminalError_t check_amount;
	for (int i = 0; i < 255; i++) //transData->cardHolderData.primaryAccountNumber
	{
		if (strcmp(transData->cardHolderData.primaryAccountNumber, account_database[i].primaryAccountNumber) == 0)
		{
			card_here = 1;
		}
	}
	if (card_here == 0)
	{
		return DECLINED_STOLEN_CARD;
	}
	else
	{
		check_amount = isBelowMaxAmount(&(transData->terminalData));
		if (check_amount == EXCEED_MAX_AMOUNT)
		{
			return DECLINED_INSUFFECIENT_FUND;
		}
		else
		{
			int j = 0;
			while (transaction_database[j].transactionSequenceNumber != OK)
			{
				j++;
			}
			memcpy(transaction_database[j].cardHolderData.primaryAccountNumber ,transData->cardHolderData.primaryAccountNumber,strlen(transaction_database[j].cardHolderData.primaryAccountNumber));
			account_database[j].balance = account_database[j].balance - transaction_database[j].terminalData.transAmount;
			transaction_database[j].terminalData.maxTransAmount = account_database[j].balance;
			transaction_database[j].transactionSequenceNumber++;
			if(transaction_database[j].terminalData.maxTransAmount == account_database[j].balance)
			return APPROVED;
			else
			{
				return INTERNAL_SERVER_ERROR;
			}

		}
	}
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	unsigned char card_here = 0;
	
	for (int i = 0; i < 255; i++)
	{
		if (strcmp(cardData->primaryAccountNumber , account_database[i].primaryAccountNumber)==0)
		{
			card_here = 1;
		}
	}
	if (card_here == 0)
	{
		return DECLINED_STOLEN_CARD;
	}
	else
		return OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount >= termData->transAmount)
	{
		return OK;
	}
	else
		return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_transStat_t state;
	state = recieveTransactionData(&transData);
	if (state == APPROVED)
	{
		transData->transState = APPROVED;
		return OK_S;
	}
	else if (state == DECLINED_INSUFFECIENT_FUND)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	else if (state == INTERNAL_SERVER_ERROR)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}

	

}
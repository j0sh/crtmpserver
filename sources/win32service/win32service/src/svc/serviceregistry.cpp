#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "svc/svcdefines.h"
#include "svc/serviceregistry.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

char * SvcRegistry(int action)
{
	HKEY hTestKey;
	HKEY hSvcKey;
	DWORD retCode;
	char * config_path = "";

	retCode = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE"),
        0,
        KEY_ALL_ACCESS,
        &hTestKey);

   if( retCode == ERROR_SUCCESS)
   {
	   hSvcKey = AddKey();//Creates the key if the key does not exist. If it does, it reads the key value.
	   if(hSvcKey != NULL){
		   switch (action)
		   {
		   case ADD_KEY:
			   if(SetKeyValue(hSvcKey) != ERROR_SUCCESS)
				   return REGIST_ERROR;
			   else {
				   RegCloseKey(hTestKey);
				   return REGIST_SUCCESS;
			   }
			   break;

		   case GET_KEY_VALUE:
			   config_path = QueryKey(hSvcKey);
			   printf("\n configuration path: %s\n", config_path);
			   RegCloseKey(hTestKey);
			   return config_path; //Return the config file path
			   break;
		   }
	   }else
	   {
		   return REGIST_ERROR;
	   }

   } else
   {
	   printf("\n ERROR opening key (%d): %s\n", retCode, FormatWindowsError(retCode));
	   return REGIST_ERROR;
   }
}

//Add the Evostream key to HKEY_LOCAL_MACHINE\SOFTWARE\
//If key already exists, it is opened instead
HKEY AddKey()
{
	HKEY hKey = NULL;
	DWORD dwDisposition;
	DWORD retCode;

	retCode = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Evostream"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	
	if (dwDisposition == REG_CREATED_NEW_KEY)
		printf("\nNew registry key created\n");
	else if (dwDisposition == REG_OPENED_EXISTING_KEY)
		printf("\nRegistry key exists. Key is read successfully.\n");
	else {
		printf("\n ERROR adding key (%d): %s\n", retCode, FormatWindowsError(retCode));
		return hKey;
	}

//	printf("\npath: %s\n", QueryKey(hKey));

	return hKey;

}

DWORD SetKeyValue(HKEY hKey)
{
	DWORD retCode;

	retCode = RegSetValueEx(hKey, TEXT("Config Path"), NULL, REG_SZ, (const BYTE *) CONFIGFILE_PATH, sizeof(CONFIGFILE_PATH)); 
	
	if ( retCode ==ERROR_SUCCESS)
	   printf("\nThe value of the config file was set successfully.\n");
	else
	   printf("\n ERROR setting path as key value (%d): %s\n", retCode, FormatWindowsError(retCode));

	return retCode;
}

char * QueryKey(HKEY hKey) 
{ 
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD i, retCode; 
 
    TCHAR  achValue[MAX_VALUE_NAME]; 
    DWORD cchValue = MAX_VALUE_NAME; 

	char * keyData = new char[MAX_PATH];
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    
    if (cSubKeys)
    {
        printf( "\nDEBUG: Number of subkeys: %d\n", cSubKeys);

        for (i=0; i<cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                     achKey, 
                     &cbName, 
                     NULL, 
                     NULL, 
                     NULL, 
                     &ftLastWriteTime); 
            if (retCode == ERROR_SUCCESS) 
            {
                _tprintf(TEXT("DEBUG:(%d) %s\n"), i+1, achKey);
            }else
			{
				printf("\n ERROR setting path as key value (%d): %s\n", retCode, FormatWindowsError(retCode));
				return REGIST_ERROR;
			}
        }
    } 
 
    // Enumerate the key values. 

    if (cValues) 
    {
        printf( "\nDEBUG: Number of values: %d\n", cValues);
		DWORD dataSize = MAX_PATH;
        for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
        { 
            cchValue = MAX_VALUE_NAME; 
            achValue[0] = '\0'; 
            retCode = RegEnumValue(hKey, i, 
                achValue, 
                &cchValue, 
                NULL, 
                NULL,
                (LPBYTE)keyData,
                &dataSize);
 
            if (retCode == ERROR_SUCCESS ) 
            { 
                _tprintf(TEXT("DEBUG: (%d) %s\n"), i+1, achValue); 
				_tprintf(TEXT("DEBUG: %s\n"), keyData); 
				return keyData;
            }else
			{
				printf("\n ERROR setting path as key value (%d): %s\n", retCode, FormatWindowsError(retCode));
				return REGIST_ERROR;
			}
        }
    }
}

char * FormatWindowsError(int nError)
{
char* szMsgBuffer;

	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, nError, 0, (LPSTR)&szMsgBuffer, 0, NULL);
/*	char * strRet(szMsgBuffer);
	LocalFree(szMsgBuffer);
	return strRet
*/
	return szMsgBuffer;
}

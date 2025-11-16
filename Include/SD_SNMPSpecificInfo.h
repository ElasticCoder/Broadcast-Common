#ifndef SD_SNMPSPECIFICINFO_H
#define SD_SNMPSPECIFICINFO_H


#ifdef _DLL_IS_SNMP_INTERFACE_
	#define SNMP_INTERFACE_EXTERNAL __declspec(dllexport) 
#else
	#define SNMP_INTERFACE_EXTERNAL __declspec(dllimport) 
#endif

SNMP_INTERFACE_EXTERNAL HRESULT ReportSNMPError(int Severity, LPCTSTR ApplicationName, int ErrorCode, LPCTSTR DescriptionString);

#endif SD_SNMPSPECIFICINFO_H

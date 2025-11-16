#ifndef SD_SNMPVARIABLEDEFINITIONS_H
#define SD_SNMPVARIABLEDEFINITIONS_H

// ============================================ //
// >>>>>>>>>>>>Severity code <<<<<<<<<<<<<<<<<< //
// ============================================ //

#define SD_SNMP_CRITICAL		5
#define	SD_SNMP_MAJOR			4
#define SD_SNMP_MINOR			3
#define SD_SNMP_WARNING			2
#define SD_SNMP_NORMAL			1
#define SD_SNMP_UNKNOWN			0


// ============================================ //
// >>>>>>>>>>>>Application Name String<<<<<<<<<<//
// ============================================ //

//
// NOTE: (AH 3/9/99) _T removed as the DLL is non-unicode
//     
#define SD_FILE_COPY_MANAGER		_T("File_Copy_Manager")
#define SD_LMS_TRANSFER				_T("LMS_Transfer")
#define SD_CATALOGUING_CLIENT		_T("Cataloguing_Client")
#define SD_GATEWAY					_T("Gateway")
#define SD_PUBLIPLAN				_T("Publiplan")
#define SD_KAVO						_T("Kavo")
#define SD_PLATO					_T("Plato")
#define SD_TX_SYNC					_T("Tx-Sync")
#define SD_PAWS						_T("PAWS")
#define SD_TDB						_T("TDB")
#define SD_PAT						_T("PAT")
#define SD_DIDL						_T("DIDL")
#define SD_AUTO_FDL					_T("AUTO_FDL")
#define SD_STREAM_SYNC				_T("STREAM_SYNC")
#define SD_PAWS_LITE				_T("Remote_Editor")
#define SD_MATERIAL_BROKER			_T("Material_Broker")

#define SD_SCHEDULE_DOWNLOADER		_T("Schedule_downloader")
#define SD_BROWSE_SERVER			_T("Browse_Server")
#define SD_BROWSE_CLIENT			_T("Browse_Client")
#define SD_COPY_FORWARDING_SERVER	_T("Copy_Forwarding_Server")
#define SD_RESOURCE_MANAGER			_T("Resource_Manager")
#define SD_REMOTE_ALARMS			_T("Remote_Alarms")


// ============================================ //
// >>>>>>>>>>>SNMP Trap specific errors<<<<<<<<<//
// ============================================ //

// =============================================
// ==Error Code to be use by SPD APPLICATION ===
// =============================================

/********************************************************/
/*	These error codes are common and will be used		*/
/*	for the same reason for all SPD projects			*/
/********************************************************/

#define SNMP_APP_STATUS				1
#define SNMP_MAV_STATUS				2
#define SNMP_TDB_STATUS				3
#define SNMP_PUBLIPLAN_STATUS		4
#define SNMP_MATERIAL_STATUS		5
#define SNMP_820_STATUS				6
#define SNMP_COMMS_STATUS			7
#define SNMP_PETASITE_STATUS		8
#define SNMP_GATEWAY_STATUS			9
#define SNMP_FCM_STATUS				10
#define SNMP_APP_VERSION			20


/**********************************************/
/*	Generic error codes                       */
/*	This can be assigned for different errors */
/**********************************************/

#define SNMP_GENERIC_11						11
#define SNMP_GENERIC_12						12
#define SNMP_GENERIC_13						13
#define SNMP_GENERIC_14						14
#define SNMP_GENERIC_15						15
#define SNMP_GENERIC_16						16
#define SNMP_GENERIC_17						17
#define SNMP_GENERIC_18						18
#define SNMP_GENERIC_19						19

// =======================================
// === Stream Synchroniser Error Codes ===
// =======================================

#define SNMP_STREAM_SYNC_PLATO_STATUS		SNMP_GENERIC_15
#define SNMP_STREAM_SYNC_STC_STATUS			SNMP_GENERIC_16
#define SNMP_STREAM_SYNC_SUBTITLING_STATUS	SNMP_GENERIC_17

// ===========================================
// ========= CATALOGUING Error Code ==========
// ===========================================

#define CAT_VERSION							SNMP_APP_VERSION

#define SNMP_CAT_APP_STATUS					SNMP_APP_STATUS
#define SNMP_CAT_MAV_STATUS					SNMP_MAV_STATUS
#define SNMP_CAT_TDB_STATUS					SNMP_TDB_STATUS
#define SNMP_CAT_MISSING_MATERIAL_STATUS	SNMP_MATERIAL_STATUS
#define SNMP_CAT_GATEWAY_STATUS				SNMP_GATEWAY_STATUS
#define SNMP_CAT_ROUTER_STATUS				SNMP_GENERIC_11
#define SNMP_CAT_BXC100_STATUS				SNMP_GENERIC_16


// ===========================================
// ================ PLATO Error Code =========
// ===========================================


#define PLATO_VERSION						SNMP_APP_VERSION

#define SNMP_PLATO_APP_STATUS				SNMP_APP_STATUS
#define SNMP_PLATO_TDB_STATUS				SNMP_TDB_STATUS
#define SNMP_PLATO_820_STATUS				SNMP_820_STATUS


// ===========================================
// ================ KAVO Error Code ==========
// ===========================================

#define KAVO_VERSION						SNMP_APP_VERSION

#define SNMP_KAVO_APP_STATUS				SNMP_APP_STATUS
#define SNMP_KAVO_TDB_STATUS				SNMP_TDB_STATUS
#define SNMP_KAVO_MISSING_MATERIAL_STATUS	SNMP_MATERIAL_STATUS
#define SNMP_KAVO_COMMS_STATUS				SNMP_COMMS_STATUS

#define SNMP_KAVO_ROUTER_STATUS				SNMP_GENERIC_11
#define SNMP_KAVO_DATA_INSERTER_STATUS		SNMP_GENERIC_12
#define SNMP_KAVO_TX_SYNC_STATUS			SNMP_GENERIC_13
#define SNMP_KAVO_STILLS_STORE_STATUS		SNMP_GENERIC_14
#define SNMP_KAVO_GPI_STATUS				SNMP_GENERIC_18


// ===========================================
// ================ TX-SYNC Error Code =======
// ===========================================

#define TX_SYNC_VERSION						SNMP_APP_VERSION

#define SNMP_TX_SYNC_APP_STATUS				SNMP_APP_STATUS
#define SNMP_TX_SYNC_TDB_STATUS				SNMP_TDB_STATUS



// ===========================================
// ================ PAWS Error Code =======
// ===========================================

#define PAWS_VERSION						SNMP_APP_VERSION

#define SNMP_PAWS_APP_STATUS				SNMP_APP_STATUS
#define SNMP_PAWS_TDB_STATUS				SNMP_TDB_STATUS


// ===========================================
// =========== LMS_TRANSFER Error Code =======
// ===========================================

#define LMS_VERSION						SNMP_APP_VERSION

#define SNMP_LMS_APP_STATUS					SNMP_APP_STATUS
#define SNMP_LMS_MAV_STATUS					SNMP_MAV_STATUS
#define SNMP_LMS_TDB_STATUS					SNMP_TDB_STATUS
#define SNMP_LMS_PUBLIPLAN_STATUS				SNMP_PUBLIPLAN_STATUS
#define SNMP_LMS_CART_STATUS					SNMP_GENERIC_11
#define SNMP_LMS_RM_CONVERTER_STATUS				SNMP_GENERIC_12
#define SNMP_LMS_BROWSE_SERVER_STATUS				SNMP_GENERIC_13
#define SNMP_LMS_BROWSE_MATERIAL_STATUS				SNMP_GENERIC_14
#define SNMP_LMS_FILLING_MATERIAL_STATUS			SNMP_GENERIC_15
#define SNMP_LMS_BXC_100_STATUS					SNMP_GENERIC_16
#define SNMP_LMS_FCM_STATUS					SNMP_FCM_STATUS


// ===========================================
// =========== STREAM SYNC Error Code ========
// ===========================================

#define STREAM_SYNC_VERSION					SNMP_APP_VERSION

#define SNMP_STREAM_SYNC_APP_STATUS			SNMP_APP_STATUS
#define SNMP_STREAM_SYNC_TDB_STATUS			SNMP_TDB_STATUS

#define SNMP_STREAM_SYNC_PLATO_STATUS		SNMP_GENERIC_15
#define SNMP_STREAM_SYNC_STC_STATUS			SNMP_GENERIC_16
#define SNMP_STREAM_SYNC_SUBTITLING			SNMP_GENERIC_17

// ===========================================
// =============== DIDL Error Code ===========
// ===========================================

#define DIDL_VERSION						SNMP_APP_VERSION
											
#define SNMP_DIDL_APP_STATUS				SNMP_APP_STATUS
#define SNMP_DIDL_TDB_STATUS				SNMP_TDB_STATUS
											
#define SNMP_DIDL_PETASITE_STATUS			SNMP_PETASITE_STATUS
#define SNMP_DIDL_GATEWAY_STATUS			SNMP_GATEWAY_STATUS


// ===========================================
// =============== FCM Error Code ============
// ===========================================

// Please note: FCM should never use
// SNMP_GENERIC_18
// SNMP_GENERIC_19
// As these are currently assigned to Remote Alarms
// (which shares the same PC as File Copy Manager)
// This is an unfortunate frig which will be resolved in the future
 
#define FCM_VERSION							SNMP_APP_VERSION

#define SNMP_FCM_APP_STATUS					SNMP_APP_STATUS
#define SNMP_FCM_TDB_STATUS					SNMP_TDB_STATUS
#define SNMP_FCM_820_STATUS					SNMP_820_STATUS
#define SNMP_FCM_MAV_STATUS					SNMP_MAV_STATUS
#define SNMP_FCM_MATERIAL_STATUS			SNMP_MATERIAL_STATUS
#define	SNMP_FCM_ARCHIVE_STATUS				SNMP_PETASITE_STATUS
#define SNMP_FCM_GENERAL_STATUS				SNMP_FCM_STATUS


// ===========================================
// =============== PUBLIPLAN Error Code ============
// ===========================================

#define PUBLIPLAN_VERSION					SNMP_APP_VERSION

#define SNMP_PUBLIPLAN_APP_STATUS			SNMP_APP_STATUS
#define SNMP_PUBLIPLAN_TDB_STATUS			SNMP_TDB_STATUS

////// TO DO ///////

// ===========================================
// =============== GATEWAY Error Code ============
// ===========================================

#define GATEWAY_VERSION						SNMP_APP_VERSION
											
#define SNMP_GATEWAY_APP_STATUS				SNMP_APP_STATUS
#define SNMP_GATEWAY_TDB_STATUS				SNMP_TDB_STATUS
////// TO DO ///////						

// ===========================================
// =============== AUTO FDL Error Code =======
// ===========================================

#define AUTO_FDL_VERSION					SNMP_APP_VERSION
											
#define SNMP_AUTO_FDL_APP_STATUS			SNMP_APP_STATUS
#define SNMP_AUTO_FDL_TDB_STATUS			SNMP_TDB_STATUS
/// TO DO ?????????????						

// ===========================================
// =============== PAT Error Code ============
// ===========================================
											
#define PAT_VERSION							SNMP_APP_VERSION
											
#define SNMP_PAT_APP_STATUS					SNMP_APP_STATUS
#define SNMP_PAT_TDB_STATUS					SNMP_TDB_STATUS
#define SNMP_PAT_FCM_STATUS					SNMP_FCM_STATUS


// ===========================================
// =============== TDB Error Code ============
// ===========================================

#define TDB_VERSION							SNMP_APP_VERSION
											
#define SNMP_TDB_APP_STATUS					SNMP_APP_STATUS


// ===========================================
// == PAWS Lite (Remote Editor) Error Codes ==
// ===========================================

#define PAWS_LITE_VERSION					SNMP_APP_VERSION

#define SNMP_PAWS_LITE_APP_STATUS			SNMP_APP_STATUS
#define SNMP_PAWS_LITE_TDB_STATUS			SNMP_TDB_STATUS
#define SNMP_PAWS_LITE_820_STATUS			SNMP_820_STATUS
#define SNMP_PAWS_LITE_BXC100_STATUS		SNMP_GENERIC_16

// ===================================
// === Material Broker Error Codes ===
// ===================================

#define MB_VERSION							SNMP_APP_VERSION

#define SNMP_MB_APP_STATUS					SNMP_APP_STATUS
#define SNMP_MB_TDB_STATUS					SNMP_TDB_STATUS		
#define SNMP_MB_FCM_STATUS					SNMP_FCM_STATUS
#define SNMP_MB_GENERAL_STATUS				SNMP_GENERIC_11

// ==========================================
// === Copy Forwarding Server Error Codes ===
// ==========================================
#define SNMP_COPY_FWD_APP_STATUS			SNMP_APP_STATUS
#define SNMP_COPY_FWD_FCM_STATUS			SNMP_FCM_STATUS
#define SNMP_COPY_FWD_823_STATUS			SNMP_GENERIC_11

// ====================================
// === Resource Manager Error Codes ===
// ====================================
#define SNMP_RSRCE_MANAGER_APP_STATUS		SNMP_APP_STATUS
#define SNMP_RSRCE_MANAGER_TDB_STATUS		SNMP_TDB_STATUS
#define SNMP_RSRCE_MANAGER_FCM_STATUS		SNMP_FCM_STATUS
#define SNMP_RSRCE_MANAGER_CAPACITY_STATUS	SNMP_GENERIC_16

// =================================
// === Remote Alarms Error Codes ===
// =================================

// Note: This application shares the same PC as PAWS Lite
// Great care should be taken if modifying these variables

#define SNMP_REMOTE_ALARMS_APP_STATUS		SNMP_GENERIC_18
#define SNMP_REMOTE_ALARMS_MAV_STATUS		SNMP_GENERIC_19


#endif SD_SNMPVARIABLEDEFINITIONS_H

#define ELPP_AS_DLL // Tells Easylogging++ that it's used for DLL
#define ELPP_EXPORT_SYMBOLS // Tells Easylogging++ to export symbols
#define MYLOG_EXPORTS

#include "Logger.h"

INITIALIZE_EASYLOGGINGPP

el::base::type::StoragePointer sharedLoggingRepository() {
	return el::Helpers::storage();
}



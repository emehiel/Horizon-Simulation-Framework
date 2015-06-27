/**
 * This file contains the HandlerType enumeration.
 * @author Brian Butler
 */

#ifndef HANDLERTYPE_H
#define HANDLERTYPE_H

namespace horizon {
	namespace net {
		namespace handler {

/**
 * The type of DataHandler that should be used.
 * This enum to specifies the type of data handler that should be used to process messages.
 */
enum HandlerType {
	/** The first DataHandler that can process the message should be used. */
	ANY_DATA_HANDLER = 0, 
	/** The first FlatFileDataHandler that can process the message should be used. */
	FLAT_FILE_DATA_HANDLER = 1, 
	/** The first XMLFileDataHandler that can process the message should be used. */
	XML_FILE_DATA_HANDLER = 2
};

}}} // horizon::net::handler

#endif
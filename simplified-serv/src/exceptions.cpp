#include "../include/TestServer.hpp"


//Exeptions


//301	Moved Permanently
const char *TestServer::MovedPermanently::what() const throw()
{
	return ("ERROR: Resource is moved permanently  301");
}


//400	Bad Request
const char *TestServer::BadRequest::what() const throw()
{
	return ("ERROR: Timeout has happened 400");
}

//403	Forbidden
const char *TestServer::Forbidden::what() const throw()
{
	return ("ERROR: Forbidden 403");
}

//404	Not Found
const char *TestServer::NotFound::what() const throw()
{
	return ("ERROR: Recourse not found 404");
}

//405	Method Not Allowed
const char *TestServer::MethodNotAllowed::what() const throw()
{
	return ("ERROR: Method is not allowed 405");
}

//408	Timeout
const char *TestServer::Timeout::what() const throw()
{
	return ("ERROR: Timeout has happened 408");
}

//411
const char *TestServer::LengthRequired::what() const throw()
{
	return ("ERROR: Content-Length is required 411");
}

//413
const char *TestServer::PayloadTooLarge::what() const throw()
{
	return ("ERROR: Payload is Too Large 413");
}

//414
const char *TestServer::URITooLong::what() const throw()
{
	return ("ERROR: URI Too Long 414");
}

//500	Internal Server Error
const char *TestServer::InternalServerError::what() const throw()
{
	return ("ERROR: Internal Server Erorr 500");
}

//501	Not Implement
const char *TestServer::NotImplemented::what() const throw()
{
	return ("ERROR: Not Implemented 501");
}


//505	HTTP version not supported
const char *TestServer::HTTPVersionNotSupported::what() const throw()
{
	return ("ERROR: HTTP Version Not Supported 505");
}

//508	Gateway timeout
const char *TestServer::GatewayTimeout::what() const throw()
{
	return ("ERROR: Gateway Timeout 508");
}
#ifndef __HTTP_H__
#define __HTTP_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace cocos2d::network;

class ChainHttpClient : public Ref {
public:
	static const int UNSET_CONTENT_LENGTH = -1;
	CREATE_FUNC(ChainHttpClient);
	ChainHttpClient();
	~ChainHttpClient();
	ChainHttpClient * setUrl(const char * url);
	ChainHttpClient * addQuery(const char * key, const char * value);
	ChainHttpClient * setCharBufferContent(const char * buffer, size_t len = UNSET_CONTENT_LENGTH);
	ChainHttpClient * addHeader(const char * key, const char * value);
	ChainHttpClient * GET(const ccHttpRequestCallback & callback, bool autoRelease = true);
	ChainHttpClient * POST(const ccHttpRequestCallback & callback, bool autoRelease = true);
protected:
private:
	bool init();
	void assertReleased();
	void addQueries();
	void addHeaders();
	HttpRequest * httpReq;
	std::map<std::string, std::string> queries;
	std::map<std::string, std::string> headers;
};

class ChainHttpResponse/* : public Ref*/ {
public:
	ChainHttpResponse(HttpResponse * res);
	~ChainHttpResponse();
	HttpResponse * getResponse();
	const char * getHeader(const char * key);
	const char * getDataString();
private:
	HttpResponse * response;
	std::map<std::string, std::string> headers;
	std::string dataString;
};

#endif // !__HTTP_H__
